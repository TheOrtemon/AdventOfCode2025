#include "day11.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>
#include <cstdlib>
#include <ranges>
#include <sys/types.h>
#include <ranges>
#include <map>
#include <vector>
#include <print>

using namespace std;
namespace adventofcode
{
Day11::Day11() : DaySolver(11) {
    // Data for tests
    test_data = R"(svr: aaa bbb
aaa: fft
fft: ccc
bbb: tty
tty: ctuplec<c>, array<bool, 2>
ccc: ddd eee
ddd: hub
hub: fff
eee: dac
dac: fff
fff: ggg hhh
ggg: out
hhh: out)";
    test_results = { 5, 2 };
}

const string FINISH = "out";
const string START = "you";
const string START2 = "svr";
const string PITSTOP1 = "fft";
const string PITSTOP2 = "dac";

uint64_t count_ways(
    string start,
    string dest,
    map<string, vector<string>>* back_itinerary,
    map<string, uint64_t>* the_ways
) {
    if (dest == start) {
        return 1;
    }
    if ((*the_ways).contains(dest)) {
        return (*the_ways)[dest];
    }
    uint64_t res = 0;
    for (auto source_dest: (*back_itinerary)[dest]) {
        res += count_ways(start, source_dest, back_itinerary, the_ways);
    }
    (*the_ways)[dest] = res;
    return res;
}

ResultType Day11::solvePart1() {
    map<string, vector<string>> itinerary = {};
    map<string, vector<string>> back_itinerary = {};
    map<string, uint64_t> the_ways = {};
    ranges::for_each(input_data | views::split('\n'),
        [&](auto line) {
            auto str = string(line.begin(), line.end());
            size_t pos = (size_t)str.find(':');
            auto start = string(str.substr(0, pos));
            auto res = string(str.substr(pos + 2));
            vector<string> destinations = res
                | views::split(' ')
                | views::transform([&](auto dest) {
                    auto dest_str = string(dest.begin(), dest.end());
                    if (back_itinerary.contains(dest_str)) {
                        back_itinerary[dest_str].push_back(start);
                    } else {
                        vector<string> tmp = {start};
                        back_itinerary[dest_str] = tmp;
                    }
                    return dest_str;
                }) | ranges::to<vector>();
            itinerary[start] = destinations;
        }
    );
    return count_ways(START, FINISH, &back_itinerary, &the_ways);
}

ResultType Day11::solvePart2() {
    map<string, vector<string>> itinerary = {};
    map<string, vector<string>> back_itinerary = {};
    map<tuple<string, array<bool, 2>>, uint64_t> the_ways = {};
    ranges::for_each(input_data | views::split('\n'),
        [&](auto line) {
            auto str = string(line.begin(), line.end());
            size_t pos = (size_t)str.find(':');
            auto start = string(str.substr(0, pos));
            auto res = string(str.substr(pos + 2));
            vector<string> destinations = res
                | views::split(' ')
                | views::transform([&](auto dest) {
                    auto dest_str = string(dest.begin(), dest.end());
                    if (back_itinerary.contains(dest_str)) {
                        back_itinerary[dest_str].push_back(start);
                    } else {
                        vector<string> tmp = {start};
                        back_itinerary[dest_str] = tmp;
                    }
                    return dest_str;
                }) | ranges::to<vector>();
            itinerary[start] = destinations;
        }
    );
    map<string, uint64_t> the_ways1 = {};
    auto mult1 = count_ways(START2, PITSTOP1, &back_itinerary, &the_ways1);
    map<string, uint64_t> the_ways2 = {};
    auto mult2 = count_ways(PITSTOP1, PITSTOP2, &back_itinerary, &the_ways2);
    map<string, uint64_t> the_ways3 = {};
    auto mult3 = count_ways(PITSTOP2, FINISH, &back_itinerary, &the_ways3);
    return mult1 * mult2 * mult3;
}

}
