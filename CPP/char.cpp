#include <iostream>

int main() {
    char ch2{'a'};
    std::cout << ch2 << std::endl;

    char ch97{97};
    std::cout << ch97 << std::endl;

    std::cout << "Input a keyboard character: " << std::endl;
    char ch{};
    std::cin >> ch;
    std::cout << "You entered: " << ch << std::endl;

    std::cout << "Input a keyboard character: " << std::endl;
    char ch3{};
    std::cin.get(ch3);
    std::cout << "You entered: " << ch3 << std::endl;

    return 0;
}
