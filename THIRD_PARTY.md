# Third-party code and provenance

- [erkyrath/glulxe](https://github.com/erkyrath/glulxe), commit `56ab8743bab565de307bd892c555d8d8897ed517`: selected upstream C sources and headers in `deps/glulxe/`, unchanged. MIT license retained in `deps/glulxe/LICENSE`.
- [erkyrath/cheapglk](https://github.com/erkyrath/cheapglk), commit `14d8aaf6e4150669762bd4646a5368e75c1eeee6`: selected upstream C sources and headers in `deps/cheapglk/`, unchanged. MIT license retained in `deps/cheapglk/LICENSE`. `firmware/cgdate_embedded.c` replaces the desktop clock implementation for this embedded build.
- [PicoPlus-devel/pico_shared](https://github.com/PicoPlus-devel/pico_shared), base commit `df683bf00ff95e1ceda88f132bbf72f8cc7cadb0`: minimal public source subset in `deps/pico_shared/`, including the HSTX video driver. Its GPLv3 license is retained in `deps/pico_shared/LICENSE`; pico_fatfs retains its BSD-style `deps/pico_shared/drivers/pico_fatfs/LICENSE` and FatFs file notices.
- The Fruit Jam `platform/wave1/` runtime, `platform/text/` console, loader-partition helper, and `firmware/playable_platform.cpp` are adapted from the jack-green-ports development workspace and related public [PokemonMini](https://github.com/jackyangantelope/PokemonMini) release. Their shared BSD-3-Clause notice is retained in `LICENSES/jack-green-ports-BSD-3-Clause.txt`. The combined firmware is distributed under GPLv3.
- Pico SDK and Pico-PIO-USB are external build dependencies and are not included in this repository. Follow their upstream notices and licenses.

This repository includes no interactive-fiction stories, user save files, or generated UF2 binaries.
