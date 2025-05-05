#ifndef TAPE_SORTER_HPP_
#define TAPE_SORTER_HPP_

#include <string>
#include "config.hpp"
#include "interfaceTape.hpp"

namespace tape {

    class TapeSorter {
        Config cfg_;

    public:
        TapeSorter() = default;
        void loadConfig(const Config&);
        void sort(const std::string&, const std::string&);
    };
}

#endif