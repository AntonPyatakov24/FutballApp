#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include "Team.hpp"
#include "Match.hpp"
#include <vector>

// Класс для сбора и анализа статистики лиги
// Содержит методы для получения различных статистических данных
class Statistics {
private:
    std::vector<Team> teams;    // Вектор команд для анализа
    std::vector<Match> matches; // Вектор матчей для анализа

public:
    Statistics();

    // Добавляет команду в статистику
    void addTeam(const Team& team);

    // Добавляет матч в статистику
    void addMatch(const Match& match);

    // Находит команду с лучшей атакой
    Team* getTopScorer();

    // Находит команду с лучшей защитой
    Team* getBestDefense();

    // Находит матч с наибольшим количеством голов
    Match* getHighestScoringMatch();

    // Вычисляет среднее количество голов за матч
    double getAverageGoalsPerGame() const;

    // Отображает общую статистику лиги
    void displayLeagueStats() const;

    // Отображает статистику конкретной команды
    void displayTeamStats(int teamId) const;
};

#endif