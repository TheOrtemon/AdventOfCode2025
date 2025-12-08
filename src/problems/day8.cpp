#include "day8.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <cstdlib>
#include <ranges>
#include <print>
#include <sys/types.h>

using namespace std;
namespace adventofcode
{
Day8::Day8() : DaySolver(8) {
    // Data for tests
    test_data = R"(162,817,812
57,618,57
906,360,560
592,479,940
352,342,300
466,668,158
542,29,236
431,825,988
739,650,466
52,470,668
216,146,977
819,987,18
117,168,530
805,96,715
346,949,466
970,615,88
941,993,340
862,61,35
984,92,344
425,690,689)";
    test_results = { 40, 25272 };
}

struct Point {
    int64_t x;
    int64_t y;
    int64_t z;
};

ResultType Day8::solvePart1() {
    auto line_range = input_data
    | views::split('\n')
    | views::transform([](auto line) {
        return string_view(line) | views::split(',');
    });

    vector<Point> points = {};

    for (auto line: line_range) {
        array<int64_t, 3> arr = {};
        for (auto [i, num]: line | views::enumerate) {
            arr[i] = stoll(string(num.begin(), num.end()));
        }
        auto p = Point { .x = arr[0], .y = arr[1], .z = arr[2] };
        // println("{}", p.x);
        points.push_back(p);
    }

    vector<tuple<size_t, size_t, size_t>> ddistances = {};
    for (auto i = 0ULL; i < points.size(); i++) {
        for (auto j = i + 1; j < points.size(); j++) {
            auto p1 = points[i];
            auto p2 = points[j];
            auto ddistance = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z);
            ddistances.push_back(tuple(ddistance, i, j));
        }
    }
    sort(ddistances.begin(), ddistances.end(),
         [](auto& a, auto& b) {
            return get<0>(a) < get<0>(b);
         });

    map<size_t, size_t> pitoci_map = {};
    vector<vector<size_t>> curcuits = {};
    for (auto i = (size_t)0; i < 1000; i++) {
        auto ddistance = ddistances[i];
        auto first = get<1>(ddistance);
        auto second = get<2>(ddistance);
        if (!pitoci_map.contains(first) && !pitoci_map.contains(second)) {
            pitoci_map[first] = curcuits.size();
            pitoci_map[second] = curcuits.size();
            vector<size_t> curcuit = {first, second};
            curcuits.push_back(curcuit);
        } else if (pitoci_map.contains(first) && pitoci_map.contains(second)) {
            if (pitoci_map[first] == pitoci_map[second]) {
                continue;
            }
            auto curcuit_id = pitoci_map[second];
            for (auto pi: curcuits[curcuit_id]) {
                curcuits[pitoci_map[first]].push_back(pi);
                pitoci_map[pi] = pitoci_map[first];
            }
            curcuits[curcuit_id].clear();
        } else if (pitoci_map.contains(first)) {
            pitoci_map[second] = pitoci_map[first];
            curcuits[pitoci_map[second]].push_back(second);
        } else {
            pitoci_map[first] = pitoci_map[second];
            curcuits[pitoci_map[second]].push_back(first);
        }
    }

    auto curcuit_sizes = curcuits
        | views::transform([](auto curc) {
            return curc.size();
        }) | ranges::to<vector>();
    sort(curcuit_sizes.begin(), curcuit_sizes.end(),
         [](auto& a, auto&b) {
            return a > b;
         });
    // println("{}, {}", points[get<1>(ddistances[0])].x, points[get<2>(ddistances[0])].x);
    // println("{}, {}", curcuit_sizes[0], curcuit_sizes[1]);
    return curcuit_sizes[0] * curcuit_sizes[1] * curcuit_sizes[2];
}

ResultType Day8::solvePart2() {
    auto line_range = input_data
    | views::split('\n')
    | views::transform([](auto line) {
        return string_view(line) | views::split(',');
    });

    vector<Point> points = {};

    for (auto line: line_range) {
        array<int64_t, 3> arr = {};
        for (auto [i, num]: line | views::enumerate) {
            arr[i] = stoll(string(num.begin(), num.end()));
        }
        auto p = Point { .x = arr[0], .y = arr[1], .z = arr[2] };
        // println("{}", p.x);
        points.push_back(p);
    }

    vector<tuple<size_t, size_t, size_t>> ddistances = {};
    for (auto i = 0ULL; i < points.size(); i++) {
        for (auto j = i + 1; j < points.size(); j++) {
            auto p1 = points[i];
            auto p2 = points[j];
            auto ddistance = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z);
            ddistances.push_back(tuple(ddistance, i, j));
        }
    }
    sort(ddistances.begin(), ddistances.end(),
         [](auto& a, auto& b) {
            return get<0>(a) < get<0>(b);
         });

    map<size_t, size_t> pitoci_map = {};
    vector<vector<size_t>> curcuits = {};
    size_t result = 0ULL;
    for (auto i = (size_t)0; i < ddistances.size(); i++) {
        auto ddistance = ddistances[i];
        auto first = get<1>(ddistance);
        auto second = get<2>(ddistance);
        if (!pitoci_map.contains(first) && !pitoci_map.contains(second)) {
            pitoci_map[first] = curcuits.size();
            pitoci_map[second] = curcuits.size();
            vector<size_t> curcuit = {first, second};
            curcuits.push_back(curcuit);
        } else if (pitoci_map.contains(first) && pitoci_map.contains(second)) {
            if (pitoci_map[first] == pitoci_map[second]) {
                continue;
            }
            auto curcuit_id = pitoci_map[second];
            for (auto pi: curcuits[curcuit_id]) {
                curcuits[pitoci_map[first]].push_back(pi);
                pitoci_map[pi] = pitoci_map[first];
            }
            curcuits[curcuit_id].clear();
            result = points[first].x * points[second].x;
        } else if (pitoci_map.contains(first)) {
            pitoci_map[second] = pitoci_map[first];
            curcuits[pitoci_map[second]].push_back(second);
            result = points[first].x * points[second].x;
        } else {
            pitoci_map[first] = pitoci_map[second];
            curcuits[pitoci_map[second]].push_back(first);
            result = points[first].x * points[second].x;
        }
    }
    return result;
}
}
