#include "suite.h"

int suite_shared() {
    int ret = 0;
    ret += suite_shared_primitive_data();
    return ret;
}