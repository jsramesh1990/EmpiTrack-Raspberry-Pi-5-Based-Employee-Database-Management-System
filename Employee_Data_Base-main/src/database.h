#ifndef EMPITRACK_DATABASE_H
#define EMPITRACK_DATABASE_H

#include "employee.h"

#define DEFAULT_INITIAL_CAPACITY 10
#define DEFAULT_DB_FILENAME "data/employees.dat"

typedef struct {
    Employee *employees;
    int count;
    int capacity;
    char filename[256];
} EmployeeSystem;

int init_system(EmployeeSystem *system, int initial_capacity);
void cleanup_system(EmployeeSystem *system);
int ensure_capacity(EmployeeSystem *system);
int add_employee(EmployeeSystem *system, const Employee *emp);
Employee *find_employee(EmployeeSystem *system, int id);
int find_employee_index(const EmployeeSystem *system, int id);
int delete_employee(EmployeeSystem *system, int id);
int update_employee(EmployeeSystem *system, int id, const Employee *updated);
void list_employees(const EmployeeSystem *system);
int find_by_name(const EmployeeSystem *system, const char *partial, int *indices, int max);
int find_by_department(const EmployeeSystem *system, const char *partial, int *indices, int max);
void generate_statistics(const EmployeeSystem *system);
void sort_employees(EmployeeSystem *system, int criterion);
int next_employee_id(const EmployeeSystem *system);

#endif
