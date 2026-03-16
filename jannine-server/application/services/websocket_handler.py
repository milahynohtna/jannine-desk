import websockets
from infrastructure.llm.ollama_client import OllamaClient
from infrastructure.communication.websocket_server import WebSocketConnection
from application.brain import Brain
from application.services.robot_session import RobotSession
from adapter.transport.websocket_output import WebSocketOutput
from adapter.transport.websocket_input import WebSocketInput
from infrastructure.tts.local_tts import LocalTTS

from config.settings import LLM_MODEL

async def websocket_handler(websocket):
    # 1️⃣ Buat transport connection
    connection = WebSocketConnection(websocket)

    # 2️⃣ Brain / LLM engine
    llm = OllamaClient(model_name=LLM_MODEL)
    brain = Brain(llm)

    # 3️⃣ Bungkus connection menjadi InputSource dan OutputSink
    input_source = WebSocketInput(connection)
    output_sink = WebSocketOutput(connection)

    my_tts = LocalTTS()

    # 4️⃣ RobotSession orchestrator
    session = RobotSession(brain=brain, input_source=input_source, output_sink=output_sink, tts=my_tts)

    # 5️⃣ Jalankan session
    try:
        await session.run()
    except websockets.ConnectionClosed:
        print("ESP32 Disconnected")
