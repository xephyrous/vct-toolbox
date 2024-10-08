#ifndef _PLAINTEXT_PARSER_H
#define _PLAINTEXT_PARSER_H

#include <fstream>
#include <vector>
#include <functional>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/**
 * \brief Parses a JSON object into valid english plaintext based on an inputted parsing function
 */
class plaintext_parser {
public:
    plaintext_parser(const json& input_json);
    
    uint8_t set_output(const std::string& path);
    void set_parse_function(std::function<void(json, std::vector<std::string>)> input_function);
    void set_parse_options(const std::vector<std::string>& options);
    void parse() const;
    
private:
    json json_data;
    std::ifstream save_path;
    std::function<void(json, std::vector<std::string>)> parse_function = nullptr;
    std::vector<std::string> parse_opts;
};

#endif