# SRAM / eMMC build change summary

## Background

The original `debug` / `release` builds used a RAM linker script name, but the ELF still had flash-backed load addresses:

- `.interrupts` / `.text` ran from SRAM addresses.
- Their LMA was mapped to XSPI flash at `0x28000000`.
- `.data` VMA was `0x20480000`, but LMA was also in the `0x280...` flash image.

This is not suitable for a board that has no external flash and only internal SRAM. The startup `.data` copy loop tried to read initialized data from a location that was not actually valid for this board.

## Added build types

Two new build types were added to `rt700_dual_core_low_power_thermometer/armgcc`:

- `sram_debug`
- `emmc_debug`

They are registered in:

- `rt700_dual_core_low_power_thermometer/armgcc/CMakePresets.json`
- `rt700_dual_core_low_power_thermometer/armgcc/CMakeLists.txt`
- `rt700_dual_core_low_power_thermometer/armgcc/flags.cmake`

## `sram_debug`

Purpose:

- J-Link / GDB direct SRAM debug.
- No flash load address.
- ELF sections are loaded directly to their runtime SRAM addresses.

Main linker script:

- `rt700_dual_core_low_power_thermometer/armgcc/examples/_boards/mimxrt700evk/lvgl_examples/linker/MIMXRT798Sxxxx_cm33_core0_sram.ld`

Key behavior:

- Removed `m_flash`.
- Removed `AT > m_flash` and flash LMA usage.
- Discarded `.flash_conf`.
- `.data` has LMA equal to VMA, so GDB loads it directly to RAM.
- `objcopy -O binary` is skipped for `sram_debug` to avoid a huge sparse binary.

Verified section layout:

```text
.interrupts  VMA 0x00060000  LMA 0x00060000
.text        VMA 0x00060300  LMA 0x00060300
.data        VMA 0x20480000  LMA 0x20480000
```

Startup change:

- `startup_MIMXRT798S_cm33_core0.S` now supports `__STARTUP_SKIP_DATA_COPY`.
- `sram_debug` defines `__STARTUP_SKIP_DATA_COPY`.
- This prevents startup from overwriting GDB-loaded `.data`.
- BSS clear remains enabled.

VS Code debug:

- `rt700_dual_core_low_power_thermometer/.vscode/launch.json`
- `executable` now points to:

```text
${workspaceFolder}/armgcc/sram_debug/lvgl_guider_cm33_core0.elf
```

Unsupported `skipPreFlashActions` was removed from this launch file.

## `emmc_debug`

Purpose:

- eMMC payload-style image.
- Not XIP.
- Image is expected to be loaded into SRAM starting at the app image base.
- Startup still copies initialized `.data` from the image area into its runtime RAM address.

Main linker script:

- `rt700_dual_core_low_power_thermometer/armgcc/examples/_boards/mimxrt700evk/lvgl_examples/linker/MIMXRT798Sxxxx_cm33_core0_emmc.ld`

Key behavior:

- Removed XSPI flash address usage.
- Discarded `.flash_conf`.
- Disabled XSPI boot header in `emmc_debug` with `BOOT_HEADER_ENABLE=0`.
- `.interrupts` and `.text` are placed at SRAM runtime addresses.
- `.data` VMA remains in normal data RAM, but its LMA is placed immediately after `.text` in the image.
- Startup `.data` copy remains enabled.

Verified section layout:

```text
.interrupts  VMA 0x00060000  LMA 0x00060000
.text        VMA 0x00060300  LMA 0x00060300
.data        VMA 0x20480000  LMA 0x000e8db8
```

Generated outputs:

```text
rt700_dual_core_low_power_thermometer/armgcc/emmc_debug/lvgl_guider_cm33_core0.elf
rt700_dual_core_low_power_thermometer/armgcc/emmc_debug/lvgl_guider_cm33_core0.bin
```

Observed `emmc_debug` binary size:

```text
974172 bytes
```

## Build commands

Configure and build SRAM debug:

```powershell
C:\Users\82102\.mcuxpressotools\cmake-3.30.0-windows-x86_64\bin\cmake.EXE --preset sram_debug
C:\Users\82102\.mcuxpressotools\cmake-3.30.0-windows-x86_64\bin\cmake.EXE --build .
```

Configure and build eMMC debug:

```powershell
C:\Users\82102\.mcuxpressotools\cmake-3.30.0-windows-x86_64\bin\cmake.EXE --preset emmc_debug
C:\Users\82102\.mcuxpressotools\cmake-3.30.0-windows-x86_64\bin\cmake.EXE --build .
```

Inspect ELF section layout:

```powershell
C:\NXP\MCUXpressoIDE_25.6.136\ide\tools\bin\arm-none-eabi-objdump.exe -h sram_debug\lvgl_guider_cm33_core0.elf
C:\NXP\MCUXpressoIDE_25.6.136\ide\tools\bin\arm-none-eabi-objdump.exe -h emmc_debug\lvgl_guider_cm33_core0.elf
```

## Notes

- `sram_debug` is for direct debugger loading into SRAM.
- `emmc_debug` is for a non-XIP SRAM-loaded image payload.
- A final ROM-bootable eMMC image may still require an additional NXP/SPSDK boot container or header wrapping step, depending on the RT700 boot flow used on the target board.
- The old `debug` / `release` configurations were not intentionally converted; they still represent the original flash-backed flow.
