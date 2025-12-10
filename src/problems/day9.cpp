#include "day9.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>
#include <cstdlib>
#include <ranges>
#include <print>
#include <sys/types.h>
#include <ranges>
#include <map>
#include <vector>


using namespace std;
namespace adventofcode
{
Day9::Day9() : DaySolver(9) {
    // Data for tests
    test_data = R"(7,1
    11,1
    11,7
    9,7
    9,5
    2,5
    2,3
    7,3)";
    test_results = { 50, 24 };

}


constexpr uint64_t abs_diff(uint64_t a, uint64_t b) noexcept {
    return std::max(a, b) - std::min(a, b);
}


ResultType Day9::solvePart1() {
    vector<array<uint64_t, 2>> tiles = {};
    ranges::for_each(input_data | views::split('\n'),
        [&](auto line) {
            auto str = string(line.begin(), line.end());
            size_t pos = (size_t)str.find(',');
            auto start = stoull(string(str.substr(0, pos)));
            auto finish = stoull(string(str.substr(pos + 1)));
            // println("{}, {}, line: {}", start, finish, str);
            array<uint64_t, 2> coord = {static_cast<uint64_t>(start), static_cast<uint64_t>(finish)};
            tiles.push_back(coord);
        });

    uint64_t result = 0LL;

    for (auto i = (size_t)0LL; i < tiles.size(); i++) {
        for (auto j = i + 1; j < tiles.size(); j++) {
            auto first = tiles[i];
            auto second = tiles[j];
            auto area = (abs_diff(second[1], first[1]) + 1) * (abs_diff(second[0], first[0]) + 1);
            // println("{}, {}; {}, {}; area = {}", first[0], first[1], second[0], second[1], area);
            result = max(result, area);
        }
    }

    return result;
}

enum TileColor {
    GREEN = 1,
    RED = 2,
};

ResultType Day9::solvePart2() {
    vector<array<uint64_t, 2>> tiles = {};
    map<array<uint64_t, 2>, TileColor> tile_map = {};

    ranges::for_each(input_data | views::split('\n'),
        [&](auto line) {
            auto str = string(line.begin(), line.end());
            size_t pos = (size_t)str.find(',');
            auto start = stoull(string(str.substr(0, pos)));
            auto finish = stoull(string(str.substr(pos + 1)));
            // println("{}, {}, line: {}", start, finish, str);
            array<uint64_t, 2> coord = {static_cast<uint64_t>(start), static_cast<uint64_t>(finish)};
            tiles.push_back(coord);
            tile_map[coord] = RED;
        });

    for (auto i = (size_t)1; i <= tiles.size(); i++) {
        auto prev = tiles[i - 1];
        auto cur = tiles[i % tiles.size()];
        array<uint64_t, 2> min_tile = {min(prev[0], cur[0]), min(prev[1], cur[1])};
        auto x_diff = abs_diff(prev[0], cur[0]);
        auto y_diff = abs_diff(prev[1], cur[1]);
        for (auto df = size_t(1); df < x_diff + y_diff; df++) {
            array<uint64_t, 2> green_tile;
            if (x_diff == 0) {
                green_tile = {min_tile[0], min_tile[1] + df};
            } else if (y_diff == 0) {
                green_tile = {min_tile[0] + df, min_tile[1]};
            } else {
                throw 123;
            }
            if (!tile_map.contains(green_tile)) {
                tile_map[green_tile] = GREEN;
            }
        }
    }

    uint64_t result = 0LL;

    for (auto i = (size_t)0LL; i < tiles.size(); i++) {
        for (auto j = i + 1; j < tiles.size(); j++) {
            auto first = tiles[i];
            auto second = tiles[j];
            uint64_t area = (abs_diff(second[1], first[1]) + 1) * (abs_diff(second[0], first[0]) + 1);
            if (area <= result) {
                continue;
            }
            auto minX = min(first[0], second[0]);
            auto minY = min(first[1], second[1]);
            auto maxX = max(first[0], second[0]);
            auto maxY = max(first[1], second[1]);
            array<uint64_t, 2> tl = {minX, maxY};
            array<uint64_t, 2> tr = {maxX, maxY};
            array<uint64_t, 2> bl = {minX, minY};
            array<uint64_t, 2> br = {maxX, minY};

            auto is_in = true;
            for (auto tile: tile_map | views::keys) {
                if (tile[0] < maxX && tile[0] > minX && tile[1] < maxY && tile[1] > minY) {
                    is_in = false;
                    break;
                }
            }
            if (!is_in) {
                continue;
            }

            array<array<uint64_t, 2>, 2> missing_corners;
            if ((first == tl && second == br) || (first == br && second == tl)) {
                missing_corners = {tr, bl};
            } else if ((first == tr && second == bl) || (first == bl && second == tr)) {
                missing_corners = {tl, br};
            } else {
                throw 234;
            }
            auto both_corners_in = true;
            for (auto corner: missing_corners) {
                if (tile_map.contains(corner)) {
                    continue;
                }
                auto crossing_counter = 0;
                auto red_counter = 0;
                auto last_dir = 99;
                auto cx = corner[0];
                for (auto x = (size_t)0; x < cx; x++) {
                    array<uint64_t, 2> tile = {x, corner[1]};
                    if (tile_map.contains(tile)) {
                        if (tile_map[tile] == RED) {
                            array<uint64_t, 2> uptile = {tile[0], tile[1] - 1};
                            if (tile_map.contains(uptile)) {
                                if (red_counter % 2 == 1 && last_dir == 99) {
                                    crossing_counter += 1;
                                }
                                last_dir = 99;
                            } else {
                                if (red_counter % 2 == 1 && last_dir == 11) {
                                    crossing_counter += 1;
                                }
                                last_dir = 11;
                            }
                            red_counter += 1;
                        }
                        if (red_counter % 2 == 0) {
                            // TODO check neighbours
                            crossing_counter += 1;
                        }
                    }
                }
                if (crossing_counter % 2 == 0) {
                    both_corners_in = false;
                    break;
                }
            }
            if (!both_corners_in) {
                continue;
            }
            // First corner
            // Second corner
            println("{}, {}; {}, {}; area = {}", first[0], first[1], second[0], second[1], area);
            result = max(result, area);
        }
    }

    return result;
}

}
