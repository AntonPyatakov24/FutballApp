#include "MatchSchedule.hpp"
#include <iostream>
#include <algorithm>

// Конструктор инициализирует следующий ID матча
MatchSchedule::MatchSchedule() : nextMatchId(1) {}

// Добавляет новый матч в расписание
void MatchSchedule::addMatch(int homeTeamId, int awayTeamId, int matchDay) {
    matches.push_back(Match(nextMatchId++, homeTeamId, awayTeamId, matchDay));
}

// Удаляет матч из расписания по ID
bool MatchSchedule::removeMatch(int matchId) {
    auto it = std::remove_if(matches.begin(), matches.end(),
        [matchId](const Match& match) { return match.getId() == matchId; });

    if (it != matches.end()) {
        matches.erase(it, matches.end());
        return true;
    }
    return false;
}

// Находит матч по его ID
Match* MatchSchedule::findMatch(int matchId) {
    for (auto& match : matches) {
        if (match.getId() == matchId) {
            return &match;
        }
    }
    return nullptr;
}

// Возвращает все матчи указанной команды
std::vector<Match*> MatchSchedule::getMatchesByTeam(int teamId) {
    std::vector<Match*> teamMatches;
    for (auto& match : matches) {
        if (match.involvesTeam(teamId)) {
            teamMatches.push_back(&match);
        }
    }

    // Сортируем матчи по возрастанию номера тура
    std::sort(teamMatches.begin(), teamMatches.end(),
        [](Match* a, Match* b) {
            return a->getMatchDay() < b->getMatchDay();
        });

    return teamMatches;
}

// Возвращает матчи указанного дня
std::vector<Match*> MatchSchedule::getMatchesByDay(int matchDay) {
    std::vector<Match*> dayMatches;
    for (auto& match : matches) {
        if (match.getMatchDay() == matchDay) {
            dayMatches.push_back(&match);
        }
    }
    return dayMatches;
}

// Возвращает матчи по указанному фильтру
std::vector<Match*> MatchSchedule::getMatchesByFilter(int filter) {
    std::vector<Match*> filteredMatches;

    for (auto& match : matches) {
        if (filter == FILTER_ALL) {
            filteredMatches.push_back(&match);  // Все матчи
        }
        else if (filter == FILTER_UPCOMING && match.getStatus() == MATCH_SCHEDULED) {
            filteredMatches.push_back(&match);  // Только запланированные
        }
        else if (filter == FILTER_PLAYED && match.getStatus() == MATCH_PLAYED) {
            filteredMatches.push_back(&match);  // Только сыгранные
        }
    }

    // Сортируем по возрастанию номера тура
    std::sort(filteredMatches.begin(), filteredMatches.end(),
        [](Match* a, Match* b) {
            return a->getMatchDay() < b->getMatchDay();
        });

    return filteredMatches;
}

// Возвращает все матчи расписания
std::vector<Match*> MatchSchedule::getAllMatches() {
    std::vector<Match*> allMatches;
    for (auto& match : matches) {
        allMatches.push_back(&match);
    }

    // Сортируем по возрастанию номера тура
    std::sort(allMatches.begin(), allMatches.end(),
        [](Match* a, Match* b) {
            return a->getMatchDay() < b->getMatchDay();
        });

    return allMatches;
}

// Отображает полное расписание матчей
void MatchSchedule::displaySchedule() const {
    if (matches.empty()) {
        std::cout << "Расписание матчей пусто.\n";
        return;
    }

    std::cout << "\n=== ПОЛНОЕ РАСПИСАНИЕ МАТЧЕЙ ===\n";
    std::cout << "Всего матчей: " << matches.size() << "\n";
    std::cout << "----------------------------------------\n";

    // Выводим информацию о каждом матче
    for (const auto& match : matches) {
        std::string status;
        if (match.getStatus() == MATCH_SCHEDULED) {
            status = "Запланирован";
        }
        else if (match.getStatus() == MATCH_PLAYED) {
            status = "Сыгран";
        }
        else if (match.getStatus() == MATCH_POSTPONED) {
            status = "Перенесен";
        }

        std::cout << "Тур " << match.getMatchDay() << ": "
            << "Команда " << match.getHomeTeamId() << " vs "
            << "Команда " << match.getAwayTeamId();

        if (match.getStatus() == MATCH_PLAYED) {
            std::cout << " - " << match.getResult();
        }

        std::cout << " [" << status << "]\n";
    }
    std::cout << "----------------------------------------\n";
}

// Отображает расписание конкретной команды
void MatchSchedule::displayTeamSchedule(int teamId) const {
    // Собираем матчи команды
    std::vector<const Match*> teamMatches;
    for (const auto& match : matches) {
        if (match.involvesTeam(teamId)) {
            teamMatches.push_back(&match);
        }
    }

    if (teamMatches.empty()) {
        std::cout << "Для команды " << teamId << " матчей не найдено.\n";
        return;
    }

    std::cout << "\n=== РАСПИСАНИЕ КОМАНДЫ " << teamId << " ===\n";
    std::cout << "Всего матчей: " << teamMatches.size() << "\n";
    std::cout << "----------------------------------------\n";

    // Выводим информацию о каждом матче команды
    for (const auto& match : teamMatches) {
        std::string status;
        if (match->getStatus() == MATCH_SCHEDULED) {
            status = "Запланирован";
        }
        else if (match->getStatus() == MATCH_PLAYED) {
            status = "Сыгран";
        }
        else if (match->getStatus() == MATCH_POSTPONED) {
            status = "Перенесен";
        }

        // Определяем роль команды в матче (дома или в гостях)
        std::string role = (match->getHomeTeamId() == teamId) ? "Дома" : "В гостях";
        int opponentId = (match->getHomeTeamId() == teamId) ? match->getAwayTeamId() : match->getHomeTeamId();

        std::cout << "Тур " << match->getMatchDay() << ": "
            << role << " против команды " << opponentId;

        if (match->getStatus() == MATCH_PLAYED) {
            std::cout << " - " << match->getResult();
        }

        std::cout << " [" << status << "]\n";
    }
    std::cout << "----------------------------------------\n";
}