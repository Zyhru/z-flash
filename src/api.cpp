#include "api.h"

API::API() {
    init();
}

API::~API() {
    free(chunk.response);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}


std::string API::getKey(const char* api_key) {
    std::string key;
    if(const char *env = std::getenv(api_key)) {
        key = env;
    } else {
        throw std::runtime_error("could not find api key!");
    }

    return key;
}

bool API::readFile(std::string& path) {
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
    std::string file_content;
    int size = 0;
    while(std::getline(file, line)) {
        file_content +=  line;
        file_content +=  "\n";
    }

    file_content.pop_back();

    std::cout << "File Info:\nSize: " << file_content.size() << "\nContents: \n" << file_content <<std::endl;
    if(file_content.size() == 0) {
        std::cerr << "file is empty" << std::endl;
        return false;
    }
    
    buffer.push_back(file_content);
    file.close();

    return true;
}

/*
* @brief: sets the environment key, initializes libcurl, and creates the header to be send as part of the request 
*/
void API::init() {
    std::cout << "initalizing z-flash" << std::endl;
    try {
         key = getKey("OPENAI_API_KEY");
    } catch (std::exception& e) {
        std::cerr << "error: "  << e.what() << std::endl;
        return;
    }
    
    std::vector<std::string> request = {
        "Content-Type: application/json",
        "Authorization: Bearer " + key,
        "https://api.openai.com/v1/chat/completions",
    };

    rb = {
        .req = request,
    };
    
    chunk.response = (char *) malloc(1);
    chunk.size = 0;
    payload = "zai.json";
   
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(!curl) {
        std::cerr << "unable to initialize libcurl" << std::endl;
        return;
    }
   
}

/*
* @brief: Puts the logic together of z-flash.
* read file -> send request -> display cards
*/
void API::process(std::string& path) {
    std::cout << "Executing..." << std::endl;
    if(readFile(path)) {
        std::cout << "Success reading file!" << std::endl;
        if(sendRequest()) {
            std::cout << "Success sending request!\n" << chunk.response << std::endl;
            std::fstream json_payload(payload);
            if(!json_payload.is_open()) {
                std::cerr << "Failed to open file: payload.json" << std::endl;
                return;
            }
            
            std::string api_res(chunk.response);
            std::cout << "API Results:\n" << api_res << std::endl;
            json json_response = json::parse(api_res); 
            json content = json_response["choices"][0]["message"]["content"];
            std::string res = content.dump();
            json_payload.close();
           
            // TODO: cleanup logic
            std::vector<FlashCard> flash_cards;
            std::vector<std::string> questions = parse("Q: ", '?', res);
            std::vector<std::string> answers   = parse("A: ", '\n', res);
            if(questions.size() != answers.size()) {
                throw std::logic_error("Q&A are not the same size.");
            } 


            // create flashcards
            for(int i = 0; i < questions.size(); ++i) {
                FlashCard card = {questions[i], answers[i]}; 
                flash_cards.push_back(card);
            }

            std::cout << "QA:" << std::endl;
            for(auto& card : flash_cards) {
                std::cout << "Q: " << card.question << "\nA: " << card.answer << std::endl;
            }
            
            std::cout << "Finished request!" << std::endl;
            
            /* UI(take in the response, to render) */
            
        } else {
            std::cerr << "failed to send request" << std::endl;
        }
    } else {
        std::cerr << "error: unable to process file." << std::endl;
        return;
    }
}


/*
* @brief: sends an API request to OpenAI 
* The response consists of a prompt + the content of the file(s) 
* that have been inputed via CLI
*/
bool API::sendRequest() {
    std::string prompt = "generate me 10 flash cards with a Q&A based on my markdown notes\n(format correctly)";
    prompt += buffer[0];

    json data = {
        {"model", "gpt-3.5-turbo"},
        {"messages", json::array({{{"role", "user"}, {"content", prompt.c_str()}}})},
    };

    rb.d = data.dump();

    if(curl) {
        CURLcode result;
        headers = curl_slist_append(headers, rb.req[0].c_str());
        headers = curl_slist_append(headers, rb.req[1].c_str());

        curl_easy_setopt(curl, CURLOPT_URL, rb.req[2].c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, rb.d.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

        result = curl_easy_perform(curl);
        if(result != CURLE_OK) {
            throw std::runtime_error("failed to send request");
        } else {
            return true;
        }
    }

    return false; // this line will never be reached?
}

std::vector<std::string> API::parse(const std::string&& first, const char second, std::string response) { // std::string& response, i
    size_t pos = 0;
    int size = first.size();
    std::vector<std::string> results;
    while((pos = response.find(first)) != std::string::npos) {
        std::string res;
        //    x
        // A: What is (size 3), 
        for(int i = pos + size; i < response.size(); ++i) {
            if(response[i] == second) break;
            res += response[i];
        }

        results.push_back(res);
        response.erase(0, pos + size);
    }

    std::cout << "Results Size: " << results.size() << std::endl;
    std::cout << "Parsing response is complete!" << std::endl;
    return results;
}

size_t API::write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    struct Memory* mem = (struct Memory *)userp;

    char* ptr = (char *)realloc(mem->response, mem->size + real_size + 1);
    if(!ptr) {
        std::cout << "unable to reallocate memory";
        return 0;
    }

    mem->response = ptr;
    std::memcpy(&(mem->response[mem->size]), buffer, real_size);
    mem->size += real_size;
    mem->response[mem->size] = 0;
    return real_size;
}
