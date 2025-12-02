#include "day2.h"
#include <cassert>
#include <cmath>
#include <string>
#include <string_view>
#include <cstdlib>
#include <ranges>
#include <print>
#include <sys/types.h>

using namespace std;
namespace adventofcode
{
Day2::Day2() : DaySolver(2) {
    // Data for tests
    test_data = R"(11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124)";
    test_results = { 1227775554, 4174379265 };
}


bool is_invalid(long long num) {
    assert(num > 0);
    int digits = (int)log10(num) + 1;
    int order = (int)pow(10, digits / 2);
    return num % order == num / order;
}

ResultType Day2::solvePart1() {
    auto line_range = input_data
    | views::split(',')
    | views::transform([](auto line) {
        return string_view(line);
    });

    u_int64_t counter = 0;

    for (auto line : line_range) {
        // println("LINE = {}", line);
        int pos = (int)line.find('-');
        u_int64_t start = stoll(string(line.substr(0, pos)));
        u_int64_t finish = stoll(string(line.substr(pos + 1)));
        // println("start = {}, finish = {}", start, finish);
        for (u_int64_t  i = start; i <= finish; i++) {
            // println("{}", i);
            if (is_invalid(i)) {
                // println("{} is invalid", i);
                counter += i;
            }
        }
    }

    return counter;
}
u_int64_t safe_pow10(int p) {
    u_int64_t res = 1;
    for (int i = 0; i < p; i++) {
        // Simple check to prevent overflow, though unlikely for p < 19
        if (__builtin_mul_overflow(res, 10, &res)) {
            // Handle error or return a max value if needed
            return 0;
        }
    }
    return res;
}
bool is_invalid2(u_int64_t num) {
    int digits = 0;
    u_int64_t temp = num;
    do {
        temp /= 10;
        digits++;
    } while (temp > 0);
    // println("num = {}, digits = {}", num, digits);
    for (int i = 1; i <= digits / 2; i++) {
        if (digits % i != 0) {
            continue;
        }
        u_int64_t order = safe_pow10(i);
        if (order == 0) return false;
        u_int64_t pattern = num % order;
        // println("comparator  = {}, i = {}", comparator, i);
        u_int64_t n_copy = num / order;
        bool is_repeating = true;
        while (n_copy > 0) {
            u_int64_t current_chunk = n_copy % order;

            if (current_chunk != pattern) {
                is_repeating = false;
                break;
            }

            n_copy /= order;
        }
        if (is_repeating == true) {
            return true;
        }

    }
    return false;
}
ResultType Day2::solvePart2() {
    auto line_range = input_data
    | views::split(',')
    | views::transform([](auto line) {
        return string_view(line);
    });

    u_int64_t counter = 0;

    for (auto line : line_range) {
        // println("LINE = {}", line);
        int pos = (int)line.find('-');
        u_int64_t start = stoll(string(line.substr(0, pos)));
        u_int64_t finish = stoll(string(line.substr(pos + 1)));
        // println("start = {}, finish = {}", start, finish);
        for (u_int64_t  i = start; i <= finish; i++) {
            // println("{}", i);
            if (is_invalid2(i)) {
                // println("{} is invalid", i);
                counter += i;
                if (i > counter) {
                    println("OVERFLOW!!!!!");
                }
            }
        }
    }

    return counter;
}

}
