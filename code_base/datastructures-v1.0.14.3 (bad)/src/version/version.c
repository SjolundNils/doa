/*
 * File to hold library version information for the code base for the
 * "Datastructures and algorithms" courses at the Department of
 * Computing Science, Umea University.
 *
 * Author: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2023-01-14: First public version.
 */

#include <util.h>

// Create a struct with all version information to be stored in the
// library file.
struct {
    const char* long_version;
    const char* version;
    const unsigned major;
    const unsigned minor;
    const unsigned revision;
    const unsigned patch;
} code_base_version = {
    CODE_BASE_LONG_VERSION, CODE_BASE_VERSION, CODE_BASE_MAJOR_VERSION,
    CODE_BASE_MINOR_VERSION, CODE_BASE_REVISION, CODE_BASE_PATCH
};
