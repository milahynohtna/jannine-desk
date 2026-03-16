import asyncio
from domain.interfaces.robot_connection_interface import RobotConnection


class WebSocketConnection(RobotConnection):
    def __init__(self, websocket):
        self.websocket = websocket

        # Queue internal untuk decouple
        self._tx_queue = asyncio.Queue()
        self._rx_queue = asyncio.Queue()

        self._running = False
        self._tasks = []

    async def start(self):
        """
        Mulai background TX dan RX loop.
        """
        if self._running:
            return

        self._running = True

        self._tasks = [
            asyncio.create_task(self._tx_loop()),
            asyncio.create_task(self._rx_loop()),
        ]

    async def stop(self):
        """
        Stop semua loop dengan clean shutdown.
        """
        self._running = False

        # Push sentinel untuk unblock queue
        await self._tx_queue.put(None)
        await self._rx_queue.put(None)

        for task in self._tasks:
            task.cancel()

        await asyncio.gather(*self._tasks, return_exceptions=True)

    # ========================
    # PUBLIC API (DOMAIN SAFE)
    # ========================

    async def send(self, data: str):
        """
        Kirim string/bytes (non-blocking).
        """
        await self._tx_queue.put(data)

    async def receive(self) -> str:
        """
        Terima string/bytes (await sampai ada data).
        """
        return await self._rx_queue.get()

    # ========================
    # INTERNAL LOOPS
    # ========================

    async def _tx_loop(self):
        """
        Dedicated send loop.
        """
        try:
            while True:
                data = await self._tx_queue.get()
                if data is None:
                    break
                await self.websocket.send(data)
        except asyncio.CancelledError:
            pass

    async def _rx_loop(self):
        """
        Dedicated receive loop.
        """
        try:
            while True:
                message = await self.websocket.recv()
                if not self._running:
                    break
                await self._rx_queue.put(message)
        except asyncio.CancelledError:
            pass
