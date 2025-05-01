#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <chrono>
#include <rapidcheck.h>
#include <functional>

inline void quickSort(std::vector<int>& arr) {
    if (arr.empty()) return;
    std::sort(arr.begin(), arr.end());
}
