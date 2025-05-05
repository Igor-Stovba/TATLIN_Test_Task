#include <gtest/gtest.h>
#include "test_utility.hpp"
#include "../include/sorterTape.hpp"

#include <iostream>

#define KB 1024
#define MB 1024 * 1024

TEST(Basic, test1) 
{
    const std::string input_filename = "./input.bin";
    const std::string output_filename = "./output.bin";

    std::vector<int32_t> numbers = {100, -5, 3, -40, 0, 15};
    tests::write_vector_to_file(numbers, input_filename);

    tape::Config config;

    tape::TapeSorter sorter;
    sorter.loadConfig(config);
    sorter.sort(input_filename, output_filename);

    std::vector<int32_t> result = tests::read_vector_from_file(output_filename);

    //Checking
    std::sort(std::begin(numbers), std::end(numbers));

    ASSERT_EQ(numbers.size(), result.size());
    ASSERT_EQ(numbers, result);
}

TEST(Basic, test2) 
{
    const std::string input_filename = "./input.bin";
    const std::string output_filename = "./output.bin";

    std::vector<int32_t> numbers = tests::genRandVector(15, 0, 50);
    tests::write_vector_to_file(numbers, input_filename);

    tape::Config config;
    config.set_max_ram(100); //100 bytes

    tape::TapeSorter sorter;
    sorter.loadConfig(config);
    sorter.sort(input_filename, output_filename);

    std::vector<int32_t> result = tests::read_vector_from_file(output_filename);

    //Checking
    std::sort(std::begin(numbers), std::end(numbers));

    ASSERT_EQ(numbers.size(), result.size());
    ASSERT_EQ(numbers, result);
}

TEST(Stress, test) 
{
    const std::string input_filename = "./input.bin";
    const std::string output_filename = "./output.bin";

    std::vector<int32_t> numbers = tests::genRandVector(150000, -10000, 500000);
    tests::write_vector_to_file(numbers, input_filename);

    tape::Config config;
    config.set_max_ram(300 * KB);  

    tape::TapeSorter sorter;
    sorter.loadConfig(config);
    sorter.sort(input_filename, output_filename);

    std::vector<int32_t> result = tests::read_vector_from_file(output_filename);

    //Checking
    std::sort(std::begin(numbers), std::end(numbers));

    ASSERT_EQ(numbers.size(), result.size());
    ASSERT_EQ(numbers, result);
}