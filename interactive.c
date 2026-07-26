#include "interactive.h"
#include <stdio.h>
#include <string.h>

/* Reads one line into buf, stripping the trailing newline. Returns false
   on EOF or a read error. */
static bool read_line(char *buf, size_t size)
{
    if (!fgets(buf, (int)size, stdin)) return false;
    buf[strcspn(buf, "\n")] = '\0';
    return true;
}

/* Reads a single integer from stdin, re-prompting on anything that isn't
   a clean whole number (empty input, letters, trailing junk). Returns
   false only on EOF/read error, not on bad input -- bad input just loops. */
static bool read_int(const char *prompt, int *out)
{
    char line[256];
    while (true)
    {
        printf("%s", prompt);
        if (!read_line(line, sizeof(line))) return false;

        char extra;
        if (sscanf(line, "%d %c", out, &extra) == 1) return true;

        printf("That doesn't look like a whole number. Please try again.\n");
    }
}

bool interactive_create_school(School *school)
{
    char name_buf[256];

    printf("Enter school name: ");
    if (!read_line(name_buf, sizeof(name_buf))) return false;
    if (!school_init(school, name_buf)) return false;

    int num_courses;
    if (!read_int("Enter number of courses: ", &num_courses) || num_courses < 0)
    {
        fprintf(stderr, "interactive_create_school: invalid course count\n");
        school_free(school);
        return false;
    }

    for (int i = 0; i < num_courses; i++)
    {
        printf("\nCourse %d\n", i + 1);

        char course_name[256];
        printf("Enter course name: ");
        if (!read_line(course_name, sizeof(course_name))) { school_free(school); return false; }

        if (!school_add_course(school, course_name))
        {
            fprintf(stderr, "interactive_create_school: could not add course \"%s\"\n", course_name);
            school_free(school);
            return false;
        }
        Course *course = &school->courses[school->num_courses - 1];

        int num_students;
        if (!read_int("How many students are enrolled: ", &num_students) || num_students < 0)
        {
            fprintf(stderr, "interactive_create_school: invalid student count\n");
            school_free(school);
            return false;
        }

        for (int j = 0; j < num_students; j++)
        {
            printf("\nStudent %d\n", j + 1);

            char student_name[256];
            printf("Enter student name: ");
            if (!read_line(student_name, sizeof(student_name))) { school_free(school); return false; }

            int ID, grade;
            if (!read_int("Enter student ID: ", &ID)) { school_free(school); return false; }
            if (!read_int("Enter student grade: ", &grade)) { school_free(school); return false; }

            if (!course_add_student(course, student_name, ID, grade))
            {
                fprintf(stderr, "interactive_create_school: could not add student (duplicate ID?)\n");
                school_free(school);
                return false;
            }
        }
    }

    return true;
}
