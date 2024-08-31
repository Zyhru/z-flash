/*  
* z-flash
* Status: Development
* Currently: Testing (Not caring what kind of code I'm writing) 
*/

#include <iostream>
#include "args.h" 

void zflash_intro() {

    std::cout << "\033[1;31m";  // Red
    std::cout << R"( ______     __         ______     __         ______     __  __    )" << std::endl;
    std::cout << "\033[1;33m";  // Yellow
    std::cout << R"(/\___  \   /\ \       /\  ___\   /\ \       /\  ___\   /\ \_\ \   )" << std::endl;
    std::cout << "\033[1;32m";  // Green
    std::cout << R"(\/_/  /_   \ \ \____  \ \ \__ \  \ \ \____  \ \___  \  \ \  __ \  )" << std::endl;
    std::cout << "\033[1;36m";  // Cyan
    std::cout << R"(  /\_____\  \ \_____\  \ \_____\  \ \_____\  \/\_____\  \ \_\ \_\ )" << std::endl;
    std::cout << "\033[1;34m";  // Blue
    std::cout << R"(  \/_____/   \/_____/   \/_____/   \/_____/   \/_____/   \/_/\/_/ )" << std::endl;
    std::cout << "\033[0m";  // Reset text color to default
}

int main(int argc, char **argv) {
    zflash_intro();
    Args arguments;
    arguments.processArgs(argc, argv);
}
