#ifndef COURSE_H
#define COURSE_H

#include <stdbool.h>
#include "student.h"

typedef struct course
{
    char *name;
    float average_grade;
    Student *students;   // dynamic array, grows via realloc
    int num_students;
    int capacity;        // allocated slots in "students"
} Course;

/* Initializes "course" as an empty course with the given name and a small
   starting capacity. Returns true on success. */
bool course_init(Course *course, const char *name);

/* Frees the course's name and every student it owns, plus the student
   array itself. Safe to call more than once. */
void course_free(Course *course);

/* Adds a student to the course, growing the internal array if needed.
   Rejects duplicate IDs. Recomputes average_grade on success.
   Returns false (and leaves the course unchanged) on a duplicate ID,
   a NULL argument, or an allocation failure. */
bool course_add_student(Course *course, const char *name, int ID, int grade);

/* Recomputes average_grade from the current students. Safe to call with
   zero students (average becomes 0). */
void course_update_average_grade(Course *course);

// Returns a pointer to the student with this ID, or NULL if not found.
Student *course_find_student(Course *course, int ID);

// Read-only version of the same lookup, usable on a const Course.
bool course_student_in(const Course *course, int ID);

/* Renames a student in place. Returns false if the ID isn't found or the
   allocation for the new name fails. */
bool course_update_student_name(Course *course, int student_ID, const char *new_name);

/* Updates a student's grade and recalculates the course average.
   Returns false if the ID isn't found. */
bool course_update_student_grade(Course *course, int student_ID, int new_grade);

/* Renames the course itself. Returns false on a NULL argument or
   allocation failure. */
bool course_set_name(Course *course, const char *new_name);

void course_print(const Course *course);
void course_print_passed_students(const Course *course, float passing_grade);
void course_print_failed_students(const Course *course, float passing_grade);

#endif
