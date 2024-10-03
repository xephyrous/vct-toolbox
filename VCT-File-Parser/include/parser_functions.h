#ifndef _PARSER_FUNCTIONS_H
#define _PARSER_FUNCTIONS_H

#include <string>

#include "plaintext_parser_error.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/**
 * \brief A helper function to save a string to a file
 * \note This function assumes the file and data are already validated and accessible
 * \param data The data to write to the file
 * \param path The location of the file to create / write to
 */
inline void save_to_file(const std::string& data, const std::string& path) {
    std::ofstream stream;
    stream.open(path, std::ios::trunc);
    stream << data;
    stream.close();
}

/**
 * \brief 
 * \param input_json 
 * \param options 
 */
static void parse_match_data(json input_json, std::vector<std::string> options) {
    std::string output;
    
    // Parse match data
    
    save_to_file(output, options[0]);
}

#endif