# EmpiTrack – Raspberry Pi 5 Based Employee Database Management System

A console-based Employee Database Management System written in **pure C**, designed to run on the **Raspberry Pi 5 (4 GB RAM)**. EmpiTrack demonstrates professional C programming practices — dynamic memory management, file handling, function pointers, modular design, and comprehensive input validation — wrapped in a clean interactive menu.

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-Raspberry%20Pi%205-red.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Build](https://img.shields.io/badge/build-GCC%20%7C%20Make-success.svg)

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Build](#build)
- [Usage](#usage)
- [Menu Reference](#menu-reference)
- [Data Storage](#data-storage)
- [Architecture](#architecture)
- [Testing](#testing)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

**EmpiTrack** is a lightweight, terminal-driven Employee Database Management System built for the Raspberry Pi 5. It provides a full **CRUD** interface (Create, Read, Update, Delete) for managing employee records, with persistent storage in a portable, human-readable database file and CSV export for external use.

The project is intended for:

- Educational demonstration of advanced C concepts
- Embedded / edge deployment on Raspberry Pi hardware
- Small-scale HR record keeping on a standalone device
- Learning structured, modular C project design

---

## Features

### Core Operations
- **Add** new employees with full details (ID, name, department, salary, level, status, email, phone)
- **Display** all employees in a formatted table
- **Search** by ID (exact), name (partial), or department (partial)
- **Update** employee records in-place
- **Delete** employees with confirmation prompt
- **Save / Load** data to/from a text database file
- **Export** to CSV for use in Excel, LibreOffice, or Python
- **Sort** by ID, name, salary, or department
- **Statistics** — counts, averages, min/max salary, per-level totals

### Engineering Highlights
- **Dynamic memory** with `malloc`, `calloc`, `realloc`, and `free` — no fixed upper limit on employee count
- **Modular design** — separation across `employee`, `database`, `file_ops`, `menu`, `utils`
- **Input validation** for email, phone, salary, and menu choices
- **Auto-save on exit** and **backup creation** before destructive operations
- **Portable text format** (`|`-delimited) with escape-safe fields
- **Clean shutdown** — all heap memory is released (Valgrind-clean)

---

## Hardware Requirements

| Component | Specification |
|---|---|
| Board | **Raspberry Pi 5 – 4 GB RAM** |
| CPU | Broadcom BCM2712, quad-core Arm Cortex-A76 @ 2.4 GHz |
| RAM | 4 GB LPDDR4X-4267 |
| Storage | 32 GB+ microSD (Class A2) or NVMe SSD |
| OS | Raspberry Pi OS 64-bit (Bookworm) |
| Power | Official 27W USB-C PD supply (5V/5A) |
| Cooling | Active Cooler recommended |
| Input | USB keyboard |
| Output | HDMI monitor |

Full hardware details: [`docs/HARDWARE_DETAILS.md`](docs/HARDWARE_DETAILS.md)

---

## Project Structure

```text
EmpiTrack/
├── Makefile
├── README.md
├── LICENSE
├── src/
│   ├── main.c            # Entry point, CLI argument parsing
│   ├── employee.h/.c     # Employee struct, enums, input & display
│   ├── database.h/.c     # Dynamic array, CRUD, search, statistics
│   ├── file_ops.h/.c     # Save / Load / CSV export / backup
│   ├── menu.h/.c         # Interactive menu loop
│   └── utils.h/.c        # Validation, I/O helpers, string utilities
├── tests/
│   └── test_employee.c   # Unit tests
├── data/
│   └── employees.dat     # Runtime database file (created on first save)
└── docs/
    ├── BUILD_STEPS.md
    ├── HARDWARE_DETAILS.md
    └── TESTING_METHODS.md

Installation
1. Update the system
bash

sudo apt update
sudo apt full-upgrade -y

2. Install build tools
bash

sudo apt install build-essential git make gdb valgrind -y

3. Clone the repository
bash

git clone https://github.com/yourusername/EmpiTrack.git
cd EmpiTrack

Build
Standard build
bash

make

Produces the executable empitrack in the project root.
Debug build
bash

make clean
make CFLAGS="-Wall -Wextra -pedantic -std=c17 -O0 -g"

Release build
bash

make clean
make CFLAGS="-Wall -Wextra -pedantic -std=c17 -O3"

Clean
bash

make clean

Full build guide: docs/BUILD_STEPS.md
Usage

Run from the project root:
bash

./empitrack

Command-line options
Option	Description
--file <path>	Load a specific database file
--capacity <n>	Set the initial employee array capacity
--silent	Suppress the welcome banner
--help	Show usage information

Examples:
bash

./empitrack --file data/employees.dat
./empitrack --capacity 100
./empitrack --silent

---

Menu Reference
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
1	Add a new employee (auto-generated ID)
2	List all employees in a formatted table
3	Search by ID, name, or department
4	Update an existing employee (blank = keep current)
5	Delete an employee (with confirmation)
6	Save database to a file
7	Load database from a file
8	Export records to CSV
9	Sort records by ID, name, salary, or department
10	Display system-wide statistics
0	Exit (auto-saves and frees memory)
Data Storage
Text Database Format

employees.dat is a pipe-delimited text file:
text

# EmpiTrack Database v1.0
# ID|NAME|DEPT|SALARY|LEVEL|STATUS|EMAIL|PHONE|JOIN_DATE
1|John Doe|Engineering|50000.00|Senior|Active|john@company.com|+91-9876543210|1717000000
2|Alice Smith|HR|45000.00|Mid-Level|On Leave|alice@company.com|+91-9123456780|1717100000

CSV Export Format

employees.csv is a standard comma-separated file:
csv

ID,Name,Department,Salary,Level,Status,Email,Phone,JoinDate
1,John Doe,Engineering,50000.00,Senior,Active,john@company.com,+91-9876543210,2026-01-15 09:30:00

Backup

Before delete and add operations, a employees.dat.bak snapshot is created automatically.
Architecture
text

                    +---------------------------+
                    |        EmpiTrack          |
                    |   Employee Database App   |
                    +-------------+-------------+
                                  |
                    +-------------+-------------+
                    |                           |
             +------+------+             +------+------+
             |  Database   |             |  File I/O   |
             |  (in-RAM)   |             |  (disk)     |
             +------+------+             +------+------+
                    |                           |
                    +-------------+-------------+
                                  |
                    +-------------+-------------+
                    |       Menu / UI Layer     |
                    +---------------------------+
                                  |
                    +-------------+-------------+
                    |    Raspberry Pi 5 OS      |
                    +---------------------------+

Module Responsibilities
Module	Responsibility
main.c	Program entry, CLI parsing, load/save orchestration
employee.c	Employee struct I/O, enums, formatted printing
database.c	Dynamic array, CRUD, search, sort, statistics
file_ops.c	Persistence, CSV export, backup
menu.c	Interactive console menu
utils.c	Input helpers, validation, string utilities
Testing

EmpiTrack includes a unit-test target and documented manual test cases.
Run unit tests
bash

make test
./test_empitrack

Memory check
bash

valgrind --leak-check=full --show-leak-kinds=all ./empitrack

Expected output ends with:
text

All heap blocks were freed -- no leaks are possible

Static analysis
bash

cppcheck --enable=all --inconclusive src/

Full test plan: docs/TESTING_METHODS.md
Documentation
Document	Purpose
docs/BUILD_STEPS.md	Complete build & installation guide
docs/HARDWARE_DETAILS.md	Raspberry Pi 5 hardware specification & BOM
docs/TESTING_METHODS.md	Unit, integration, and hardware tests
Contributing

---

Contributions are welcome.

    Fork the repository

    Create a feature branch
    bash

    git checkout -b feature/YourFeature

    Commit your changes
    bash

    git commit -m 'Add YourFeature'

    Push the branch
    bash

    git push origin feature/YourFeature

    Open a Pull Request

Code Style

    Follow the existing formatting and naming conventions

    Add function-level comments for new code

    Validate all user input

    Handle errors explicitly (-1 return, stderr messages)

    Test changes locally before opening a PR

---

    Raspberry Pi Foundation — hardware and OS platform

    GCC and GNU Make — build toolchain

    The C Programming Language (Kernighan & Ritchie) — design inspiration

EmpiTrack – Raspberry Pi 5 Based Employee Database Management System
Built with C, for the edge.
