#pragma once

// Структура для хранения результата одномерной минимизации
struct MinimizationResult {
    double x_min;           // Точка минимума (аргумент)
    double f_min;           // Значение функции в точке минимума
    int actual_calls;       // Фактическое число обращений к функции
    int theoretical_calls;  // Теоретически ожидаемое число обращений
};
