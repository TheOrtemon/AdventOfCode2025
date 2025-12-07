#include "day7.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <iterator>
#include <optional>
#include <string>
#include <cstdlib>
#include <ranges>
#include <print>
#include <sys/types.h>
#include <ranges>
#include <map>
#include <vector>
#include <utility>
#include <functional>
#include <set>
#include <memory_resource>

using namespace std;
namespace adventofcode
{
Day7::Day7() : DaySolver(7) {
    // Data for tests
    test_data = R"(.......S.......
...............
.......^.......
...............
......^.^......
...............
.....^.^.^.....
...............
....^.^...^....
...............
...^.^...^.^...
...............
..^...^.....^..
...............
.^.^.^.^.^...^.
...............)";
    test_results = { 21, 40 };
}

using coord = pair<size_t, size_t>;
using coord_pair = pair<coord, coord>;

ResultType Day7::solvePart1() {
    auto matrix =
        input_data
        | views::split('\n')
        | views::transform([](auto line) {
            return vector<char>(line.begin(), line.end());
        })
        | ranges::to<vector<vector<char>>>();
    set<coord> been = {};
    size_t start_x = distance(matrix[0].begin(), find(matrix[0].begin(), matrix[0].end(), 'S'));
    coord start = pair(start_x, 0);
    pmr::deque<coord> q = {};
    q.push_back(start);
    auto counter = 0;
    while (!q.empty()) {
        auto cur = q.front();
        q.pop_front();
        auto next = pair(cur.first, cur.second + 1);
        if (next.second >= matrix.size()) {
            continue;
        }
        switch (matrix[next.second][next.first]) {
            case '.':
                q.push_back(next);
                break;
            case '^':
                if (!been.contains(next)) {
                    counter += 1;
                    been.insert(next);
                    if (next.first - 1 >= 0) {
                        q.push_back(pair(next.first - 1, next.second));
                    }
                    if (next.first + 1 < matrix[0].size()) {
                        q.push_back(pair(next.first + 1, next.second));
                    }
                }
                break;
            default:
                throw 123;
        }
    }
    return counter;
}

ResultType Day7::solvePart2() {
    auto matrix =
        input_data
        | views::split('\n')
        | views::transform([](auto line) {
            return vector<char>(line.begin(), line.end());
        })
        | ranges::to<vector<vector<char>>>();
    size_t start_x = distance(matrix[0].begin(), find(matrix[0].begin(), matrix[0].end(), 'S'));
    coord start_coord = pair(start_x, 0);
    coord_pair start = pair(start_coord, start_coord); // cur_coord, par_coord
    map<coord, vector<coord>> ctp_map = {};
    map<coord, vector<coord>> ptc_map = {};
    pmr::deque<coord_pair> q = {};
    q.push_back(start);
    while (!q.empty()) {
        auto coord_pair = q.front();
        q.pop_front();
        auto [cur, paren] = coord_pair;

        auto next = pair(cur.first, cur.second + 1);
        if (next.second >= matrix.size()) {
            if (!ptc_map.contains(paren)) {
                ptc_map[paren] = {next};
            } else {
                ptc_map[paren].push_back(next);
            }
            continue;
        }
        switch (matrix[next.second][next.first]) {
            case '.':
                q.push_back(pair(next, paren));
                break;
            case '^':
                if (!ctp_map.contains(next)) {
                    ctp_map[next] = {paren};

                    auto left = pair(next.first - 1, next.second);
                    auto right = pair(next.first + 1, next.second);
                    q.push_back(pair(left, next));
                    q.push_back(pair(right, next));
                } else {
                    ctp_map[next].push_back(paren);
                }
                if (!ptc_map.contains(paren)) {
                    ptc_map[paren] = {next};
                } else {
                    ptc_map[paren].push_back(next);
                }
                break;
            default:
                throw 123;
        }
    }
    map<coord, uint64_t> child_count_map = {};

    function<uint64_t(coord)> count_children;
    
    count_children = [&](coord node) -> uint64_t {
        if (child_count_map.contains(node)) {
            return child_count_map[node];
        }

        if (!ptc_map.contains(node) || ptc_map[node].size() == 0) {
            child_count_map[node] = 1LL;
            return child_count_map[node];
        }
        
        auto res = 0LL;
        for (auto child: ptc_map[node]) {
            res += count_children(child);
        }
        
        // println("x = {}, y = {}, res = {}", node.first, node.second, res);
        child_count_map[node] = res;
        return child_count_map[node];
    };

    auto main_child = ptc_map[start_coord][0];

    return count_children(main_child);
}

}
