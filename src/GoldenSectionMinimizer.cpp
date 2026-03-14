#include "GoldenSectionMinimizer.h"
#include <cmath>

// Золотое отношение: phi = (sqrt(5) - 1) / 2 ≈ 0.6180
// Две внутренние точки делят отрезок в пропорции (1-phi) : phi : (1-phi)
static constexpr double PHI = 0.6180339887498949;

MinimizationResult GoldenSectionMinimizer::minimize(
    const std::function<double(double)>& f,
    double a, double b, double eps)
{
    int calls = 0;

    // Сохраняем исходные границы для расчёта теоретического числа обращений
    // (a и b изменяются в ходе итераций, поэтому нужны оригинальные значения)
    const double a0 = a, b0 = b;

    // Начальное расположение двух внутренних точек золотого сечения
    double x1 = a + (1.0 - PHI) * (b - a);   // левая  точка деления
    double x2 = a + PHI         * (b - a);   // правая точка деления
    double f1 = f(x1);                         // f в левой точке
    double f2 = f(x2);                         // f в правой точке
    calls += 2;                                // два начальных обращения

    // Итерационное сужение отрезка неопределённости
    while ((b - a) > eps) {
        if (f1 < f2) {
            // Минимум в левой половине: сдвигаем правую границу
            b  = x2;
            x2 = x1;  f2 = f1;                // старая x1 становится новой x2
            x1 = a + (1.0 - PHI) * (b - a);  // новая левая точка
            f1 = f(x1);
            ++calls;
        } else {
            // Минимум в правой половине: сдвигаем левую границу
            a  = x1;
            x1 = x2;  f1 = f2;                // старая x2 становится новой x1
            x2 = a + PHI * (b - a);           // новая правая точка
            f2 = f(x2);
            ++calls;
        }
    }

    // Финальная оценка: середина оставшегося отрезка неопределённости
    double x_min = (a + b) / 2.0;
    double f_min = f(x_min);
    ++calls;

    return { x_min, f_min, calls, theoretical_calls(a0, b0, eps) };
}

int GoldenSectionMinimizer::theoretical_calls(double a, double b, double eps) const
{
    // Условие остановки: (b-a) * phi^n <= eps
    // => n >= log((b-a)/eps) / log(1/phi)
    // Итого обращений: n итерационных + 2 начальных + 1 финальное
    int n = static_cast<int>(std::ceil(std::log((b - a) / eps) / std::log(1.0 / PHI)));
    return n + 2 + 1;
}
