from dataclasses import dataclass


class Message:
    pass

@dataclass
class PromptMessage(Message):
    data: str

@dataclass
class TokenMessage(Message):
    data: str

class DoneMessage(Message):
    pass

@dataclass
class StringMessage(Message):
    type: str
    data: str

class AudioMessage:
    def __init__(self, data: bytes):
        self.data = data