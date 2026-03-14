#include "UniformSearchMinimizer.h"
#include <cmath>

MinimizationResult UniformSearchMinimizer::minimize(
    const std::function<double(double)>& f,
    double a, double b, double eps)
{
    int calls = 0;

    // Число равных подотрезков (шаг сетки не превышает eps)
    int n = static_cast<int>(std::ceil((b - a) / eps));

    // Длина одного подотрезка
    double step = (b - a) / n;

    // Инициализируем минимум в левом конце отрезка
    double x_min = a;
    double f_min = f(a);
    ++calls;

    // Перебираем все внутренние узлы и правый конец
    for (int i = 1; i <= n; ++i) {
        double x  = a + i * step;   // текущий узел сетки
        double fx = f(x);            // значение функции в узле
        ++calls;
        if (fx < f_min) {
            f_min = fx;   // обновляем минимальное значение
            x_min = x;    // обновляем точку минимума
        }
    }

    return { x_min, f_min, calls, theoretical_calls(a, b, eps) };
}

int UniformSearchMinimizer::theoretical_calls(double a, double b, double eps) const
{
    // N = ceil((b-a)/eps) подотрезков => N+1 узел => N+1 обращений к функции
    return static_cast<int>(std::ceil((b - a) / eps)) + 1;
}
