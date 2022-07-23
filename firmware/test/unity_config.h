#include <external/ansi-color.h>

// Additional newline between suites
#define UNITY_OUTPUT_COMPLETE() UNITY_PRINT_EOL();

// Enable Unity printf formatting functionality
#define UNITY_INCLUDE_PRINT_FORMATTED

// Colorize pass/fail messages
#define UNITY_OUTPUT_COLOR

// Abbreviate test result lines and indent
#define UNITY_EXCLUDE_FILE_IN_TEST_RESULT
#define UNITY_EXCLUDE_LINE_NUMBER_IN_TEST_RESULT
#define UNITY_TEST_RESULT_PREFIX() 
#define UNITY_TEST_RESULT_PASS_PREFIX() printf(BGRN "✔" CRESET " ");
#define UNITY_TEST_RESULT_FAIL_PREFIX() printf(BRED "✖" CRESET " ");
#define UNITY_TEST_RESULT_MESSAGE_PREFIX() printf("\n  ↳");

// Additional newline between suites
#define UNITY_OUTPUT_COMPLETE() UNITY_PRINT_EOL();