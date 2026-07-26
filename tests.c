#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "student.h"
#include "course.h"
#include "school.h"
#include "file_io.h"

static void test_student_basic(void);
static void test_course_add_and_average(void);
static void test_course_grows_past_initial_capacity(void);
static void test_school_add_course_and_lookup(void);
static void test_file_roundtrip(void);
static void test_error_handling_null_and_missing_inputs(void);

int main(void)
{    
    test_student_basic();
    test_course_add_and_average();
    test_course_grows_past_initial_capacity();
    test_school_add_course_and_lookup();
    test_file_roundtrip();
    test_error_handling_null_and_missing_inputs();

    printf("\nAll tests passed.\n");
    
    return 0;
}

static void test_student_basic(void)
{
    Student s;
    assert(student_init(&s, "Ada Lovelace", 1, 95));
    assert(strcmp(s.name, "Ada Lovelace") == 0);
    assert(s.ID == 1);
    assert(s.grade == 95);

    assert(student_set_name(&s, "Ada L."));
    assert(strcmp(s.name, "Ada L.") == 0);

    student_free(&s);
    printf("test_student_basic passed\n");
}

static void test_course_add_and_average(void)
{
    Course c;
    assert(course_init(&c, "Algorithms"));
    assert(c.num_students == 0);
    assert(c.average_grade == 0.0f);

    assert(course_add_student(&c, "Grace Hopper", 10, 90));
    assert(course_add_student(&c, "Alan Turing", 11, 100));
    assert(c.num_students == 2);
    assert(c.average_grade == 95.0f);

    /* duplicate ID must be rejected, and must not change the course */
    assert(!course_add_student(&c, "Someone Else", 10, 50));
    assert(c.num_students == 2);

    Student *found = course_find_student(&c, 11);
    assert(found != NULL);
    assert(strcmp(found->name, "Alan Turing") == 0);
    assert(course_find_student(&c, 999) == NULL);

    assert(course_update_student_grade(&c, 10, 70));
    assert(c.average_grade == 85.0f);
    assert(!course_update_student_grade(&c, 999, 70));

    course_free(&c);
    printf("test_course_add_and_average passed\n");
}

static void test_course_grows_past_initial_capacity(void)
{
    Course c;
    assert(course_init(&c, "Big Course"));
    int initial_capacity = c.capacity;

    for (int i = 0; i < initial_capacity + 3; i++)
    {
        char name[32];
        snprintf(name, sizeof(name), "Student%d", i);
        assert(course_add_student(&c, name, i, 80));
    }
    assert(c.num_students == initial_capacity + 3);
    assert(c.capacity >= c.num_students);

    course_free(&c);
    printf("test_course_grows_past_initial_capacity passed\n");
}

static void test_school_add_course_and_lookup(void)
{
    School sch;
    assert(school_init(&sch, "Test University"));
    assert(school_add_course(&sch, "Physics I"));
    assert(school_add_course(&sch, "Chemistry I"));
    assert(!school_add_course(&sch, "Physics I")); /* duplicate rejected */
    assert(sch.num_courses == 2);

    assert(school_course_in(&sch, "Chemistry I"));
    assert(!school_course_in(&sch, "Biology I"));

    Course *phys = school_find_course(&sch, "Physics I");
    assert(phys != NULL);
    assert(course_add_student(phys, "Marie Curie", 1, 99));
    assert(school_student_in(&sch, 1));
    assert(!school_student_in(&sch, 42));

    float avg;
    assert(school_overall_average(&sch, &avg));

    const Course *best;
    assert(school_highest_avg_course(&sch, &best));
    assert(strcmp(best->name, "Physics I") == 0);

    school_free(&sch);
    printf("test_school_add_course_and_lookup passed\n");
}

static void test_file_roundtrip(void)
{
    School sch;
    assert(school_init(&sch, "Roundtrip School"));
    assert(school_add_course(&sch, "Course A"));
    Course *ca = school_find_course(&sch, "Course A");
    assert(course_add_student(ca, "Student One", 1, 88));
    assert(course_add_student(ca, "Student Two", 2, 72));

    const char *path = "test_roundtrip.txt";
    assert(school_save_to_file(&sch, path));

    School loaded;
    assert(school_load_from_file(&loaded, path));
    assert(strcmp(loaded.name, sch.name) == 0);
    assert(loaded.num_courses == sch.num_courses);
    assert(loaded.courses[0].num_students == 2);
    assert(course_find_student(&loaded.courses[0], 1) != NULL);
    assert(loaded.courses[0].average_grade == ca->average_grade);

    school_free(&sch);
    school_free(&loaded);
    remove(path);
    printf("test_file_roundtrip passed\n");
}

static void test_error_handling_null_and_missing_inputs(void)
{
    assert(!student_init(NULL, "x", 1, 1));
    assert(!course_init(NULL, "x"));
    assert(!school_init(NULL, "x"));

    Course c;
    assert(course_init(&c, "Edge Cases"));
    assert(!course_add_student(&c, NULL, 1, 1));
    assert(course_find_student(&c, 1) == NULL);
    course_free(&c);

    School missing;
    assert(!school_load_from_file(&missing, "this_file_should_not_exist.txt"));

    printf("test_error_handling_null_and_missing_inputs passed\n");
}