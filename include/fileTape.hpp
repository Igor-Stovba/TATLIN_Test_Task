#ifndef FILE_TAPE_IMPL
#define FILE_TAPE_IMPL

#include "config.hpp"
#include "interfaceTape.hpp"
#include <fstream>

namespace tape {

    class FileTape: public ITape {
        std::fstream file_;
        Config cfg_;
    public:
        FileTape(const std::string&, Config, std::ios_base::openmode);
        ~FileTape();

        int32_t read() override;
        void write(int32_t) override;
        void move_left() override;
        void move_right() override;
        void rewind_to_beginning() override;
        void delay(uint64_t) const;
        bool isEOF();
    };
}

#endif