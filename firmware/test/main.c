#include <external/unity/unity.h>
#include <stdio.h>

int suite_commands();

int main(void) {
    int ret = 0;
    ret += suite_commands();
    return ret;
}