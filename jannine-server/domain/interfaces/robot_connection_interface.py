from abc import ABC, abstractmethod


class RobotConnection(ABC):

    @abstractmethod
    async def start(self):
        pass

    @abstractmethod
    async def send(self, data: dict):
        pass

    @abstractmethod
    async def receive(self) -> dict:
        pass


    @abstractmethod
    async def stop(self):
        pass
