#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdalign.h>

typedef union {
    int i;
    float f;
    char str[20];
} UnionData;

typedef struct {
    int i;
    float f;
    char str[20];
} StructData;

typedef enum {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_TEXT
} ValueType;

typedef struct {
    ValueType type;
    UnionData data;
} TaggedRecord;

void set_int(TaggedRecord *rec, int val) {
    if (rec != NULL) {
        rec->type = VALUE_INT;
        rec->data.i = val;
    }
}

void set_float(TaggedRecord *rec, float val) {
    if (rec != NULL) {
        rec->type = VALUE_FLOAT;
        rec->data.f = val;
    }
}

int set_text(TaggedRecord *rec, const char *str) {
    if (rec == NULL || str == NULL) {
        return 0;
    }

    if (strlen(str) >= sizeof(rec->data.str)) {
        printf("Error: String exceeds maximum length of 19 characters.\n");
        return 0;
    }

    rec->type = VALUE_TEXT;
    strcpy(rec->data.str, str);
    return 1;
}

void display_tagged_record(const TaggedRecord *rec) {
    if (rec == NULL) {
        return;
    }

    switch (rec->type) {
        case VALUE_INT:
            printf("Type: INT   | Value: %d\n", rec->data.i);
            break;
        case VALUE_FLOAT:
            printf("Type: FLOAT | Value: %.2f\n", rec->data.f);
            break;
        case VALUE_TEXT:
            printf("Type: TEXT  | Value: \"%s\"\n", rec->data.str);
            break;
        default:
            printf("Error: Unknown type tag.\n");
            break;
    }
}

int main(void) {
    StructData s;
    UnionData u;

    printf("sizeof(StructData) = %zu, alignof = %zu\n", sizeof(StructData), alignof(StructData));
    printf("sizeof(UnionData)  = %zu, alignof = %zu\n\n", sizeof(UnionData), alignof(UnionData));

    printf("Struct member addresses and offsets:\n");
    printf("s.i   : address = %p, offset = %zu\n", (void *)&s.i, offsetof(StructData, i));
    printf("s.f   : address = %p, offset = %zu\n", (void *)&s.f, offsetof(StructData, f));
    printf("s.str : address = %p, offset = %zu\n\n", (void *)&s.str, offsetof(StructData, str));

    printf("Union member addresses:\n");
    printf("u.i   : address = %p\n", (void *)&u.i);
    printf("u.f   : address = %p\n", (void *)&u.f);
    printf("u.str : address = %p\n\n", (void *)&u.str);

    size_t struct_100 = 100 * sizeof(StructData);
    size_t union_100 = 100 * sizeof(UnionData);
    printf("Memory for 100 Struct records : %zu bytes\n", struct_100);
    printf("Memory for 100 Union records  : %zu bytes (Saves %zu bytes)\n\n",
           union_100, struct_100 - union_100);

    TaggedRecord records[5];

    set_int(&records[0], 10);
    set_float(&records[1], 220.5f);
    set_text(&records[2], "Union Programming");
    set_int(&records[3], 42);
    set_text(&records[4], "Safe C Code");

    for (int i = 0; i < 5; i++) {
        printf("Record [%d]: ", i);
        display_tagged_record(&records[i]);
    }

    printf("\nSize of TaggedRecord: %zu bytes\n\n", sizeof(TaggedRecord));

    printf("Demonstrating safe type switching on record 0:\n");
    printf("Initial   : ");
    display_tagged_record(&records[0]);

    set_text(&records[0], "Switched to Text");
    printf("After text: ");
    display_tagged_record(&records[0]);

    set_float(&records[0], 99.95f);
    printf("After float: ");
    display_tagged_record(&records[0]);

    return 0;
}
