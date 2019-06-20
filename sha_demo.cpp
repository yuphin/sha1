#include <iostream>
#include "sha.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    sha1::digest("aaa");
    return 0;
}