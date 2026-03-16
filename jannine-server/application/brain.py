from domain.interfaces.llm_interface import LLMClient
from typing import Generator
from domain.models.message import TokenMessage, DoneMessage


class Brain:

    def __init__(self, llm: LLMClient):
        self.llm = llm

    def respond(self, user_text: str) -> Generator[str, None, None]:
        for chunk in self.llm.stream(user_text):
            yield chunk

    async def stream(self, user_text: str):
        async for chunk in self.llm.stream(user_text):
            yield TokenMessage(data=chunk)

        yield DoneMessage()
