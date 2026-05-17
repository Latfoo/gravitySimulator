#!/usr/bin/env python3
"""Capture frames from the GravitySim OpenGL window and save as GIF to docs/."""

import argparse
import io
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

import numpy as np
from PIL import Image

WINDOW_NAME = "GLFW Window"
DOCS_DIR = Path(__file__).parent.parent / "docs"
OUTPUT_GIF = DOCS_DIR / "preview.gif"


def find_window_id(name: str) -> str:
    result = subprocess.run(["wmctrl", "-l"], capture_output=True, text=True)
    for line in result.stdout.splitlines():
        if name in line:
            return line.split()[0]
    print(f"Error: Could not find window '{name}'. Is the simulator running?")
    sys.exit(1)


def xwd_to_image(data: bytes) -> Image.Image:
    """Parse raw XWD bytes into a PIL Image without spawning ffmpeg."""
    header_size   = struct.unpack_from(">I", data,  0)[0]
    width         = struct.unpack_from(">I", data, 16)[0]
    height        = struct.unpack_from(">I", data, 20)[0]
    bits_per_pixel= struct.unpack_from(">I", data, 44)[0]
    bytes_per_line= struct.unpack_from(">I", data, 48)[0]
    red_mask      = struct.unpack_from(">I", data, 52)[0]
    green_mask    = struct.unpack_from(">I", data, 56)[0]
    blue_mask     = struct.unpack_from(">I", data, 60)[0]
    ncolors       = struct.unpack_from(">I", data, 76)[0]

    pixel_offset = header_size + ncolors * 12
    pixels = np.frombuffer(data, dtype=np.uint8, offset=pixel_offset)

    if bits_per_pixel == 32:
        pixels = pixels[: height * bytes_per_line].reshape(height, bytes_per_line)
        pixels = pixels[:, : width * 4].reshape(height, width, 4)

        def mask_byte(mask: int) -> int:
            return {0x000000FF: 0, 0x0000FF00: 1, 0x00FF0000: 2, 0xFF000000: 3}.get(mask, 0)

        r, g, b = mask_byte(red_mask), mask_byte(green_mask), mask_byte(blue_mask)
        return Image.fromarray(
            np.stack([pixels[:, :, r], pixels[:, :, g], pixels[:, :, b]], axis=2), "RGB"
        )

    raise RuntimeError(f"Unsupported XWD bits_per_pixel={bits_per_pixel}")


def capture_frame(win_id: str) -> Image.Image:
    result = subprocess.run(["xwd", "-id", win_id, "-silent"], capture_output=True)
    if result.returncode != 0:
        raise RuntimeError("xwd failed — is the window still open?")
    return xwd_to_image(result.stdout)


def scale_image(img: Image.Image, width: int) -> Image.Image:
    ratio = width / img.width
    return img.resize((width, int(img.height * ratio)), Image.LANCZOS)


def capture_gif(duration: float, fps: int, scale: int, output: Path):
    win_id = find_window_id(WINDOW_NAME)
    print(f"Found '{WINDOW_NAME}' (id={win_id})")

    interval = 1.0 / fps
    frames: list[Image.Image] = []
    durations: list[int] = []
    deadline = time.perf_counter() + duration

    print(f"Recording for {duration}s at target {fps}fps...")
    while time.perf_counter() < deadline:
        t0 = time.perf_counter()
        frames.append(scale_image(capture_frame(win_id), scale))
        elapsed_ms = (time.perf_counter() - t0) * 1000
        durations.append(max(int(elapsed_ms), 20))  # GIF minimum is 20ms
        wait = interval - elapsed_ms / 1000
        if wait > 0:
            time.sleep(wait)
            durations[-1] = int(1000 / fps)

    actual_fps = len(frames) / duration
    print(f"Captured {len(frames)} frames ({actual_fps:.1f}fps actual)")

    print(f"Saving {output} ...")
    frames[0].save(
        output,
        save_all=True,
        append_images=frames[1:],
        optimize=False,
        loop=0,
        duration=durations,
    )
    size_kb = output.stat().st_size // 1024
    print(f"Saved {output} ({size_kb} KB)")


def main():
    parser = argparse.ArgumentParser(description="Capture GravitySim window as GIF")
    parser.add_argument("--duration", type=float, default=5.0, help="Recording duration in seconds (default: 5)")
    parser.add_argument("--fps", type=int, default=30, help="Frames per second (default: 30)")
    parser.add_argument("--scale", type=int, default=800, help="Output width in pixels (default: 800)")
    parser.add_argument("--output", type=Path, default=OUTPUT_GIF, help=f"Output path (default: {OUTPUT_GIF})")
    args = parser.parse_args()

    args.output.parent.mkdir(parents=True, exist_ok=True)
    capture_gif(args.duration, args.fps, args.scale, args.output)


if __name__ == "__main__":
    main()
