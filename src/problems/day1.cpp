#include "day1.h"
// #include "gtest/gtest.h"
#include <cstdlib>
#include <string_view>
#include <ranges>
#include <print>

namespace adventofcode
{
    Day1::Day1() : DaySolver(1) {
        test_data = R"(L68
L30
R48
L5
R60
L55
L1
L99
R14
L82)";
        test_results = { 3, 6 };
    }

    ResultType Day1::solvePart1() {
        auto line_range = input_data
        | std::views::split('\n')
        | std::views::transform([](auto line) {
            return std::string_view(line);
        });

        int last = 50;
        int counter = 0;

        for (auto line : line_range) {
            auto substr = std::string{line.substr(1)};
            auto num = std::stoi(substr);
            if (line[0] == 'L') {
                num = -num;
            }

            int unchecked_sum = last + num;
            last = (unchecked_sum + 100) % 100;
            if (last == 0) { counter ++; }
        }
        return counter;
    }

    ResultType Day1::solvePart2() {

        auto line_range = input_data
        | std::views::split('\n')
        | std::views::transform([](auto line) {
            return std::string_view(line);
        });

        int last = 50;
        int counter = 0;

        for (auto line : line_range) {
            auto substr = std::string{line.substr(1)};
            auto num = std::stoi(substr);
            if (num == 0) {
                continue;
            }
            if (line[0] == 'L') {
                num = -num;
                if (last == 0) {
                    counter -= 1;
                }
            }

            int unchecked_sum = last + num;
            if (unchecked_sum >= 100) {
                counter += unchecked_sum / 100;
            } else if (unchecked_sum <= 0) {
                counter += 1 + std::abs(unchecked_sum) / 100;
            }
            last = ((unchecked_sum % 100) + 100) % 100;
            // std::println("{}, {}", last, counter);
        }
        // std::println("ANSWER:   {}", counter);
        return counter;
    }

}
