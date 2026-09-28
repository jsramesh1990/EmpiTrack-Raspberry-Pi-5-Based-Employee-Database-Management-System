#include "menu.h"
#include "file_ops.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void display_menu(void) {
    printf("\n");
    printf("╔══════════════════════════════════════╗\n");
    printf("║   EMPITRACK EMPLOYEE SYSTEM v1.0     ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  1. Add New Employee                 ║\n");
    printf("║  2. Display All Employees            ║\n");
    printf("║  3. Search Employee                  ║\n");
    printf("║  4. Update Employee Information      ║\n");
    printf("║  5. Delete Employee                  ║\n");
    printf("║  6. Save Data to File                ║\n");
    printf("║  7. Load Data from File              ║\n");
    printf("║  8. Export to CSV                    ║\n");
    printf("║  9. Sort Employees                   ║\n");
    printf("║ 10. System Statistics                ║\n");
    printf("║  0. Exit                             ║\n");
    printf("╚══════════════════════════════════════╝\n");
}

static void menu_add_employee(EmployeeSystem *system) {
    printf("\n--- Add New Employee ---\n");
    int id = next_employee_id(system);
    printf("Auto-generated ID: %d\n", id);
    Employee e;
    input_employee(&e, id);
    if (add_employee(system, &e) >= 0) {
        printf("Employee added successfully (ID %d).\n", id);
        create_backup(system->filename);
    } else {
        printf("Failed to add employee.\n");
    }
}

static void menu_display_employees(EmployeeSystem *system) {
    printf("\n--- All Employees ---\n");
    list_employees(system);
}

static void menu_search_employee(EmployeeSystem *system) {
    printf("\n--- Search Employee ---\n");
    printf("1. By ID\n2. By Name\n3. By Department\n");
    int choice = 0;
    if (read_int("Choice: ", &choice) != 0) {
        printf("Invalid choice.\n");
        return;
    }
    if (choice == 1) {
        int id;
        if (read_int("Enter ID: ", &id) != 0) { printf("Invalid.\n"); return; }
        Employee *e = find_employee(system, id);
        if (e) print_employee_detail(e);
        else printf("Employee with ID %d not found.\n", id);
    } else if (choice == 2) {
        char partial[MAX_NAME_LEN];
        if (read_string("Enter name (partial): ", partial, sizeof(partial)) != 0) return;
        int max = system->count > 0 ? system->count : 1;
        int *indices = (int*)malloc((size_t)max * sizeof(int));
        if (!indices) { printf("Memory error.\n"); return; }
        int n = find_by_name(system, partial, indices, max);
        if (n == 0) printf("No matching employees.\n");
        else {
            print_employee_header();
            for (int i = 0; i < n; ++i) print_employee_row(&system->employees[indices[i]]);
            print_separator('-', 100);
            printf("Found: %d\n", n);
        }
        free(indices);
    } else if (choice == 3) {
        char partial[MAX_DEPT_LEN];
        if (read_string("Enter department (partial): ", partial, sizeof(partial)) != 0) return;
        int max = system->count > 0 ? system->count : 1;
        int *indices = (int*)malloc((size_t)max * sizeof(int));
        if (!indices) { printf("Memory error.\n"); return; }
        int n = find_by_department(system, partial, indices, max);
        if (n == 0) printf("No matching employees.\n");
        else {
            print_employee_header();
            for (int i = 0; i < n; ++i) print_employee_row(&system->employees[indices[i]]);
            print_separator('-', 100);
            printf("Found: %d\n", n);
        }
        free(indices);
    } else {
        printf("Invalid choice.\n");
    }
}

static void menu_update_employee(EmployeeSystem *system) {
    printf("\n--- Update Employee ---\n");
    int id;
    if (read_int("Enter employee ID to update: ", &id) != 0) {
        printf("Invalid ID.\n");
        return;
    }
    Employee *e = find_employee(system, id);
    if (!e) {
        printf("Employee with ID %d not found.\n", id);
        return;
    }
    printf("Editing employee ID %d\n", id);
    Employee copy = *e;
    if (input_employee_edit(&copy) == 0) {
        *e = copy;
        printf("Employee updated successfully.\n");
        create_backup(system->filename);
    } else {
        printf("Update cancelled.\n");
    }
}

static void menu_delete_employee(EmployeeSystem *system) {
    printf("\n--- Delete Employee ---\n");
    int id;
    if (read_int("Enter employee ID to delete: ", &id) != 0) {
        printf("Invalid ID.\n");
        return;
    }
    Employee *e = find_employee(system, id);
    if (!e) {
        printf("Employee with ID %d not found.\n", id);
        return;
    }
    print_employee_detail(e);
    char confirm[16];
    if (read_string("Confirm delete? (yes/no): ", confirm, sizeof(confirm)) != 0) return;
    to_lowercase(confirm);
    if (strcmp(confirm, "yes") != 0 && strcmp(confirm, "y") != 0) {
        printf("Delete cancelled.\n");
        return;
    }
    create_backup(system->filename);
    if (delete_employee(system, id) == 0) {
        printf("Employee deleted.\n");
    } else {
        printf("Failed to delete employee.\n");
    }
}

static void menu_save(EmployeeSystem *system) {
    printf("\n--- Save Data ---\n");
    char filename[256];
    snprintf(filename, sizeof(filename), "%s", system->filename[0] ? system->filename : "data/employees.dat");
    if (read_string("Filename to save to: ", filename, sizeof(filename)) != 0) return;
    if (filename[0] == '\0') {
        snprintf(filename, sizeof(filename), "%s", system->filename);
    }
    int n = save_to_file(system, filename);
    if (n >= 0) printf("Saved %d employees to '%s'.\n", n, filename);
    else printf("Failed to save data.\n");
}

static void menu_load(EmployeeSystem *system) {
    printf("\n--- Load Data ---\n");
    char filename[256];
    snprintf(filename, sizeof(filename), "%s", system->filename);
    if (read_string("Filename to load from: ", filename, sizeof(filename)) != 0) return;
    if (filename[0] == '\0') {
        snprintf(filename, sizeof(filename), "%s", system->filename);
    }
    int n = load_from_file(system, filename);
    if (n >= 0) printf("Loaded %d employees from '%s'.\n", n, filename);
    else printf("Failed to load data.\n");
}

static void menu_export_csv(EmployeeSystem *system) {
    printf("\n--- Export to CSV ---\n");
    char filename[256];
    snprintf(filename, sizeof(filename), "data/employees.csv");
    if (read_string("CSV filename: ", filename, sizeof(filename)) != 0) return;
    if (filename[0] == '\0') {
        snprintf(filename, sizeof(filename), "data/employees.csv");
    }
    int n = export_to_csv(system, filename);
    if (n >= 0) printf("Exported %d employees to '%s'.\n", n, filename);
    else printf("Failed to export CSV.\n");
}

static void menu_sort(EmployeeSystem *system) {
    printf("\n--- Sort Employees ---\n");
    printf("1. By ID\n2. By Name\n3. By Salary\n4. By Department\n");
    int choice = 0;
    if (read_int("Sort by: ", &choice) != 0) {
        printf("Invalid choice.\n");
        return;
    }
    sort_employees(system, choice);
    printf("Sorted.\n");
    list_employees(system);
}

static void menu_statistics(EmployeeSystem *system) {
    printf("\n--- System Statistics ---\n");
    generate_statistics(system);
}

void run_menu(EmployeeSystem *system) {
    int choice;
    while (1) {
        display_menu();
        if (read_int("Enter your choice: ", &choice) != 0) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        switch (choice) {
            case 1: menu_add_employee(system); break;
            case 2: menu_display_employees(system); break;
            case 3: menu_search_employee(system); break;
            case 4: menu_update_employee(system); break;
            case 5: menu_delete_employee(system); break;
            case 6: menu_save(system); break;
            case 7: menu_load(system); break;
            case 8: menu_export_csv(system); break;
            case 9: menu_sort(system); break;
            case 10: menu_statistics(system); break;
            case 0:
                printf("Saving data before exit...\n");
                if (system->filename[0]) save_to_file(system, system->filename);
                printf("Goodbye!\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
        pause_console();
    }
}
