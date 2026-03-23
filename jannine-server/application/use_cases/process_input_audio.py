from typing import Optional
import numpy as np
import whisper
import json

from domain.models.message import PromptMessage

_model = whisper.load_model("base")

_audio_buffer = bytearray()


def process_input_audio(raw) -> Optional[PromptMessage]:
    global _audio_buffer

    # ───────────────────────────────
    # AUDIO BYTES → BUFFER
    # ───────────────────────────────
    if isinstance(raw, (bytes, bytearray)):
        _audio_buffer.extend(raw)
        return None

    # ───────────────────────────────
    # TEXT → CHECK CONTROL SIGNAL
    # ───────────────────────────────
    if isinstance(raw, str):
        try:
            data = json.loads(raw)
        except:
            return None

        if data.get("type") == "audio_end":

            if len(_audio_buffer) == 0:
                print("[STT] buffer kosong")
                return None

            try:
                print(f"[STT] processing {len(_audio_buffer)} bytes")

                # 🔥 FIX 1: COPY buffer (WAJIB)
                raw_copy = bytes(_audio_buffer)

                # 🔥 clear buffer SEBELUM dipakai numpy
                _audio_buffer.clear()

                # 🔥 FIX 2: baru convert
                audio_np = np.frombuffer(raw_copy, dtype=np.int16)

                # normalize
                audio_float = audio_np.astype(np.float32) / 32768.0

                # whisper
                result = _model.transcribe(
                    audio_float,
                    language="en",
                    fp16=False
                )

                text = result.get("text", "").strip()

                print(f"[STT RESULT] {text}")

                if text == '':
                    text = 'who are you?'

                if not text:
                    return None

                return PromptMessage(data=text)

            except Exception as e:
                print("STT error:", e)
                _audio_buffer.clear()
                return None

    return None