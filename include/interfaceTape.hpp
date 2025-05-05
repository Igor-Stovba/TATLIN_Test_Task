#ifndef INTERFACE_TAPE_HPP_
#define INTERFACE_TAPE_HPP_

#include <cstdint>

namespace tape {

    class ITape {
    public:
        virtual int32_t read() = 0;
        virtual void write(int32_t) = 0;
        virtual void move_left() = 0;
        virtual void move_right() = 0;
        virtual void rewind_to_beginning() = 0;
        virtual ~ITape() = default;
    };

}

#endif