#include "file_ops.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void escape_pipe(const char *in, char *out, size_t out_size) {
    size_t j = 0;
    for (size_t i = 0; in[i] && j + 1 < out_size; ++i) {
        if (in[i] == '|') {
            if (j + 2 < out_size) {
                out[j++] = '\\';
                out[j++] = '|';
            }
        } else {
            out[j++] = in[i];
        }
    }
    out[j] = '\0';
}

static void unescape_pipe(const char *in, char *out, size_t out_size) {
    size_t j = 0;
    for (size_t i = 0; in[i] && j + 1 < out_size; ++i) {
        if (in[i] == '\\' && in[i+1] == '|') {
            out[j++] = '|';
            i++;
        } else {
            out[j++] = in[i];
        }
    }
    out[j] = '\0';
}

int save_to_file(EmployeeSystem *system, const char *filename) {
    if (!system || !filename) return -1;
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error: cannot open '%s' for writing\n", filename);
        return -1;
    }
    fprintf(fp, "# EmpiTrack Database v1.0\n");
    fprintf(fp, "# ID|NAME|DEPT|SALARY|LEVEL|STATUS|EMAIL|PHONE|JOIN_DATE\n");
    int saved = 0;
    for (int i = 0; i < system->count; ++i) {
        const Employee *e = &system->employees[i];
        char name[MAX_NAME_LEN * 2];
        char dept[MAX_DEPT_LEN * 2];
        char email[MAX_EMAIL_LEN * 2];
        char phone[MAX_PHONE_LEN * 2];
        escape_pipe(e->name, name, sizeof(name));
        escape_pipe(e->department, dept, sizeof(dept));
        escape_pipe(e->email, email, sizeof(email));
        escape_pipe(e->phone, phone, sizeof(phone));
        fprintf(fp, "%d|%s|%s|%.2f|%s|%s|%s|%s|%lld\n",
                e->id, name, dept, e->salary,
                level_to_string(e->level), status_to_string(e->status),
                email, phone, (long long)e->join_date);
        saved++;
    }
    fclose(fp);
    snprintf(system->filename, sizeof(system->filename), "%s", filename);
    return saved;
}

int load_from_file(EmployeeSystem *system, const char *filename) {
    if (!system || !filename) return -1;
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: cannot open '%s' for reading\n", filename);
        return -1;
    }
    system->count = 0;
    char line[512];
    int loaded = 0;
    while (fgets(line, sizeof(line), fp)) {
        trim_newline(line);
        if (line[0] == '\0' || line[0] == '#') continue;

        char *fields[9];
        int n = 0;
        char *p = line;
        fields[n++] = p;
        while (*p && n < 9) {
            if (*p == '|' && (p == line || *(p-1) != '\\')) {
                *p = '\0';
                fields[n++] = p + 1;
            }
            p++;
        }
        if (n != 9) continue;

        if (ensure_capacity(system) != 0) {
            fclose(fp);
            return -1;
        }
        Employee e;
        memset(&e, 0, sizeof(e));
        e.id = atoi(fields[0]);
        unescape_pipe(fields[1], e.name, sizeof(e.name));
        unescape_pipe(fields[2], e.department, sizeof(e.department));
        e.salary = strtof(fields[3], NULL);

        EmployeeLevel level;
        if (string_to_level(fields[4], &level) == 0) e.level = level;
        else e.level = LEVEL_JUNIOR;

        EmployeeStatus status;
        if (string_to_status(fields[5], &status) == 0) e.status = status;
        else e.status = STATUS_ACTIVE;

        unescape_pipe(fields[6], e.email, sizeof(e.email));
        unescape_pipe(fields[7], e.phone, sizeof(e.phone));
        e.join_date = (time_t)atoll(fields[8]);

        system->employees[system->count++] = e;
        loaded++;
    }
    fclose(fp);
    snprintf(system->filename, sizeof(system->filename), "%s", filename);
    return loaded;
}

int export_to_csv(const EmployeeSystem *system, const char *filename) {
    if (!system || !filename) return -1;
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error: cannot open '%s' for writing\n", filename);
        return -1;
    }
    fprintf(fp, "ID,Name,Department,Salary,Level,Status,Email,Phone,JoinDate\n");
    int exported = 0;
    for (int i = 0; i < system->count; ++i) {
        const Employee *e = &system->employees[i];
        char datebuf[32];
        format_date(e->join_date, datebuf, sizeof(datebuf));
        // Simple CSV escape: wrap in quotes and escape quotes
        // For simplicity, we replace commas with semicolons
        char name[MAX_NAME_LEN];
        char dept[MAX_DEPT_LEN];
        char email[MAX_EMAIL_LEN];
        char phone[MAX_PHONE_LEN];
        snprintf(name, sizeof(name), "%s", e->name);
        snprintf(dept, sizeof(dept), "%s", e->department);
        snprintf(email, sizeof(email), "%s", e->email);
        snprintf(phone, sizeof(phone), "%s", e->phone);
        for (char *q = name; *q; ++q) if (*q == ',') *q = ';';
        for (char *q = dept; *q; ++q) if (*q == ',') *q = ';';
        for (char *q = email; *q; ++q) if (*q == ',') *q = ';';
        for (char *q = phone; *q; ++q) if (*q == ',') *q = ';';
        fprintf(fp, "%d,%s,%s,%.2f,%s,%s,%s,%s,%s\n",
                e->id, name, dept, e->salary,
                level_to_string(e->level), status_to_string(e->status),
                email, phone, datebuf);
        exported++;
    }
    fclose(fp);
    return exported;
}

int create_backup(const char *filename) {
    if (!filename) return -1;
    FILE *src = fopen(filename, "rb");
    if (!src) return -1;
    char backup[300];
    snprintf(backup, sizeof(backup), "%s.bak", filename);
    FILE *dst = fopen(backup, "wb");
    if (!dst) { fclose(src); return -1; }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), src)) > 0) {
        if (fwrite(buf, 1, n, dst) != n) {
            fclose(src); fclose(dst);
            return -1;
        }
    }
    fclose(src);
    fclose(dst);
    return 0;
}
