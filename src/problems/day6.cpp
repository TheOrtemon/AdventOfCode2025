#include "day6.h"
#include <array>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <numeric>
#include <string>
#include <string_view>
#include <cstdlib>
#include <ranges>
#include <sys/types.h>
#include <ranges>
#include <vector>
#include <algorithm>
#include <print>
#include <string>


using namespace std;
namespace adventofcode
{
Day6::Day6() : DaySolver(6) {
    // Data for tests
    test_data = R"(123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  )";
    test_results = { 4277556, 3263827 };
}

string trim(string s) {
    auto not_space = [](unsigned char c){ return !isspace(c); };
    s.erase(s.begin(), ranges::find_if(s, not_space));
    s.erase(ranges::find_if(s | views::reverse, not_space).base(), s.end());
    return s;
}

ResultType Day6::solvePart1() {
    auto line_range = input_data
        | views::split('\n')
        | views::transform([](auto line) {
            return string_view(line);
        });

    vector<vector<int64_t>> num_grid = {};
    vector<string> ops = {};

    for (auto [row, line] : line_range | views::enumerate) {
        auto str_line = string(line);
        auto trimmed = trim(str_line);
        // println(">{}<", trimmed);
        for (auto [column, elem]: trimmed
            | views::split(' ')
            | views::filter([](auto&& elem) { return !elem.empty(); })
            | views::enumerate
        ) {
            auto str_elem = string(elem.begin(), elem.end());
            // println("{}", str_elem);
            if (str_elem == "*" || str_elem == "+") {
                ops.push_back(str_elem);
            } else {
                if (row == 0) {
                    num_grid.push_back({});
                }
                int64_t num = stoll(str_elem);
                num_grid[(size_t)column].push_back(num);
            }
        }
    }
    // for (auto n: num_grid[0]) {
    //     println("{}", n);
    // }
    auto results = views::zip(num_grid, ops)
        | views::transform([](auto&& pair) {
            auto&& [row, op] = pair;
            return op == "*"
                ? accumulate(row.begin(), row.end(), 1LL, multiplies{})
                : accumulate(row.begin(), row.end(), 0LL, plus{});
        });
    auto result = accumulate(results.begin(), results.end(), 0LL);

    return result;
}

ResultType Day6::solvePart2() {
    auto input = string(input_data);
    auto pos = input.rfind('\n');
    auto main_body = input.substr(0, pos);
    auto ops_line = input.substr(pos + 1);
    auto temp_ops = trim(ops_line)
        | views::split(' ')
        | views::filter([](auto&& elem) { return !elem.empty(); });
    vector<string> ops;
    for (auto&& rng : temp_ops) {
        ops.emplace_back(rng.begin(), rng.end());
    }
    ranges::reverse(ops);
    auto rev_grid = ranges::to<vector>(main_body
        | views::split('\n')
        | views::transform([](auto line) {
            vector str(line.begin(), line.end());
            ranges::reverse(str);
            return str;
        }));

    vector<vector<int64_t>> res_nums = {{}};
    for (size_t col = 0; col < rev_grid[0].size(); col++) {
        string res_str = "";
        for (size_t row = 0; row < rev_grid.size(); row++) {
            res_str.push_back(rev_grid[row][col]);
        }
        auto trimmed_str = trim(res_str);
        if (trimmed_str.empty()) {
            res_nums.push_back({});
        } else {
            auto num = stoll(trimmed_str);
            res_nums.back().push_back(num);
        }
    }

    auto results = views::zip(res_nums, ops)
        | views::transform([](auto&& pair) {
            auto&& [row, op] = pair;
            return op == "*"
                ? accumulate(row.begin(), row.end(), 1LL, multiplies{})
                : accumulate(row.begin(), row.end(), 0LL, plus{});
        });
    auto result = accumulate(results.begin(), results.end(), 0LL);

    return result;
}

}
