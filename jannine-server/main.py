import asyncio
import websockets
from application.services.websocket_handler import websocket_handler
from config.settings import WS_PORT, WS_HOST

async def main():

    async with websockets.serve(websocket_handler, WS_HOST, WS_PORT):
        print(f"WebSocket server running on ws://{WS_HOST}:{WS_PORT}")
        await asyncio.Future()  # keep server running

if __name__ == "__main__":
    asyncio.run(main())
