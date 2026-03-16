from application.brain import Brain
from adapter.transport.websocket_input import InputSource
from adapter.transport.websocket_output import OutputSink
from adapter.message.message_processor import MessageProcessor
from domain.models.message import TokenMessage, DoneMessage

from application.use_cases.sentence_buffer import SentenceBuffer

class RobotSession:
    def __init__(self, brain, input_source, output_sink, tts):
        self.brain = brain
        self.input_source = input_source
        self.output_sink = output_sink
        self.message_processor = MessageProcessor()
        self.tts = tts
        self._running = True

    async def _stream_tts(self, sentence: str):
        async for audio_chunk in self.tts.stream(sentence):
            await self.output_sink.send_binary(audio_chunk)

    async def run(self):
        try:
            async for raw_message in self.input_source.listen():
                prompt = self.message_processor.process(raw_message)
                if not prompt:
                    continue

                sentence_buffer = SentenceBuffer()

                async for message in self.brain.stream(prompt.data):

                    if isinstance(message, TokenMessage):
                        # 1️⃣ Kirim token ke ESP32
                        await self.output_sink.send(message)

                        # 2️⃣ Cek apakah sudah terbentuk kalimat lengkap
                        ready_sentences = sentence_buffer.push(message.data)
                        for sentence in ready_sentences:
                            await self._stream_tts(sentence)

                    elif isinstance(message, DoneMessage):
                        # 3️⃣ Flush sisa token yang belum membentuk kalimat lengkap
                        for sentence in sentence_buffer.flush_remaining():
                            await self._stream_tts(sentence)

                        await self.output_sink.send(message)

        except Exception as e:
            print(f"Session error: {e}")
        finally:
            await self.cleanup()

    async def cleanup(self):
        self._running = False
        print("RobotSession cleanup complete")
