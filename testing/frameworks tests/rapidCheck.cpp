// TestingToolsCons.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <rapidcheck.h>
 // #include <boost/test/included/unit_test.hpp>
#include <boost_test/include/rapidcheck/boost_test.h>



void main() {

    setlocale(LC_ALL, "ru");


    std::cout << "---------------------------------------" << std::endl;

    // всегда истина   
    rc::check([](int x) {
        RC_ASSERT(x + 1 > x);
        });

    std::cout << "---------------------------------------" << std::endl;

    // всегда истина
    rc::check([](int x) {
        RC_ASSERT_FALSE(x * 0 != 0);
        });

    std::cout << "---------------------------------------" << std::endl;

    // std::stoi("abc") должно выбросить std::invalid_argument по адресу памяти 0x..
    rc::check([]() {
        RC_ASSERT_THROWS(std::stoi("abc"));
        });

    std::cout << "---------------------------------------" << std::endl;

    // всегда истина
    rc::check([]() {
        RC_ASSERT_THROWS_AS(std::stoi("abc"), std::invalid_argument);
        });
    // провал
    rc::check([]() {
        RC_ASSERT_THROWS_AS(std::stoi("52"), std::invalid_argument);
        });

    std::cout << "---------------------------------------" << std::endl;

    // всегда провал
    rc::check([]() {
        RC_FAIL("IT ALWAYS FAILS");
        });
    // всегда успех
    rc::check([]() {
        RC_SUCCEED("IT ALWAYS PASSES");
        });
    std::cout << "---------------------------------------" << std::endl;


    rc::check([](int x) {
        RC_SUCCEED_IF(x == 52);
        RC_ASSERT(x - 52 != 0);
        });

    std::cout << "---------------------------------------" << std::endl;

    rc::check([](int x) {
        RC_PRE(x > 0);
        RC_ASSERT(x + x >= x);
        });

    std::cout << "---------------------------------------" << std::endl;

    rc::check([](int x) {
        if (x < 0) {
            RC_DISCARD("Отбрасываем тест, если x < 0");
        }
        RC_ASSERT(x >= 0);
        });



    rc::check("Коммутативность сложения", [](int a, int b) {
        RC_ASSERT(a + b == b + a);
        });

    rc::check("Коммутативность умножения", [](int a, int b) {
        RC_ASSERT(a * b == b * a);
        });

    rc::check("Ассоциативность сложения", [](int a, int b, int c) {
        RC_ASSERT((a + b) + c == a + (b + c));
        });

    rc::check("Ассоциативность умножения", [](int a, int b, int c) {
        RC_ASSERT((a * b) * c == a * (b * c));
        });

    rc::check("Дистрибутивность умножения", [](int a, int b, int c) {
        RC_ASSERT(a * (b + c) == a * b + a * c);
        });

    rc::check("Cложения c нулем", [](int a) {
        RC_ASSERT(a + 0 == a);
        });

    rc::check("Умножение на единицу", [](int a) {
        RC_ASSERT(a * 1 == a);
        });

    rc::check("Свойство деления", [](int a, int b) {
        RC_PRE(b != 0);
        RC_ASSERT((a / b) * b == a - (a % b));
        });

    rc::check("Четность чисел", [](int x) {
        RC_ASSERT((x % 2 == 0) == ((x / 2) * 2 == x));
        });

    rc::check("Нечетность чисел", [](int x) {
        RC_PRE(x >= 0);
        RC_ASSERT((x % 2 != 0) == ((x / 2) * 2 + 1 == x));
        });

    rc::check("Корень из квадрата", [](unsigned int x) {
        x = *rc::gen::inRange(0, 25000);
        // RC_PRE(x < 10000);
        RC_ASSERT(std::sqrt(x * x) == x);
        });

    rc::check("Обратный элемент сложения", [](int a) {
        RC_ASSERT(a - a == 0);
        });

    rc::check("Обратный элемент умножения", [](int a) {
        RC_PRE(a != 0);
        RC_ASSERT(a / a == 1);
        });


    rc::check([](const std::vector<int>& vec) {
        std::vector<int> copy = vec;
        std::vector<int> copy2 = copy;
        RC_ASSERT(vec == copy2);
        });


    rc::check([](const std::vector<int>& values) {
        std::set<int> uniqueSet(values.begin(), values.end());

        RC_ASSERT(uniqueSet.size() <= values.size());
        });


    rc::check([](const std::vector<int>& values) {
        std::stack<int> stack;
        for (int val : values) {
            stack.push(val);
        }

        std::vector<int> reversed;
        while (!stack.empty()) {
            reversed.push_back(stack.top());
            stack.pop();
        }

        RC_ASSERT(reversed == std::vector<int>(values.rbegin(), values.rend()));
        });
}
