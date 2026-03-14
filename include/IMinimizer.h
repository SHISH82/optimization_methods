#pragma once

#include "MinimizationResult.h"
#include <functional>

// Абстрактный интерфейс метода одномерной минимизации.
// Соответствует принципам SOLID:
//   O — открыт для расширения (новый метод = новый класс-наследник),
//   L — наследники взаимозаменяемы через указатель IMinimizer*,
//   I — интерфейс минимален и не содержит лишних методов.
class IMinimizer {
public:
    virtual ~IMinimizer() = default;

    // Найти минимум функции f на отрезке [a, b] с точностью eps.
    // Возвращает структуру с точкой минимума, значением функции
    // и счётчиками обращений (фактическим и теоретическим).
    virtual MinimizationResult minimize(
        const std::function<double(double)>& f,
        double a, double b, double eps) = 0;

    // Название метода (для вывода в таблицу сравнения)
    virtual const char* name() const = 0;

    // Теоретическое число обращений к функции для достижения точности eps
    // на отрезке [a, b] согласно формуле конкретного метода
    virtual int theoretical_calls(double a, double b, double eps) const = 0;
};
