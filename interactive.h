#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <stdbool.h>
#include "school.h"

/* Builds a School by prompting the user on stdin for its courses and
 * students. Validates all numeric input and re-prompts on invalid entries
 * instead of crashing or reading garbage. Returns true on success; `out`
 * is only valid if this returns true (on failure it has already been
 * freed, if it was partially built). */
bool interactive_create_school(School *out);

#endif
