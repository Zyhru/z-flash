#include <stdexcept>
#include "args.h"

Args::Args() {
    initCommands();
}

void Args::initCommands() {
    commands.push_back({"-f",     "-f to specify a single markdown file containing content to generate flashcards",  CommandType::FILE});
    commands.push_back({"-d",     "-d to specify multiple markdown files containing content to generate flashcards", CommandType::DIRECTORY});
    commands.push_back({"--help", "print help usage",                                                                CommandType::HELP});
    commands.push_back({"-v",     "print zflash version",                                                            CommandType::VERSION});
}


void Args::processArgs(int argc, char **argv) {
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
        }
    }

    if(flag.help) {
        help();
        return;
    }

    if(flag.version) {
        version();
        return;
    }
    
    if(flag.file) {
        processFile(path);
        return;
    }
   
    if(flag.directory) {
        processDirectory(path);
        return;
    }
}

                        /* Commands */
/*------------------------------------------------------------- */
void Args::version() {
    std::cout << "zflash version v.0.1.0" << std::endl;
}

void Args::help() {
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

bool Args::processFile(std::string& path) {
    // handle extension, should only be .md
    size_t md_pos = path.find(".");
    if(md_pos != std::string::npos) {
        std::string extension = path.substr(md_pos, path.size());
        std::cout << "File Extension: " << extension << std::endl;
        if(extension != ".md") {
            std::cerr << "file must .md (Markdown)" << std::endl;
            return false;
        }
    }
    

    std::ifstream file(path);
    if(!file.is_open()) {
        std::cerr << "failed to open file or file doesnt exist @ path: "  << path << std::endl; 
        return false;
    }

    std::string line;
    int size = 0;
    while(std::getline(file, line)) {
        file_content += line;
        file_content += "\n";
    }

    file_content.pop_back();

    std::cout << "size: " << file_content.size() << std::endl;
    if(file_content.size() == 0) {
        std::cerr << "file is empty" << std::endl;
        return false;
    }


    file.close();
    return true;
}

bool Args::processDirectory(std::string& path) {
    std::cout << "under construction" << std::endl;
    return true;


}
