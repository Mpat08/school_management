#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>

typedef struct student
{
    char *name;
    int ID;
    int grade;
} Student;

/* Fills "student" with a heap-allocated copy of "name", plus the given ID and
   grade. Returns true on success. On failure (NULL args or allocation
   failure), "student" is left untouched and an error is printed to stderr. */
bool student_init(Student *student, const char *name, int ID, int grade);

/* Frees the memory owned by "student" (its name). Does not free "student"
   itself, since Student is normally stored by value inside a Course's
   array. Safe to call more than once. */
void student_free(Student *student);

// Prints a student's fields to stdout. No-op if student is NULL.
void student_print(const Student *student);

/* Replaces a student's name with a new heap-allocated copy of "new_name".
   Returns true on success. On allocation failure the student is left with
   its original name unchanged. */
bool student_set_name(Student *student, const char *new_name);

#endif
