#include <stdio.h>
#include "student.h"
#include "course.h"
#include "school.h"
#include "file_io.h"
#include "interactive.h"


int main(void)
{
    
    School ucf;
    // INTERACTIVE VERSION
    /*
    if (!interactive_create_school(&ucf))
    {
        fprintf(stderr, "Failed to build school from input\n");
        return 1;
    }
    printf("\n\n");
    school_print(&ucf);
    school_free(&ucf);
    */
    
    // DEMO VERSION
    if (!school_init(&ucf, "UCF"))
    {
        fprintf(stderr, "Failed to create school\n");
        return 1;
    }

    school_add_course(&ucf, "Data Structures");
    school_add_course(&ucf, "Calculus II");
    school_add_course(&ucf, "Circuits Lab");

    Course *ds = school_find_course(&ucf, "Data Structures");
    course_add_student(ds, "Amara Chen", 101, 92);
    course_add_student(ds, "Luis Ortega", 102, 68);
    course_add_student(ds, "Priya Nair", 103, 74);

    Course *calc = school_find_course(&ucf, "Calculus II");
    course_add_student(calc, "Amara Chen", 101, 55);
    course_add_student(calc, "Jamal Reed", 104, 88);

    Course *circuits = school_find_course(&ucf, "Circuits Lab");
    course_add_student(circuits, "Luis Ortega", 102, 81);
    course_add_student(circuits, "Priya Nair", 103, 79);

    printf("========================================\n");
    printf(" FULL SCHOOL OVERVIEW\n");
    printf("========================================\n\n");
    school_print(&ucf);

    printf("========================================\n");
    printf(" ERROR HANDLING DEMO\n");
    printf("========================================\n");
    if (!course_add_student(ds, "Duplicate ID Test", 101, 50))
    {
        printf("(expected) rejected duplicate student ID 101 in Data Structures\n");
    }
    if (!school_add_course(&ucf, "Data Structures"))
    {
        printf("(expected) rejected duplicate course name \"Data Structures\"\n");
    }
    if (!course_update_student_grade(ds, 9999, 100))
    {
        printf("(expected) rejected grade update for nonexistent student ID 9999\n");
    }
    printf("\n");

    printf("========================================\n");
    printf(" COURSES FOR STUDENT ID 101\n");
    printf("========================================\n");
    school_print_student_courses(&ucf, 101);
    printf("\n");

    printf("========================================\n");
    printf(" PASS / FAIL BREAKDOWN (Data Structures, passing = 70)\n");
    printf("========================================\n");
    course_print_passed_students(ds, 70.0f);
    printf("\n");
    course_print_failed_students(ds, 70.0f);
    printf("\n");

    printf("========================================\n");
    printf(" SCHOOL-WIDE COURSE STANDINGS (passing = 75)\n");
    printf("========================================\n");
    school_print_passed_courses(&ucf, 75.0f);
    school_print_failed_courses(&ucf, 75.0f);

    float overall;
    if (school_overall_average(&ucf, &overall))
    {
        printf("Overall average grade across all courses: %f\n", overall);
    }

    const Course *best;
    if (school_highest_avg_course(&ucf, &best))
    {
        printf("\nCourse with the HIGHEST average grade:\n");
        course_print(best);
    }
    printf("\n");

    printf("========================================\n");
    printf(" UPDATE DEMO: bump Jamal Reed's Calculus II grade to 95\n");
    printf("========================================\n");
    printf("Calculus II average BEFORE: %f\n", calc->average_grade);
    course_update_student_grade(calc, 104, 95);
    printf("Calculus II average AFTER:  %f\n\n", calc->average_grade);

    printf("========================================\n");
    printf(" UPDATE DEMO: rename \"Circuits Lab\" to \"Digital Circuits\"\n");
    printf("========================================\n");
    course_set_name(circuits, "Digital Circuits");
    printf("New name: %s\n\n", circuits->name);

    printf("========================================\n");
    printf(" DYNAMIC GROWTH DEMO: enroll 6 more students in Data Structures\n");
    printf("========================================\n");
    printf("Capacity before: %d\n", ds->capacity);
    for (int i = 0; i < 6; i++)
    {
        char name[32];
        snprintf(name, sizeof(name), "Extra Student %d", i);
        course_add_student(ds, name, 200 + i, 60 + i);
    }
    printf("Capacity after enrolling 6 more students: %d (num_students = %d)\n\n", ds->capacity, ds->num_students);

    printf("========================================\n");
    printf(" FILE SAVE / LOAD DEMO\n");
    printf("========================================\n");
    const char *save_path = "ucf_save.txt";
    if (school_save_to_file(&ucf, save_path))
    {
        printf("Saved school to %s\n", save_path);
    }

    School reloaded;
    if (school_load_from_file(&reloaded, save_path))
    {
        printf("Reloaded school \"%s\" with %d course(s):\n", reloaded.name, reloaded.num_courses);
        for (int i = 0; i < reloaded.num_courses; i++)
        {
            printf(" - %s (%d students, avg %.2f)\n",
                   reloaded.courses[i].name,
                   reloaded.courses[i].num_students,
                   reloaded.courses[i].average_grade);
        }
        school_free(&reloaded);
    }
    printf("\n");

    // Second school, to demo cross-school comparisons
    School east_valley;
    school_init(&east_valley, "East Valley Tech");
    school_add_course(&east_valley, "Data Structures");
    school_add_course(&east_valley, "Signals and Systems");
    Course *ev_ds = school_find_course(&east_valley, "Data Structures");
    course_add_student(ev_ds, "Devon Blake", 201, 84);
    Course *ev_sig = school_find_course(&east_valley, "Signals and Systems");
    course_add_student(ev_sig, "Nina Farouk", 202, 90);

    printf("========================================\n");
    printf(" COMPARING SCHOOLS\n");
    printf("========================================\n");
    school_print_common_courses(&ucf, &east_valley);
    school_print_unique_courses(&ucf, &east_valley);
    printf("\n");

    school_free(&ucf);
    school_free(&east_valley);

    return 0;
}
