#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "GoldenSectionMinimizer.h"
#include "IMinimizer.h"
#include "UniformSearchMinimizer.h"

// Тестовая функция 1: f(x) = x*sin(x) + 2*cos(x) на отрезке [-5, -4].
// Функция унимодальна на данном отрезке (проверяется графически через plot.py).
static double testFunction1(double x) {
    return x * std::sin(x) + 2.0 * std::cos(x);
}

// -----------------------------------------------------------------------
// Выравнивает UTF-8 строку по ширине отображения.
// setw() считает байты, а не символы: кириллица — 2 байта на символ, но
// отображается как 1. Эта функция добавляет пробелы по display-ширине.
// -----------------------------------------------------------------------
static std::string utf8pad(const std::string& s, int display_width) {
    int display_len = 0;
    for (unsigned char c : s) {
        // Байты-продолжения UTF-8 имеют вид 10xxxxxx — не считаем их
        if ((c & 0xC0) != 0x80) ++display_len;
    }
    int padding = std::max(0, display_width - display_len);
    return s + std::string(padding, ' ');
}

// Форматирует вещественное число в строку заданной display-ширины (left-align)
static std::string col(double v, int prec, int width) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(prec) << v;
    std::string s = oss.str();
    if ((int)s.size() < width) s += std::string(width - s.size(), ' ');
    return s;
}

// Форматирует целое число в строку заданной display-ширины (left-align)
static std::string col(int v, int width) {
    std::string s = std::to_string(v);
    if ((int)s.size() < width) s += std::string(width - s.size(), ' ');
    return s;
}

// -----------------------------------------------------------------------
// Вывод теоретических формул для числа обращений к функции
// -----------------------------------------------------------------------
void printFormulas() {
    std::cout << "=== Теоретические формулы для числа обращений к функции ===\n\n";

    std::cout << "Метод равномерного поиска:\n";
    std::cout << "  Отрезок [a,b] делится на N = ceil((b-a)/ε) равных частей.\n";
    std::cout << "  Функция вычисляется в N+1 узлах  =>  N_вызовов = ceil((b-a)/ε) + 1\n";
    std::cout << "  Гарантия точности: |x* - x_найденное| <= ε.\n\n";

    std::cout << "Метод золотого сечения:\n";
    std::cout << "  φ = (sqrt(5)-1)/2 ≈ 0.6180\n";
    std::cout << "  Каждая итерация сокращает отрезок в φ раз.\n";
    std::cout << "  Число итераций: n = ceil( log((b-a)/ε) / log(1/φ) )\n";
    std::cout << "  Всего вызовов:  N_вызовов = n + 2 (начальные) + 1 (финальное)\n\n";
}

// -----------------------------------------------------------------------
// Таблица результатов: отдельный блок на каждый метод, строки — значения ε
// -----------------------------------------------------------------------
void runComparison(
    const std::vector<std::unique_ptr<IMinimizer>>& methods,
    const std::function<double(double)>& f,
    double a, double b,
    const std::vector<double>& epsilons,
    const std::string& funcName)
{
    // Ширины колонок (в отображаемых символах)
    const int W_EPS  = 10;
    const int W_X    = 15;
    const int W_F    = 15;
    const int W_CALL = 18;
    const int W_TEOR = 10;
    const int LINE   = W_EPS + W_X + W_F + W_CALL + W_TEOR;

    std::cout << "=== Минимизация " << funcName
              << " на [" << a << ", " << b << "] ===\n\n";

    for (auto& method : methods) {
        // Название метода как заголовок блока
        std::cout << method->name() << "\n\n";

        // Шапка таблицы (utf8pad выравнивает с учётом многобайтовых символов)
        std::cout << utf8pad("ε",              W_EPS)
                  << utf8pad("x*",             W_X)
                  << utf8pad("f(x*)",          W_F)
                  << utf8pad("Вызовы функции", W_CALL)
                  << utf8pad("Теория",         W_TEOR)
                  << "\n";
        std::cout << std::string(LINE, '-') << "\n";

        for (double eps : epsilons) {
            MinimizationResult r = method->minimize(f, a, b, eps);
            std::cout << col(eps,                  3, W_EPS)
                      << col(r.x_min,              6, W_X)
                      << col(r.f_min,              6, W_F)
                      << col(r.actual_calls,           W_CALL)
                      << col(r.theoretical_calls,      W_TEOR)
                      << "\n";
        }
        std::cout << "\n";
    }
}

// -----------------------------------------------------------------------
// Сравнение числа обращений по теоретическим формулам
// -----------------------------------------------------------------------
void printAnalysis(
    const std::vector<std::unique_ptr<IMinimizer>>& methods,
    double a, double b,
    const std::vector<double>& epsilons)
{
    const int W_EPS  = 12;
    const int W_U    = 22;
    const int W_G    = 20;

    std::cout << "=== Сравнение методов по числу обращений ===\n\n";
    std::cout << utf8pad("ε",                  W_EPS)
              << utf8pad("Равномерный поиск",  W_U)
              << utf8pad("Золотое сечение",    W_G)
              << "Быстрее\n";
    std::cout << std::string(W_EPS + W_U + W_G + 10, '-') << "\n";

    for (double eps : epsilons) {
        int u = methods[0]->theoretical_calls(a, b, eps);
        int g = methods[1]->theoretical_calls(a, b, eps);
        std::cout << col(eps, 3, W_EPS)
                  << col(u,       W_U)
                  << col(g,       W_G)
                  << (g < u ? "Золотое сечение" : "Равномерный поиск")
                  << "\n";
    }

    std::cout << "\nВывод:\n"
              << "  Равномерный поиск требует O(1/ε) обращений — растёт очень быстро при ε → 0.\n"
              << "  Метод золотого сечения требует O(log(1/ε)) обращений — растёт значительно медленнее.\n"
              << "  Поэтому золотое сечение предпочтительнее при высокой требуемой точности (малых ε).\n\n";
}

int main() {
    const double a = -5.0, b = -4.0;                           // границы отрезка минимизации
    const std::vector<double> epsilons = { 0.1, 0.01, 0.001 }; // три уровня точности

    // Список методов (принцип открытости/закрытости: добавить метод = добавить объект в вектор)
    std::vector<std::unique_ptr<IMinimizer>> methods;
    methods.push_back(std::make_unique<UniformSearchMinimizer>());
    methods.push_back(std::make_unique<GoldenSectionMinimizer>());

    printFormulas();

    runComparison(methods, testFunction1, a, b, epsilons, "f(x) = x*sin(x) + 2*cos(x)");
    printAnalysis(methods, a, b, epsilons);

    return 0;
}
