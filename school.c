#include "school.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCHOOL_INITIAL_CAPACITY 4

bool school_init(School *school, const char *name)
{
    if (!school || !name)
    {
        fprintf(stderr, "school_init: NULL argument\n");
        return false;
    }

    char *name_copy = malloc(strlen(name) + 1);
    if (!name_copy)
    {
        fprintf(stderr, "school_init: allocation failed for name\n");
        return false;
    }
    strcpy(name_copy, name);

    Course *courses = malloc(SCHOOL_INITIAL_CAPACITY * sizeof(Course));
    if (!courses)
    {
        fprintf(stderr, "school_init: allocation failed for courses array\n");
        free(name_copy);
        return false;
    }

    school->name = name_copy;
    school->courses = courses;
    school->num_courses = 0;
    school->capacity = SCHOOL_INITIAL_CAPACITY;
    return true;
}

void school_free(School *school)
{
    if (!school) 
    {
        return;
    }

    free(school->name);
    school->name = NULL;

    for (int i = 0; i < school->num_courses; i++)
    {
        course_free(&school->courses[i]);
    }
    free(school->courses);
    school->courses = NULL;
    school->num_courses = 0;
    school->capacity = 0;
}

static bool school_grow_if_needed(School *school)
{
    if (school->num_courses < school->capacity) 
    {
        return true;
    }

    int new_capacity = school->capacity * 2;
    Course *new_courses = realloc(school->courses, (size_t)new_capacity * sizeof(Course));
    if (!new_courses)
    {
        fprintf(stderr, "school_grow_if_needed: reallocation failed for school \"%s\"\n", school->name);
        return false;
    }

    school->courses = new_courses;
    school->capacity = new_capacity;
    return true;
}

bool school_add_course(School *school, const char *course_name)
{
    if (!school || !course_name)
    {
        fprintf(stderr, "school_add_course: NULL argument\n");
        return false;
    }

    if (school_course_in(school, course_name))
    {
        fprintf(stderr, "school_add_course: course \"%s\" already exists in \"%s\"\n", course_name, school->name);
        return false;
    }

    if (!school_grow_if_needed(school))
    {
        return false;
    }

    Course new_course;
    if (!course_init(&new_course, course_name)) 
    {
        return false;
    }

    school->courses[school->num_courses] = new_course;
    school->num_courses++;
    return true;
}

Course *school_find_course(School *school, const char *course_name)
{
    if (!school || !course_name)
    {
        return NULL;
    }
    for (int i = 0; i < school->num_courses; i++)
    {
        if (strcmp(school->courses[i].name, course_name) == 0)
        {
            return &school->courses[i];
        }
    }
    return NULL;
}

bool school_course_in(const School *school, const char *course_name)
{
    if (!school || !course_name) 
    {
        return false;
    }
    for (int i = 0; i < school->num_courses; i++)
    {
        if (strcmp(school->courses[i].name, course_name) == 0) 
        {
            return true;
        }
    }
    return false;
}

bool school_student_in(const School *school, int ID)
{
    if (!school) 
    {
        return false;
    }
    for (int i = 0; i < school->num_courses; i++)
    {
        if (course_student_in(&school->courses[i], ID)) 
        {
            return true;
        }
    }
    return false;
}

void school_print_student_courses(const School *school, int ID)
{
    if (!school) 
    {
        return;
    }

    int found = 0;
    printf("Courses for student ID: %d\n", ID);
    for (int i = 0; i < school->num_courses; i++)
    {
        if (course_student_in(&school->courses[i], ID))
        {
            printf(" - %s\n", school->courses[i].name);
            found++;
        }
    }
    if (!found)
    {
        printf("Student is not enrolled in any course\n");
    }
}

void school_print(const School *school)
{
    if (!school) 
    {
        return;
    }

    printf("-----%s-----\n\n", school->name);
    printf("School Name: %s\n", school->name);
    printf("Number of courses at %s: %d\n\n", school->name, school->num_courses);
    for (int i = 0; i < school->num_courses; i++)
    {
        course_print(&school->courses[i]);
        printf("\n");
    }
}

void school_print_passed_courses(const School *school, float passing_grade)
{
    if (!school) 
    {
        return;
    }
    if (school->num_courses == 0)
    {
        printf("No courses available\n");
        return;
    }
    printf("Course with a PASSING average (>= %f):\n", passing_grade);
    for (int i = 0; i < school->num_courses; i++)
    {
        if (school->courses[i].average_grade >= passing_grade)
        {
            course_print(&school->courses[i]);
        }
    }
}

void school_print_failed_courses(const School *school, float passing_grade)
{
    if (!school) 
    {
        return;
    }
    if (school->num_courses == 0)
    {
        printf("No courses available\n");
        return;
    }
    printf("Course with a FAILING average (< %f):\n", passing_grade);
    for (int i = 0; i < school->num_courses; i++)
    {
        if (school->courses[i].average_grade < passing_grade)
        {
            course_print(&school->courses[i]);
        }
    }
}

bool school_overall_average(const School *school, float *school_average)
{
    if (!school || !school_average || school->num_courses == 0) 
    {
        return false;
    }

    float sum = 0;
    for (int i = 0; i < school->num_courses; i++)
    {
        sum += school->courses[i].average_grade;
    }
    *school_average = sum / school->num_courses;
    return true;
}

bool school_highest_avg_course(const School *school, const Course **school_course)
{
    if (!school || !school_course || school->num_courses == 0) 
    {
        return false;
    }

    int best = 0;
    for (int i = 1; i < school->num_courses; i++)
    {
        if (school->courses[i].average_grade > school->courses[best].average_grade)
        {
            best = i;
        }
    }
    *school_course = &school->courses[best];
    return true;
}

void school_print_common_courses(const School *school1, const School *school2)
{
    if (!school1 || !school2)
    {
        return;
    }

    printf("COMMON COURSES BETWEEN %s AND %s:\n", school1->name, school2->name);
    for (int i = 0; i < school1->num_courses; i++)
    {
        if (school_course_in(school2, school1->courses[i].name))
        {
            printf(" - %s\n", school1->courses[i].name);
        }
    }
}

void school_print_unique_courses(const School *school1, const School *school2)
{
    if (!school1 || !school2) 
    {
        return;
    }

    printf("UNIQUE COURSES IN %s:\n", school1->name);
    for (int i = 0; i < school1->num_courses; i++)
    {
        if (!school_course_in(school2, school1->courses[i].name))
        {
            printf(" - %s\n", school1->courses[i].name);
        }
    }
}
