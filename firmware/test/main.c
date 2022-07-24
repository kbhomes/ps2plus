#include <external/unity/unity.h>
#include <stdio.h>

int suite_commands();
int suite_shared();

int main(void) {
    int ret = 0;
    ret += suite_commands();
    ret += suite_shared();
    return ret;
}