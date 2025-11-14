#include "Statistics.hpp"
#include <iostream>
#include <algorithm>

// Конструктор по умолчанию
Statistics::Statistics() {}

// Добавляет команду в коллекцию для статистического анализа
void Statistics::addTeam(const Team& team) {
    teams.push_back(team);
}

// Добавляет матч в коллекцию для статистического анализа
void Statistics::addMatch(const Match& match) {
    matches.push_back(match);
}

// Находит команду с наибольшим количеством забитых голов
Team* Statistics::getTopScorer() {
    if (teams.empty()) return nullptr;

    Team* topScorer = &teams[0];
    for (auto& team : teams) {
        if (team.getGoalsFor() > topScorer->getGoalsFor()) {
            topScorer = &team;
        }
    }
    return topScorer;
}

// Находит команду с наименьшим количеством пропущенных голов
Team* Statistics::getBestDefense() {
    if (teams.empty()) return nullptr;

    Team* bestDefense = &teams[0];
    for (auto& team : teams) {
        if (team.getGoalsAgainst() < bestDefense->getGoalsAgainst()) {
            bestDefense = &team;
        }
    }
    return bestDefense;
}

// Находит матч с наибольшим общим количеством голов
Match* Statistics::getHighestScoringMatch() {
    if (matches.empty()) return nullptr;

    Match* highestScoring = &matches[0];
    for (auto& match : matches) {
        if (match.getStatus() == MATCH_PLAYED) {
            int currentTotal = match.getHomeScore() + match.getAwayScore();
            int highestTotal = highestScoring->getHomeScore() + highestScoring->getAwayScore();
            if (currentTotal > highestTotal) {
                highestScoring = &match;
            }
        }
    }
    return highestScoring;
}

// Вычисляет среднее количество голов за матч
double Statistics::getAverageGoalsPerGame() const {
    if (matches.empty()) return 0.0;

    int totalGoals = 0;
    int playedMatches = 0;

    for (const auto& match : matches) {
        if (match.getStatus() == MATCH_PLAYED) {
            totalGoals += match.getHomeScore() + match.getAwayScore();
            playedMatches++;
        }
    }

    return playedMatches > 0 ? static_cast<double>(totalGoals) / playedMatches : 0.0;
}

// Отображает общую статистику лиги
void Statistics::displayLeagueStats() const {
    std::cout << "\n=== СТАТИСТИКА ЛИГИ ===\n";

    if (teams.empty()) {
        std::cout << "Нет данных о командах.\n";
        return;
    }

    // Основная статистика
    std::cout << "Общее количество команд: " << teams.size() << "\n";
    std::cout << "Среднее количество голов за матч: " << getAverageGoalsPerGame() << "\n";

    // Статистика по матчам
    if (!matches.empty()) {
        int playedMatches = 0;
        int postponedMatches = 0;

        for (const auto& match : matches) {
            if (match.getStatus() == MATCH_PLAYED) playedMatches++;
            else if (match.getStatus() == MATCH_POSTPONED) postponedMatches++;
        }

        std::cout << "\n--- СТАТИСТИКА МАТЧЕЙ ---\n";
        std::cout << "Всего матчей: " << matches.size() << "\n";
        std::cout << "Сыграно: " << playedMatches << "\n";
        std::cout << "Запланировано: " << matches.size() - playedMatches - postponedMatches << "\n";
        std::cout << "Перенесено: " << postponedMatches << "\n";
    }
}

// Отображает детальную статистику команды
void Statistics::displayTeamStats(int teamId) const {
    auto teamIt = std::find_if(teams.begin(), teams.end(),
        [teamId](const Team& team) { return team.getId() == teamId; });

    if (teamIt == teams.end()) {
        std::cout << "Команда с ID " << teamId << " не найдена.\n";
        return;
    }

    const Team& team = *teamIt;

    std::cout << "\n=== СТАТИСТИКА КОМАНДЫ " << team.getName() << " ===\n";
    std::cout << "Сыграно матчей: " << team.getGamesPlayed() << "\n";
    std::cout << "Победы/Ничьи/Поражения: " << team.getWins() << "/"
        << team.getDraws() << "/" << team.getLosses() << "\n";
    std::cout << "Забито/Пропущено: " << team.getGoalsFor() << "/"
        << team.getGoalsAgainst() << "\n";
    std::cout << "Разница мячей: " << team.getGoalDifference() << "\n";
    std::cout << "Очки: " << team.getPoints() << "\n";

    // Вычисляем процент побед
    if (team.getGamesPlayed() > 0) {
        double winPercentage = (static_cast<double>(team.getWins()) / team.getGamesPlayed()) * 100;
        std::cout << "Процент побед: " << winPercentage << "%\n";
    }
}