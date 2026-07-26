#ifndef SCHOOL_H
#define SCHOOL_H

#include <stdbool.h>
#include "course.h"

typedef struct school
{
    char *name;
    Course *courses;      // dynamic array, grows via realloc 
    int num_courses;
    int capacity;         // allocated slots in `courses` 
} School;

bool school_init(School *school, const char *name);
void school_free(School *school);

/* Adds an empty course with this name, growing the internal array if
   needed. Rejects duplicate course names. */
bool school_add_course(School *school, const char *course_name);

// Returns a pointer to the course with this name, or NULL if absent.
Course *school_find_course(School *school, const char *course_name);

// Read-only version of the same lookup. 
bool school_course_in(const School *school, const char *course_name);

bool school_student_in(const School *school, int ID);
void school_print_student_courses(const School *school, int ID);

void school_print(const School *school);
void school_print_passed_courses(const School *school, float passing_grade);
void school_print_failed_courses(const School *school, float passing_grade);

/* Writes the school-wide average into *school_average. Returns false (and
   leaves *school_average untouched) if the school has no courses. */
bool school_overall_average(const School *school, float *school_average);

/* Writes a pointer to the highest-average course into *school_course.
   Returns false if the school has no courses. */
bool school_highest_avg_course(const School *school, const Course **school_course);

void school_print_common_courses(const School *school1, const School *school2);
void school_print_unique_courses(const School *school1, const School *school2);

#endif
