from infrastructure.communication.websocket_server import WebSocketConnection
import json
from domain.models.message import (
    Message,
    TokenMessage,
    DoneMessage,
)


class OutputSink:
    async def send(self, message: Message):
        raise NotImplementedError()

    async def send_binary(self, data: bytes):
        raise NotImplementedError()


# digunakan untuk komunikasi antara server dan device
class WebSocketOutput(OutputSink):

    def __init__(self, websocket):
        self.websocket = websocket

    async def send(self, message: Message):
        payload = self._serialize(message)
        await self.websocket.send(payload)

    async def send_binary(self, data: bytes):
        """
        Kirim raw PCM bytes langsung ke ESP32.
        Harus berupa bytes, bukan string.
        """
        #print("Sending binary:", type(data), len(data))

        await self.websocket.send(data)

    def _serialize(self, message: Message) -> str:

        if isinstance(message, TokenMessage):
            return json.dumps({
                "type": "token",
                "data": message.data,
            })

        if isinstance(message, DoneMessage):
            return json.dumps({
                "type": "done",
            })

        raise ValueError(f"Unknown message type: {type(message)}")