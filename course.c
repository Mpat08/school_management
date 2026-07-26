#include "course.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COURSE_INITIAL_CAPACITY 4

bool course_init(Course *course, const char *name)
{
    if (!course || !name)
    {
        fprintf(stderr, "course_init: NULL argument\n");
        return false;
    }

    char *name_copy = malloc(strlen(name) + 1);
    if (!name_copy)
    {
        fprintf(stderr, "course_init: allocation failed for name\n");
        return false;
    }
    strcpy(name_copy, name);

    Student *students = malloc(COURSE_INITIAL_CAPACITY * sizeof(Student));
    if (!students)
    {
        fprintf(stderr, "course_init: allocation failed for students array\n");
        free(name_copy);
        return false;
    }

    course->name = name_copy;
    course->students = students;
    course->num_students = 0;
    course->capacity = COURSE_INITIAL_CAPACITY;
    course->average_grade = 0.0f;
    return true;
}

void course_free(Course *course)
{
    if (!course)
    {
        return;
    }

    free(course->name);
    course->name = NULL;

    for (int i = 0; i < course->num_students; i++)
    {
        student_free(&course->students[i]);
    }
    free(course->students);
    course->students = NULL;
    course->num_students = 0;
    course->capacity = 0;
}

/* Doubles the students array when full. Returns false (course unchanged)
   if realloc fails. */
static bool course_grow_if_needed(Course *course)
{
    if (course->num_students < course->capacity)
    {
        return true;
    }

    int new_capacity = course->capacity * 2;
    Student *new_students = realloc(course->students, (size_t)new_capacity * sizeof(Student));
    if (!new_students)
    {
        fprintf(stderr, "course_grow_if_needed: reallocation failed for \"%s\"\n", course->name);
        return false;
    }

    course->students = new_students;
    course->capacity = new_capacity;
    return true;
}

bool course_add_student(Course *course, const char *name, int ID, int grade)
{
    if (!course || !name)
    {
        fprintf(stderr, "course_add_student: NULL argument\n");
        return false;
    }

    if (course_find_student(course, ID))
    {
        fprintf(stderr, "course_add_student: student ID %d already exists in \"%s\"\n", ID, course->name);
        return false;
    }

    if (!course_grow_if_needed(course)) 
    {
        return false;
    }

    Student new_student;
    if (!student_init(&new_student, name, ID, grade)) 
    {
        return false;
    }

    course->students[course->num_students] = new_student;
    course->num_students++;
    course_update_average_grade(course);
    return true;
}

void course_update_average_grade(Course *course)
{
    if (!course)
    {
        return;
    }

    if (course->num_students == 0)
    {
        course->average_grade = 0.0f;
        return;
    }

    float total = 0;
    for (int i = 0; i < course->num_students; i++)
    {
        total += course->students[i].grade;
    }
    course->average_grade = total / course->num_students;
}

Student *course_find_student(Course *course, int ID)
{
    if (!course) return NULL;
    for (int i = 0; i < course->num_students; i++)
    {
        if (course->students[i].ID == ID)
        {
            return &course->students[i];
        }
    }
    return NULL;
}

bool course_student_in(const Course *course, int ID)
{
    if (!course)
    {
        return false;
    }
    for (int i = 0; i < course->num_students; i++)
    {
        if (course->students[i].ID == ID)
        {
            return true;
        }
    }
    return false;
}

bool course_update_student_name(Course *course, int student_ID, const char *new_name)
{
    if (!course || !new_name)
    {
        fprintf(stderr, "course_update_student_name: NULL argument\n");
        return false;
    }

    Student *student = course_find_student(course, student_ID);
    if (!student)
    {
        fprintf(stderr, "course_update_student_name: student ID %d not found in \"%s\"\n", student_ID, course->name);
        return false;
    }

    return student_set_name(student, new_name);
}

bool course_update_student_grade(Course *course, int student_ID, int new_grade)
{
    if (!course)
    {
        fprintf(stderr, "course_update_student_grade: NULL argument\n");
        return false;
    }

    Student *student = course_find_student(course, student_ID);
    if (!student)
    {
        fprintf(stderr, "course_update_student_grade: student ID %d not found in \"%s\"\n", student_ID, course->name);
        return false;
    }

    student->grade = new_grade;
    course_update_average_grade(course);
    return true;
}

bool course_set_name(Course *course, const char *new_name)
{
    if (!course || !new_name)
    {
        fprintf(stderr, "course_set_name: NULL argument\n");
        return false;
    }

    char *name_copy = malloc(strlen(new_name) + 1);
    if (!name_copy)
    {
        fprintf(stderr, "course_set_name: allocation failed\n");
        return false;
    }
    strcpy(name_copy, new_name);

    free(course->name);
    course->name = name_copy;
    return true;
}

void course_print(const Course *course)
{
    if (!course)
    {
        return;
    }

    printf("-----%s-----\n\n", course->name);
    printf("Course Name: %s\n", course->name);
    printf("Average grade of %s: %f\n", course->name, course->average_grade);
    printf("Number of students in %s: %d\n\n", course->name, course->num_students);
    for (int i = 0; i < course->num_students; i++)
    {
        student_print(&course->students[i]);
        printf("\n");
    }
}

void course_print_passed_students(const Course *course, float passing_grade)
{
    if (!course)
    {
        return;
    }

    printf("Students who passed %s (passing grade: %f):\n", course->name, passing_grade);
    for (int i = 0; i < course->num_students; i++)
    {
        if (course->students[i].grade >= passing_grade)
        {
            student_print(&course->students[i]);
        }
    }
}

void course_print_failed_students(const Course *course, float passing_grade)
{
    if (!course)
    {
        return;
    }

    printf("Students who failed %s (passing grade: %f):\n", course->name, passing_grade);
    for (int i = 0; i < course->num_students; i++)
    {
        if (course->students[i].grade < passing_grade)
        {
            student_print(&course->students[i]);
        }
    }
}
