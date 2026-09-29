# ClassicIF for Adafruit Fruit Jam

ClassicIF is a public-source candidate for Glulx interactive fiction on Adafruit Fruit Jam (RP2350B). This version combines the Glulxe interpreter, CheapGlk, an SD story picker, and a text interface with USB or on-screen keyboard input. It targets the resident loader's application partition, not a standalone full-flash UF2.

The earlier 0.1.0 Glulxe/CheapGlk build passed basic playability and FPS checks on development hardware, as reported by the user. The Fruit Jam adaptation in this repository builds and passes a UF2 layout check, but has **not** been physically tested on Fruit Jam. No tested Fruit Jam binary release is claimed here. Although the original project plan mentioned Level9, Magnetic, and ScottFree, those interpreters are **not included** in this playable build.

## Stories and input

Supply your own lawfully obtained `.ulx`, `.gblorb`, `.blb`, or `.glb` Glulx story in `/roms/IF/` on the SD card. The picker opens even when there is only one story. No story or user save is bundled.

For on-screen input, D-pad moves the keyboard cursor, A adds a character, B deletes, X adds a space, Y enters `look`, and Start submits the line. USB keyboard input is also supported by the frontend. The Glk file API is mapped to SD through FatFs, but story-specific save/restore compatibility has not been fully validated. The embedded date adapter currently reports a fixed date (2026-01-01), not a real-time clock. Graphics, sound, and mouse behavior are outside this release candidate's verified scope.

## Build

Install CMake, Ninja, an ARM GCC toolchain, Pico SDK 2.3.0 or newer, and [Pico-PIO-USB](https://github.com/sekigon-gonnoc/Pico-PIO-USB). Then run:

```powershell
./scripts/build-release.ps1 -PicoSdkPath C:\path\to\pico-sdk -PicoPioUsbPath C:\path\to\Pico-PIO-USB -ToolchainPath C:\path\to\toolchain -NinjaPath C:\path\to\ninja.exe
```

Optional `-PioasmDir`, `-PicotoolDir`, and `-TinyUsbPath` parameters select prebuilt host tools or a TinyUSB checkout. The output is `build/fruit-jam-release/picoClassicIF.uf2`. The build targets Fruit Jam, its 8 MiB PSRAM, and DVI video without HDMI audio. Actual Fruit Jam video, USB input, SD access, and clock stability still require hardware validation.

The UF2 starts at `0x10080000`, after the resident loader. Do **not** flash it as a standalone bootloader image. Verify the generated file with:

```powershell
python ./scripts/validate-uf2-layout.py ./build/fruit-jam-release/picoClassicIF.uf2 --app-base 0x10080000 --metadata 0x10FFFF00
```

## Source and license

The interpreter is [erkyrath/glulxe](https://github.com/erkyrath/glulxe) commit `56ab8743bab565de307bd892c555d8d8897ed517` (MIT), paired with [erkyrath/cheapglk](https://github.com/erkyrath/cheapglk) commit `14d8aaf6e4150669762bd4646a5368e75c1eeee6` (MIT). Both upstream trees are copied without source modifications. The project-owned `firmware/` and `platform/` files provide the embedded date adapter, Glk-to-FatFs/stdio bridge, story picker, text console, PSRAM arena, and Fruit Jam video/input/SD runtime. [THIRD_PARTY.md](THIRD_PARTY.md) records the component revisions and licenses. The combined source is distributed under GPLv3; upstream notices remain in their respective directories. Building or running the project does not grant rights to distribute game stories.
