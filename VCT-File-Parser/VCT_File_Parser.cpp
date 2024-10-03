#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(const int argc, char *argv[]) {
    if (argc != 4) {
        (void)fprintf(stderr, "Invalid arguments!\nUsage: vct-json <input file> <output file> <filetype>\n");
    }

    // Change file from UTF16 to UTF8
    const std::string command = std::string("powershell -Command \"Get-Content '") + argv[1] + std::string("' | Out-File 'valid_utf8.json' -Encoding ascii\"");
    system(command.c_str());

    std::ifstream input_file("valid_utf8.json");
    if (!input_file.good()) {
        (void)fprintf(stderr, "Error accessing processed JSON file!\n");
    }

    try {
        const json json_obj = json::parse(input_file);
    } catch (const json::exception& e) {
        (void)fprintf(stderr, "Error Parsing JSON!\n%s", e.what());
        exit(-1);   
    }
}