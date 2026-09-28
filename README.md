### Raspberry Pi 5 Based Employee Database Management System

A modular, console-based **Employee Database Management System** written in **pure C** and deployed on the **Raspberry Pi 5 (4 GB RAM)**. EmpiTrack demonstrates professional C programming — dynamic memory, file handling, structured menus, input validation, and clean shutdown — in a compact, real-world application.

---

## Table of Contents

1. [Project Information](#1-project-information)
2. [Introduction](#2-introduction)
3. [Objectives](#3-objectives)
4. [Features](#4-features)
5. [Hardware Requirements](#5-hardware-requirements)
6. [Software Requirements](#6-software-requirements)
7. [System Architecture](#7-system-architecture)
8. [Project Structure](#8-project-structure)
9. [Installation](#9-installation)
10. [Build Instructions](#10-build-instructions)
11. [Running the Program](#11-running-the-program)
12. [Menu Reference](#12-menu-reference)
13. [Data Storage Format](#13-data-storage-format)
14. [Testing](#14-testing)
15. [Documentation](#15-documentation)
16. [Troubleshooting](#16-troubleshooting)
17. [Future Enhancements](#17-future-enhancements)
18. [License](#18-license)
19. [Author](#19-author)

---

## 1. Project Information

| Field | Details |
|---|---|
| **Project Title** | EmpiTrack – Raspberry Pi 5 Based Employee Database Management System |
| **Language** | C (C17 standard) |
| **Platform** | Raspberry Pi 5 – 4 GB RAM |
| **Operating System** | Raspberry Pi OS 64-bit (Bookworm) |
| **Compiler** | GCC |
| **Build Tool** | GNU Make |
| **Interface** | Console (text menu) |
| **Storage** | Text database file (`employees.dat`) + CSV export |
| **License** | MIT |

---

## 2. Introduction

EmpiTrack is a compact yet complete employee record management system built to run on the Raspberry Pi 5. It provides a full **CRUD** interface — Create, Read, Update, Delete — for employee data, with persistent storage on disk and CSV export for use with spreadsheets or data analysis tools.

The project is intended as:

- A practical, portable HR utility for standalone devices
- A teaching reference for structured C programming on embedded Linux
- A foundation for extending into networked or GUI-based variants

---

## 3. Objectives

- Provide a clean, menu-driven console application for managing employee records
- Demonstrate modular C project design with separated headers and sources
- Apply dynamic memory management for unbounded record counts
- Persist data reliably to disk with safe, human-readable formats
- Validate all user input and handle errors gracefully
- Run efficiently on low-power ARM hardware (Raspberry Pi 5)

---

## 4. Features

### 4.1 Core Features
- Add new employees (auto-generated ID)
- Display all employees in a formatted table
- Search by **ID**, **name** (partial), or **department** (partial)
- Update existing employee records
- Delete employees with confirmation prompt
- Save / load the database to/from a text file
- Export records to **CSV**
- Sort by ID, name, salary, or department
- Generate system statistics (counts, averages, salary range)

### 4.2 Technical Features
- Dynamic array growth via `realloc`
- Clean heap cleanup on exit (Valgrind-verified)
- Modular source layout (six modules)
- Input validation (email, phone, salary, menu choices)
- Automatic backup before destructive operations
- Auto-save on program exit
- Portable text format with escaped fields

---

## 5. Hardware Requirements

| Component | Specification |
|---|---|
| Board | Raspberry Pi 5 – 4 GB RAM |
| CPU | Broadcom BCM2712, quad-core Arm Cortex-A76 @ 2.4 GHz |
| RAM | 4 GB LPDDR4X-4267 |
| Storage | 32 GB+ microSD (Class A2) or NVMe SSD |
| Power Supply | Official 27W USB-C PD (5V/5A) |
| Cooling | Active Cooler recommended |
| Keyboard | USB keyboard |
| Display | HDMI monitor (micro-HDMI to HDMI cable) |
| Network | Not required (offline-capable) |

Full hardware breakdown: [`docs/HARDWARE_DETAILS.md`](docs/HARDWARE_DETAILS.md)

---

## 6. Software Requirements

| Software | Version / Notes |
|---|---|
| Raspberry Pi OS | 64-bit Bookworm or later |
| GCC | 10 or higher |
| GNU Make | 4.x |
| Bash | Default shell |
| Valgrind | Optional, for memory testing |
| Git | Optional, for cloning |

---

## 7. System Architecture

```text
                +-----------------------------------+
                |            EMPITRACK              |
                |  Employee Database Management     |
                +----------------+------------------+
                                 |
                                 v
                +-----------------------------------+
                |         Raspberry Pi 5            |
                |            4 GB RAM               |
                +----------------+------------------+
                                 |
                +----------------+------------------+
                |                                   |
                v                                   v
        +---------------+                   +---------------+
        | USB Keyboard  |                   | HDMI Monitor  |
        +-------+-------+                   +-------+-------+
                |                                   |
                +----------------+------------------+
                                 |
                                 v
                +-----------------------------------+
                |     EmpiTrack C Application       |
                |  (Menu + CRUD + File Handling)    |
                +----------------+------------------+
                                 |
                +----------------+------------------+
                |                |                  |
                v                v                  v
        +-------------+  +-------------+   +-------------+
        | Add / Edit  |  | Search /    |   | Delete /    |
        | Employee    |  | Sort        |   | Statistics  |
        +------+------+  +------+------+   +------+------+
               |                |                 |
               +----------------+-----------------+
                                 |
                                 v
                +-----------------------------------+
                |         employees.dat             |
                |      (Text Database File)         |
                +----------------+------------------+
                                 |
                                 v
                +-----------------------------------+
                |         microSD Card              |
                +-----------------------------------+

Module Diagram
text

    main.c
      |
      +---> database  <---> file_ops
      |         ^              ^
      |         |              |
      +---> menu ---------------+
                |
                v
             employee  <--->  utils

8. Project Structure
text

EmpiTrack/
├── Makefile
├── README.md
├── LICENSE
├── .gitignore
├── src/
│   ├── main.c            # Entry point, CLI parsing
│   ├── employee.h
│   ├── employee.c        # Employee struct, enums, I/O
│   ├── database.h
│   ├── database.c        # Dynamic array, CRUD, search, stats
│   ├── file_ops.h
│   ├── file_ops.c        # Save / Load / CSV / Backup
│   ├── menu.h
│   ├── menu.c            # Interactive console menu
│   ├── utils.h
│   └── utils.c           # Validation, string helpers
├── tests/
│   └── test_employee.c   # Unit tests
├── data/
│   └── employees.dat     # Runtime database (created on first save)
└── docs/
    ├── BUILD_STEPS.md
    ├── HARDWARE_DETAILS.md
    └── TESTING_METHODS.md

9. Installation
Step 1 — Update the system
bash

sudo apt update
sudo apt full-upgrade -y

Step 2 — Install build tools
bash

sudo apt install build-essential git make gdb valgrind -y

Step 3 — Clone or copy the project
bash

git clone https://github.com/yourusername/EmpiTrack.git
cd EmpiTrack

Or copy the EmpiTrack folder manually into /home/pi/.
10. Build Instructions
Standard build
bash

make

Produces the executable empitrack.
Debug build
bash

make clean
make CFLAGS="-Wall -Wextra -pedantic -std=c17 -O0 -g"

Release build
bash

make clean
make CFLAGS="-Wall -Wextra -pedantic -std=c17 -O3"

Clean build artifacts
bash

make clean

Detailed build guide: docs/BUILD_STEPS.md
11. Running the Program
bash

./empitrack

Command-Line Options
Option	Description
--file <path>	Use a specific database file
--capacity <n>	Set initial employee array capacity
--silent	Suppress the welcome banner
--help	Show usage information

Examples
bash

./empitrack
./empitrack --file data/employees.dat
./empitrack --capacity 100
./empitrack --silent

12. Menu Reference
text

╔══════════════════════════════════════╗
║   EMPITRACK EMPLOYEE SYSTEM v1.0     ║
╠══════════════════════════════════════╣
║  1. Add New Employee                 ║
║  2. Display All Employees            ║
║  3. Search Employee                  ║
║  4. Update Employee Information      ║
║  5. Delete Employee                  ║
║  6. Save Data to File                ║
║  7. Load Data from File              ║
║  8. Export to CSV                    ║
║  9. Sort Employees                   ║
║ 10. System Statistics                ║
║  0. Exit                             ║
╚══════════════════════════════════════╝

Option	Action
1	Add new employee (auto-generated ID)
2	Display all employees
3	Search by ID / name / department
4	Update employee information
5	Delete employee (with confirmation)
6	Save database to file
7	Load database from file
8	Export records to CSV
9	Sort by ID, name, salary, or department
10	Display system statistics
0	Exit (auto-saves and frees memory)
13. Data Storage Format
13.1 Database File — employees.dat

Pipe-delimited text format:
text

# EmpiTrack Database v1.0
# ID|NAME|DEPT|SALARY|LEVEL|STATUS|EMAIL|PHONE|JOIN_DATE
1|John Doe|Engineering|50000.00|Senior|Active|john@company.com|+91-9876543210|1717000000
2|Alice Smith|HR|45000.00|Mid-Level|On Leave|alice@company.com|+91-9123456780|1717100000

13.2 CSV Export — employees.csv
csv

ID,Name,Department,Salary,Level,Status,Email,Phone,JoinDate
1,John Doe,Engineering,50000.00,Senior,Active,john@company.com,+91-9876543210,2026-01-15 09:30:00

13.3 Backup

Before add and delete operations, a snapshot is written to:
text

employees.dat.bak

14. Testing
14.1 Run Unit Tests
bash

make test
./test_empitrack

Expected:
text

PASS: test_add_employee
PASS: test_find_employee
PASS: test_delete_employee
All unit tests passed.

14.2 Memory Check
bash

valgrind --leak-check=full --show-leak-kinds=all ./empitrack

Expected:
text

All heap blocks were freed -- no leaks are possible

14.3 Static Analysis
bash

cppcheck --enable=all --inconclusive src/

Full test plan: docs/TESTING_METHODS.md
15. Documentation
Document	Description
docs/BUILD_STEPS.md	Build, install, and auto-start guide
docs/HARDWARE_DETAILS.md	Raspberry Pi 5 hardware spec and BOM
docs/TESTING_METHODS.md	Unit, integration, and hardware tests
16. Troubleshooting
Problem	Cause	Solution
gcc: command not found	Build tools missing	sudo apt install build-essential
make: command not found	Make missing	sudo apt install make
undefined reference	Missing source file	Verify all .c files included in Makefile
employees.dat not found	Wrong working directory	Run from project root or pass --file
Permission denied on run	Executable bit missing	chmod +x empitrack
High CPU temperature	Insufficient cooling	Install Active Cooler; check vcgencmd measure_temp
Undervoltage warning	Weak power supply	Use official 27W USB-C PD supply
Data not saving	Read-only path	Check file permissions with ls -l
17. Future Enhancements

    Graphical interface using GTK or Qt

    SQLite backend for scalable storage

    Multi-user support with authentication

    Network access via REST API

    Web dashboard on the Pi 5

    Payroll and leave management extensions

    Data encryption for sensitive records

    Automated PDF report generation


Built with C, for the edge.
text


---
