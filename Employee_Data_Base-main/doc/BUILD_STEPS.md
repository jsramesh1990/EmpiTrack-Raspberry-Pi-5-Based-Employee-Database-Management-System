docs/BUILD_STEPS.md
markdown

# EmpiTrack – Build Steps

## 1. Purpose
This document explains how to build, compile, run, and install the EmpiTrack Employee Database Management System on a Raspberry Pi 5 with 4 GB RAM.

## 2. Target Platform
- Board: Raspberry Pi 5 – 4 GB RAM
- OS: Raspberry Pi OS 64-bit / Bookworm
- Language: C
- Compiler: GCC
- Build tool: Make
- Database file: `employees.dat`
- Optional export: `employees.csv`

## 3. Recommended Project Structure

```text
EmpiTrack/
├── src/
│   ├── main.c
│   ├── employee.c
│   ├── employee.h
│   ├── database.c
│   ├── database.h
│   ├── file_ops.c
│   ├── file_ops.h
│   ├── menu.c
│   ├── menu.h
│   ├── utils.c
│   └── utils.h
├── data/
│   └── employees.dat
├── tests/
│   ├── test_employee.c
│   └── test_database.c
├── docs/
│   ├── BUILD_STEPS.md
│   ├── HARDWARE_DETAILS.md
│   └── TESTING_METHODS.md
├── Makefile
└── README.md

If you are using the single-file version from the existing repository, the structure can be:
text

EmpiTrack/
├── employee_system.c
├── employees.txt
├── Makefile
└── README.md

4. Install Prerequisites

Open Terminal on Raspberry Pi 5:
bash

sudo apt update
sudo apt full-upgrade -y
sudo apt install build-essential git make gdb valgrind -y

Check versions:
bash

gcc --version
make --version

5. Get the Source Code

Using Git:
bash

git clone https://github.com/yourusername/Employee_Data_Base.git
cd Employee_Data_Base

Or copy your project manually:
bash

cd ~
mkdir EmpiTrack
cd EmpiTrack
# copy your .c and .h files here

6. Build Option A – Single-File Version

If everything is inside employee_system.c:
bash

gcc -Wall -Wextra -pedantic -std=c17 -O2 -g employee_system.c -o empitrack

If you use math functions:
bash

gcc -Wall -Wextra -pedantic -std=c17 -O2 -g employee_system.c -o empitrack -lm

Run:
bash

./empitrack

7. Build Option B – Modular Version with Makefile

Create a Makefile in the project root:
make

.RECIPEPREFIX = >
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c17 -O2 -g
SRC = src/main.c src/employee.c src/database.c src/file_ops.c src/menu.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = empitrack
TEST_SRC = tests/test_employee.c tests/test_database.c
TEST_TARGET = test_empitrack

all: $(TARGET)

$(TARGET): $(OBJ)
> $(CC) $(CFLAGS) -o $@ $^

%.o: %.c
> $(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC) $(filter-out src/main.o,$(OBJ))
> $(CC) $(CFLAGS) -o $@ $^

run: all
> ./$(TARGET)

clean:
> rm -f $(OBJ) $(TARGET) $(TEST_TARGET)

Build:
bash

make clean
make

Run:
bash

make run

or:
bash

./empitrack

8. Debug Build

For GDB debugging:
bash

make clean
make CFLAGS="-Wall -Wextra -pedantic -std=c17 -O0 -g"
gdb ./empitrack

Common GDB commands:
gdb

break main
run
next
print variable_name
backtrace
quit

9. Release Build

For optimized release:
bash

make clean
make CFLAGS="-Wall -Wextra -pedantic -std=c17 -O3"

10. Database File Location

Recommended:
text

EmpiTrack/data/employees.dat

If your program expects a different path, either:

    change the path in the C source, or

    run the program from the directory containing employees.dat.

Example:
bash

cd ~/EmpiTrack
./empitrack

11. Optional Auto-Start on Boot

Create a systemd service:
bash

sudo nano /etc/systemd/system/empitrack.service

Add:
ini

[Unit]
Description=EmpiTrack Employee Database
After=multi-user.target

[Service]
Type=simple
User=pi
WorkingDirectory=/home/pi/EmpiTrack
ExecStart=/home/pi/EmpiTrack/empitrack
StandardInput=tty
TTYPath=/dev/tty1
StandardOutput=tty

[Install]
WantedBy=multi-user.target

Enable:
bash

sudo systemctl daemon-reload
sudo systemctl enable empitrack.service
sudo systemctl start empitrack.service

Check status:
bash

sudo systemctl status empitrack.service

12. Clean Rebuild
bash

make clean
make

Or single-file:
bash

rm -f empitrack
gcc -Wall -Wextra -pedantic -std=c17 -O2 -g employee_system.c -o empitrack

13. Common Build Problems
Problem	Solution
gcc: command not found	sudo apt install build-essential
make: command not found	sudo apt install make
undefined reference	Check all .c files are included in build
fatal error: employee.h: No such file	Use -Iinclude or keep headers in same folder
Permission denied when running	chmod +x empitrack
employees.dat not found	Run from correct folder or fix path
Many warnings with -Werror	First build with -Wall -Wextra only, fix warnings, then add -Werror
