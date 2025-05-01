
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>

//namespace cppqc = autocheck;
using namespace autocheck;



// Проверка коммутативности сложения
struct prop_commutative_add {
    bool operator() (int a, int b) const {
        return (a + b) == (b + a);
    }
};

// Проверка коммутативности умножения
struct prop_commutative_mul {
    bool operator() (int a, int b) const {
        return (a * b) == (b * a);
    }
};

struct prop_associative_add {
    bool operator() (int a, int b, int c) const {
        return ((a + b) + c) == (a + (b + c));
    }
};

struct prop_associative_mul {
    bool operator() (int a, int b, int c) const {
        return ((a * b) * c) == (a * (b * c));
    }
};

struct prop_distributive {
    bool operator() (int a, int b, int c) const {
        return (a * (b + c)) == (a * b + a * c);
    }
};

struct prop_square_non_negative {
    bool operator() (int a) const {
        return (a * a) >= 0;
    }
};

struct prop_square_root {
    bool operator() (double a) const {
        double squared = a * a;
        double root = std::sqrt(squared);
        return std::abs(root - std::abs(a)) < 1e-9; // Учет погрешности double
    }
};

// Тест для чётных чисел (аналог RC_PRE)
auto even_arb = make_arbitrary<int>().discard_if([](int x) { return x % 2 != 0; });

struct CapitalizedStringGen {
    generator<std::string> source;

    typedef std::string result_type;

    std::string operator() (size_t size) {
        std::string s = source(size);
        if (!s.empty()) s[0] = toupper(s[0]);
        return s;
    }
};

auto sorted_arb = make_arbitrary<std::vector<int>>()
.prep([](std::vector<int>& xs) {
    std::sort(xs.begin(), xs.end());
    });

int main() {
    // Проверка коммутативности сложения
    check<int, int>(prop_commutative_add(), 100, make_arbitrary<int, int>());

    // Проверка коммутативности умножения
    check<int, int>(prop_commutative_mul(), 100, make_arbitrary<int, int>());

    // Проверка ассоциативности сложения
    check<int, int, int>(prop_associative_add(), 100, make_arbitrary<int, int, int>());

    // Проверка ассоциативности умножения
    check<int, int, int>(prop_associative_mul(), 100, make_arbitrary<int, int, int>());

    check<int, int, int>(prop_distributive(), 100, make_arbitrary<int, int, int>());

    // Проверка неотрицательности квадрата
    check<int>(prop_square_non_negative(), 100, make_arbitrary<int>());

    // Проверка корня из квадрата (для double)
    check<double>(prop_square_root(), 100, make_arbitrary<double>());

    check<int>([](int x) {
        return (x % 2) == 0;
        }, 100, even_arb);

    check<std::string>([](const std::string& s) {
        return s.empty() || isupper(s[0]);
        }, 100, make_arbitrary(CapitalizedStringGen()).discard_if([](std::string s) { return !isalpha(s[0]); }));

    check<std::vector<int>>([](const std::vector<int>& xs) {
        return std::is_sorted(xs.begin(), xs.end());
        }, 100, sorted_arb);


    // классификатор
    classifier<int> cls;
    cls.trivial([](int x) { return x == 0; });
    cls.collect([](int x) { return x % 10; });
    cls.classify([](int x) { return x < 0; }, "negative");

    // репортер (опционально)
    ostream_reporter rep(std::cout);

    check<int>(
        [](int x) { return x == x; },
        50, make_arbitrary<int>(), rep, cls);

    return 0;
}

