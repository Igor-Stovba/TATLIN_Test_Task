#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <cstdint>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace tape {

    class Config {
        /*
        Default values
        */
        uint64_t read_delay_ms = 0;
        uint64_t write_delay_ms = 0;
        uint64_t step_move_ms = 0;
        uint64_t rewind_to_beginning_ms = 500;
        uint64_t max_ram_bytes = 1024;
        std::string path_tmp_dir = "./";
    public:

        void set_read_delay(uint64_t ms) {
            read_delay_ms = ms;
        }
        uint64_t get_read_delay() const {
            return read_delay_ms;
        }

        void set_write_delay(uint64_t ms) {
            write_delay_ms = ms;
        }
        uint64_t get_write_delay() const {
            return write_delay_ms;
        }

        void set_move_step(uint64_t ms) {
            step_move_ms = ms;
        }
        uint64_t get_move_step() const {
            return step_move_ms;
        }

        void set_rewind_to_beginning(uint64_t ms) {
            rewind_to_beginning_ms = ms;
        }
        uint64_t get_rewind_to_beginning() const {
            return rewind_to_beginning_ms;
        }

        void set_max_ram(uint64_t bytes) {
            max_ram_bytes = bytes;
        } 
        uint64_t get_max_ram() const {
            return max_ram_bytes;
        }

        void set_path_tmp_dir(std::string path) {
            path_tmp_dir = std::move(path);
        }
        
        std::string get_path_tmp_dir() const {
            return path_tmp_dir;
        }
    };

    inline void parse_config(const std::string& config_fn, Config& config) {
        std::ifstream f(config_fn);
        json data = json::parse(f);

        config.set_read_delay(data.at("read_delay_ms"));
        config.set_write_delay(data.at("write_delay_ms"));
        config.set_move_step(data.at("step_move_ms"));
        config.set_rewind_to_beginning(data.at("rewind_to_beginning_ms"));
        config.set_max_ram(data.at("max_ram_bytes"));
        config.set_path_tmp_dir(data.at("path_tmp_dir"));
    }
}

#endif