#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <array>
#include <utility>


// Общий класс генераторов
template<typename T>
class Gen {
public:
    Gen(std::function<T()> genFunc) : gen(genFunc) {}
    Gen() = default;

    T generate() { return gen(); }

private:
    std::function<T()> gen;
};

template<typename T>
//Gen<T> arbitrary();
Gen<T> arbitrary() {
    static_assert(sizeof(T) == 0, "No specialization exists for this type");
    return Gen<T>();
}

// Фабрики
template<>
Gen<int> arbitrary<int>() {
    return Gen<int>([]() {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_int_distribution<int> dist(-100, 100);
        return dist(rng);
        });
}
template<>
Gen<double> arbitrary<double>() {
    return Gen<double>([]() {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_real_distribution<double> dist(-100.0, 100.0);
        return dist(rng);
        });
}
template<>
Gen<char> arbitrary<char>() {
    return Gen<char>([]() {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_int_distribution<int> dist('A', 'z');
        return static_cast<char>(dist(rng));
        });
}
template<>
Gen<std::string> arbitrary<std::string>() {
    return Gen<std::string>([]() {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_int_distribution<int> dist('A', 'z');
        static std::uniform_int_distribution<int> lengthDist(5, 10);

        int len = lengthDist(rng);
        std::string result;
        for (int i = 0; i < len; ++i) {
            result += static_cast<char>(dist(rng));
        }
        return result;
        });
}
//// Генератор для std::vector
//template<typename T>
//Gen<std::vector<T>> arbitraryVector() {
//    return Gen<std::vector<T>>([]() {
//        static std::random_device rd;
//        static std::mt19937 rng(rd());
//        static std::uniform_int_distribution<int> lengthDist(1, 10);
//
//        int len = lengthDist(rng);
//        std::vector<T> result;
//        for (int i = 0; i < len; ++i) {
//            result.push_back(arbitrary<T>().generate());
//        }
//        return result;
//        });
//}
//// Генератор для std::list
//template<typename T>
//Gen<std::list<T>> arbitraryList() {
//    return Gen<std::list<T>>([]() {
//        static std::random_device rd;
//        static std::mt19937 rng(rd());
//        static std::uniform_int_distribution<int> lengthDist(1, 10);
//
//        int len = lengthDist(rng);
//        std::list<T> result;
//        for (int i = 0; i < len; ++i) {
//            result.push_back(arbitrary<T>().generate());
//        }
//        return result;
//        });
//}
//// Генератор для std::array (фиксированная длина)
//template<typename T, size_t N>
//Gen<std::array<T, N>> arbitraryArray() {
//    return Gen<std::array<T, N>>([]() {
//        std::array<T, N> result;
//        for (size_t i = 0; i < N; ++i) {
//            result[i] = arbitrary<T>().generate();
//        }
//        return result;
//        });
//}
// Генератор для std::vector (рекурсивный)
template<typename T>
Gen<std::vector<T>> arbitraryVector() {
    return Gen<std::vector<T>>([gen = arbitrary<T>()]() mutable {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_int_distribution<int> lengthDist(1, 5);

        int len = lengthDist(rng);
        std::vector<T> result;
        for (int i = 0; i < len; ++i) {
            result.push_back(gen.generate());
        }
        return result;
    });
}
// Генератор для std::list (рекурсивный)
template<typename T>
Gen<std::list<T>> arbitraryList() {
    return Gen<std::list<T>>([gen = arbitrary<T>()]() mutable {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_int_distribution<int> lengthDist(1, 5);

        int len = lengthDist(rng);
        std::list<T> result;
        for (int i = 0; i < len; ++i) {
            result.push_back(gen.generate());
        }
        return result;
    });
}
// Генератор для std::array (рекурсивный)
template<typename T, size_t N>
Gen<std::array<T, N>> arbitraryArray() {
    return Gen<std::array<T, N>>([gen = arbitrary<T>()]() mutable {
        std::array<T, N> result;
        for (size_t i = 0; i < N; ++i) {
            result[i] = gen.generate();
        }
        return result;
    });
}
// Генератор для std::pair (рекурсивный)
template<typename T1, typename T2>
Gen<std::pair<T1, T2>> arbitraryPair() {
    return Gen<std::pair<T1, T2>>([gen1 = arbitrary<T1>(), gen2 = arbitrary<T2>()]() mutable {
        return std::make_pair(gen1.generate(), gen2.generate());
    });
}

template<typename T>
void quickCheck(const std::function<bool(T)>& p, size_t n = 10) {
    Gen<T> g = arbitrary<T>();

    for (size_t i = 0; i < n; ++i) {
        T value = g.generate();
        bool result = p(value);

        // Выводим результат
        if (result) {
            std::cout << "[OK] value: " << value << std::endl;
        }
        else {
            std::cout << "[Failed] value: " << value << std::endl;
        }
    }
}



bool isWithinRange(int value) {
    return value >= -50 && value <= 50;
}

void main() {

    setlocale(LC_ALL, "ru");

    // Пример использования генераторов
    Gen<int> genInt = arbitrary<int>();
    Gen<double> genDouble = arbitrary<double>();
    Gen<char> genChar = arbitrary<char>();
    Gen<std::string> genString = arbitrary<std::string>();

    std::cout << "Random int: " << genInt.generate() << std::endl;
    std::cout << "Random double: " << genDouble.generate() << std::endl;
    std::cout << "Random char: " << genChar.generate() << std::endl;
    std::cout << "Random string: " << genString.generate() << std::endl;


    // Генерация и вывод случайного вектора
    Gen<std::vector<int>> vecGen = arbitraryVector<int>();
    std::vector<int> vec = vecGen.generate();
    std::cout << "Generated vector: ";
    for (const auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    // Генерация и вывод случайного Списка
    Gen<std::list<int>> lstGen = arbitraryList<int>();
    std::list<int> lst = lstGen.generate();
    std::cout << "Generated list: ";
    for (const auto& val : lst) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    // Генерация и вывод случайного массива
    Gen<std::array<int, 10>> arrGen = arbitraryArray<int, 10>();
    std::array<int, 10> arr = arrGen.generate();
    std::cout << "Generated array: ";
    for (const auto& val : arr) {
        std::cout << val << " ";
    }
    std::cout << std::endl;


    quickCheck<int>(isWithinRange, 10);


}

