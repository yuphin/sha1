#include <iostream>
#include "sha.h"
int main() {
    std::cout << sha1::digest("Hello World!") << std::endl;
    return 0;
}