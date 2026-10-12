// week5_task2_struct_save_load.c
// Name: Aadityapratap Singh  Surname: Baghel  Student ID: 241ADB122
//
// Task 2: Save and load structured records from a file
// Week 5 – Files & Modular Programming
// TODO: Complete the function implementations and the file handling logic.
//
// Build: make lab5   Run: ./bin/week5_task2_struct_save_load

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

typedef struct Student {
    char name[MAX_NAME_LEN];
    int age;
    float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls them directly)
int save_student(const Student *s, const char *filename);  // 0 on success, -1 on error
int load_student(const char *filename, Student *out);      // 0 on success, -1 on error

int main(void) {
    Student s1 = {0};
    strcpy(s1.name, "Alice");
    s1.age = 21;
    s1.gpa = 3.75f;

    Student loaded = {0};
    const char *filename = "student.txt";

    printf("Saving student to file...\n");
    if (save_student(&s1, filename) != 0) {
        fprintf(stderr, "Error: could not save student to %s\n", filename);
        return EXIT_FAILURE;
    }

    printf("Loading student from file...\n");
    if (load_student(filename, &loaded) != 0) {
        fprintf(stderr, "Error: could not load student from %s\n", filename);
        return EXIT_FAILURE;
    }

    printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age, loaded.gpa);

    return EXIT_SUCCESS;
}

// writes "name age gpa" on one line, name must be a single word
// no exit() here, main decides what to do with the result
int save_student(const Student *s, const char *filename) {
    if (s == NULL || filename == NULL) {
        return -1;
    }
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return -1;
    }
    int ok = fprintf(fp, "%s %d %.2f\n", s->name, s->age, s->gpa);
    // fclose can fail too (buffered write), so check it
    if (fclose(fp) != 0 || ok < 0) {
        return -1;
    }
    return 0;
}

// reads the three fields back, only touches out if all 3 were read
int load_student(const char *filename, Student *out) {
    if (filename == NULL || out == NULL) {
        return -1;
    }
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return -1;
    }
    Student tmp = {0};
    int got = fscanf(fp, "%49s %d %f", tmp.name, &tmp.age, &tmp.gpa);
    fclose(fp);
    if (got != 3) {
        return -1;
    }
    *out = tmp;
    return 0;
}
