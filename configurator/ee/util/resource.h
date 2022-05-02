#ifndef UTIL_RESOURCE_H
#define UTIL_RESOURCE_H

/**
 * Creates extern declarations for the given resource. Resources are files placed
 * anywhere in the `resources` directory which are converted to binary and linked
 * into the executable (see the EE Makefile at `configurator/ee/Makefile`).
 * 
 * The name of the resource to use with these macros is the "ld-ified" version of
 * the resource's filename (relative to the `resources` directory). This is just 
 * the filename with non-alphanumeric characters replaced with _ (underscore). 
 * For example, a resource file at `resources/fonts/playstation.ttf` will have a
 * resource name of `fonts_playstation_ttf`.
 * 
 * This macro must be used at the top-level of any translation unit that wants to
 * access a resource using the other macros in this file.
 */
#define RESOURCE_EXTERNS(NAME) \
  extern const uint8_t _binary_resources_##NAME##_start; \
  extern const uint8_t _binary_resources_##NAME##_end; \
  extern const uint8_t _binary_resources_##NAME##_size;

/**
 * Returns a `void *` pointer to the given resource.
 */
#define RESOURCE_POINTER(NAME) ((void *)(&_binary_resources_##NAME##_start))

/**
 * Returns a `size_t` representing the size of the given resource.
 */
#define RESOURCE_SIZE(NAME) ((size_t)(&_binary_resources_##NAME##_size))

#endif