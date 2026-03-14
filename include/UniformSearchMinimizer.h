#pragma once

#include "IMinimizer.h"

// Метод равномерного поиска (перебор по сетке).
// Алгоритм: отрезок [a,b] делится на N = ceil((b-a)/eps) равных частей,
// функция вычисляется в N+1 узлах; возвращается точка с наименьшим значением.
// Сложность: O(1/eps) — число обращений линейно растёт с уменьшением eps.
class UniformSearchMinimizer : public IMinimizer {
public:
    MinimizationResult minimize(
        const std::function<double(double)>& f,
        double a, double b, double eps) override;

    const char* name() const override { return "Метод равномерного поиска"; }

    // Теоретическое число обращений: N_calls = ceil((b-a)/eps) + 1
    int theoretical_calls(double a, double b, double eps) const override;
};
