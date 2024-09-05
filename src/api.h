#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cstring>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Construct a prompt with the buffer
// Prepare to send as a request
// grab response, parse
// create object of flashcard

using json = nlohmann::json;
    
class API {
public:

    struct Memory {
        char* response;
        size_t size;
    };

    struct FlashCard {
        std::string question;
        std::string answer;
    };

    struct RequestBody { 
        std::vector<std::string> req;
        std::string d;
    };

    Memory chunk;
    RequestBody rb;

    API();
    ~API();

    /* the magic */
    void process(std::string& path);

    
    /* -f | -d commands */
    bool readFile(std::string& path);
    //void readDirectory(std::string& path);

    static size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp);

private:
    struct curl_slist* headers = NULL;
    std::vector<std::string> buffer;
    std::string key;
    std::string payload;
    CURL* curl;
    

    /* function logic */
    void init();
    std::vector<std::string> parse(const std::string&& first, const char second, std::string response);
    std::vector<FlashCard> generateFlashCards(std::vector<std::string>& q, std::vector<std::string>& a);
    bool sendRequest();
    void grabResponse();
    std::string getKey(const char* api_key);

};

// transfer buffer to prompt
