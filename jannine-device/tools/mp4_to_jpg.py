
import subprocess
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent

VIDEO_PATH = PROJECT_ROOT / "assets/raw/mp4/talking.mp4"
OUTPUT_DIR = PROJECT_ROOT / "assets/raw"
OUTPUT_PATTERN = OUTPUT_DIR / "talking_%04d.jpg"

OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

FPS = 10

cmd = [
    "ffmpeg",
    "-i", str(VIDEO_PATH),
    "-vf", f"fps={FPS}",
    "-vsync", "0",
    str(OUTPUT_PATTERN)
]

subprocess.run(cmd, check=True)
print(f"✅ listening.mp4 → JPG @ {FPS} FPS")

#python tools/mp4_to_jpg.py