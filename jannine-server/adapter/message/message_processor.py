from typing import Optional

from domain.models.message import Message, PromptMessage
from application.use_cases.process_input_string import process_input_string


class MessageProcessor:
    """Processes incoming messages"""

    @staticmethod
    def process(raw_message: str) -> Optional[PromptMessage]:
        """
        Process raw message string into PromptMessage.
        Returns None if not a valid prompt.
        """
        msg: Optional[Message] = process_input_string(raw_message)

        if isinstance(msg, PromptMessage):
            return msg

        return None