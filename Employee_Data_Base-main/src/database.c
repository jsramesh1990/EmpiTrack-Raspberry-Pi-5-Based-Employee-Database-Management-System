#include "database.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_system(EmployeeSystem *system, int initial_capacity) {
    if (!system) return -1;
    if (initial_capacity <= 0) initial_capacity = DEFAULT_INITIAL_CAPACITY;
    system->employees = (Employee*)calloc((size_t)initial_capacity, sizeof(Employee));
    if (!system->employees) {
        fprintf(stderr, "Error: memory allocation failed\n");
        return -1;
    }
    system->count = 0;
    system->capacity = initial_capacity;
    snprintf(system->filename, sizeof(system->filename), "%s", DEFAULT_DB_FILENAME);
    return 0;
}

void cleanup_system(EmployeeSystem *system) {
    if (!system) return;
    if (system->employees) {
        free(system->employees);
        system->employees = NULL;
    }
    system->count = 0;
    system->capacity = 0;
}

int ensure_capacity(EmployeeSystem *system) {
    if (!system) return -1;
    if (system->count < system->capacity) return 0;
    int new_capacity = system->capacity * 2;
    if (new_capacity <= 0) new_capacity = DEFAULT_INITIAL_CAPACITY;
    Employee *tmp = (Employee*)realloc(system->employees, (size_t)new_capacity * sizeof(Employee));
    if (!tmp) {
        fprintf(stderr, "Error: memory reallocation failed\n");
        return -1;
    }
    system->employees = tmp;
    system->capacity = new_capacity;
    return 0;
}

int next_employee_id(const EmployeeSystem *system) {
    if (!system) return 1;
    int max_id = 0;
    for (int i = 0; i < system->count; ++i) {
        if (system->employees[i].id > max_id) max_id = system->employees[i].id;
    }
    return max_id + 1;
}

int add_employee(EmployeeSystem *system, const Employee *emp) {
    if (!system || !emp) return -1;
    if (ensure_capacity(system) != 0) return -1;
    system->employees[system->count] = *emp;
    system->count++;
    return emp->id;
}

Employee *find_employee(EmployeeSystem *system, int id) {
    if (!system) return NULL;
    int idx = find_employee_index(system, id);
    if (idx < 0) return NULL;
    return &system->employees[idx];
}

int find_employee_index(const EmployeeSystem *system, int id) {
    if (!system) return -1;
    for (int i = 0; i < system->count; ++i) {
        if (system->employees[i].id == id) return i;
    }
    return -1;
}

int delete_employee(EmployeeSystem *system, int id) {
    if (!system) return -1;
    int idx = find_employee_index(system, id);
    if (idx < 0) return -1;
    for (int i = idx; i < system->count - 1; ++i) {
        system->employees[i] = system->employees[i + 1];
    }
    system->count--;
    return 0;
}

int update_employee(EmployeeSystem *system, int id, const Employee *updated) {
    if (!system || !updated) return -1;
    int idx = find_employee_index(system, id);
    if (idx < 0) return -1;
    system->employees[idx] = *updated;
    system->employees[idx].id = id;
    return 0;
}

void list_employees(const EmployeeSystem *system) {
    if (!system) return;
    if (system->count == 0) {
        printf("No employees in the system.\n");
        return;
    }
    print_employee_header();
    for (int i = 0; i < system->count; ++i) {
        print_employee_row(&system->employees[i]);
    }
    print_separator('=', 100);
    printf("Total: %d employee(s)\n", system->count);
}

int find_by_name(const EmployeeSystem *system, const char *partial, int *indices, int max) {
    if (!system || !partial || !indices || max <= 0) return 0;
    int found = 0;
    for (int i = 0; i < system->count && found < max; ++i) {
        if (str_contains_ci(system->employees[i].name, partial)) {
            indices[found++] = i;
        }
    }
    return found;
}

int find_by_department(const EmployeeSystem *system, const char *partial, int *indices, int max) {
    if (!system || !partial || !indices || max <= 0) return 0;
    int found = 0;
    for (int i = 0; i < system->count && found < max; ++i) {
        if (str_contains_ci(system->employees[i].department, partial)) {
            indices[found++] = i;
        }
    }
    return found;
}

void generate_statistics(const EmployeeSystem *system) {
    if (!system) return;
    if (system->count == 0) {
        printf("No employees to compute statistics.\n");
        return;
    }
    int active = 0, on_leave = 0, terminated = 0, resigned = 0;
    float total_salary = 0.0f;
    float min_salary = system->employees[0].salary;
    float max_salary = system->employees[0].salary;
    int level_counts[5] = {0, 0, 0, 0, 0};

    for (int i = 0; i < system->count; ++i) {
        const Employee *e = &system->employees[i];
        total_salary += e->salary;
        if (e->salary < min_salary) min_salary = e->salary;
        if (e->salary > max_salary) max_salary = e->salary;
        switch (e->status) {
            case STATUS_ACTIVE:     active++; break;
            case STATUS_ON_LEAVE:   on_leave++; break;
            case STATUS_TERMINATED: terminated++; break;
            case STATUS_RESIGNED:   resigned++; break;
        }
        if (e->level >= 0 && e->level <= LEVEL_MANAGER) {
            level_counts[e->level]++;
        }
    }

    print_separator('=', 50);
    printf("SYSTEM STATISTICS\n");
    print_separator('=', 50);
    printf("Total employees : %d\n", system->count);
    printf("Total salary    : %.2f\n", total_salary);
    printf("Average salary  : %.2f\n", total_salary / (float)system->count);
    printf("Minimum salary  : %.2f\n", min_salary);
    printf("Maximum salary  : %.2f\n", max_salary);
    print_separator('-', 50);
    printf("Active          : %d\n", active);
    printf("On Leave        : %d\n", on_leave);
    printf("Terminated      : %d\n", terminated);
    printf("Resigned        : %d\n", resigned);
    print_separator('-', 50);
    for (int i = 0; i <= LEVEL_MANAGER; ++i) {
        printf("%-10s      : %d\n", level_to_string((EmployeeLevel)i), level_counts[i]);
    }
    print_separator('=', 50);
}

static int cmp_by_id(const void *a, const void *b) {
    const Employee *ea = (const Employee*)a;
    const Employee *eb = (const Employee*)b;
    return (ea->id > eb->id) - (ea->id < eb->id);
}
static int cmp_by_name(const void *a, const void *b) {
    const Employee *ea = (const Employee*)a;
    const Employee *eb = (const Employee*)b;
    return strcmp(ea->name, eb->name);
}
static int cmp_by_salary(const void *a, const void *b) {
    const Employee *ea = (const Employee*)a;
    const Employee *eb = (const Employee*)b;
    if (ea->salary < eb->salary) return -1;
    if (ea->salary > eb->salary) return 1;
    return 0;
}
static int cmp_by_department(const void *a, const void *b) {
    const Employee *ea = (const Employee*)a;
    const Employee *eb = (const Employee*)b;
    return strcmp(ea->department, eb->department);
}

void sort_employees(EmployeeSystem *system, int criterion) {
    if (!system || system->count < 2) return;
    int (*cmp)(const void*, const void*) = NULL;
    switch (criterion) {
        case 1: cmp = cmp_by_id; break;
        case 2: cmp = cmp_by_name; break;
        case 3: cmp = cmp_by_salary; break;
        case 4: cmp = cmp_by_department; break;
        default: return;
    }
    qsort(system->employees, (size_t)system->count, sizeof(Employee), cmp);
}
