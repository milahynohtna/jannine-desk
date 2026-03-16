#!/usr/bin/env python3
import os
import argparse
from PIL import Image

def rgb888_to_rgb565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def convert(image_path, base_out, name, swap):
    img = Image.open(image_path).convert("RGB")
    width, height = img.size
    pixels = img.load()

    data = []
    for y in range(height):
        for x in range(width):
            r, g, b = pixels[x, y]
            v = rgb888_to_rgb565(r, g, b)
            if swap:
                v = ((v & 0xFF) << 8) | (v >> 8)
            data.append(v)

    # === TARGET PATHS (FIXED & CLEAN) ===
    include_dir = os.path.join(base_out, "include", "images")
    src_dir     = os.path.join(base_out, "src")

    os.makedirs(include_dir, exist_ok=True)
    os.makedirs(src_dir, exist_ok=True)

    h_path   = os.path.join(include_dir, f"{name}.h")
    cpp_path = os.path.join(src_dir, f"{name}.cpp")

    macro = name.upper()

    # ================= HEADER =================
    with open(h_path, "w") as f:
        f.write(f"""#pragma once
#include "images/ImageDescriptor.h"

extern const ImageDescriptor {macro};
""")

    # ================= CPP ====================
    with open(cpp_path, "w") as f:
        f.write(f"""#include "images/{name}.h"
#include <pgmspace.h>

static const uint16_t {macro}_DATA[] PROGMEM = {{
""")

        for i in range(0, len(data), 12):
            row = ", ".join(f"0x{v:04X}" for v in data[i:i+12])
            f.write(f"    {row},\n")

        f.write(f"""}};

const ImageDescriptor {macro} = {{
    .x = 0,
    .y = 0,
    .width = {width},
    .height = {height},
    .data = {macro}_DATA
}};
""")

    print(f"✅ {h_path}")
    print(f"✅ {cpp_path}")

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("input")
    ap.add_argument("-o", "--out", default="lib/images")
    ap.add_argument("-n", "--name")
    ap.add_argument("--swap", action="store_true")
    ap.add_argument("--resize", nargs=2, type=int)

    args = ap.parse_args()

    name = args.name or os.path.splitext(os.path.basename(args.input))[0]
    name = name.replace("-", "_")

    path = args.input
    if args.resize:
        img = Image.open(path)
        img = img.resize((args.resize[0], args.resize[1]), Image.Resampling.LANCZOS)
        tmp = "/tmp/_img.png"
        img.save(tmp)
        path = tmp

    convert(path, args.out, name, args.swap)

if __name__ == "__main__":
    main()


'''
python3 tools/jpg_to_rgb565.py assets/raw/talking_0011.jpg \
                                --resize 240 240
'''

