#include "employee.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

const char *level_to_string(EmployeeLevel level) {
    switch (level) {
        case LEVEL_JUNIOR:    return "Junior";
        case LEVEL_MID_LEVEL: return "Mid-Level";
        case LEVEL_SENIOR:    return "Senior";
        case LEVEL_LEAD:      return "Lead";
        case LEVEL_MANAGER:   return "Manager";
        default:              return "Unknown";
    }
}

const char *status_to_string(EmployeeStatus status) {
    switch (status) {
        case STATUS_ACTIVE:     return "Active";
        case STATUS_ON_LEAVE:   return "On Leave";
        case STATUS_TERMINATED: return "Terminated";
        case STATUS_RESIGNED:   return "Resigned";
        default:                return "Unknown";
    }
}

int string_to_level(const char *s, EmployeeLevel *out) {
    if (!s || !out) return -1;
    char buf[32];
    size_t n = strlen(s);
    if (n >= sizeof(buf)) n = sizeof(buf) - 1;
    for (size_t i = 0; i < n; ++i) buf[i] = (char)tolower((unsigned char)s[i]);
    buf[n] = '\0';
    if (strcmp(buf, "junior") == 0)   { *out = LEVEL_JUNIOR; return 0; }
    if (strcmp(buf, "mid") == 0 ||
        strcmp(buf, "mid-level") == 0 ||
        strcmp(buf, "mid_level") == 0 ||
        strcmp(buf, "midlevel") == 0) { *out = LEVEL_MID_LEVEL; return 0; }
    if (strcmp(buf, "senior") == 0)   { *out = LEVEL_SENIOR; return 0; }
    if (strcmp(buf, "lead") == 0)     { *out = LEVEL_LEAD; return 0; }
    if (strcmp(buf, "manager") == 0)  { *out = LEVEL_MANAGER; return 0; }
    return -1;
}

int string_to_status(const char *s, EmployeeStatus *out) {
    if (!s || !out) return -1;
    char buf[32];
    size_t n = strlen(s);
    if (n >= sizeof(buf)) n = sizeof(buf) - 1;
    for (size_t i = 0; i < n; ++i) buf[i] = (char)tolower((unsigned char)s[i]);
    buf[n] = '\0';
    if (strcmp(buf, "active") == 0)     { *out = STATUS_ACTIVE; return 0; }
    if (strcmp(buf, "leave") == 0 ||
        strcmp(buf, "on_leave") == 0 ||
        strcmp(buf, "on leave") == 0)   { *out = STATUS_ON_LEAVE; return 0; }
    if (strcmp(buf, "terminated") == 0) { *out = STATUS_TERMINATED; return 0; }
    if (strcmp(buf, "resigned") == 0)   { *out = STATUS_RESIGNED; return 0; }
    return -1;
}

void print_employee_header(void) {
    print_separator('=', 100);
    printf("%-5s %-20s %-15s %-10s %-10s %-10s %-25s %-15s\n",
           "ID", "Name", "Department", "Salary", "Level", "Status", "Email", "Phone");
    print_separator('-', 100);
}

void print_employee_row(const Employee *e) {
    if (!e) return;
    printf("%-5d %-20.20s %-15.15s %-10.2f %-10s %-10s %-25.25s %-15.15s\n",
           e->id, e->name, e->department, e->salary,
           level_to_string(e->level), status_to_string(e->status),
           e->email, e->phone);
}

void format_date(time_t t, char *buf, size_t size) {
    if (!buf || size == 0) return;
    if (t == 0) {
        snprintf(buf, size, "N/A");
        return;
    }
    struct tm *tm_info = localtime(&t);
    if (!tm_info) {
        snprintf(buf, size, "N/A");
        return;
    }
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

void print_employee_detail(const Employee *e) {
    if (!e) return;
    char datebuf[32];
    format_date(e->join_date, datebuf, sizeof(datebuf));
    print_separator('=', 60);
    printf("Employee ID  : %d\n", e->id);
    printf("Name         : %s\n", e->name);
    printf("Department   : %s\n", e->department);
    printf("Salary       : %.2f\n", e->salary);
    printf("Level        : %s\n", level_to_string(e->level));
    printf("Status       : %s\n", status_to_string(e->status));
    printf("Email        : %s\n", e->email);
    printf("Phone        : %s\n", e->phone);
    printf("Join Date    : %s\n", datebuf);
    print_separator('=', 60);
}

static void prompt_level(EmployeeLevel *level) {
    printf("Select Level:\n");
    for (int i = 0; i <= LEVEL_MANAGER; ++i) {
        printf("  %d. %s\n", i + 1, level_to_string((EmployeeLevel)i));
    }
    int choice = 0;
    while (1) {
        if (read_int("Level choice [1-5]: ", &choice) != 0) {
            printf("Invalid input. Try again.\n");
            continue;
        }
        if (choice >= 1 && choice <= 5) {
            *level = (EmployeeLevel)(choice - 1);
            return;
        }
        printf("Out of range. Try again.\n");
    }
}

static void prompt_status(EmployeeStatus *status) {
    printf("Select Status:\n");
    for (int i = 0; i <= STATUS_RESIGNED; ++i) {
        printf("  %d. %s\n", i + 1, status_to_string((EmployeeStatus)i));
    }
    int choice = 0;
    while (1) {
        if (read_int("Status choice [1-4]: ", &choice) != 0) {
            printf("Invalid input. Try again.\n");
            continue;
        }
        if (choice >= 1 && choice <= 4) {
            *status = (EmployeeStatus)(choice - 1);
            return;
        }
        printf("Out of range. Try again.\n");
    }
}

void input_employee(Employee *e, int id) {
    if (!e) return;
    memset(e, 0, sizeof(*e));
    e->id = id;
    e->status = STATUS_ACTIVE;
    e->level = LEVEL_JUNIOR;
    e->join_date = time(NULL);

    // name
    while (1) {
        if (read_string("Enter name: ", e->name, sizeof(e->name)) != 0 ||
            e->name[0] == '\0') {
            printf("Invalid name. Try again.\n");
            continue;
        }
        break;
    }

    // department
    while (1) {
        if (read_string("Enter department: ", e->department, sizeof(e->department)) != 0 ||
            e->department[0] == '\0') {
            printf("Invalid department. Try again.\n");
            continue;
        }
        break;
    }

    // salary
    while (1) {
        if (read_float("Enter salary: ", &e->salary) != 0 ||
            e->salary < 0.0f) {
            printf("Invalid salary. Try again.\n");
            continue;
        }
        break;
    }

    // level
    prompt_level(&e->level);

    // status
    prompt_status(&e->status);

    // email
    while (1) {
        if (read_string("Enter email: ", e->email, sizeof(e->email)) != 0 ||
            !validate_email(e->email)) {
            printf("Invalid email. Try again.\n");
            continue;
        }
        break;
    }

    // phone
    while (1) {
        if (read_string("Enter phone: ", e->phone, sizeof(e->phone)) != 0 ||
            !validate_phone(e->phone)) {
            printf("Invalid phone. Try again.\n");
            continue;
        }
        break;
    }
}

int input_employee_edit(Employee *e) {
    if (!e) return -1;
    char buf[128];
    printf("Leave blank to keep current value.\n");

    // name
    snprintf(buf, sizeof(buf), "Name [%s]: ", e->name);
    char tmp[MAX_NAME_LEN];
    if (read_string(buf, tmp, sizeof(tmp)) != 0) return -1;
    if (tmp[0] != '\0') {
        strncpy(e->name, tmp, sizeof(e->name) - 1);
        e->name[sizeof(e->name) - 1] = '\0';
    }

    // department
    snprintf(buf, sizeof(buf), "Department [%s]: ", e->department);
    char tmp2[MAX_DEPT_LEN];
    if (read_string(buf, tmp2, sizeof(tmp2)) != 0) return -1;
    if (tmp2[0] != '\0') {
        strncpy(e->department, tmp2, sizeof(e->department) - 1);
        e->department[sizeof(e->department) - 1] = '\0';
    }

    // salary
    snprintf(buf, sizeof(buf), "Salary [%.2f]: ", e->salary);
    char tmp3[64];
    if (read_string(buf, tmp3, sizeof(tmp3)) != 0) return -1;
    if (tmp3[0] != '\0') {
        char *end = NULL;
        float v = strtof(tmp3, &end);
        if (end != tmp3 && *end == '\0' && v >= 0.0f) {
            e->salary = v;
        } else {
            printf("Invalid salary, keeping current.\n");
        }
    }

    // level
    snprintf(buf, sizeof(buf), "Level [%s] (1-5 or blank): ", level_to_string(e->level));
    char tmp4[32];
    if (read_string(buf, tmp4, sizeof(tmp4)) != 0) return -1;
    if (tmp4[0] != '\0') {
        int v = atoi(tmp4);
        if (v >= 1 && v <= 5) e->level = (EmployeeLevel)(v - 1);
        else printf("Invalid level, keeping current.\n");
    }

    // status
    snprintf(buf, sizeof(buf), "Status [%s] (1-4 or blank): ", status_to_string(e->status));
    char tmp5[32];
    if (read_string(buf, tmp5, sizeof(tmp5)) != 0) return -1;
    if (tmp5[0] != '\0') {
        int v = atoi(tmp5);
        if (v >= 1 && v <= 4) e->status = (EmployeeStatus)(v - 1);
        else printf("Invalid status, keeping current.\n");
    }

    // email
    snprintf(buf, sizeof(buf), "Email [%s]: ", e->email);
    char tmp6[MAX_EMAIL_LEN];
    if (read_string(buf, tmp6, sizeof(tmp6)) != 0) return -1;
    if (tmp6[0] != '\0') {
        if (validate_email(tmp6)) {
            strncpy(e->email, tmp6, sizeof(e->email) - 1);
            e->email[sizeof(e->email) - 1] = '\0';
        } else {
            printf("Invalid email, keeping current.\n");
        }
    }

    // phone
    snprintf(buf, sizeof(buf), "Phone [%s]: ", e->phone);
    char tmp7[MAX_PHONE_LEN];
    if (read_string(buf, tmp7, sizeof(tmp7)) != 0) return -1;
    if (tmp7[0] != '\0') {
        if (validate_phone(tmp7)) {
            strncpy(e->phone, tmp7, sizeof(e->phone) - 1);
            e->phone[sizeof(e->phone) - 1] = '\0';
        } else {
            printf("Invalid phone, keeping current.\n");
        }
    }

    return 0;
}
