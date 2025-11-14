#include "StandingCalculator.hpp"
#include <iostream>

// Конструктор по умолчанию
StandingCalculator::StandingCalculator() {}

// Устанавливает команды для расчета таблицы
void StandingCalculator::setTeams(const StandingsType& teamList) {
    teams = teamList;
}

// Рассчитывает турнирную таблицу по указанному критерию
StandingCalculator::StandingsType StandingCalculator::calculateStandings(int criteria) {
    StandingsType sortedTeams = teams;  // Копируем команды для сортировки

    switch (criteria) {
    case 0: // Сортировка по очкам (основной критерий)
        std::sort(sortedTeams.begin(), sortedTeams.end(),
            [](Team* a, Team* b) {
                if (a->getPoints() != b->getPoints()) {
                    return a->getPoints() > b->getPoints();  // По убыванию очков
                }
                if (a->getGoalDifference() != b->getGoalDifference()) {
                    return a->getGoalDifference() > b->getGoalDifference(); // По разнице голов
                }
                return a->getGoalsFor() > b->getGoalsFor(); // По забитым голам
            });
        break;

    case 1: // Сортировка по разнице голов
        std::sort(sortedTeams.begin(), sortedTeams.end(),
            [](Team* a, Team* b) {
                if (a->getGoalDifference() != b->getGoalDifference()) {
                    return a->getGoalDifference() > b->getGoalDifference();
                }
                if (a->getPoints() != b->getPoints()) {
                    return a->getPoints() > b->getPoints();
                }
                return a->getGoalsFor() > b->getGoalsFor();
            });
        break;

    case 2: // Сортировка по забитым голам
        std::sort(sortedTeams.begin(), sortedTeams.end(),
            [](Team* a, Team* b) {
                if (a->getGoalsFor() != b->getGoalsFor()) {
                    return a->getGoalsFor() > b->getGoalsFor();
                }
                if (a->getPoints() != b->getPoints()) {
                    return a->getPoints() > b->getPoints();
                }
                return a->getGoalDifference() > b->getGoalDifference();
            });
        break;
    }

    return sortedTeams;
}

// Отображает турнирную таблицу в форматированном виде
void StandingCalculator::displayStandings(const StandingsType& standings) const {
    if (standings.empty()) {
        std::cout << "Таблица пуста.\n";
        return;
    }

    std::cout << "\n=== ТУРНИРНАЯ ТАБЛИЦА ===\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "Поз. Команда            И   В   Н   П   ГЗ   ГП   РМ   О\n";
    std::cout << "------------------------------------------------------------\n";

    // ИСПРАВЛЕНИЕ: отображаем только существующие команды
    for (size_t i = 0; i < standings.size(); ++i) {
        Team* team = standings[i];
        printf("%-4zu %-17s %-3d %-3d %-3d %-3d %-4d %-4d %-4d %-3d\n",
            i + 1, team->getName().c_str(), team->getGamesPlayed(),
            team->getWins(), team->getDraws(), team->getLosses(),
            team->getGoalsFor(), team->getGoalsAgainst(),
            team->getGoalDifference(), team->getPoints());
    }
    std::cout << "------------------------------------------------------------\n";
}