#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    CMD_ADD,
    CMD_DELETE,
    CMD_VIEW,
    CMD_UPDATE,
    CMD_COMPLETE,
    CMD_FIND,
    CMD_HELP,
    CMD_EXIT,
    CMD_UNKNOWN
} CommandType;

typedef enum {
    TASK_PENDING,
    TASK_DONE
} TaskStatus;

typedef struct {
    int id;
    char description[100];
    TaskStatus status;
} Task;

CommandType get_command_type(const char *cmd_str) {
    char upper[50];
    size_t i = 0;
    while (cmd_str[i] != '\0' && i < sizeof(upper) - 1) {
        upper[i] = (char)toupper((unsigned char)cmd_str[i]);
        i++;
    }
    upper[i] = '\0';

    if (strcmp(upper, "ADD") == 0) return CMD_ADD;
    if (strcmp(upper, "DELETE") == 0) return CMD_DELETE;
    if (strcmp(upper, "VIEW") == 0) return CMD_VIEW;
    if (strcmp(upper, "UPDATE") == 0) return CMD_UPDATE;
    if (strcmp(upper, "COMPLETE") == 0) return CMD_COMPLETE;
    if (strcmp(upper, "FIND") == 0) return CMD_FIND;
    if (strcmp(upper, "HELP") == 0) return CMD_HELP;
    if (strcmp(upper, "EXIT") == 0) return CMD_EXIT;

    return CMD_UNKNOWN;
}

int find_task(const Task *tasks, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {
            return i;
        }
    }
    return -1;
}

int add_task(Task **tasks, int *count, int *capacity, int id, const char *description) {
    if (id <= 0) {
        printf("Error: Task ID must be positive.\n");
        return 0;
    }

    if (find_task(*tasks, *count, id) != -1) {
        printf("Error: Task with ID %d already exists.\n", id);
        return 0;
    }

    if (*count >= *capacity) {
        int new_capacity = (*capacity == 0) ? 2 : (*capacity * 2);
        Task *temp = (Task *)realloc(*tasks, new_capacity * sizeof(Task));
        if (temp == NULL) {
            printf("Error: Memory allocation failed.\n");
            return 0;
        }
        *tasks = temp;
        *capacity = new_capacity;
    }

    (*tasks)[*count].id = id;
    strncpy((*tasks)[*count].description, description, sizeof((*tasks)[*count].description) - 1);
    (*tasks)[*count].description[sizeof((*tasks)[*count].description) - 1] = '\0';
    (*tasks)[*count].status = TASK_PENDING;
    (*count)++;

    printf("Task %d added successfully.\n", id);
    return 1;
}

int delete_task(Task *tasks, int *count, int id) {
    int idx = find_task(tasks, *count, id);
    if (idx == -1) {
        printf("Error: Task %d not found.\n", id);
        return 0;
    }

    for (int i = idx; i < *count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    (*count)--;

    printf("Task %d deleted successfully.\n", id);
    return 1;
}

void view_tasks(const Task *tasks, int count) {
    if (count == 0) {
        printf("No tasks found.\n");
        return;
    }

    printf("\n--- Task List ---\n");
    for (int i = 0; i < count; i++) {
        printf("[%d] [%s] %s\n",
               tasks[i].id,
               tasks[i].status == TASK_DONE ? "DONE   " : "PENDING",
               tasks[i].description);
    }
}

int update_task(Task *tasks, int count, int id, const char *new_description) {
    int idx = find_task(tasks, count, id);
    if (idx == -1) {
        printf("Error: Task %d not found.\n", id);
        return 0;
    }

    strncpy(tasks[idx].description, new_description, sizeof(tasks[idx].description) - 1);
    tasks[idx].description[sizeof(tasks[idx].description) - 1] = '\0';

    printf("Task %d updated successfully.\n", id);
    return 1;
}

int complete_task(Task *tasks, int count, int id) {
    int idx = find_task(tasks, count, id);
    if (idx == -1) {
        printf("Error: Task %d not found.\n", id);
        return 0;
    }

    tasks[idx].status = TASK_DONE;
    printf("Task %d marked as DONE.\n", id);
    return 1;
}

void find_task_and_display(const Task *tasks, int count, int id) {
    int idx = find_task(tasks, count, id);
    if (idx == -1) {
        printf("Task %d not found.\n", id);
        return;
    }

    printf("Found: [%d] [%s] %s\n",
           tasks[idx].id,
           tasks[idx].status == TASK_DONE ? "DONE   " : "PENDING",
           tasks[idx].description);
}

void show_help(void) {
    printf("\nSupported Commands:\n");
    printf("  ADD      - Add a new task\n");
    printf("  DELETE   - Delete a task by ID\n");
    printf("  VIEW     - View all tasks\n");
    printf("  UPDATE   - Update a task's description\n");
    printf("  COMPLETE - Mark a task as done\n");
    printf("  FIND     - Find a task by ID\n");
    printf("  HELP     - Show this help message\n");
    printf("  EXIT     - Exit the program\n");
}

int main(void) {
    int capacity = 4;
    int count = 0;
    Task *tasks = (Task *)malloc(capacity * sizeof(Task));
    if (tasks == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    char input[100];
    CommandType cmd;

    while (1) {
        printf("\nEnter command (ADD, DELETE, VIEW, UPDATE, COMPLETE, FIND, HELP, EXIT): ");
        if (scanf("%99s", input) != 1) {
            break;
        }

        cmd = get_command_type(input);

        switch (cmd) {
            case CMD_ADD: {
                int id = 0;
                char desc[100];
                printf("Enter task ID: ");
                scanf("%d", &id);
                printf("Enter description: ");
                scanf(" %99[^\n]", desc);
                add_task(&tasks, &count, &capacity, id, desc);
                break;
            }

            case CMD_DELETE: {
                int id = 0;
                printf("Enter task ID to delete: ");
                scanf("%d", &id);
                delete_task(tasks, &count, id);
                break;
            }

            case CMD_VIEW: {
                view_tasks(tasks, count);
                break;
            }

            case CMD_UPDATE: {
                int id = 0;
                char desc[100];
                printf("Enter task ID to update: ");
                scanf("%d", &id);
                printf("Enter new description: ");
                scanf(" %99[^\n]", desc);
                update_task(tasks, count, id, desc);
                break;
            }

            case CMD_COMPLETE: {
                int id = 0;
                printf("Enter task ID to complete: ");
                scanf("%d", &id);
                complete_task(tasks, count, id);
                break;
            }

            case CMD_FIND: {
                int id = 0;
                printf("Enter task ID to find: ");
                scanf("%d", &id);
                find_task_and_display(tasks, count, id);
                break;
            }

            case CMD_HELP: {
                show_help();
                break;
            }

            case CMD_EXIT: {
                printf("Exiting program.\n");
                free(tasks);
                return 0;
            }

            case CMD_UNKNOWN:
            default: {
                printf("Unknown command. Type HELP for available commands.\n");
                break;
            }
        }
    }

    free(tasks);
    return 0;
}
