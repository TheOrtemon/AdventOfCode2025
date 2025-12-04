#include "day4.h"
#include <cmath>
#include <string>
#include <cstdlib>
#include <ranges>
#include <print>
#include <sys/types.h>
#include <ranges>
#include <vector>

using namespace std;
namespace adventofcode
{
Day4::Day4() : DaySolver(4) {
    // Data for tests
    test_data = R"(..@@.@@@@.
@@@.@.@.@@
@@@@@.@.@@
@.@@@@..@.
@@.@@@@.@@
.@@@@@@@.@
.@.@.@.@@@
@.@@@.@@@@
.@@@@@@@@.
@.@.@@@.@.)";
    test_results = { 13, 43 };
}

ResultType Day4::solvePart1() {
    auto matrix =
        input_data
        | views::split('\n')
        | views::transform([](auto line) {
            return vector<char>(line.begin(), line.end());
        })
        | ranges::to<vector<vector<char>>>();

    auto HEIGHT = matrix.size();
    auto WIDTH = matrix[0].size();
    vector<pair<int, int>> adj = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    auto accessed = 0;
    for (uint y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            auto counter = 0;
            if (matrix[y][x] == '.') {
                continue;
            }
            for (auto p: adj) {
                auto xi = p.first + x;
                auto yi = p.second + y;
                if (yi >= 0 && yi < HEIGHT && xi >= 0 && xi < WIDTH && matrix[yi][xi] == '@') {
                    counter += 1;
                }
            }
            if (counter < 4) {
                accessed += 1;
            }
        }
    }
    return accessed;
}

ResultType Day4::solvePart2() {
    auto matrix =
        input_data
        | views::split('\n')
        | views::transform([](auto line) {
            return vector<char>(line.begin(), line.end());
        })
        | ranges::to<vector<vector<char>>>();

    auto HEIGHT = matrix.size();
    auto WIDTH = matrix[0].size();
    vector<pair<int, int>> adj = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    auto lifted = 0;
    auto changing = true;
    while (changing) {
        changing = false;
        for (uint y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                auto counter = 0;
                if (matrix[y][x] == '.') {
                    continue;
                }
                for (auto p: adj) {
                    auto xi = p.first + x;
                    auto yi = p.second + y;
                    if (yi >= 0 && yi < HEIGHT && xi >= 0 && xi < WIDTH && matrix[yi][xi] == '@') {
                        counter += 1;
                    }
                }
                if (counter < 4) {
                    lifted += 1;
                    changing = true;
                    matrix[y][x] = '.';
                }
            }
        }
    }
    // for (auto line: matrix) {
    //     for (auto c: line) {
    //         print("{}", c);
    //     }
    //     println();
    // }
    return lifted;
}

}
