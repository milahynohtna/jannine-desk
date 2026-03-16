from dataclasses import dataclass

from domain.models.message import Message

@dataclass
class AudioChunkMessage(Message):
    data: bytes
