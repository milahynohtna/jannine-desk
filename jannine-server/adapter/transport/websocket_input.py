from typing import AsyncGenerator
from infrastructure.communication.websocket_server import WebSocketConnection


class InputSource:
    async def listen(self) -> AsyncGenerator[str, None]:
        raise NotImplementedError() #refactor kalau sudah ada protocol lain, misalnya bluetooth


class WebSocketInput(InputSource):
    def __init__(self, connection: WebSocketConnection):
        self.connection = connection

    async def listen(self) -> AsyncGenerator[str, None]:
        await self.connection.start()
        try:
            while True:
                raw = await self.connection.receive()
                if raw is None:
                    break
                yield raw
        finally:
            await self.connection.stop()
