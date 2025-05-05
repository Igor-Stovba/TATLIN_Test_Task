
#include "../include/fileTape.hpp"
#include <cstdint>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <vector>

using namespace std::chrono;

namespace tape {

    FileTape::FileTape(const std::string& filename, Config cfg, std::ios_base::openmode mode)
        : file_(filename, mode)
    {

        cfg_ = std::move(cfg);
        if (!file_) 
            throw std::runtime_error("error: failed to open file!!");

        /*
        Set internal buf of std::fstream to our limit.
        It's caller's responsibility to distribute given RAM limit to 
            different tapes (in their Config)
        */
        std::vector<char> buf(cfg_.get_max_ram());
        file_.rdbuf()->pubsetbuf(buf.data(), buf.size());
    }

    FileTape::~FileTape() {
        file_.close();
    }

    void FileTape::delay(uint64_t ms) const {
        if (ms > 0)
            std::this_thread::sleep_for(milliseconds(ms));
    }

    int32_t FileTape::read() {
        int32_t value;
        std::streampos prev_pos = file_.tellg();
        file_.read(reinterpret_cast<char*>(&value), sizeof(value));
        file_.seekg(prev_pos);

        delay(cfg_.get_read_delay());
        return value;
    }
    void FileTape::write(int32_t value) {
        std::streampos prev_pos = file_.tellp();
        file_.write(reinterpret_cast<char*>(&value), sizeof(value));
        file_.seekp(prev_pos);

        delay(cfg_.get_write_delay());
    }

    void FileTape::move_left() {
        file_.seekg(-sizeof(int32_t), std::ios::cur);
        file_.seekp(file_.tellg());
    }

    void FileTape::move_right() {
        file_.seekg(sizeof(int32_t), std::ios::cur);
        file_.seekp(file_.tellg());
    }

    void FileTape::rewind_to_beginning() {
        file_.seekg(0);
        file_.seekp(file_.tellg());
    }

    bool FileTape::isEOF() {
        std::streampos prev_pos = file_.tellp();

        file_.seekp(0, std::ios::end);
    
        // Getting the last position
        std::streampos end_pos = file_.tellp();

        file_.seekp(prev_pos);
        return end_pos == prev_pos;
    }
}