<p align="center">
  <img src="docs/img/ratatron2.png" alt="Ratatron II: 3D render of the board with its wheels, motors, battery and suction fan" width="640">
</p>

<h1 align="center">Ratatron II</h1>

<p align="center">
  Micromouse robot with four-wheel drive and a suction fan.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/STM32F446-180%20MHz-03234B?logo=stmicroelectronics&logoColor=white" alt="STM32F446 at 180 MHz">
  <a href="hardware/"><img src="https://img.shields.io/badge/KiCad-9-314CB0?logo=kicad&logoColor=white" alt="KiCad 9"></a>
  <a href="mechanics/"><img src="https://img.shields.io/badge/FreeCAD-1.1-418FDE?logo=freecad&logoColor=white" alt="FreeCAD 1.1"></a>
  <a href="firmware/"><img src="https://img.shields.io/badge/PlatformIO-libopencm3-F5822A?logo=platformio&logoColor=white" alt="PlatformIO + libopencm3"></a>
  <a href="LICENSE"><img src="https://img.shields.io/badge/code-GPLv3-blue" alt="Code: GPLv3"></a>
  <a href="hardware/LICENSE"><img src="https://img.shields.io/badge/hardware-CC%20BY--SA%204.0-EF9421" alt="Hardware: CC BY-SA 4.0"></a>
</p>

> **Work in progress.** The board is being routed and the firmware is waiting for it. The picture is the KiCad 3D
> view, not a photo (yet).

## What's inside

| | |
|---|---|
| **MCU** | STM32F446RET6, Cortex-M4F at 180 MHz |
| **Wall sensors** | 4 IR pairs: SFH4550 emitters and ST-1KL3A phototransistors |
| **IMU** | LSM6DSRX (gyro and accelerometer) over SPI |
| **Drive** | 4 wheels, 2 motors, 2 DRV8874 H-bridges at 20 kHz with current sensing |
| **Encoders** | 2 AS5147P, magnetic, on little boards that snap off the main one |
| **Suction** | a fan driven by a coreless motor, over a hole in the middle of the board |
| **Battery** | 2S LiPo |
| **Interface** | 6 blue LEDs, 2 buttons and a power switch |
| **Connectors** | SWD through a magnetic connector, serial on a header |

## Where things are

- [firmware](firmware/): C on libopencm3, built with PlatformIO (`cd firmware && pio run`, flashed with an ST-Link)
- [hardware](hardware/): KiCad 9 schematic, PCB and custom library
- [mechanics](mechanics/): FreeCAD model (wheels, motor and fan supports)

## Family

- [Ratatron](https://github.com/JavierIH/ratatron), the first one: an STM32F103 that already finds its way out of
  mazes
- [Ratasim](https://github.com/JavierIH/ratasim), the simulator

## Licence

- Firmware: GPLv3 ([LICENSE](LICENSE))
- Electronics: CC BY-SA 4.0 ([hardware/LICENSE](hardware/LICENSE))
- Mechanics: CC BY-SA 4.0 ([mechanics/LICENSE](mechanics/LICENSE))
