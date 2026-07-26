#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool student_init(Student *student, const char *name, int ID, int grade)
{
    if (!student || !name)
    {
        fprintf(stderr, "student_init: NULL argument\n");
        return false;
    }

    char *name_copy = malloc(strlen(name) + 1);
    if (!name_copy)
    {
        fprintf(stderr, "student_init: allocation failed for name\n");
        return false;
    }
    strcpy(name_copy, name);

    student->name = name_copy;
    student->ID = ID;
    student->grade = grade;
    return true;
}

void student_free(Student *student)
{
    if (!student) return;
    free(student->name);
    student->name = NULL;
}

void student_print(const Student *student)
{
    if (!student)
    {
        return;
    } 
    printf("Student Name: %s\n", student->name);
    printf("Student ID: %d\n", student->ID);
    printf("Student Grade: %d\n", student->grade);
}

bool student_set_name(Student *student, const char *new_name)
{
    if (!student || !new_name)
    {
        fprintf(stderr, "student_set_name: NULL argument\n");
        return false;
    }

    char *name_copy = malloc(strlen(new_name) + 1);
    if (!name_copy)
    {
        fprintf(stderr, "student_set_name: allocation failed\n");
        return false;
    }
    strcpy(name_copy, new_name);

    free(student->name);
    student->name = name_copy;
    return true;
}
