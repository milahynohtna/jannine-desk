import json
from typing import Optional
from domain.models.message import PromptMessage, Message


def process_input_string(raw: str) -> Optional[Message]:
    """
    Ambil string JSON, parse jadi model domain.
    Return None kalau format salah.
    """
    try:
        data = json.loads(raw)
        msg_type = data.get("type")
        msg_data = data.get("data", "")

        if msg_type == "prompt":
            return PromptMessage(data=msg_data)

        return None

    except json.JSONDecodeError:
        return None
