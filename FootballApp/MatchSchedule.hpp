#ifndef MATCHSCHEDULE_HPP
#define MATCHSCHEDULE_HPP

#include "Match.hpp"
#include <vector>

// Фильтры для отбора матчей
const int FILTER_ALL = 0;       // Все матчи
const int FILTER_UPCOMING = 1;  // Предстоящие матчи
const int FILTER_PLAYED = 2;    // Сыгранные матчи

// Класс для управления расписанием матчей
// Содержит методы для работы с коллекцией матчей
class MatchSchedule {
private:
    std::vector<Match> matches;  // Вектор всех матчей
    int nextMatchId;             // Следующий доступный ID матча

public:
    MatchSchedule();

    // Добавляет матч в расписание
    void addMatch(int homeTeamId, int awayTeamId, int matchDay);

    // Удаляет матч из расписания
    bool removeMatch(int matchId);

    // Находит матч по ID
    Match* findMatch(int matchId);

    // Возвращает матчи указанной команды
    std::vector<Match*> getMatchesByTeam(int teamId);

    // Возвращает матчи указанного дня
    std::vector<Match*> getMatchesByDay(int matchDay);

    // Возвращает матчи по фильтру
    std::vector<Match*> getMatchesByFilter(int filter);

    // Возвращает все матчи
    std::vector<Match*> getAllMatches();

    // Отображает полное расписание
    void displaySchedule() const;

    // Отображает расписание конкретной команды
    void displayTeamSchedule(int teamId) const;
};

#endif