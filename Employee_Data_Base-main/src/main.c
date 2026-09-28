#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "file_ops.h"
#include "menu.h"

static void print_usage(const char *prog) {
    printf("Usage: %s [options]\n", prog);
    printf("Options:\n");
    printf("  --file <path>       Use specific database file\n");
    printf("  --capacity <n>      Initial capacity (default 10)\n");
    printf("  --silent            Do not show welcome banner\n");
    printf("  --help              Show this help\n");
}

int main(int argc, char **argv) {
    const char *filename = DEFAULT_DB_FILENAME;
    int capacity = DEFAULT_INITIAL_CAPACITY;
    int silent = 0;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
            filename = argv[++i];
        } else if (strcmp(argv[i], "--capacity") == 0 && i + 1 < argc) {
            capacity = atoi(argv[++i]);
            if (capacity <= 0) capacity = DEFAULT_INITIAL_CAPACITY;
        } else if (strcmp(argv[i], "--silent") == 0) {
            silent = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (!silent) {
        printf("╔══════════════════════════════════════════╗\n");
        printf("║  EmpiTrack - Employee Database System    ║\n");
        printf("║  Raspberry Pi 5 Edition                  ║\n");
        printf("╚══════════════════════════════════════════╝\n");
    }

    EmployeeSystem system;
    if (init_system(&system, capacity) != 0) {
        fprintf(stderr, "Failed to initialize system.\n");
        return 1;
    }

    // Try to load existing data
    int loaded = load_from_file(&system, filename);
    if (loaded >= 0) {
        if (!silent) printf("Loaded %d employee(s) from '%s'.\n", loaded, filename);
    } else {
        if (!silent) printf("Starting with empty database.\n");
        snprintf(system.filename, sizeof(system.filename), "%s", filename);
    }

    run_menu(&system);

    cleanup_system(&system);
    return 0;
}
