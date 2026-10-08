/*
 * week4_3_struct_database.c
 * Author: Aadityapratap Singh Baghel
 * Student ID: 241ADB122
 * Description:
 *   Simple in memory "database" using an array of structs.
 *   Use malloc to allocate space for n Student records,
 *   read each record from the user, print them as a table,
 *   and then free the memory.
 *
 *   Output must match the format in the Week 4 instructions exactly
 *   (it is checked by the autograder).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// same struct as task 2
struct Student {
    char name[50];
    int id;
    float grade;
};

int main(void) {
    int n;
    struct Student *students = NULL;

    printf("Enter number of students: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number.\n");
        return 1;
    }

    // one block big enough for n students
    students = malloc(n * sizeof(struct Student));

    // check malloc before touching the memory
    if (students == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        // i starts at 0 but the prompt counts from 1
        printf("Enter data for student %d: ", i + 1);

        // %49s leaves room for the '\0' so a long name can't overflow name[50]
        if (scanf("%49s %d %f", students[i].name, &students[i].id,
                  &students[i].grade) != 3) {
            printf("Invalid input.\n");
            // memory is allocated already, free it before leaving
            free(students);
            return 1;
        }
    }

    // blank line, then the header and one row per student in input order
    printf("\n");
    printf("%-6s %-11s %s\n", "ID", "Name", "Grade");
    for (int i = 0; i < n; i++) {
        printf("%-6d %-11s %.1f\n", students[i].id, students[i].name,
               students[i].grade);
    }

    // all done with the records
    free(students);

    return 0;
}
