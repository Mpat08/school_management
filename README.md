# School Management System

A C project for managing schools, courses, and students — built around
dynamic, ownership-safe data structures rather than fixed-size arrays.

## Structure

Each entity gets its own module (header + source), instead of one file:

- `student.h` / `student.c` — a single student (name, ID, grade)
- `course.h` / `course.c` — a course with a dynamically growing array
  of students (starts at capacity 4, doubles via `realloc` as needed)
- `school.h` / `school.c` — a school with a dynamically growing array of
  courses, same growth strategy
- `file_io.h` / `file_io.c` — save a school to a plain-text file and load
  it back
- `interactive.h` / `interactive.c` — builds a school from stdin, with
  input validation and re-prompting on bad numeric input
- `main.c` — a runnable demo exercising every module (no stdin required)
- `tests.c` — an assert-based test suite covering normal use, edge cases,
  and error paths

## Design decisions

**Pointers, not values, for anything that mutates.** Every function that
changes a struct takes a pointer to it; read-only functions take a
`const` pointer.

**Every allocating function returns `bool` (or a pointer/NULL) and checks
its inputs.** `malloc`/`realloc` failures are checked and reported rather
than assumed to succeed; `NULL` arguments are checked before dereferencing;
duplicate IDs/course names are rejected rather than silently overwriting
data. Nothing here calls `exit()` — callers decide what a failure means
for them.

**Dynamic arrays over fixed-size ones.** `course_add_student` and
`school_add_course` grow their backing array automatically, so courses
and schools can be built up incrementally instead of requiring the final
count up front.

## Requirements

This project builds with a plain Makefile and gcc, so it needs a
Unix-like environment:

- **macOS / Linux** — works out of the box (`make` and `gcc`/`clang` are
  already present, or a single package install away).
- **Windows** — use WSL (Windows Subsystem for Linux), or install
  MSYS2/MinGW, which provides `make`, `gcc`, and the Unix shell commands
  the Makefile relies on (e.g. `rm -f` in the `clean` target). It won't
  build directly from `cmd.exe` or plain PowerShell without one of these.

## Building

```
make            # builds ./school_management (the demo)
make test       # builds and runs ./run_tests (the test suite)
make clean      # removes build artifacts
```

The demo (`main.c`) needs no input — it builds two schools with hardcoded
data and walks through printing, pass/fail filtering, school-wide stats,
live updates, dynamic growth, and file save/load.

To build the fully interactive version instead (reads a school from
stdin), swap `main.c`'s body for a call to `interactive_create_school`.

## Testing

`tests.c` uses plain `assert()` — no external test framework required:

```
gcc -Wall -Wextra -o run_tests tests.c student.c course.c school.c file_io.c interactive.c
./run_tests
```

It covers: basic construction, average recalculation, capacity growth
past the initial allocation, duplicate rejection, lookups on missing
IDs/names, a full file save/load round-trip, and NULL/missing-file error
paths.

Note: AddressSanitizer (`-fsanitize=address`) is a useful addition during
development for catching memory errors, but is left out of the default
`make test` target since it can fail silently on some macOS setups
without extra codesigning entitlements. Feel free to add `-fsanitize=address` to the
compile line yourself if your environment supports it.
