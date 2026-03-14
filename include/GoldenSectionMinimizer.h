#pragma once

#include "IMinimizer.h"

// Метод золотого сечения.
// На каждой итерации длина отрезка неопределённости сокращается в phi ≈ 0.618 раз,
// при этом достаточно одного нового вычисления функции (одно из двух значений
// переиспользуется). Сложность: O(log(1/eps)) — значительно эффективнее
// равномерного поиска при малых eps.
class GoldenSectionMinimizer : public IMinimizer {
public:
    MinimizationResult minimize(
        const std::function<double(double)>& f,
        double a, double b, double eps) override;

    const char* name() const override { return "Метод золотого сечения"; }

    // Теоретическое число обращений:
    //   n = ceil( log((b-a)/eps) / log(1/phi) )  итераций,
    //   плюс 2 начальных вычисления + 1 финальное => итого n+3
    int theoretical_calls(double a, double b, double eps) const override;
};
