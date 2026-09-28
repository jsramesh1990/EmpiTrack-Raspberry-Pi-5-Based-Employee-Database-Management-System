docs/TESTING_METHODS.md`

```markdown
# EmpiTrack – Testing Methods

## 1. Test Objectives

Verify that EmpiTrack:
- Compiles without errors on Raspberry Pi 5.
- Adds, displays, searches, updates, and deletes employees correctly.
- Saves and loads data reliably.
- Handles invalid input safely.
- Uses dynamic memory without leaks.
- Exports CSV correctly.
- Runs stably on Raspberry Pi OS.

## 2. Test Environment

| Item | Details |
|---|---|
| Board | Raspberry Pi 5 – 4 GB RAM |
| OS | Raspberry Pi OS 64-bit |
| Compiler | GCC |
| Build Tool | Make |
| Memory Checker | Valgrind |
| Debugger | GDB |
| Database File | `employees.dat` |
| Export File | `employees.csv` |

## 3. Build Verification

```bash
make clean
make

Expected result:

    No compiler errors.

    Executable empitrack created.

Check:
bash

ls -lh empitrack
file empitrack

4. Unit Testing

Create tests/test_employee.c:
c

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "employee.h"

void test_add_employee(void) {
    EmployeeSystem system;
    init_system(&system, 5);

    Employee emp;
    memset(&emp, 0, sizeof(emp));
    emp.id = 1;
    strcpy(emp.name, "John Doe");
    strcpy(emp.department, "Engineering");
    emp.salary = 50000.0f;
    emp.level = SENIOR;
    emp.status = ACTIVE;

    int result = add_employee(&system, &emp);
    assert(result == 1);
    assert(system.count == 1);

    cleanup_system(&system);
    printf("PASS: test_add_employee\n");
}

void test_find_employee(void) {
    EmployeeSystem system;
    init_system(&system, 5);

    Employee emp;
    memset(&emp, 0, sizeof(emp));
    emp.id = 10;
    strcpy(emp.name, "Alice");
    emp.salary = 60000.0f;

    add_employee(&system, &emp);

    Employee *found = find_employee(&system, 10);
    assert(found != NULL);
    assert(found->id == 10);
    assert(strcmp(found->name, "Alice") == 0);

    cleanup_system(&system);
    printf("PASS: test_find_employee\n");
}

void test_delete_employee(void) {
    EmployeeSystem system;
    init_system(&system, 5);

    Employee emp;
    memset(&emp, 0, sizeof(emp));
    emp.id = 20;
    strcpy(emp.name, "Bob");

    add_employee(&system, &emp);
    assert(system.count == 1);

    int result = delete_employee(&system, 20);
    assert(result == 0);
    assert(system.count == 0);

    cleanup_system(&system);
    printf("PASS: test_delete_employee\n");
}

int main(void) {
    test_add_employee();
    test_find_employee();
    test_delete_employee();
    printf("All unit tests passed.\n");
    return 0;
}

Compile and run:
bash

make test
./test_empitrack

5. Integration Testing

Integration tests check menu flow, file operations, and database persistence.

Example command-line test:
bash

printf '1\nJohn Doe\nEngineering\n50000\n2\n1\njohn@company.com\n1234567890\n0\n' | ./empitrack

Expected:

    Employee added.

    Program exits cleanly.

    employees.dat updated if auto-save is enabled.

Test save/load:
bash

printf '6\nemployees.dat\n7\nemployees.dat\n0\n' | ./empitrack

Expected:

    Data saved.

    Data loaded.

    No crash.

Test CSV export:
bash

printf '8\nemployees.csv\n0\n' | ./empitrack
head -n 5 employees.csv

Expected:

    CSV file created.

    Header row and employee rows present.

    Comma-separated fields.

6. Manual Test Cases
Test ID	Test Case	Input	Expected Result
TC-01	Add valid employee	Valid name, dept, salary	Employee added
TC-02	Add invalid salary	Negative or text	Rejected with error
TC-03	Display all employees	Menu option 2	Formatted table shown
TC-04	Search by exact ID	Existing ID	Employee found
TC-05	Search by invalid ID	Non-existing ID	Not found message
TC-06	Search by partial name	Part of name	Matching employees shown
TC-07	Update employee	Existing ID + new salary	Record updated
TC-08	Delete employee	Existing ID	Record removed
TC-09	Delete invalid ID	Non-existing ID	Error message
TC-10	Save data	Menu option 6	File created/updated
TC-11	Load data	Menu option 7	Records restored
TC-12	Export CSV	Menu option 8	employees.csv created
TC-13	Empty database	No records	Display shows 0 employees
TC-14	Duplicate ID	Same ID twice	Rejected or auto-generated
TC-15	Max capacity	Add beyond initial capacity	Realloc works
TC-16	Invalid menu choice	99	Error and menu redisplayed
TC-17	Exit program	0	Clean exit, data saved
TC-18	Long name input	500 characters	No buffer overflow
TC-19	Corrupt file	Bad employees.dat	Error, no crash
TC-20	Read-only file	chmod -w employees.dat	Save fails safely
7. File and Database Testing

Check file existence:
bash

ls -lh employees.dat
file employees.dat

Backup test:
bash

cp employees.dat employees_backup.dat
./empitrack
# delete or modify records
cp employees_backup.dat employees.dat
./empitrack

Expected:

    Restored data loads correctly.

CSV validation:
bash

awk -F, 'NR==1 {print "Columns:", NF}' employees.csv
wc -l employees.csv

8. Edge Case Testing

Test the following:

    Empty database

    Missing employees.dat

    Corrupt binary file

    Very long strings

    Negative salary

    Zero salary

    Duplicate employee ID

    Delete from empty database

    Search in empty database

    Save to read-only location

    Load from wrong path

    Add more than 1000 employees

    Sudden power loss during save

9. Memory and Static Analysis

Valgrind:
bash

sudo apt install valgrind -y
valgrind --leak-check=full --show-leak-kinds=all ./empitrack

Expected:
text

All heap blocks were freed -- no leaks are possible

AddressSanitizer:
bash

gcc -fsanitize=address,undefined -g -Wall -Wextra -pedantic -std=c17 src/*.c -o empitrack_asan
./empitrack_asan

Static analysis:
bash

sudo apt install cppcheck -y
cppcheck --enable=all --inconclusive src/

10. Performance Testing

Test with 1,000 / 10,000 records.

Measure load time:
bash

time ./empitrack

Measure save time from inside the program or using shell:
bash

/usr/bin/time -v ./empitrack

Acceptance targets:

    1,000 records load in under 1 second.

    10,000 records load in under 3 seconds.

    Search by ID returns immediately.

    Save completes without memory errors.

11. Hardware Testing on Raspberry Pi 5
Test	Method	Expected
Boot test	Power on Pi 5	OS boots
Keyboard test	Type in terminal	Input works
Monitor test	HDMI output	Display works
Power test	Use 27W supply	No undervoltage warning
Thermal test	vcgencmd measure_temp	Below 85°C
SD card test	Save/load database	Data persists
Reboot test	Reboot Pi	Data still available
Power loss test	Disconnect power during save	Backup/recovery works
Long run test	Run for 1 hour	No crash

Temperature check:
bash

vcgencmd measure_temp

Undervoltage check:
bash

vcgencmd get_throttled

Expected: throttled=0x0
12. Acceptance Criteria

The project is accepted when:

    It compiles with GCC on Raspberry Pi 5.

    All CRUD operations work.

    File save/load works.

    CSV export works.

    No memory leaks under Valgrind.

    No crashes on invalid input.

    Data persists after reboot.

    Temperature remains safe.

    Power supply is stable.

    All manual test cases pass.
