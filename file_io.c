#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* File format (plain text, one record per line):
 *   SCHOOL <name>
 *   COURSES <num_courses>
 *   COURSE <name>
 *   STUDENTS <num_students>
 *   STUDENT <ID> <grade> <name>
 *   (repeated per student, then per course)
 */

bool school_save_to_file(const School *school, const char *filename)
{
    if (!school || !filename)
    {
        fprintf(stderr, "school_save_to_file: NULL argument\n");
        return false;
    }

    FILE *f = fopen(filename, "w");
    if (!f)
    {
        fprintf(stderr, "school_save_to_file: could not open \"%s\" for writing\n", filename);
        return false;
    }

    fprintf(f, "SCHOOL %s\n", school->name);
    fprintf(f, "COURSES %d\n", school->num_courses);
    for (int i = 0; i < school->num_courses; i++)
    {
        const Course *c = &school->courses[i];
        fprintf(f, "COURSE %s\n", c->name);
        fprintf(f, "STUDENTS %d\n", c->num_students);
        for (int j = 0; j < c->num_students; j++)
        {
            const Student *s = &c->students[j];
            fprintf(f, "STUDENT %d %d %s\n", s->ID, s->grade, s->name);
        }
    }

    fclose(f);
    return true;
}

bool school_load_from_file(School *school, const char *filename)
{
    if (!school || !filename)
    {
        fprintf(stderr, "school_load_from_file: NULL argument\n");
        return false;
    }

    FILE *f = fopen(filename, "r");
    if (!f)
    {
        fprintf(stderr, "school_load_from_file: could not open \"%s\" for reading\n", filename);
        return false;
    }

    char line[1024];
    char name_buf[256];

    if (!fgets(line, sizeof(line), f) || sscanf(line, "SCHOOL %255[^\n]", name_buf) != 1)
    {
        fprintf(stderr, "school_load_from_file: malformed or missing SCHOOL header in \"%s\"\n", filename);
        fclose(f);
        return false;
    }
    if (!school_init(school, name_buf))
    {
        fclose(f);
        return false;
    }

    int num_courses = 0;
    if (!fgets(line, sizeof(line), f) || sscanf(line, "COURSES %d", &num_courses) != 1 || num_courses < 0)
    {
        fprintf(stderr, "school_load_from_file: malformed or missing COURSES count in \"%s\"\n", filename);
        school_free(school);
        fclose(f);
        return false;
    }

    for (int i = 0; i < num_courses; i++)
    {
        if (!fgets(line, sizeof(line), f) || sscanf(line, "COURSE %255[^\n]", name_buf) != 1)
        {
            fprintf(stderr, "school_load_from_file: malformed COURSE header (course %d)\n", i);
            school_free(school);
            fclose(f);
            return false;
        }
        if (!school_add_course(school, name_buf))
        {
            school_free(school);
            fclose(f);
            return false;
        }
        Course *course = &school->courses[school->num_courses - 1];

        int num_students = 0;
        if (!fgets(line, sizeof(line), f) || sscanf(line, "STUDENTS %d", &num_students) != 1 || num_students < 0)
        {
            fprintf(stderr, "school_load_from_file: malformed STUDENTS count for course \"%s\"\n", course->name);
            school_free(school);
            fclose(f);
            return false;
        }

        for (int j = 0; j < num_students; j++)
        {
            if (!fgets(line, sizeof(line), f))
            {
                fprintf(stderr, "school_load_from_file: unexpected end of file while reading students\n");
                school_free(school);
                fclose(f);
                return false;
            }

            int ID, grade, consumed;
            if (sscanf(line, "STUDENT %d %d %n", &ID, &grade, &consumed) != 2)
            {
                fprintf(stderr, "school_load_from_file: malformed STUDENT line: %s", line);
                school_free(school);
                fclose(f);
                return false;
            }
            char *student_name = line + consumed;
            student_name[strcspn(student_name, "\n")] = '\0';

            if (!course_add_student(course, student_name, ID, grade))
            {
                school_free(school);
                fclose(f);
                return false;
            }
        }
    }

    fclose(f);
    return true;
}
