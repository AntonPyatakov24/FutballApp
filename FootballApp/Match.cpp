#include "Match.hpp"
#include <iostream>
#include <sstream>

// Конструктор матча инициализирует все поля
// Матч создается запланированным с нулевым счетом
Match::Match(int matchId, int homeId, int awayId, int day)
    : DataEntity(matchId, "Match " + std::to_string(matchId)), // Базовый конструктор
    homeTeamId(homeId), awayTeamId(awayId),                 // ID команд
    homeScore(0), awayScore(0),                             // Счет обнулен
    status(MATCH_SCHEDULED),                                // Статус - запланирован
    matchDay(day) {                                         // Номер тура
}

// Отмечает матч как сыгранный с указанным счетом
void Match::playMatch(int homeGoals, int awayGoals) {
    homeScore = homeGoals;         // Устанавливает голы домашней команды
    awayScore = awayGoals;         // Устанавливает голы гостевой команды
    status = MATCH_PLAYED;         // Меняет статус на "сыгран"
}

// Переносит матч на другое время
void Match::postponeMatch() {
    status = MATCH_POSTPONED;      // Меняет статус на "перенесен"
}

// Геттер для ID домашней команды
int Match::getHomeTeamId() const {
    return homeTeamId;
}

// Геттер для ID гостевой команды
int Match::getAwayTeamId() const {
    return awayTeamId;
}

// Геттер для голов домашней команды
int Match::getHomeScore() const {
    return homeScore;
}

// Геттер для голов гостевой команды
int Match::getAwayScore() const {
    return awayScore;
}

// Возвращает результат матча в виде строки
// Если матч не сыгран, возвращает " - : - "
std::string Match::getResult() const {
    if (status != MATCH_PLAYED) {
        return " - : - ";
    }
    std::stringstream ss;
    ss << homeScore << " : " << awayScore;
    return ss.str();
}

// Геттер для статуса матча
int Match::getStatus() const {
    return status;
}

// Геттер для номера тура
int Match::getMatchDay() const {
    return matchDay;
}

// Проверяет, участвует ли указанная команда в матче
bool Match::involvesTeam(int teamId) const {
    return homeTeamId == teamId || awayTeamId == teamId;
}

// Выводит основную информацию о матче
void Match::displayInfo() const {
    std::cout << "Матч " << id << ": Тур " << matchDay << " - "
        << getResult() << "\n";
}