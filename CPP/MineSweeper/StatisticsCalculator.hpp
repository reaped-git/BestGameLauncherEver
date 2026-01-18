#pragma once

#include <type_traits>
#include <numeric>
#include <algorithm>
#include <vector>
#include <cmath>

// ВНЕ пространства имен MineSweeper, в глобальном пространстве имен
template<typename Container, typename ValueType = typename Container::value_type>
auto CalculateStatistics(const Container& data)
-> typename std::enable_if<std::is_arithmetic<ValueType>::value,
    struct StatisticsResult>::type {

    struct StatisticsResult {
        ValueType min;
        ValueType max;
        double average;
        double median;
        double standardDeviation;
    };

    StatisticsResult result{};

    if (data.empty()) {
        return result;
    }

    // Минимальное значение (алгоритм поиска)
    result.min = *std::min_element(data.begin(), data.end());

    // Максимальное значение (алгоритм поиска)
    result.max = *std::max_element(data.begin(), data.end());

    // Среднее значение
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    result.average = sum / data.size();

    // Медиана
    std::vector<ValueType> sortedData(data.begin(), data.end());
    std::sort(sortedData.begin(), sortedData.end());

    size_t size = sortedData.size();
    if (size % 2 == 0) {
        result.median = (sortedData[size / 2 - 1] + sortedData[size / 2]) / 2.0;
    }
    else {
        result.median = sortedData[size / 2];
    }

    // Стандартное отклонение
    double variance = 0.0;
    for (const auto& value : data) {
        variance += std::pow(value - result.average, 2);
    }
    variance /= data.size();
    result.standardDeviation = std::sqrt(variance);

    return result;
}

// Другие шаблонные функции тоже вне пространства имен
template<typename Container, typename Predicate>
Container FilterData(const Container& data, Predicate pred) {
    Container filtered;
    std::copy_if(data.begin(), data.end(), std::back_inserter(filtered), pred);
    return filtered;
}

template<typename Container, typename TransformFunc>
auto TransformData(const Container& data, TransformFunc func)
-> std::vector<decltype(func(*data.begin()))> {

    std::vector<decltype(func(*data.begin()))> transformed;
    transformed.reserve(data.size());

    std::transform(data.begin(), data.end(),
        std::back_inserter(transformed), func);

    return transformed;
}

// Только namespace MineSweeper объявляет классы и типы
namespace MineSweeper {
    // Здесь объявляем только то, что должно быть в MineSweeper
    using ::CalculateStatistics;  // импортируем из глобального пространства
    using ::FilterData;
    using ::TransformData;
}