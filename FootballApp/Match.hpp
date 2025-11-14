#ifndef MATCH_HPP
#define MATCH_HPP

#include "DataEntity.hpp"
#include <string>

// Статусы матча для отслеживания состояния игры
const int MATCH_SCHEDULED = 0;   // Матч запланирован
const int MATCH_PLAYED = 1;      // Матч сыгран
const int MATCH_POSTPONED = 2;   // Матч перенесен

// Класс футбольного матча, наследуется от DataEntity
// Представляет один матч между двумя командами
class Match : public DataEntity {
private:
    int homeTeamId;        // ID домашней команды
    int awayTeamId;        // ID гостевой команды
    int homeScore;         // Голы домашней команды
    int awayScore;         // Голы гостевой команды
    int status;            // Статус матча
    int matchDay;          // Номер тура

public:
    // Конструктор создает матч с указанными параметрами
    Match(int matchId, int homeId, int awayId, int day);

    // Отмечает матч как сыгранный с указанным счетом
    void playMatch(int homeGoals, int awayGoals);

    // Откладывает матч (меняет статус на перенесенный)
    void postponeMatch();

    // Геттеры для доступа к информации о матче
    int getHomeTeamId() const;     // ID домашней команды
    int getAwayTeamId() const;     // ID гостевой команды
    int getHomeScore() const;      // Голы домашней команды
    int getAwayScore() const;      // Голы гостевой команды
    std::string getResult() const; // Результат в формате "X : Y"
    int getStatus() const;         // Текущий статус матча
    int getMatchDay() const;       // Номер тура

    // Проверяет, участвует ли команда в этом матче
    bool involvesTeam(int teamId) const;

    // Выводит информацию о матче
    void displayInfo() const override;
};

#endif