#include <iostream>
#include <string>
#include <string_view>

void printSV(std::string_view s) {
    std::cout << s << std::endl;
}

int main() {
    // std::string name{"AN"};
    // std::cout << "Name: " << name << std::endl;
    //
    // name = "An Ha";
    // std::cout << "Name: " << name << std::endl;
    //
    // std::cout << "Enter full name:ge ";
    // std::string fullName;
    // // std::cin >> fullName;
    // std::getline(std::cin, fullName);
    //
    // std::cout << "Full name: " << fullName << std::endl;


    // std::cout << "Pick 1 or 2: ";
    // int choice{};
    // std::cin >> choice;
    //
    // std::cout << "Now enter your name: ";
    // std::string name{};
    // std::getline(std::cin >> std::ws, name); // note: added std::ws here
    //
    // std::cout << "Hello, " << name << ", you picked " << choice << '\n';
    // std::cout << "Name " << name << " has " << name.length() << " characters" << std::endl;

    using namespace std::string_literals;
    using namespace std::string_view_literals;

    std::cout << "foo\n"; // no suffix is a C-string style literal
    std::cout << "bar"s; // s suffix is a std::string literal
    std::cout << "test"sv; // sv suffix is a std::string_view literal

    // std::cout << "Enter your full name: ";
    // std::string name;
    // std::getline(std::cin, name);
    // std::cout << "Enter your age: ";
    // int age;
    // std::cin >> age;
    //
    // std::cout << "Your age + length name is: " << age + name.length() << std::endl;

    constexpr std::string_view s{"Hello, world!"}; // s is a string symbolic constant

    std::cout << s << std::endl;

    printSV("Hello...");
    printSV(s);

    return 0;
}
