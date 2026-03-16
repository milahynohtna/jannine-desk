from abc import ABC, abstractmethod
from typing import AsyncGenerator


class TTSService(ABC):

    @abstractmethod
    async def stream(self, text: str) -> AsyncGenerator[bytes, None]:
        """
        Convert text into streaming PCM audio chunks.
        Yields raw PCM 16-bit little-endian bytes.
        """
        pass