// week5_task4_struct_binary_io.c   (BONUS)
// Name: Aadityapratap Singh  Surname: Baghel  Student ID: 241ADB122
//
// Task 4: Save and load a struct in binary format with fwrite()/fread()
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task4_struct_binary_io

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
int  save_student_binary(const Student *s, const char *filename);  // 0 on success, -1 on error
int  load_student_binary(const char *filename, Student *out);      // 0 on success, -1 on error
long file_size(const char *filename);                              // bytes, -1 on error
static int save_student_text(const Student *s, const char *filename);  // helper for the size comparison

int main(void) {
    Student s1 = {0};          // zero-initialised so unused name bytes are not garbage on disk
    strcpy(s1.name, "Alice");
    s1.age = 21;
    s1.gpa = 3.75f;

    Student loaded = {0};
    const char *bin_file = "student.bin";
    const char *txt_file = "student.txt";

    printf("Saving student in binary format...\n");
    if (save_student_binary(&s1, bin_file) != 0) {
        fprintf(stderr, "Error: could not save %s\n", bin_file);
        return EXIT_FAILURE;
    }
    printf("Saved binary record to %s\n", bin_file);

    // same record as text so we can compare the sizes
    if (save_student_text(&s1, txt_file) != 0) {
        fprintf(stderr, "Error: could not save %s\n", txt_file);
        return EXIT_FAILURE;
    }
    printf("Saved text record to %s\n", txt_file);

    printf("Loading student from binary file...\n");
    if (load_student_binary(bin_file, &loaded) != 0) {
        fprintf(stderr, "Error: could not load %s\n", bin_file);
        return EXIT_FAILURE;
    }
    printf("Loaded binary record from %s\n", bin_file);

    printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age, loaded.gpa);
    printf("Text file size: %ld bytes\n", file_size(txt_file));
    printf("Binary file size: %ld bytes\n", file_size(bin_file));

    return EXIT_SUCCESS;
}

// dumps the whole struct to disk in one fwrite
int save_student_binary(const Student *s, const char *filename) {
    if (s == NULL || filename == NULL) {
        return -1;
    }
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return -1;
    }
    int ok = (fwrite(s, sizeof(Student), 1, fp) == 1);
    if (fclose(fp) != 0 || !ok) {
        return -1;
    }
    return 0;
}

// reads the struct back, out only changes if the read worked
int load_student_binary(const char *filename, Student *out) {
    if (filename == NULL || out == NULL) {
        return -1;
    }
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return -1;
    }
    Student tmp = {0};
    int ok = (fread(&tmp, sizeof(Student), 1, fp) == 1);
    fclose(fp);
    if (!ok) {
        return -1;
    }
    *out = tmp;
    return 0;
}

// jumps to the end of the file and asks where we are
long file_size(const char *filename) {
    if (filename == NULL) {
        return -1;
    }
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return -1;
    }
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return -1;
    }
    long size = ftell(fp);
    fclose(fp);
    return size;
}

// text version, "name age gpa" on one line
static int save_student_text(const Student *s, const char *filename) {
    if (s == NULL || filename == NULL) {
        return -1;
    }
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return -1;
    }
    int ok = fprintf(fp, "%s %d %.2f\n", s->name, s->age, s->gpa);
    if (fclose(fp) != 0 || ok < 0) {
        return -1;
    }
    return 0;
}
