#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <random>

namespace tests {

    inline void write_vector_to_file(const std::vector<int32_t>& vec, const std::string& filename) {
        std::ofstream out_file(filename, std::ios::binary);
        if (!out_file) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        out_file.write(reinterpret_cast<const char*>(vec.data()), 
                    vec.size() * sizeof(int32_t));
        
        if (!out_file) {
            std::cerr << "Failed to write to file" << std::endl;
        }
        
        out_file.close();
    }

    inline std::vector<int32_t> read_vector_from_file(const std::string& filename) {
        std::ifstream in_file(filename, std::ios::binary | std::ios::ate);
        if (!in_file) {
            throw std::runtime_error("Failed to open file");
        }
    
        const size_t file_size = in_file.tellg();
        in_file.seekg(0);
        
        std::vector<int32_t> result(file_size / sizeof(int32_t));
        in_file.read(reinterpret_cast<char*>(result.data()), file_size);
        
        if (!in_file) {
            throw std::runtime_error("Failed to read file");
        }
        
        return result;
    }

    inline std::vector<int32_t> genRandVector(size_t count, int32_t min, int32_t max) {
        std::vector<int32_t> result;
        result.reserve(count);
    
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int32_t> dist(min, max);
    
        for (size_t i = 0; i < count; ++i) {
            result.push_back(dist(gen));
        }
    
        return result;
    }
}