#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

#include "parser_functions.h"
#include "plaintext_parser.h"

using json = nlohmann::json;

// Prototypes
std::vector<std::string> split (const std::string &s, char delim);

// A numerical representation of the parse type argument
enum parse_type {
    MATCH_DATA
};

// Used to map the the parse type argument to an index
const std::string parse_type_str[] = {
    "match_data"
};

int main(const int argc, char *argv[]) {
    if (argc < 4) {
        (void)fprintf(stderr, "Invalid arguments!\nUsage: vct-json <input file> <output file> <parse type> <parse options>\n");
    }

    // Change file from UTF16 to UTF8
    const std::string command = std::string("powershell -Command \"Get-Content '") + argv[1] + std::string("' | Out-File 'valid_utf8.json' -Encoding ascii\"");
    system(command.c_str());
    
    std::ifstream input_file("valid_utf8.json");
    if (!input_file.good()) {
        (void)fprintf(stderr, "Error accessing processed JSON file!\n");
    }

    // Parse JSON string into JSON object
    json json_obj;
    try {
        json_obj = json::parse(input_file);
    } catch (const json::exception& e) {
        (void)fprintf(stderr, "Error Parsing JSON!\n%s", e.what());
        exit(-1);   
    }

    // Convert parse type argument to enum value
    const uint8_t pos = std::distance(
           parse_type_str,
           std::find(parse_type_str, parse_type_str + std::size(parse_type_str), argv[3])
           );

    if (pos == std::size(parse_type_str)) {
        (void)fprintf(stderr, "Invalid parse type '%s'!", argv[3]);
    }

    // Parser setup & execution
    plaintext_parser parser = plaintext_parser(json_obj);
    int result = parser.set_output(argv[2]);

    // Extract & validate parser options
    if (argc > 4) {
        std::vector<std::string> opts;

        size_t c_pos;
        for (int i = 4; i < argc; i++) {
            if (argv[i][0] != '-') {
                (void)fprintf(stderr, "Invalid parse option definition '%s' at position %d. Did you mean '-%s'?\n", argv[i], i, argv[i]);
                exit(-1);
            }

            opts.emplace_back(std::string(argv[i]).substr(1, std::string(argv[i]).length()));
        }

        parser.set_parse_options(opts);
    }

    if (!result) {
        (void)fprintf(stderr, "Invalid save path '%s'!", argv[2]);
    }

    // Set parsing function
    switch (static_cast<parse_type>(pos)) {
    case MATCH_DATA:
        parser.set_parse_function(parse_match_data);
        break;
    }
    
    try {
        // Parses and automatically saves file to output path (passed in as option 0)
        parser.parse();
    } catch (plaintext_parser_error& e) {
        (void)fprintf(stderr, "%s\n", e.what());
        exit(-1);
    }
}
