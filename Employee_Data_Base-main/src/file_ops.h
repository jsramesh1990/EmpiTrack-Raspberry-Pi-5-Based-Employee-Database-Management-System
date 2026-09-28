#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "database.h"

int save_to_file(EmployeeSystem *system, const char *filename);
int load_from_file(EmployeeSystem *system, const char *filename);
int export_to_csv(const EmployeeSystem *system, const char *filename);
int create_backup(const char *filename);

#endif
