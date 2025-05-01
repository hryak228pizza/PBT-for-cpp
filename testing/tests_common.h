#pragma once
#include "mutants/sort_current.h"
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <rapidcheck/include/rapidcheck.h>

template<typename Dummy>
bool isSorted(const std::vector<int>& vec) {
    return std::is_sorted(vec.begin(), vec.end());
}

template<typename Dummy>
bool hasSameContent(const std::vector<int>& a, const std::vector<int>& b) {
    std::unordered_map<int, int> countA, countB;
    for (int i : a) countA[i]++;
    for (int i : b) countB[i]++;
    return countA == countB;
}

template<typename Dummy>
int sort_property_tests() {
    bool ok = rc::check("Mutation test", [](const std::vector<int>& vec) {
        std::vector<int> original = vec;
        std::vector<int> sorted = vec;

        quickSort(sorted);

        RC_ASSERT(sorted.size() == original.size());
        RC_ASSERT(hasSameContent<Dummy>(sorted, original));
        RC_ASSERT(isSorted<Dummy>(sorted));
    });

    return ok ? 0 : 1;
}
