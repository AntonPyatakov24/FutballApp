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

// Статический метод для создания динамической копии матча с новым результатом
Match* Match::createPlayedCopy(const Match* original, int newHomeScore, int newAwayScore) {
    if (original == nullptr) {
        return nullptr;
    }

    // ДИНАМИЧЕСКОЕ СОЗДАНИЕ объекта Match через new
    Match* newMatch = new Match(
        original->getId(),
        original->getHomeTeamId(),
        original->getAwayTeamId(),
        original->getMatchDay()
    );

    // Устанавливаем новый результат
    newMatch->playMatch(newHomeScore, newAwayScore);

    return newMatch;  // Возвращаем указатель на динамический объект
}

// Статический метод для создания нового матча
Match* Match::createNewMatch(int matchId, int homeId, int awayId, int day) {
    // ДИНАМИЧЕСКОЕ СОЗДАНИЕ через new
    Match* newMatch = new Match(matchId, homeId, awayId, day);
    return newMatch;
}

// Статический метод для удаления матча
void Match::deleteMatch(Match* match) {
    if (match != nullptr) {
        delete match;  // Явное удаление динамического объекта
        // После delete указатель становится "висящим" - хорошая практика обнулять
    }
}