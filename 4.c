#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int roll_no;
    char name[50];
    int age;
    float gpa;
} Student;

int search_student(const Student *students, int count, int roll_no) {
    for (int i = 0; i < count; i++) {
        if (students[i].roll_no == roll_no) {
            return i;
        }
    }
    return -1;
}

int add_student(Student **students, int *count, int *capacity, int roll_no, const char *name, int age, float gpa) {
    if (roll_no <= 0) {
        printf("Error: Roll number must be positive.\n");
        return 0;
    }

    if (search_student(*students, *count, roll_no) != -1) {
        printf("Error: Roll number %d already exists.\n", roll_no);
        return 0;
    }

    if (age < 16 || age > 100) {
        printf("Error: Age must be between 16 and 100.\n");
        return 0;
    }

    if (gpa < 0.0f || gpa > 4.0f) {
        printf("Error: GPA must be between 0.00 and 4.00.\n");
        return 0;
    }

    if (*count >= *capacity) {
        int new_capacity = (*capacity == 0) ? 2 : (*capacity * 2);
        Student *temp = (Student *)realloc(*students, new_capacity * sizeof(Student));
        if (temp == NULL) {
            printf("Error: Memory allocation failed during resize.\n");
            return 0;
        }
        *students = temp;
        *capacity = new_capacity;
    }

    (*students)[*count].roll_no = roll_no;
    strncpy((*students)[*count].name, name, sizeof((*students)[*count].name) - 1);
    (*students)[*count].name[sizeof((*students)[*count].name) - 1] = '\0';
    (*students)[*count].age = age;
    (*students)[*count].gpa = gpa;
    (*count)++;

    printf("Student %s (Roll No: %d) added successfully.\n", name, roll_no);
    return 1;
}

void display_students(const Student *students, int count) {
    if (count == 0) {
        printf("No student records found.\n");
        return;
    }

    printf("\n--- Student Records ---\n");
    for (int i = 0; i < count; i++) {
        printf("Roll No: %d, Name: %s, Age: %d, GPA: %.2f\n",
               students[i].roll_no, students[i].name, students[i].age, students[i].gpa);
    }
}

int update_student(Student *students, int count, int roll_no, const char *new_name, int new_age, float new_gpa) {
    int idx = search_student(students, count, roll_no);
    if (idx == -1) {
        printf("Error: Student with roll number %d not found.\n", roll_no);
        return 0;
    }

    if (new_age < 16 || new_age > 100) {
        printf("Error: Age must be between 16 and 100.\n");
        return 0;
    }

    if (new_gpa < 0.0f || new_gpa > 4.0f) {
        printf("Error: GPA must be between 0.00 and 4.00.\n");
        return 0;
    }

    strncpy(students[idx].name, new_name, sizeof(students[idx].name) - 1);
    students[idx].name[sizeof(students[idx].name) - 1] = '\0';
    students[idx].age = new_age;
    students[idx].gpa = new_gpa;

    printf("Student with roll number %d updated successfully.\n", roll_no);
    return 1;
}

int delete_student(Student *students, int *count, int roll_no) {
    int idx = search_student(students, *count, roll_no);
    if (idx == -1) {
        printf("Error: Student with roll number %d not found.\n", roll_no);
        return 0;
    }

    for (int i = idx; i < *count - 1; i++) {
        students[i] = students[i + 1];
    }
    (*count)--;

    printf("Student with roll number %d deleted.\n", roll_no);
    return 1;
}

void display_analytics(const Student *students, int count) {
    if (count == 0) {
        printf("No records available for analytics.\n");
        return;
    }

    float sum = 0.0f;
    int highest_idx = 0;
    int lowest_idx = 0;

    for (int i = 0; i < count; i++) {
        sum += students[i].gpa;
        if (students[i].gpa > students[highest_idx].gpa) {
            highest_idx = i;
        }
        if (students[i].gpa < students[lowest_idx].gpa) {
            lowest_idx = i;
        }
    }

    printf("\n--- Academic Analytics ---\n");
    printf("Total Students : %d\n", count);
    printf("Average GPA    : %.2f\n", sum / count);
    printf("Highest GPA    : %.2f (Roll No: %d, Name: %s)\n",
           students[highest_idx].gpa, students[highest_idx].roll_no, students[highest_idx].name);
    printf("Lowest GPA     : %.2f (Roll No: %d, Name: %s)\n",
           students[lowest_idx].gpa, students[lowest_idx].roll_no, students[lowest_idx].name);
}

int compare_students(const void *a, const void *b) {
    const Student *s1 = (const Student *)a;
    const Student *s2 = (const Student *)b;

    if (s1->gpa < s2->gpa) {
        return 1;
    }
    if (s1->gpa > s2->gpa) {
        return -1;
    }
    return s1->roll_no - s2->roll_no;
}

void display_sorted_by_gpa(const Student *students, int count) {
    if (count == 0) {
        printf("No student records to sort.\n");
        return;
    }

    Student *copy = (Student *)malloc(count * sizeof(Student));
    if (copy == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    memcpy(copy, students, count * sizeof(Student));
    qsort(copy, count, sizeof(Student), compare_students);

    printf("\n--- Students Sorted by GPA (Descending) ---\n");
    display_students(copy, count);

    free(copy);
}

int main(void) {
    int capacity = 0;
    int count = 0;

    printf("Enter initial capacity: ");
    if (scanf("%d", &capacity) != 1 || capacity <= 0) {
        printf("Invalid capacity.\n");
        return 1;
    }

    Student *students = (Student *)malloc(capacity * sizeof(Student));
    if (students == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    int choice = 0;
    while (1) {
        printf("\n1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by Roll Number\n");
        printf("4. Update Student Details\n");
        printf("5. Delete Student\n");
        printf("6. Academic Analytics\n");
        printf("7. Display Sorted by GPA\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int roll = 0;
            int age = 0;
            float gpa = 0.0f;
            char name[50];

            printf("Enter roll number: ");
            scanf("%d", &roll);
            printf("Enter name: ");
            scanf(" %49[^\n]", name);
            printf("Enter age: ");
            scanf("%d", &age);
            printf("Enter GPA: ");
            scanf("%f", &gpa);

            add_student(&students, &count, &capacity, roll, name, age, gpa);

        } else if (choice == 2) {
            display_students(students, count);

        } else if (choice == 3) {
            int roll = 0;
            printf("Enter roll number to search: ");
            scanf("%d", &roll);

            int idx = search_student(students, count, roll);
            if (idx == -1) {
                printf("Student with roll number %d not found.\n", roll);
            } else {
                printf("Found: Roll No: %d, Name: %s, Age: %d, GPA: %.2f\n",
                       students[idx].roll_no, students[idx].name, students[idx].age, students[idx].gpa);
            }

        } else if (choice == 4) {
            int roll = 0;
            int age = 0;
            float gpa = 0.0f;
            char name[50];

            printf("Enter roll number to update: ");
            scanf("%d", &roll);
            printf("Enter new name: ");
            scanf(" %49[^\n]", name);
            printf("Enter new age: ");
            scanf("%d", &age);
            printf("Enter new GPA: ");
            scanf("%f", &gpa);

            update_student(students, count, roll, name, age, gpa);

        } else if (choice == 5) {
            int roll = 0;
            printf("Enter roll number to delete: ");
            scanf("%d", &roll);
            delete_student(students, &count, roll);

        } else if (choice == 6) {
            display_analytics(students, count);

        } else if (choice == 7) {
            display_sorted_by_gpa(students, count);

        } else if (choice == 8) {
            break;

        } else {
            printf("Invalid choice.\n");
        }
    }

    free(students);
    return 0;
}
