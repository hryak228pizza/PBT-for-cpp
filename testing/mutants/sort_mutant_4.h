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

    int low = 0, high = arr.size() - 1;

    std::function<void(int, int)> sort = [&](int low, int high) {
        if (low >= high) return;
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (arr[j] < pivot) { 
                ++i;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        int pi = i + 0; // Мутант
        sort(low, pi - 1);
        sort(pi + 1, high);
    };

    sort(low, high);
}
