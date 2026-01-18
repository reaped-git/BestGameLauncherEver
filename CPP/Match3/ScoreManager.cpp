#include "ScoreManager.hpp"
#include <iostream>

namespace Match3 {

    /// <summary>
    /// Конструктор менеджера счета
    /// </summary>
    /// <param name="scoreLabel">Метка для отображения счета</param>
    ScoreManager::ScoreManager()
        : currentScore(0), playerName("Player"),
        scoreHistory("PlayerScoreHistory")
    {
        bonusStrategy = std::make_shared<AdaptiveStrategy>();
        bonuses = { {
             { 15, 5 },
             { 10, 3 },
             { 5,  2 }
         } };
    }

    ScoreManager::ScoreManager(const std::string& name)
        : currentScore(0), playerName(name),
        scoreHistory("PlayerScoreHistory")
    {
        bonusStrategy = std::make_shared<AdaptiveStrategy>();
        bonuses = { {
             { 15, 5 },
             { 10, 3 },
             { 5,  2 }
         } };
    }

    // Конструктор копирования
    ScoreManager::ScoreManager(const ScoreManager& other)
        : bonuses(other.bonuses),
        currentScore(other.currentScore),
        playerName(other.playerName),
        scoreHistory(other.scoreHistory),
        recentScores(other.recentScores),
        bonusStrategy(other.bonusStrategy)
    {
    }

    // Оператор присваивания
    ScoreManager& ScoreManager::operator=(const ScoreManager& other)
    {
        if (this != &other)
        {
            bonuses = other.bonuses;
            currentScore = other.currentScore;
            playerName = other.playerName;
            scoreHistory = other.scoreHistory;
            recentScores = other.recentScores;
        }
        return *this;
    }

    ScoreManager& ScoreManager::operator+=(Int64 points)
    {
        currentScore += points;
        return *this;
    }

    ScoreManager ScoreManager::operator+(Int64 points) const
    {
        ScoreManager result(*this);
        result.currentScore += points;
        return result;
    }

    bool ScoreManager::operator>(const ScoreManager& other) const
    {
        return currentScore > other.currentScore;
    }

    bool ScoreManager::operator<(const ScoreManager& other) const
    {
        return currentScore < other.currentScore;
    }


    /// <summary>
    /// Добавляет очки к текущему счету
    /// </summary>
    /// <param name="points">Количество очков для добавления</param>
    String^ ScoreManager::AddScore(Int64 points)
    {
        currentScore += points;
        String^ score = FormatScore(currentScore);
        return score;
    }

    /// <summary>
    /// Сбрасывает счет до нуля
    /// </summary>
    Void ScoreManager::ResetScore()
    {
        currentScore = 0;
        FormatScore(currentScore);
    }

    /// <summary>
    /// Обновляет отображение счета на форме
    /// </summary>
    String^ ScoreManager::FormatScore(Int64 currentScore)
    {
        return "Score: " + currentScore.ToString();
    }

    /// <summary>
    /// Возвращает текущий счет
    /// </summary>
    Int64 ScoreManager::GetCurrentScore()
    {
        return currentScore;
    }

    /// <summary>
    /// Рассчитывает бонусные очки
    /// </summary>
    /// <param name="tilesRemoved">Количество удаленных плиток</param>
    Int64 ScoreManager::CalculateBonus(Int64 tilesRemoved)
    {
        if (!bonusStrategy) {
            // Устанавливаем стратегию по умолчанию
            bonusStrategy = std::make_shared<AdaptiveStrategy>();
        }
        return bonusStrategy->CalculateBonus(tilesRemoved, SCORE_PER_TILE);
    }

    // Управление стратегией бонусов
    void ScoreManager::SetBonusStrategy(std::shared_ptr<IBonusStrategy> strategy) {
        bonusStrategy = strategy; 
    }

    // Управление стратегией бонусов
    void ScoreManager::SetBonusStrategy(std::shared_ptr<IBonusStrategy> strategy, std::string str) {
        bonusStrategy = strategy;
    }

    std::shared_ptr<IBonusStrategy> ScoreManager::GetBonusStrategy() const {
        return bonusStrategy;
    }

    std::string ScoreManager::GetBonusStrategyDescription() const {
        return bonusStrategy ? bonusStrategy->GetDescription() : "No bonus strategy set";
    }

    /// <summary>
    /// Добавляет очки на основе количества удаленных плиток
    /// </summary>
    /// <param name="tilesRemoved">Количество удаленных плиток</param>
    String^ ScoreManager::AddScoreForTiles(Int64 tilesRemoved)
    {
        if (tilesRemoved > 0)
        {
            // Базовые очки за каждую удаленную плитку
            Int64 baseScore = tilesRemoved * SCORE_PER_TILE;

            // Бонус за комбо (чем больше плиток удалено за один ход - тем больше множитель)
            Int64 comboBonus = CalculateBonus(tilesRemoved);
            Int64 bonus = CalculateBonus(tilesRemoved);

            Int64 totalScore = baseScore + comboBonus;
            String^ score = AddScore(totalScore);

            // Сохраняем в историю
            SaveScoreToHistory(totalScore);

            return score;
        }
        return FormatScore(currentScore);
    }

    void ScoreManager::SetPlayerName(const std::string& name)
    {
        playerName = name; // Простое присваивание
    }

    std::string ScoreManager::GetPlayerName() const
    {
        return playerName;
    }

    std::string ScoreManager::GetFullScoreInfo() const
    {
        // Конкатенация строк
        std::string info = "Player: " + playerName + " | Score: " +
            std::to_string(currentScore);
        return info;
    }

    ScoreManager::operator std::string() const
    {
        return GetFullScoreInfo();
    }

    void ScoreManager::AddScoreToHistory(const std::string& playerName, Int64 score, Int64 level) {
        scoreHistory.AddItem(playerName, score, level);
        recentScores.push_back(score);

        // Ограничиваем размер истории
        if (recentScores.size() > 100) {
            recentScores.erase(recentScores.begin());
        }
    }

    void ScoreManager::SaveScoreToHistory(Int64 score) {
        AddScoreToHistory(playerName, score, 1);
    }

    std::vector<std::shared_ptr<PlayerScoreData>> ScoreManager::GetTopScores(Int64 count) {
        // Сортируем по убыванию
        scoreHistory.SortByValue(false);

        std::vector<std::shared_ptr<PlayerScoreData>> topScores;
        Int64 limit = (count < static_cast<Int64>(scoreHistory.Size())) ? count : static_cast<Int64>(scoreHistory.Size());

        for (Int64 i = 0; i < limit; ++i) {
            topScores.push_back(scoreHistory.GetItem(i));
        }

        return topScores;
    }

    double ScoreManager::GetAverageRecentScore() const {
        // Использование шаблонной функции
        if (recentScores.empty()) {
            return 0.0;
        }
        return CalculateAverage(recentScores);  // Теперь компилятор знает тип
    }

    Int64 ScoreManager::GetMaxRecentScore() const {
        if (recentScores.empty()) {
            return 0;
        }
        return FindMaxValue(recentScores);  // Теперь компилятор знает тип
    }

    void ScoreManager::DisplayScoreHistory() const {
        scoreHistory.DisplayAll();
    }

    void ScoreManager::CalculateAndDisplayStatistics() const {
        auto stats = scoreHistory.CalculateStatistics();

        std::cout << "=== Score Statistics ===" << std::endl;
        std::cout << "Total records: " << scoreHistory.Size() << std::endl;
        std::cout << "Average score: " << stats.average << std::endl;
        std::cout << "Minimum score: " << stats.min << std::endl;
        std::cout << "Maximum score: " << stats.max << std::endl;
        std::cout << "Total sum: " << stats.total << std::endl;
    }
}