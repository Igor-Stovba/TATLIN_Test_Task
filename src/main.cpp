#include <iostream>
#include <string>
#include "../include/config.hpp"
#include "../include/sorterTape.hpp"


int main(int argc, char* argv[]) {
    if (!(argc == 4 || argc == 3)) {
        std::cerr << "error: invalid arguments" << std::endl;
        std::cerr << "example1: ./bin input_filename output_filename" << std::endl;
        std::cerr << "example2: ./bin input_filename output_filename config_filename" << std::endl;
        exit(1);
    }

    std::string input_fn = argv[1];
    std::string output_fn = argv[2];
    tape::Config config;

    if (argc ==4) {
        std::string config_fn = argv[3];

        try {
            tape::parse_config(config_fn, config);
        } catch (...) {
            std::cerr << "error: invalid configuration file" << std::endl;
            exit(2);
        }
    }
    
    tape::TapeSorter sorter;
    sorter.loadConfig(config);
    sorter.sort(std::move(input_fn), std::move(output_fn));
    return 0;
}