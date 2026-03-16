# infrastructure/ollama_client.py

import ollama
import asyncio
from typing import AsyncGenerator, Optional
from domain.interfaces.llm_interface import LLMClient


SYSTEM_PROMPT = (
    "You are Jannine, a cute robot companion made by Anthony."
    "You are energetic and playful, but never give emoticons for your response."
)


class OllamaClient(LLMClient):

    def __init__(self, model_name: str = "openchat:latest"):
        self.model_name = model_name

    async def stream(self, prompt: str) -> AsyncGenerator[str, None]:
        loop = asyncio.get_running_loop()
        queue: asyncio.Queue = asyncio.Queue()

        def producer():
            try:
                stream = ollama.chat(
                    model=self.model_name,
                    messages=[
                        {"role": "system", "content": SYSTEM_PROMPT},
                        {"role": "user", "content": prompt},
                    ],
                    stream=True,
                    think=False
                )

                for chunk in stream:
                    text = self._extract(chunk)
                    if text:
                        asyncio.run_coroutine_threadsafe(
                            queue.put(text),
                            loop
                        )

            except Exception:
                asyncio.run_coroutine_threadsafe(
                    queue.put("Maaf, Jannine sedang bermasalah."),
                    loop
                )
            finally:
                asyncio.run_coroutine_threadsafe(
                    queue.put(None),
                    loop
                )

        # Jalankan blocking stream di thread terpisah
        asyncio.create_task(asyncio.to_thread(producer))

        # Async consumer
        while True:
            item = await queue.get()
            if item is None:
                break
            yield item

    @staticmethod
    def _extract(chunk) -> Optional[str]:
        if isinstance(chunk, dict):
            msg = chunk.get("message")
            if isinstance(msg, dict):
                return msg.get("content")
            return chunk.get("completion")

        if hasattr(chunk, "message"):
            return getattr(chunk.message, "content", None)

        if isinstance(chunk, str):
            return chunk

        return None
