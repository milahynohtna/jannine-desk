from typing import Optional, Union

from domain.models.message import PromptMessage
from application.use_cases.process_input_string import process_input_string
from application.use_cases.process_input_audio import process_input_audio


class MessageProcessor:

    @staticmethod
    def process(raw_message: Union[str, bytes]) -> Optional[PromptMessage]:

        # 🔊 AUDIO (bytes)
        if isinstance(raw_message, (bytes, bytearray)):
            return process_input_audio(raw_message)

        # 🔤 TEXT (JSON / audio_end / prompt)
        elif isinstance(raw_message, str):

            # 🔥 coba audio handler dulu
            audio_msg = process_input_audio(raw_message)
            if audio_msg:
                return audio_msg

            # 🔤 fallback ke prompt biasa
            return process_input_string(raw_message)

        return None