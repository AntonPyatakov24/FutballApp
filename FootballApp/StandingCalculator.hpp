#ifndef STANDINGSCALCULATOR_HPP
#define STANDINGSCALCULATOR_HPP

#include "Team.hpp"
#include <vector>
#include <algorithm>

// Класс для расчета и отображения турнирной таблицы
// Альтернативная реализация для демонстрации разных подходов
class StandingCalculator {
public:
    using StandingsType = std::vector<Team*>;  // Псевдоним для вектора команд

    // Критерии сортировки таблицы
    const int SORT_POINTS = 0;        // Сортировка по очкам
    const int SORT_GOAL_DIFFERENCE = 1; // Сортировка по разнице голов
    const int SORT_GOALS_FOR = 2;     // Сортировка по забитым голам

private:
    StandingsType teams;  // Вектор команд для расчета

public:
    StandingCalculator();

    // Устанавливает команды для расчета
    void setTeams(const StandingsType& teamList);

    // Рассчитывает таблицу по указанному критерию
    StandingsType calculateStandings(int criteria = 0);

    // Отображает таблицу в консоли
    void displayStandings(const StandingsType& standings) const;
};

#endif