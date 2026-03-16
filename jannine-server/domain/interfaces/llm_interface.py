# domain/llm_interface.py

from abc import ABC, abstractmethod
from typing import AsyncGenerator

class LLMClient(ABC):

    @abstractmethod
    async def stream(self, prompt: str) -> AsyncGenerator[str, None]:
        pass

