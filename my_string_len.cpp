#include <cstddef>

#include "my_string_len.h"

int count_till_zero(const char * test) {
    for (std::size_t i = 0; ; i++) {
        if (test[i] == '\0') {
            return i;
        }
    }
}
