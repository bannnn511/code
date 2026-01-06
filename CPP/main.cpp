#include <iostream>

int main() {
    int n{3};
    float f{};
    std::memcpy(&f, &n, sizeof(float));
    std::cout << f << std::endl;

    return 0;
}
