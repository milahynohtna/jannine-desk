import cv2
import time
from pathlib import Path

# ====== KONFIGURASI ======
FPS = 10
WINDOW_NAME = "Jannine Preview"

PROJECT_ROOT = Path(__file__).resolve().parent.parent
IMAGE_DIR = PROJECT_ROOT / "assets/raw"

# Filter hanya listening_*.jpg
images = sorted(IMAGE_DIR.glob("listening_*.jpg"))

if not images:
    raise RuntimeError("❌ Tidak ada file listening_*.jpg")

frame_delay = 1.0 / FPS

cv2.namedWindow(WINDOW_NAME, cv2.WINDOW_AUTOSIZE)

print(f"▶ Preview {len(images)} frame @ {FPS} FPS")
print("Tekan ESC untuk keluar")

while True:
    for img_path in images:
        img = cv2.imread(str(img_path))
        if img is None:
            continue

        cv2.imshow(WINDOW_NAME, img)

        key = cv2.waitKey(1)
        if key == 27:  # ESC
            cv2.destroyAllWindows()
            exit(0)

        time.sleep(frame_delay)

#python tools/preview_jpg_sequence.py
