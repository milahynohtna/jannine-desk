import asyncio
import pyttsx3
import tempfile
import subprocess
from typing import AsyncGenerator


class LocalTTS:

    def __init__(self, rate: int = 200, sample_rate: int = 16000):
        self.rate = rate
        self.sample_rate = sample_rate

    async def stream(self, text: str) -> AsyncGenerator[bytes, None]:
        """
        TTS lokal → AIFF → convert PCM 16-bit mono → stream bytes async
        """

        with tempfile.NamedTemporaryFile(suffix=".aiff") as tmp_aiff, \
             tempfile.NamedTemporaryFile(suffix=".pcm") as tmp_pcm:

            # 1️⃣ Generate AIFF dengan pyttsx3
            engine = pyttsx3.init()
            engine.setProperty("rate", self.rate)

            # voices = engine.getProperty("voices")
            # engine.setProperty("voice", voices[97].id)

            engine.save_to_file(text, tmp_aiff.name)
            engine.runAndWait()

            # 2️⃣ Convert AIFF → PCM 16-bit mono 44100 Hz via ffmpeg
            subprocess.run([
                "ffmpeg",
                "-y",
                "-i", tmp_aiff.name,
                "-f", "s16le",
                "-acodec", "pcm_s16le",
                "-ac", "1",
                "-ar", str(self.sample_rate),
                tmp_pcm.name
            ], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

            # 3️⃣ Stream PCM ke WebSocket / ESP32
            chunk_size = 1024
            with open(tmp_pcm.name, "rb") as f:
                while True:
                    chunk = f.read(chunk_size)
                    if not chunk:
                        break
                    yield chunk
                    await asyncio.sleep(0)