#include "day3.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <string>
#include <string_view>
#include <cstdlib>
#include <ranges>
#include <print>
#include <sys/types.h>
#include <ranges>
#include <vector>

using namespace std;
namespace adventofcode
{
Day3::Day3() : DaySolver(3) {
    // Data for tests
    test_data = R"(987654321111111
811111111111119
234234234234278
818181911112111)";
    test_results = { 357, 3121910778619 };
}

ResultType Day3::solvePart1() {
    auto line_range = input_data
        | views::split('\n')
        | views::transform([](auto line) {
            return string_view(line);
        });

    uint counter = 0;

    for (auto line : line_range) {
        // println("{}", line);
        uint len = line.length();
        uint digit = line.at(len - 1) - '0';
        uint dec = line.at(len - 2) - '0';
        for (uint i = len - 3; i < len; i--) {
            uint cur = line.at(i) - '0';
            if (cur >= dec) {
                digit = max(digit, dec);
                dec = cur;
            }
        }
        uint number = digit + (10 * dec);
        // println("number {}", number);
        counter += number;
    }
    return (int)counter;
}

ResultType Day3::solvePart2() {

    auto line_range = input_data
        | views::split('\n')
        | views::transform([](auto line) {
            return string_view(line);
        });

    uint64_t counter = 0;
    auto ORDER = 12;

    for (auto line : line_range) {
        // println("{}", line);
        uint len = line.length();
        auto transformed = line | views::transform([](auto c) { return c - '0'; }) ;
        auto digits = ranges::to<vector>(transformed);
        vector<int> res(digits.end() - ORDER, digits.end());
        for (uint i = len - 1 - ORDER; i < len; i--) {
            uint cur = digits.at(i);
            if (cur >= res.at(0)) {
                auto sub = cur;
                for (int j = 0; j < res.size(); j++) {
                    if (sub >= res.at(j)) {
                        auto temp = res.at(j);
                        res.at(j) = sub;
                        sub = temp;
                    } else {
                        break;
                    }
                }
            }
        }
        auto number = accumulate(
            res.begin(), \
            res.end(), \
            0LL, \
            [](u_int64_t cur, int digit) {
                return cur * 10 + digit;
            }
        );

        // println("number {}", number);
        counter += (u_int64_t)number;
        // println("{}", counter);
        if (counter < number) {
            println("OVERFLOW!!!");
        }
    }
    return (u_int64_t)counter;
}

}
