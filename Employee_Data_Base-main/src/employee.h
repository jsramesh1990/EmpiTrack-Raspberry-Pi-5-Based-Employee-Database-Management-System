#ifndef EMPITRACK_EMPLOYEE_H
#define EMPITRACK_EMPLOYEE_H

#include <time.h>
#include <stddef.h>

#define MAX_NAME_LEN  64
#define MAX_DEPT_LEN  64
#define MAX_EMAIL_LEN 128
#define MAX_PHONE_LEN 32

typedef enum {
    LEVEL_JUNIOR = 0,
    LEVEL_MID_LEVEL,
    LEVEL_SENIOR,
    LEVEL_LEAD,
    LEVEL_MANAGER
} EmployeeLevel;

typedef enum {
    STATUS_ACTIVE = 0,
    STATUS_ON_LEAVE,
    STATUS_TERMINATED,
    STATUS_RESIGNED
} EmployeeStatus;

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char department[MAX_DEPT_LEN];
    float salary;
    EmployeeLevel level;
    EmployeeStatus status;
    char email[MAX_EMAIL_LEN];
    char phone[MAX_PHONE_LEN];
    time_t join_date;
} Employee;

const char *level_to_string(EmployeeLevel level);
const char *status_to_string(EmployeeStatus status);
int string_to_level(const char *s, EmployeeLevel *out);
int string_to_status(const char *s, EmployeeStatus *out);

void print_employee_header(void);
void print_employee_row(const Employee *e);
void print_employee_detail(const Employee *e);

void input_employee(Employee *e, int id);
int input_employee_edit(Employee *e);

void format_date(time_t t, char *buf, size_t size);

#endif
