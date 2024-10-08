#ifndef _PARSER_FUNCTIONS_H
#define _PARSER_FUNCTIONS_H

#include <string>
#include <map>
#include <regex>

#include "plaintext_parser_error.h"
#include "nlohmann/json.hpp"
#include "timer.h"

using json = nlohmann::json;

static void extractStrings(const json& obj, std::multimap<std::string, std::string>& key_val_map, const std::string& parent_key = "");

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
    printf("Object Count : %llu\n", input_json[0].size());

    // Iterate over each snapshot's items
    for (const auto& obj : input_json[0]) {
        
    }
    
    save_to_file(output, options[0]);
}

/**
 * \brief 
 * \param input_json 
 * \param options 
 */
static void parse_data_graph(json input_json, std::vector<std::string> options) {
    std::string output;
    std::multimap<std::string, std::string> key_val_map;

    timer stepTimer = timer();

    printf("  ┆ Building key-value mappings...");
    stepTimer.startTimer();

    // Build unique key-value map
    for (const auto& obj : input_json) {
        extractStrings(obj, key_val_map);
    }
    
    printf(" Took -> %s", stepTimer.resetTimer().c_str());
    printf("  ┆ Found %llu unique key-value pairs\n", key_val_map.size());
    printf("  ┆ Building JSON graph...");
    stepTimer.startTimer();
    
    // Parse map into gdf graph
    for (auto it = key_val_map.begin(); it != key_val_map.end();) {
        std::string currentKey = it->first;
        
        currentKey.erase(std::remove(currentKey.begin(), currentKey.end(), '\n'), currentKey.end());
        currentKey.erase(std::remove(currentKey.begin(), currentKey.end(), '\r'), currentKey.end());

        output += "{\n\"" + currentKey + "\":[\n";
        
        while (it != key_val_map.end() && it->first == currentKey) {
            std::string currentValue = it->second;
            
            currentValue.erase(std::remove(currentValue.begin(), currentValue.end(), '\n'), currentValue.end());
            currentValue.erase(std::remove(currentValue.begin(), currentValue.end(), '\r'), currentValue.end());

            output += "\"" + currentValue + "\",\n";
            ++it;
        }

        output[output.length() - 2] = ' ';

        output += "\n]\n},";

        if (it == key_val_map.end()) { break; }
        ++it;
    }
    
    printf(" Took -> %s", stepTimer.resetTimer().c_str());

    save_to_file(output, options[0]);
}


static void extractStrings(const json& obj, std::multimap<std::string, std::string>& key_val_map, const std::string& parent_key) {
    for (const auto& [key, value] : obj.items()) {
        // Append parent/key pair to multimap
        if (!parent_key.empty()) {
            key_val_map.insert({parent_key, key});
        }

        if (value.is_string()) {
            // Check if the key-value pair already exists
            auto range = key_val_map.equal_range(key);
            bool found = false;

            // Check for duplicate
            for (auto it = range.first; it != range.second; ++it) {
                if (it->second == value.get<std::string>()) {
                    found = true;
                    break;
                }
            }

            // If the key-value pair does not exist, add it to the multimap
            if (!found) {
                key_val_map.insert({key, value.get<std::string>()});
            }
        }
        else if (value.is_object()) {
            // If the value is an object, recursively call extractStrings with the parent key
            extractStrings(value, key_val_map, parent_key.empty() ? key : parent_key + "." + key);
        }
    }
}

#endif