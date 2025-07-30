#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#include "errors.h"

/**
 * Given two integers, returns the smaller of the two.
 *
 * @param a The first integer.
 * @param b The second integer.
 * @return The smallest of the two integers.
 */
int min(int a, int b);

/**
 * Runs `malloc` with the given size, and checks that it ran successfully (returned a non-null pointer).
 * May fail if allocating memory did not succeed.
 *
 * @param size The amount of memory to allocate, in bytes.
 * @param ptr The pointer where the pointer to the newly allocated memory will be stored.
 * @return The operation's result.
 */
result_t check_malloc(size_t size, void **ptr);

/**
 * This macro must be run in a function that returns `result_t`.
 * The parameter must be a variable that is a pointer to a pointer to a non-void type.
 * It runs `check_malloc` with the size of the pointer's underlying type and stores the new pointer there.
 * If allocation fails, causes the running function to return an "out of memory" error.
 *
 * @param ptr The pointer where the pointer to the newly allocated memory will be stored.
 */
#define TRY_MALLOC(ptr) TRY(check_malloc(sizeof(**ptr), (void **) ptr))

/**
 * Creates a copy of the given string, and stores its pointer in `out`.
 * The resulting pointer must be freed when it is no longer needed.
 * May fail if memory allocations did not succeed.
 *
 * @param orig The string to clone
 * @param out The pointer where the pointer to the cloned string will be stored.
 * @return The operation's result.
 */
result_t clone_string(char *orig, char **out);

/**
 * Creates a new string whose value is strings `a` and `b` joined together, and stores its pointer in `out`.
 * The resulting pointer must be freed when it is no longer needed.
 * May fail if memory allocations did not succeed.
 *
 * @param a The first string.
 * @param b The second string.
 * @param out The pointer where the pointer to the newly created string will be stored.
 * @return The operation's result.
 */
result_t join_strings(char *a, char *b, char **out);

#endif
