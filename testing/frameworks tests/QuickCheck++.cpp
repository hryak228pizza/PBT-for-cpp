// TestingToolsCons.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include <quickcheck/quickcheck.hh>

#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace quickcheck;



class PAdditionIsCommutative : public Property<int, int> {
public:
    // a + b должно равняться b + a
    bool holdsFor(const int& a, const int& b) {
        return (a + b) == (b + a);
    }
};

void testAdditionCommutativity() {
    PAdditionIsCommutative prop;
    prop.check(1000);
}

class PMultiplicationIsAssociative : public Property<int, int, int> {
public:
    // (a*b)*c == a*(b*c)
    bool holdsFor(const int& a, const int& b, const int& c) {
        return (a * b) * c == a * (b * c);
    }
};

void testMultiplicationAssociativity() {
    PMultiplicationIsAssociative prop;
    prop.check(1000);
}

class PMultiplicationDistributesOverAddition : public Property<int, int, int> {
public:
    // a*(b+c) == a*b + a*c
    bool holdsFor(const int& a, const int& b, const int& c) {
        return a * (b + c) == (a * b) + (a * c);
    }
};

void testDistributivity() {
    PMultiplicationDistributesOverAddition prop;
    prop.check(1000);
}

class PPowerProperty : public Property<int> {
public:
    bool holdsFor(const int& n) {
        // Проверяем, что n^2 == n * n
        return pow(n, 2) == n * n;
    }

    // Исключаем слишком большие числа, чтобы избежать переполнения
    bool accepts(const int& n) {
        return abs(n) < 10000;
    }
};

void testPower() {
    PPowerProperty prop;
    prop.check(200);
}

class PSubtractionInverse : public Property<int, int> {
public:
    // (a + b) - b = a
    bool holdsFor(const int& a, const int& b) {
        return (a + b) - b == a;
    }
};

void testSubtraction() {
    PSubtractionInverse prop;
    prop.check(300);
}

void insert_sorted(int x, std::vector<int>& vec) {
    auto it = std::lower_bound(vec.begin(), vec.end(), x);
    vec.insert(it, x);
}

class PInsertIntoSorted : public Property<int, std::vector<int>> {
public:
    bool holdsFor(const int& x, const std::vector<int>& xs) {
        std::vector<int> ys = xs;
        insert_sorted(x, ys);
        return is_sorted(ys.begin(), ys.end());
    }

    bool accepts(const int& x, const std::vector<int>& xs) {
        return is_sorted(xs.begin(), xs.end());
    }

    const std::string classify(const int& x, const std::vector<int>& xs) {
        if (xs.empty()) return "empty";
        if (x < xs.front()) return "head";
        if (x > xs.back()) return "tail";
        return "middle";
    }
};

void testSortedInsert() {
    PInsertIntoSorted prop;
    prop.addFixed(5, { 1, 2, 3 });
    prop.check(500, 50000, "Вставка в сортированный вектор");
}

struct Triangle {
    double a, b, c;
    friend std::ostream& operator<<(std::ostream& os, const Triangle& t) {
        return os << "{" << t.a << ", " << t.b << ", " << t.c << "}";
    }
};
void generate(size_t n, Triangle& out) {
    const double min_side = 0.1;
    const double max_side = 100.0;

    // Генерация двух сторон
    out.a = std::max(min_side, generateInRange(min_side, max_side));
    out.b = std::max(min_side, generateInRange(min_side, max_side));

    // Упорядочивание
    if (out.a > out.b) std::swap(out.a, out.b);

    // Генерация третьей стороны
    const double min_c = out.b - out.a;
    const double max_c = out.a + out.b;
    out.c = generateInRange(min_c + 0.001, max_c - 0.001);

    // Дополнительная проверка
    if (!(out.a + out.b > out.c && out.a + out.c > out.b && out.b + out.c > out.a)) {
        generate(n, out);
    }
}
class PTriangleInequality : public Property<Triangle> {
public:
    bool holdsFor(const Triangle& t) {
        const double epsilon = 1e-10;
        bool valid = (t.a + t.b > t.c - epsilon) &&
            (t.a + t.c > t.b - epsilon) &&
            (t.b + t.c > t.a - epsilon);

        return valid;
    }
    bool isTrivialFor(const Triangle& t) {
        return t.a < 1.0 || t.b < 1.0 || t.c < 1.0;
    }
};
void testTriangleInequality() {
    PTriangleInequality prop;
    prop.check(100, 1000, "Треугольники");
}

class PIntAddition : public Property<int, int> {
public:
    bool holdsFor(const int& a, const int& b) override {
        return (a + b) == (b + a);
    }
};

void testVerbose() {
    PIntAddition prop;
    prop.check(3, 0, true);
}

class PSumProperty : public Property<int, int> {
public:
    bool holdsFor(const int& a, const int& b) override {
        return (a + b) > a && (a + b) > b;
    }

    void generateInput(size_t n, int& a, int& b) override {
        a = generateInRange(1, 1000);  // Генератор для первого числа
        b = generateInRange(1, 1000);  // Генератор для второго числа
    }
};

void testSum() {
    PSumProperty prop;
    prop.check(10);
}

class PNextNumber : public Property<int> {
public:
    bool holdsFor(const int& n) override {
        return n < n + 1;  // Всегда true, кроме INT_MAX
    }

    void setUp() {
        addFixed(0);
        addFixed(-100);
        addFixed(100);
        addFixed(INT_MAX);
        addFixed(INT_MIN);
    }
};

void testNumberLimits() {
    PNextNumber prop;
    prop.check(5, 0, true);
}


int main() {
    //testAdditionCommutativity();
    //testMultiplicationAssociativity();
    //testDistributivity();
    //testPower();
    //testSubtraction();

    //testSortedInsert();
    //testTriangleInequality();
    //testVerbose();
    //testSum();
    testNumberLimits();

    return 0;
}
