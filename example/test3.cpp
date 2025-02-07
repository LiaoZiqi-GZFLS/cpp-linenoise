#include <iostream>
#include <string>

int main() {
    std::string line;
    while(std::getline(std::cin, line)){
        std::cout << "Enter a line of text: ";
        std::cout << "You entered: " << line << std::endl;
    }
    system("pause");
    return 0;
}