#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.0001f

typedef struct {
    int id;
    float width;
    float height;
    float area;
    float perimeter;
} Rectangle;

void calculate_geometry(Rectangle *r) {
    r->area = r->width * r->height;
    r->perimeter = 2.0f * (r->width + r->height);
}

int is_square(const Rectangle *r) {
    return fabsf(r->width - r->height) < EPSILON;
}

int search_rectangle(const Rectangle *rectangles, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (rectangles[i].id == id) {
            return i;
        }
    }
    return -1;
}

int add_rectangle(Rectangle **rectangles, int *count, int *capacity, int id, float width, float height) {
    if (id <= 0) {
        printf("Error: Rectangle ID must be positive.\n");
        return 0;
    }

    if (search_rectangle(*rectangles, *count, id) != -1) {
        printf("Error: Rectangle with ID %d already exists.\n", id);
        return 0;
    }

    if (isnan(width) || isinf(width) || width <= 0.0f ||
        isnan(height) || isinf(height) || height <= 0.0f) {
        printf("Error: Dimensions must be positive finite numbers.\n");
        return 0;
    }

    if (*count >= *capacity) {
        int new_capacity = (*capacity == 0) ? 2 : (*capacity * 2);
        Rectangle *temp = (Rectangle *)realloc(*rectangles, new_capacity * sizeof(Rectangle));
        if (temp == NULL) {
            printf("Error: Memory allocation failed.\n");
            return 0;
        }
        *rectangles = temp;
        *capacity = new_capacity;
    }

    (*rectangles)[*count].id = id;
    (*rectangles)[*count].width = width;
    (*rectangles)[*count].height = height;
    calculate_geometry(&(*rectangles)[*count]);
    (*count)++;

    printf("Rectangle %d added (Area: %.2f, Perimeter: %.2f, %s).\n",
           id, (*rectangles)[*count - 1].area, (*rectangles)[*count - 1].perimeter,
           is_square(&(*rectangles)[*count - 1]) ? "Square" : "Non-Square");
    return 1;
}

void display_rectangles(const Rectangle *rectangles, int count) {
    if (count == 0) {
        printf("No rectangles in inventory.\n");
        return;
    }

    printf("\n--- Rectangle Inventory ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Width: %.2f | Height: %.2f | Area: %.2f | Perimeter: %.2f | %s\n",
               rectangles[i].id, rectangles[i].width, rectangles[i].height,
               rectangles[i].area, rectangles[i].perimeter,
               is_square(&rectangles[i]) ? "Square" : "Non-Square");
    }
}

int update_rectangle(Rectangle *rectangles, int count, int id, float new_width, float new_height) {
    int idx = search_rectangle(rectangles, count, id);
    if (idx == -1) {
        printf("Error: Rectangle with ID %d not found.\n", id);
        return 0;
    }

    if (isnan(new_width) || isinf(new_width) || new_width <= 0.0f ||
        isnan(new_height) || isinf(new_height) || new_height <= 0.0f) {
        printf("Error: Dimensions must be positive finite numbers.\n");
        return 0;
    }

    rectangles[idx].width = new_width;
    rectangles[idx].height = new_height;
    calculate_geometry(&rectangles[idx]);

    printf("Rectangle %d updated (Area: %.2f, Perimeter: %.2f).\n",
           id, rectangles[idx].area, rectangles[idx].perimeter);
    return 1;
}

int delete_rectangle(Rectangle *rectangles, int *count, int id) {
    int idx = search_rectangle(rectangles, *count, id);
    if (idx == -1) {
        printf("Error: Rectangle with ID %d not found.\n", id);
        return 0;
    }

    for (int i = idx; i < *count - 1; i++) {
        rectangles[i] = rectangles[i + 1];
    }
    (*count)--;

    printf("Rectangle %d deleted.\n", id);
    return 1;
}

void display_analytics(const Rectangle *rectangles, int count) {
    if (count == 0) {
        printf("No rectangles available for analytics.\n");
        return;
    }

    float total_area = 0.0f;
    int largest_idx = 0;
    int smallest_idx = 0;

    for (int i = 0; i < count; i++) {
        total_area += rectangles[i].area;
        if (rectangles[i].area > rectangles[largest_idx].area) {
            largest_idx = i;
        }
        if (rectangles[i].area < rectangles[smallest_idx].area) {
            smallest_idx = i;
        }
    }

    printf("\n--- Geometry Analytics ---\n");
    printf("Total Rectangles : %d\n", count);
    printf("Total Area       : %.2f\n", total_area);
    printf("Average Area     : %.2f\n", total_area / count);
    printf("Largest Area     : %.2f (ID: %d)\n", rectangles[largest_idx].area, rectangles[largest_idx].id);
    printf("Smallest Area    : %.2f (ID: %d)\n", rectangles[smallest_idx].area, rectangles[smallest_idx].id);
}

int compare_rectangles(const void *a, const void *b) {
    const Rectangle *r1 = (const Rectangle *)a;
    const Rectangle *r2 = (const Rectangle *)b;

    if (r1->area < r2->area) {
        return -1;
    }
    if (r1->area > r2->area) {
        return 1;
    }
    return r1->id - r2->id;
}

void display_sorted_by_area(const Rectangle *rectangles, int count) {
    if (count == 0) {
        printf("No rectangles to sort.\n");
        return;
    }

    Rectangle *copy = (Rectangle *)malloc(count * sizeof(Rectangle));
    if (copy == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        copy[i] = rectangles[i];
    }
    qsort(copy, count, sizeof(Rectangle), compare_rectangles);

    printf("\n--- Rectangles Sorted by Area (Ascending) ---\n");
    display_rectangles(copy, count);

    free(copy);
}

void filter_by_area(const Rectangle *rectangles, int count, float threshold) {
    if (count == 0) {
        printf("No rectangles in inventory.\n");
        return;
    }

    int found = 0;
    printf("\n--- Rectangles with Area > %.2f ---\n", threshold);
    for (int i = 0; i < count; i++) {
        if (rectangles[i].area > threshold) {
            printf("ID: %d | Width: %.2f | Height: %.2f | Area: %.2f (%s)\n",
                   rectangles[i].id, rectangles[i].width, rectangles[i].height,
                   rectangles[i].area, is_square(&rectangles[i]) ? "Square" : "Non-Square");
            found++;
        }
    }

    if (found == 0) {
        printf("No rectangles found with area greater than %.2f.\n", threshold);
    }
}

int main(void) {
    int capacity = 0;
    int count = 0;

    printf("Enter initial capacity: ");
    if (scanf("%d", &capacity) != 1 || capacity <= 0) {
        printf("Invalid capacity.\n");
        return 1;
    }

    Rectangle *rectangles = (Rectangle *)malloc(capacity * sizeof(Rectangle));
    if (rectangles == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    int choice = 0;
    while (1) {
        printf("\n1. Add Rectangle\n");
        printf("2. Display All Rectangles\n");
        printf("3. Search Rectangle by ID\n");
        printf("4. Update Rectangle Dimensions\n");
        printf("5. Delete Rectangle\n");
        printf("6. Geometry Analytics\n");
        printf("7. Display Sorted by Area\n");
        printf("8. Filter by Area Threshold\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int id = 0;
            float w = 0.0f;
            float h = 0.0f;

            printf("Enter rectangle ID: ");
            scanf("%d", &id);
            printf("Enter width: ");
            scanf("%f", &w);
            printf("Enter height: ");
            scanf("%f", &h);

            add_rectangle(&rectangles, &count, &capacity, id, w, h);

        } else if (choice == 2) {
            display_rectangles(rectangles, count);

        } else if (choice == 3) {
            int id = 0;
            printf("Enter rectangle ID to search: ");
            scanf("%d", &id);

            int idx = search_rectangle(rectangles, count, id);
            if (idx == -1) {
                printf("Rectangle with ID %d not found.\n", id);
            } else {
                printf("Found: ID: %d | Width: %.2f | Height: %.2f | Area: %.2f | Perimeter: %.2f (%s)\n",
                       rectangles[idx].id, rectangles[idx].width, rectangles[idx].height,
                       rectangles[idx].area, rectangles[idx].perimeter,
                       is_square(&rectangles[idx]) ? "Square" : "Non-Square");
            }

        } else if (choice == 4) {
            int id = 0;
            float w = 0.0f;
            float h = 0.0f;

            printf("Enter rectangle ID to update: ");
            scanf("%d", &id);
            printf("Enter new width: ");
            scanf("%f", &w);
            printf("Enter new height: ");
            scanf("%f", &h);

            update_rectangle(rectangles, count, id, w, h);

        } else if (choice == 5) {
            int id = 0;
            printf("Enter rectangle ID to delete: ");
            scanf("%d", &id);
            delete_rectangle(rectangles, &count, id);

        } else if (choice == 6) {
            display_analytics(rectangles, count);

        } else if (choice == 7) {
            display_sorted_by_area(rectangles, count);

        } else if (choice == 8) {
            float threshold = 0.0f;
            printf("Enter area threshold: ");
            scanf("%f", &threshold);
            filter_by_area(rectangles, count, threshold);

        } else if (choice == 9) {
            break;

        } else {
            printf("Invalid choice.\n");
        }
    }

    free(rectangles);
    return 0;
}
