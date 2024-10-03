#include "include\plaintext_parser.h"

#include "plaintext_parser_error.h"

plaintext_parser::plaintext_parser(const json& input_json) {
    this->json_data = input_json;
}

/**
 * \brief Sets the output path for the parser
 * \param path The target save path
 * \return The path's validity, 0 if invalid
 */
uint8_t plaintext_parser::set_output(const std::string& path) {
    if (std::ifstream(path).good()) { return 0; }
    parse_opts.emplace_back(path);
    return 1;
}

/**
 * \brief Sets the function to parse the JSON to plaintext
 * \note The function's parameters are the json object to parse, and a vector containing the parser arguments
 * \param input_function The target parsing function
 */
void plaintext_parser::set_parse_function(std::function<void(json, std::vector<std::string>)> input_function) {
    this->parse_function = input_function;
}

/**
 * \brief Sets the parser options to be passed to the parsing function
 * \param options The target options
 */
void plaintext_parser::set_parse_options(const std::vector<std::string>& options) {
    if (options.empty()) { return; }
    this->parse_opts = options;
}

/**
 * \brief Runs the parsing function on the loaded JSON with the assigned parse options
 * \note Any error will fall through and be caught by the caller
 */
void plaintext_parser::parse() const {
    parse_function(json_data, parse_opts);
}





