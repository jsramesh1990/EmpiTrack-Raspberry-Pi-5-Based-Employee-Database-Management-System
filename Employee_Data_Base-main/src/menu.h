#ifndef MENU_H
#define MENU_H

#include "database.h"

void display_menu(void);
void run_menu(EmployeeSystem *system);
void menu_add_employee(EmployeeSystem *system);
void menu_display_employees(EmployeeSystem *system);
void menu_search_employee(EmployeeSystem *system);
void menu_update_employee(EmployeeSystem *system);
void menu_delete_employee(EmployeeSystem *system);
void menu_save(EmployeeSystem *system);
void menu_load(EmployeeSystem *system);
void menu_export_csv(EmployeeSystem *system);
void menu_generate_report(EmployeeSystem *system);
void menu_statistics(EmployeeSystem *system);
void menu_sort(EmployeeSystem *system);

#endif
