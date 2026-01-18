#pragma once

#include <type_traits>
#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>

namespace Match3 {

    /// <summary>
    /// Проверяет, является ли тип числовым (целым или с плавающей точкой)
    /// </summary>
    template<typename T>
    struct is_numeric {
        static constexpr bool value =
            std::is_integral<T>::value ||
            std::is_floating_point<T>::value;
    };

    /// <summary>
    /// Шаблонная функция для вычисления среднего значения в контейнере
    /// Работает только с числовыми типами
    /// </summary>
    template<typename Container>
    typename std::enable_if<is_numeric<typename Container::value_type>::value, double>::type
        CalculateAverage(const Container& container) {

        if (container.empty()) {
            return 0.0;
        }

        double sum = 0.0;
        for (const auto& value : container) {
            sum += static_cast<double>(value);
        }

        return sum / container.size();
    }

    /// <summary>
    /// Перегрузка для простых массивов
    /// </summary>
    template<typename T>
    typename std::enable_if<is_numeric<T>::value, double>::type
        CalculateAverage(const T* array, size_t size) {

        if (size == 0) {
            return 0.0;
        }

        double sum = 0.0;
        for (size_t i = 0; i < size; ++i) {
            sum += static_cast<double>(array[i]);
        }

        return sum / size;
    }

    /// <summary>
    /// Шаблонная функция для поиска максимального значения в контейнере
    /// Работает с любыми типами, поддерживающими оператор сравнения
    /// </summary>
    template<typename Container>
    typename Container::value_type FindMaxValue(const Container& container) {
        if (container.empty()) {
            throw std::runtime_error("Container is empty");
        }

        return *std::max_element(container.begin(), container.end());
    }

    /// <summary>
    /// Перегрузка для простых массивов
    /// </summary>
    template<typename T>
    T FindMaxValue(const T* array, size_t size) {
        if (size == 0) {
            throw std::runtime_error("Array is empty");
        }

        T maxVal = array[0];
        for (size_t i = 1; i < size; ++i) {
            if (array[i] > maxVal) {
                maxVal = array[i];
            }
        }

        return maxVal;
    }

}