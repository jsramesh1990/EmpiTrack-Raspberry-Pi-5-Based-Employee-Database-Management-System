#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum for department
typedef enum {
    HR = 1,
    ENGINEERING,
    SALES,
    MARKETING
} Department;

// Structure for Employee
typedef struct {
    int id;
    char name[50];
    Department dept;
    float salary;
} Employee;

// Function declarations
void addEmployee(Employee **list, int *count);
void displayEmployees(Employee *list, int count);
void searchEmployee(Employee *list, int count);
void saveToFile(Employee *list, int count);
void loadFromFile(Employee **list, int *count);
const char* getDeptName(Department dept);

// Function pointer type for menu actions
typedef void (*Action)(Employee **, int *);

// Helper function
const char* getDeptName(Department dept) {
    switch (dept) {
        case HR: return "HR";
        case ENGINEERING: return "Engineering";
        case SALES: return "Sales";
        case MARKETING: return "Marketing";
        default: return "Unknown";
    }
}

// Add new employee
void addEmployee(Employee **list, int *count) {
    *list = realloc(*list, (*count + 1) * sizeof(Employee));
    if (!*list) {
        printf("Memory allocation failed!\n");
        return;
    }

    Employee *emp = &(*list)[*count];
    emp->id = *count + 1;

    printf("Enter name: ");
    scanf(" %[^\n]", emp->name);

    printf("Enter department (1-HR, 2-ENG, 3-SALES, 4-MKT): ");
    int dept;
    scanf("%d", &dept);
    emp->dept = (Department)dept;

    printf("Enter salary: ");
    scanf("%f", &emp->salary);

    (*count)++;
    printf("Employee added successfully!\n\n");
}

// Display all employees
void displayEmployees(Employee *list, int count) {
    if (count == 0) {
        printf("No employees found!\n");
        return;
    }

    printf("\n--- Employee List ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Name: %s | Dept: %s | Salary: %.2f\n",
               list[i].id, list[i].name, getDeptName(list[i].dept), list[i].salary);
    }
    printf("----------------------\n\n");
}

// Search employee by ID
void searchEmployee(Employee *list, int count) {
    if (count == 0) {
        printf("No employees to search!\n");
        return;
    }

    int id;
    printf("Enter Employee ID to search: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (list[i].id == id) {
            printf("Found: %s from %s with salary %.2f\n",
                   list[i].name, getDeptName(list[i].dept), list[i].salary);
            return;
        }
    }
    printf("Employee not found!\n");
}

// Save data to file & add photo also for more clear entry capture featues
void saveToFile(Employee *list, int count) {
    FILE *f = fopen("employees.txt", "w");
    if (!f) {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(f, "%d,%s,%d,%.2f\n",
                list[i].id, list[i].name, list[i].dept, list[i].salary);
    }
    fclose(f);
    printf("Data saved to employees.txt\n\n");
}

// Load data from file
void loadFromFile(Employee **list, int *count) {
    FILE *f = fopen("employees.txt", "r");
    if (!f) {
        printf("No saved file found.\n");
        return;
    }

    *count = 0;
    *list = NULL;
    Employee temp;

    while (fscanf(f, "%d,%49[^,],%d,%f\n",
                  &temp.id, temp.name, (int*)&temp.dept, &temp.salary) == 4) {
        *list = realloc(*list, (*count + 1) * sizeof(Employee));
        (*list)[*count] = temp;
        (*count)++;
    }
    fclose(f);
    printf("Data loaded from employees.txt\n\n");
}

// Main function
int main() {
    Employee *list = NULL;
    int count = 0, choice;

    // Menu actions mapped to function pointers
    Action actions[] = {
        (Action)addEmployee,
        (Action)displayEmployees,
        (Action)searchEmployee,
        (Action)saveToFile,
        (Action)loadFromFile
    };

    while (1) {
        printf("\n====== Employee Management System ======\n");
        printf("1. Add Employee\n");
        printf("2. Display Employees\n");
        printf("3. Search Employee\n");
        printf("4. Save to File\n");
        printf("5. Load from File\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 6) break;
        if (choice >= 1 && choice <= 5)
            actions[choice - 1](&list, &count);
        else
            printf("Invalid choice!\n");
    }

    free(list);
    printf("Exiting... Goodbye!\n");
    return 0;
}

