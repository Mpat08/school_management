#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdbool.h>
#include "school.h"

/* Saves a school (all courses and students) to a plain-text file.
   Returns false if the file can't be opened for writing. */
bool school_save_to_file(const School *school, const char *filename);

/* Loads a school from a file previously written by school_save_to_file.
  "school" should not already be initialized (this function calls
  school_init internally). Returns false on any I/O error or malformed
  file, in which case "school" is either untouched or has been freed,
  either way it is not left half-built. */
bool school_load_from_file(School *school, const char *filename);

#endif
