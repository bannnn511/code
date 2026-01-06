#include <iostream>

int getValue() {
    return 5;
}

int max(int x, int y) {
    return x > y ? x : y;
}

constexpr int cmax(int a, int b) {
    return a > b ? a : b;
}


int main() {
    std::cout << "Enter your age: ";
    int age{};
    std::cin >> age;

    const int constAge{age};
    age = 5;

    int test = getValue();
    test = 6;

    constexpr double gravity{9.8};
    constexpr int sum{4 + 5};
    constexpr int something{sum};
    std::cout << "Enter your age: " << std::endl;
    int age2{};
    std::cin >> age2;


    // constexpr int myAge{age};

    std::cout << "Age: " << age2 << std::endl;

    int m1{max(5, 6)};
    const int m2{max(6, 7)};
    // constexpr int m3{max(7, 8)};

    int m4{cmax(5, 6)};
    const int m5{cmax(6, 7)};
    constexpr int m6{cmax(7, 9)};

    return 0;
}
