#include "libmotor.h"
#include <vector>
#include <string>

int main() {
    libmotor();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    libmotor_print_vector(vec);
}
