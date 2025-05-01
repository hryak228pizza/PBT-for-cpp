#include <autocheck/autocheck.hpp>
#include "TestingToolsCons/testing/buggy_sort.h"
#include <chrono>
#include <iostream>

using namespace autocheck;

struct IsSortedProperty {
    bool operator()(const std::vector<int>& vec) const {
        std::vector<int> sorted = vec;
        quickSort(sorted);
        return std::is_sorted(sorted.begin(), sorted.end());
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    auto start = std::chrono::high_resolution_clock::now();

    check<std::vector<int>>(IsSortedProperty(), 1000);

    auto end = std::chrono::high_resolution_clock::now();
    long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\n[CppQuickCheck] Время до ошибки: " << elapsed << " мс\n";
    return 0;
}
