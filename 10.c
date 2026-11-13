#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TYPE_EMPTY,
    INT,
    FLOAT,
    CHAR,
    STRING
} DataType;

typedef union {
    int int_value;
    float float_value;
    char char_value;
    char *string_value;
} Data;

typedef struct {
    Data data;
    DataType type;
} Container;

void container_init(Container *c) {
    if (c != NULL) {
        c->type = TYPE_EMPTY;
        c->data.string_value = NULL;
    }
}

void container_clear(Container *c) {
    if (c == NULL) {
        return;
    }

    if (c->type == STRING && c->data.string_value != NULL) {
        free(c->data.string_value);
        c->data.string_value = NULL;
    }

    c->type = TYPE_EMPTY;
}

void set_int(Container *c, int value) {
    if (c == NULL) return;
    container_clear(c);
    c->data.int_value = value;
    c->type = INT;
}

void set_float(Container *c, float value) {
    if (c == NULL) return;
    container_clear(c);
    c->data.float_value = value;
    c->type = FLOAT;
}

void set_char(Container *c, char value) {
    if (c == NULL) return;
    container_clear(c);
    c->data.char_value = value;
    c->type = CHAR;
}

int set_string(Container *c, const char *value) {
    if (c == NULL || value == NULL) {
        return 0;
    }

    size_t len = strlen(value);
    char *new_str = (char *)malloc(len + 1);
    if (new_str == NULL) {
        return 0;
    }
    memcpy(new_str, value, len + 1);

    container_clear(c);
    c->data.string_value = new_str;
    c->type = STRING;
    return 1;
}

int get_int(const Container *c, int *out_value) {
    if (c == NULL || out_value == NULL || c->type != INT) {
        return 0;
    }
    *out_value = c->data.int_value;
    return 1;
}

int get_float(const Container *c, float *out_value) {
    if (c == NULL || out_value == NULL || c->type != FLOAT) {
        return 0;
    }
    *out_value = c->data.float_value;
    return 1;
}

int get_char(const Container *c, char *out_value) {
    if (c == NULL || out_value == NULL || c->type != CHAR) {
        return 0;
    }
    *out_value = c->data.char_value;
    return 1;
}

int get_string(const Container *c, const char **out_value) {
    if (c == NULL || out_value == NULL || c->type != STRING) {
        return 0;
    }
    *out_value = c->data.string_value;
    return 1;
}

const char* container_type_name(DataType type) {
    switch (type) {
        case INT:        return "INT";
        case FLOAT:      return "FLOAT";
        case CHAR:       return "CHAR";
        case STRING:     return "STRING";
        case TYPE_EMPTY: return "EMPTY";
        default:         return "UNKNOWN";
    }
}

void container_print(const Container *c, int index) {
    if (c == NULL) return;

    printf("Container [%d] | Type: %-6s | Value: ", index, container_type_name(c->type));
    switch (c->type) {
        case INT:
            printf("%d\n", c->data.int_value);
            break;
        case FLOAT:
            printf("%.2f\n", c->data.float_value);
            break;
        case CHAR:
            printf("'%c'\n", c->data.char_value);
            break;
        case STRING:
            printf("\"%s\"\n", c->data.string_value);
            break;
        case TYPE_EMPTY:
            printf("<EMPTY>\n");
            break;
        default:
            printf("<INVALID>\n");
            break;
    }
}

int container_copy(Container *dest, const Container *src) {
    if (dest == NULL || src == NULL) {
        return 0;
    }

    if (dest == src) {
        return 1;
    }

    if (src->type == STRING) {
        size_t len = strlen(src->data.string_value);
        char *copy = (char *)malloc(len + 1);
        if (copy == NULL) {
            return 0;
        }
        memcpy(copy, src->data.string_value, len + 1);

        container_clear(dest);
        dest->data.string_value = copy;
        dest->type = STRING;
    } else {
        container_clear(dest);
        dest->type = src->type;
        dest->data = src->data;
    }

    return 1;
}

int main(void) {
    Container containers[6];
    for (int i = 0; i < 6; i++) {
        container_init(&containers[i]);
    }

    set_int(&containers[0], 10);
    set_float(&containers[1], 3.14f);
    set_char(&containers[2], 'A');
    set_string(&containers[3], "Hello, World!");
    set_int(&containers[4], 0);
    set_string(&containers[5], "Data");

    printf("--- Array of 6 Containers ---\n");
    for (int i = 0; i < 6; i++) {
        container_print(&containers[i], i);
    }

    printf("\n--- Type-Safe Getters Test ---\n");
    int int_val = 0;
    if (get_int(&containers[4], &int_val)) {
        printf("get_int on container 4: %d\n", int_val);
    }

    float float_val = 0.0f;
    if (!get_float(&containers[3], &float_val)) {
        printf("get_float on container 3 correctly failed (holds STRING)\n");
    }

    printf("\n--- Deep Copy Test ---\n");
    Container copied;
    container_init(&copied);
    container_copy(&copied, &containers[5]);

    printf("Original: ");
    container_print(&containers[5], 5);
    printf("Copy    : ");
    container_print(&copied, 99);

    set_string(&containers[5], "Modified");
    printf("After changing original:\n");
    printf("Original: ");
    container_print(&containers[5], 5);
    printf("Copy    : ");
    container_print(&copied, 99);

    printf("\n--- Type Transition Test (STRING -> INT -> STRING -> FLOAT) ---\n");
    Container temp;
    container_init(&temp);

    set_string(&temp, "First String");
    container_print(&temp, 0);

    set_int(&temp, 42);
    container_print(&temp, 0);

    set_string(&temp, "Second String");
    container_print(&temp, 0);

    set_float(&temp, 12.34f);
    container_print(&temp, 0);

    for (int i = 0; i < 6; i++) {
        container_clear(&containers[i]);
    }
    container_clear(&copied);
    container_clear(&temp);

    return 0;
}
