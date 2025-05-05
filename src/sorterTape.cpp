#include "../include/sorterTape.hpp"
#include "../include/fileTape.hpp"
#include <cstdint>
#include <ios>
#include <memory>
#include <queue>
#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

namespace tape {

    void TapeSorter::loadConfig(const Config& cfg) {
        cfg_ = cfg;
    }

    namespace  {
        std::vector<fs::path> createSortedChunks(const std::string& in, const Config& cfg) {
            std::vector<fs::path> paths;
            uint64_t ram_bytes = cfg.get_max_ram();
            const uint64_t ram_part_bytes = ram_bytes / 3;
            size_t tape_index = 0;

            /*
            Divide the common RAM limit into 3 equal parts: 
                1) for internal buffer in FileTape -> cache_buffer (input_tape)
                2) for buffer where data will be read from input_tape
                3) for internal buffer in FileTape -> cache_buffer (output_tape)
            */
            Config config_custom = cfg;
            config_custom.set_max_ram(ram_part_bytes);

            FileTape input_tape(in, config_custom, std::ios_base::in | std::ios_base::binary);

            while (!input_tape.isEOF()) {

                std::vector<int32_t> buffer;
                buffer.reserve(ram_part_bytes / sizeof(int32_t));

                while (buffer.size() < ram_part_bytes / sizeof(int32_t) && !input_tape.isEOF()) {
                    buffer.push_back(input_tape.read());
                    input_tape.move_right();
                }

                std::sort(std::begin(buffer), std::end(buffer));
                std::string chunkPath = cfg.get_path_tmp_dir() + std::to_string(tape_index++) + ".bin";

                FileTape output_tape(chunkPath, config_custom, std::ios_base::out | std::ios_base::binary);
                for (auto item: buffer) {
                    output_tape.write(item);
                    output_tape.move_right();
                }

                paths.push_back({chunkPath});
            }
            return paths;
        }

        void mergeChunks(const std::vector<fs::path>& chunks, 
            const std::string& out, const Config& cfg) {
            /*
            Algorithm:
                1) Create heap where all chunks will be stored as appropriate FileTape
                2) Iteratevaly we will pick up the smallest elem from heap 
                    and fill output_tape (result)
            Note: std::runtime_error will be thrown if the memory limit is less than
                  (number of chunks + 1) * sizeof(int32_t). It makes sense since every
                  FileTape should have enough space for storing at least one element
                  Although, this restriction may not be imposed, but implementation would 
                  be harder
            */
            struct Node_ {
                int32_t value;
                size_t tape_index;

                bool operator<(const Node_& other) const {
                    return value > other.value;
                }
            };

            #ifdef DEBUG 
                std::cerr << "chunks.size() = " << chunks.size() << std::endl;

                for (size_t i = 0 ; i < chunks.size(); i++) {
                    std::cerr << chunks[i] << std::endl;
                }
            #endif

            uint64_t ram_bytes = cfg.get_max_ram();

            if (ram_bytes < (chunks.size() + 1) * sizeof(int32_t))
                throw std::runtime_error("error: too little ram, try to increase");
            
            /*
            Maybe, it would be better if we give more memory for output_tape,
                not in equal proportion, since it has higher load
            */
            const uint64_t ram_part_bytes = ram_bytes / (chunks.size() + 1);
            Config config_custom = cfg;
            config_custom.set_max_ram(ram_part_bytes);

            std::vector<std::unique_ptr<FileTape>> tapes;
            for (const auto& p: chunks) {
                tapes.emplace_back(std::make_unique<FileTape>(p, config_custom, std::ios_base::in | std::ios_base::binary));
            }

            std::priority_queue<Node_> heap;
            for (size_t i = 0; i < tapes.size(); i++) {
                if (!tapes[i]->isEOF()) {
                    int32_t read_vl = tapes[i]->read();

                    heap.push({read_vl, i});
                    tapes[i]->move_right();
                }
            }

            FileTape output_tape(out, config_custom, std::ios_base::out | std::ios_base::binary);

            while (!heap.empty()) {
                Node_ el = heap.top();
                heap.pop();

                output_tape.write(el.value);
                output_tape.move_right();

                if (!tapes[el.tape_index]->isEOF()) {
                    heap.push({tapes[el.tape_index]->read(), el.tape_index});
                    tapes[el.tape_index]->move_right();
                }
            }
        }
    }

    void TapeSorter::sort(const std::string& in, const std::string& out) {
        /*
        chunks are paths to these sorted tapes (files)
        */
        #ifdef DEBUG 
            std::cerr << "Sort started\n";
        #endif
        
        std::vector<fs::path> chunks = createSortedChunks(in, cfg_);
        #ifdef DEBUG 
            std::cerr << "createSortedChunks completed\n";
        #endif

        mergeChunks(chunks, out, cfg_);
        #ifdef DEBUG 
            std::cerr << "mergeChunks completed\n";
        #endif

        // Resource clean up
        for (const auto& tmp: chunks) {
            fs::remove(tmp);
        }
        #ifdef DEBUG 
            std::cerr << "cleaning up completed\n";
        #endif
    }
}