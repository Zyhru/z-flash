/*  
* z-flash
* Status: Development
* Currently: Testing (Not caring what kind of code I'm writing) 
*/

#include <iostream> 
#include "api.h" 

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

struct Flag {
    bool help;
    bool version;
    bool file;
    bool directory;
};

void version() {
    std::cout << "zflash version v.0.1.0" << std::endl;
}

void help() {
    const std::string usageMessage = R"(
    Usage: zflash [OPTIONS]

    Options:
      -f, <path>                 Specify the path to the file to process.
      -d, <dir_path>             Specify the path to the directory to process.
      --help                     Display this help message and exit.
      --version                  Display the version information and exit.

    Description:
      The zflash utility generates flashcards from the specified file or directory. 
      You must provide either a file or a directory to process.

      - Use -f or --file to specify a single markdown file containing content to generate flashcards.
      - Use -d or --directory to specify a directory containing multiple markdown files to process.

    Examples:
      zflash -f /path/to/file.md
      zflash -d /path/to/directory
    )";

    std::cout << usageMessage << std::endl; 
}

int main(int argc, char **argv) {
    API api;

    if(argc < 2) {
        help();
        throw std::out_of_range("invalid range of arguments");
    } 
    
    Flag flag = {
        .help      = false,
        .version   = false,
        .file      = false,
        .directory = false,
    };
    
    std::string cmd = argv[1];
    std::string path; 
    for(int i = 1; i < argc; ++i) {
        if(i + 1 == argc) {
            if(cmd == "-h" || cmd == "--help") {
                flag.help = true;
            } else if(cmd == "-v" || cmd == "--version") {
                flag.version = true;
            } else if(cmd == "-f" || cmd == "-d") {
                help();
            }
            break;
        } else if(i + 1 < argc) {
            if(cmd == "-f" || cmd == "--file") {
                flag.file = true;
            } else if(cmd == "-d" || cmd == "--directory") {
                flag.directory = true;
            } else {
                help();
            }
            
            path = argv[2];
            break;
        } else {
            //return 1;
        }
    }

    zflash_intro();
    if(flag.help) {
        help();
        return 0;
    }

    if(flag.version) {
        version();
        return 0;
    }
    
    if(flag.file) {
        api.process(path);
        return 0;
    }
   
    if(flag.directory) {
        //api.readDirectory(path);
        return 0;
    }
}
