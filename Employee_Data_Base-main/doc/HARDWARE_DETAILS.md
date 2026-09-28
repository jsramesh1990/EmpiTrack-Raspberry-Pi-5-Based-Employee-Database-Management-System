## 2. `docs/HARDWARE_DETAILS.md`

```markdown
# EmpiTrack – Hardware Details

## 1. Final Hardware Specification

| Component | Specification |
|---|---|
| Processor Board | Raspberry Pi 5 – 4 GB RAM |
| CPU | Broadcom BCM2712, quad-core Arm Cortex-A76 64-bit @ 2.4 GHz |
| GPU | VideoCore VII |
| RAM | 4 GB LPDDR4X-4267 SDRAM |
| Storage | 32 GB or larger microSD card, Class A2 recommended |
| Operating System | Raspberry Pi OS 64-bit / Bookworm |
| Power Supply | Official Raspberry Pi 27W USB-C PD power supply, 5V/5A |
| Cooling | Active cooler or heatsink + fan |
| Input | USB keyboard |
| Output | HDMI monitor |
| Optional Input | USB mouse |
| Network | Gigabit Ethernet, Wi-Fi 802.11ac, Bluetooth 5.0 |
| Database Storage | `employees.dat` on microSD |
| Export Format | `employees.csv` |

## 2. Raspberry Pi 5 – 4 GB Key Specifications

- Broadcom BCM2712 quad-core Arm Cortex-A76 64-bit CPU
- 2.4 GHz clock speed
- 4 GB LPDDR4X-4267 RAM
- VideoCore VII GPU
- Dual 4Kp60 HDMI output
- microSD card slot
- PCIe 2.0 x1 connector
- 2 × USB 3.0 ports
- 2 × USB 2.0 ports
- Gigabit Ethernet
- Wi-Fi 802.11ac
- Bluetooth 5.0
- 40-pin GPIO header
- USB-C power input
- Power button
- Real-time clock with battery connector

## 3. Required Peripherals

| Item | Recommended Specification |
|---|---|
| microSD Card | 32 GB, 64 GB, or 128 GB Class A2 |
| Power Supply | Official 27W USB-C PD |
| Keyboard | USB keyboard |
| Monitor | HDMI monitor or TV |
| HDMI Cable | micro-HDMI to HDMI, Raspberry Pi 5 compatible |
| Case | Official Raspberry Pi 5 case or compatible |
| Cooling | Active cooler recommended |

## 4. Optional Hardware

| Item | Purpose |
|---|---|
| USB Mouse | Easier menu navigation if GUI is used |
| NVMe SSD + PCIe HAT | Faster database storage |
| UPS / Power Bank | Prevent data loss during power failure |
| Buzzer / LED | Optional status indication via GPIO |
| RTC Battery | Keeps time when powered off |
| Ethernet Cable | Stable network connection |
| Serial Console Cable | Debug boot and kernel issues |

## 5. System Block Diagram

```text
                 +----------------------+
                 |      EmpiTrack       |
                 |  Employee Database   |
                 +----------+-----------+
                            |
                            v
                 +----------------------+
                 |   Raspberry Pi 5     |
                 |       4 GB RAM       |
                 +----------+-----------+
                            |
        +-------------------+-------------------+
        |                                       |
        v                                       v
+---------------+                       +---------------+
| USB Keyboard  |                       | HDMI Monitor  |
+---------------+                       +---------------+
        |                                       |
        +-------------------+-------------------+
                            |
                            v
                 +----------------------+
                 |   EmpiTrack C App    |
                 +----------+-----------+
                            |
        +-------------------+-------------------+
        |                   |                   |
        v                   v                   v
+---------------+   +---------------+   +---------------+
| Add Employee  |   | Search/Update |   | Delete/Report |
+---------------+   +---------------+   +---------------+
                            |
                            v
                 +----------------------+
                 |    employees.dat     |
                 +----------+-----------+
                            |
                            v
                 +----------------------+
                 |      microSD Card    |
                 +----------------------+

6. Power Requirements
Device	Approximate Power
Raspberry Pi 5 4GB	5V/5A via USB-C PD, 27W recommended
Keyboard	USB bus power
Monitor	External power
Active Cooler	Powered from Pi board
microSD	Powered from Pi board

Use the official 27W USB-C power supply for stable operation.
7. Thermal Management

Raspberry Pi 5 can run warm under load. Recommended:

    Official Active Cooler

    Case with ventilation

    Keep vents clear

    Avoid direct sunlight

    Monitor temperature:

bash

vcgencmd measure_temp

Normal idle range: around 40–55°C
Heavy load: keep below 80–85°C
8. Storage Layout

Recommended partition and file layout:
text

/boot/firmware/        Raspberry Pi boot files
/                      Root filesystem
/home/pi/EmpiTrack/    Project source and executable
/home/pi/EmpiTrack/data/employees.dat
/home/pi/EmpiTrack/data/employees.csv
/home/pi/EmpiTrack/data/backup/

9. GPIO Usage

For the basic EmpiTrack project, GPIO is not required.

Optional GPIO usage:
GPIO	Optional Use
GPIO 17	Status LED
GPIO 27	Buzzer
GPIO 22	Backup button
GND	Common ground

If GPIO is not used, leave the 40-pin header unconnected.
10. Assembly Steps

    Attach the active cooler to the Raspberry Pi 5.

    Insert the microSD card with Raspberry Pi OS.

    Connect HDMI monitor.

    Connect USB keyboard.

    Connect USB-C power supply.

    Power on and complete Raspberry Pi OS setup.

    Open Terminal.

    Install build tools and compile EmpiTrack.

    Run the application from /home/pi/EmpiTrack.

11. Bill of Materials – BOM
Qty	Component	Specification	Notes
1	Raspberry Pi 5	4 GB RAM	Main board
1	microSD Card	32 GB+ Class A2	OS + database
1	USB-C Power Supply	27W, 5V/5A	Official recommended
1	Active Cooler	Raspberry Pi 5 compatible	Thermal safety
1	HDMI Cable	micro-HDMI to HDMI	For monitor
1	Monitor	HDMI	Output
1	USB Keyboard	Standard	Input
1	Case	Raspberry Pi 5 compatible	Protection
1	RTC Battery	Optional	Time keeping
1	NVMe SSD + HAT	Optional	Faster storage
12. Safety Notes

    Do not power the Pi 5 from a low-quality phone charger.

    Do not remove the microSD card while the program is saving data.

    Shut down properly:

bash

sudo shutdown -h now

    Keep the board on a non-conductive surface.

    Avoid static discharge when handling the board.
