#ifndef MATCH_HPP
#define MATCH_HPP

#include "DataEntity.hpp"
#include <string>

// Исправляем enum class на обычные константы для совместимости
const int MATCH_SCHEDULED = 0;
const int MATCH_PLAYED = 1;
const int MATCH_POSTPONED = 2;

class Match : public DataEntity {
private:
    int homeTeamId;
    int awayTeamId;
    int homeScore;
    int awayScore;
    int status;  // Используем int вместо MatchStatus
    int matchDay;

public:
    // Конструктор
    Match(int matchId, int homeId, int awayId, int day);

    // Методы управления матчем
    void playMatch(int homeGoals, int awayGoals);
    void postponeMatch();

    // Геттеры
    int getHomeTeamId() const;
    int getAwayTeamId() const;
    int getHomeScore() const;
    int getAwayScore() const;
    std::string getResult() const;
    int getStatus() const;
    int getMatchDay() const;
    bool involvesTeam(int teamId) const;

    // Переопределение виртуальной функции
    void displayInfo() const override;

    // Статические методы для работы с динамической памятью
    static Match* createPlayedCopy(const Match* original, int newHomeScore, int newAwayScore);
    static Match* createNewMatch(int matchId, int homeId, int awayId, int day);
    static void deleteMatch(Match* match);
};

#endif