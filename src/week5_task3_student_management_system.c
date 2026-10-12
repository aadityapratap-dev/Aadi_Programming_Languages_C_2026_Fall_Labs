// week5_task3_student_management_system.c
// Name: Aadityapratap Singh  Surname: Baghel  Student ID: 241ADB122
//
// Task 3: Mini-project – Student management system with file persistence
// Week 5 – Files & Modular Programming
// TODO: Implement the functions to load, save, add, and list students.
//
// Build: make lab5   Run: ./bin/week5_task3_student_management_system

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
    char name[NAME_LEN];
    int id;
    float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls them directly)
int  load_students(Student arr[], int max);          // returns number of records loaded
int  save_students(const Student arr[], int count);  // 0 on success, -1 on error
void add_student(Student arr[], int *count);
void list_students(const Student arr[], int count);
int  read_line(const char *prompt, char *buf, size_t size);

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    char input[LINE_LEN];
    int running = 1;

    count = load_students(students, MAX_STUDENTS);
    printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

    while (running) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");

        int choice;
        if (read_line("Select an option: ", input, sizeof input) != 0) {
            choice = 3;                         // end of input: save and exit instead of looping forever
        } else {
            char *end;
            long value = strtol(input, &end, 10);
            choice = (end != input && *end == '\0') ? (int)value : 0;   // non-numbers become invalid
        }

        switch (choice) {
            case 1:
                list_students(students, count);
                break;
            case 2:
                add_student(students, &count);
                break;
            case 3:
                if (save_students(students, count) != 0) {
                    fprintf(stderr, "Error: could not save to %s\n", DATA_FILE);
                }
                printf("Data saved. Goodbye!\n");
                running = 0;
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return EXIT_SUCCESS;
}

// Provided helper – prints a prompt and reads one line from stdin into buf.
// The trailing newline is removed. Returns 0 on success, -1 on end of input (EOF).
int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buf, (int)size, stdin) == NULL) {
        return -1;
    }
    size_t len = strcspn(buf, "\r\n");
    if (buf[len] == '\0' && len == size - 1) {
        // Line was longer than the buffer: discard the rest of it.
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    buf[len] = '\0';
    return 0;
}

// reads students.txt, a missing file just means first run so we return 0
int load_students(Student arr[], int max) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        return 0;
    }
    char line[LINE_LEN];
    int n = 0;
    while (n < max && fgets(line, sizeof line, fp) != NULL) {
        Student tmp = {0};
        // blank or broken lines don't give 3 matches so they get skipped
        if (sscanf(line, "%49[^,],%d,%f", tmp.name, &tmp.id, &tmp.gpa) == 3) {
            arr[n] = tmp;
            n++;
        }
    }
    fclose(fp);
    return n;
}

// overwrites students.txt with everything in the array
int save_students(const Student arr[], int count) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        return -1;
    }
    int bad = 0;
    for (int i = 0; i < count; i++) {
        if (fprintf(fp, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa) < 0) {
            bad = 1;
        }
    }
    if (fclose(fp) != 0 || bad) {
        return -1;
    }
    return 0;
}

// asks for name, id and gpa, checks them and adds the student if all is ok
void add_student(Student arr[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students (limit reached).\n");
        return;
    }

    char name[NAME_LEN];
    char id_text[LINE_LEN];
    char gpa_text[LINE_LEN];
    if (read_line("Enter name: ", name, sizeof name) != 0) {
        return;
    }
    if (read_line("Enter ID: ", id_text, sizeof id_text) != 0) {
        return;
    }
    if (read_line("Enter GPA: ", gpa_text, sizeof gpa_text) != 0) {
        return;
    }

    // name: not empty (spaces only counts as empty) and no comma
    if (strspn(name, " \t") == strlen(name) || strchr(name, ',') != NULL) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // id: whole text has to be a number that fits in an int
    char *end;
    errno = 0;
    long id = strtol(id_text, &end, 10);
    if (end == id_text || *end != '\0' || errno != 0 || id < INT_MIN || id > INT_MAX) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // gpa: whole text has to be a number between 0 and 4 (the not form also catches nan)
    float gpa = strtof(gpa_text, &end);
    if (end == gpa_text || *end != '\0' || !(gpa >= 0.0f && gpa <= 4.0f)) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    strcpy(arr[*count].name, name);
    arr[*count].id = (int)id;
    arr[*count].gpa = gpa;
    (*count)++;
    printf("Student added successfully!\n");
}

// prints a header and one line per student
void list_students(const Student arr[], int count) {
    if (count <= 0) {
        printf("No students found.\n");
        return;
    }
    printf("%-5s %-20s %s\n", "ID", "Name", "GPA");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
    }
}
