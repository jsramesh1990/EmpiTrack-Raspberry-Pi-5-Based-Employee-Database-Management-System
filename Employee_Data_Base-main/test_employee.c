#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "employee.h"
#include "database.h"
#include "utils.h"

static void test_add_employee(void) {
    EmployeeSystem system;
    init_system(&system, 5);
    Employee emp;
    memset(&emp, 0, sizeof(emp));
    emp.id = 1;
    snprintf(emp.name, sizeof(emp.name), "John Doe");
    snprintf(emp.department, sizeof(emp.department), "Engineering");
    emp.salary = 50000.0f;
    emp.level = LEVEL_SENIOR;
    emp.status = STATUS_ACTIVE;
    int result = add_employee(&system, &emp);
    assert(result == 1);
    assert(system.count == 1);
    cleanup_system(&system);
    printf("PASS: test_add_employee\n");
}

static void test_find_employee(void) {
    EmployeeSystem system;
    init_system(&system, 5);
    Employee emp;
    memset(&emp, 0, sizeof(emp));
    emp.id = 10;
    snprintf(emp.name, sizeof(emp.name), "Alice");
    add_employee(&system, &emp);
    Employee *found = find_employee(&system, 10);
    assert(found != NULL);
    assert(found->id == 10);
    assert(strcmp(found->name, "Alice") == 0);
    cleanup_system(&system);
    printf("PASS: test_find_employee\n");
}

static void test_delete_employee(void) {
    EmployeeSystem system;
    init_system(&system, 5);
    Employee emp;
    memset(&emp, 0, sizeof(emp));
    emp.id = 20;
    snprintf(emp.name, sizeof(emp.name), "Bob");
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
    printf("All tests passed.\n");
    return 0;
}
