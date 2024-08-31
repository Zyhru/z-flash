#pragma once

#include <iostream>
#include <fstream>
#include <vector> 
//#include <functional> 
    


class Args {
public:
    struct Flag {
        bool help;
        bool version;
        bool file;
        bool directory;
    };

    enum CommandType {
        FILE,
        DIRECTORY,
        VERSION,
        HELP
    };

    typedef struct {
        std::string command;
        std::string decsription;
        //std::function<void()> execute;
        enum CommandType type;
    } Command;

    Args();
    void processArgs(int argc, char **argv);
    void initCommands();
    Flag parseFlags();

private:
    std::vector<Command> commands;
    std::string file_content;
    std::vector<std::string> directories_content;


    bool processFile(std::string& path);
    bool processDirectory(std::string& path);
    void help();
    void version();
};
