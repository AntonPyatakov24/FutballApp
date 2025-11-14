#ifndef TEAM_HPP
#define TEAM_HPP

#include "DataEntity.hpp"
#include <string>

// Класс футбольной команды, наследуется от DataEntity
// Содержит информацию о команде и ее статистику
class Team : public DataEntity {
private:
    // Основная информация о команде
    std::string shortName;      // Короткое название (аббревиатура)
    std::string stadium;        // Название домашнего стадиона
    int stadiumCapacity;        // Вместимость стадиона
    int foundedYear;            // Год основания команды

    // Статистические показатели
    int gamesPlayed;            // Количество сыгранных матчей
    int wins;                   // Количество побед
    int draws;                  // Количество ничьих
    int losses;                 // Количество поражений
    int goalsFor;               // Количество забитых голов
    int goalsAgainst;           // Количество пропущенных голов
    int currentForm;            // Текущая форма: 0-хорошая, 1-средняя, 2-плохая

public:
    // Конструктор инициализирует все поля команды
    Team(int teamId, const std::string& teamName, const std::string& teamShortName,
        const std::string& teamStadium, int capacity, int founded);

    // Обновляет статистику команды после сыгранного матча
    // gf - забитые голы, ga - пропущенные голы
    // isWin - true если победа, isDraw - true если ничья
    void updateStats(int gf, int ga, bool isWin, bool isDraw);

    // Сбрасывает всю статистику в ноль
    // Используется при пересчете статистики из матчей
    void resetStats();

    // Геттеры для основной информации
    std::string getShortName() const;     // Возвращает короткое название
    std::string getStadium() const;       // Возвращает название стадиона
    int getStadiumCapacity() const;       // Возвращает вместимость
    int getFoundedYear() const;           // Возвращает год основания

    // Вычисляемые геттеры (рассчитываются на основе статистики)
    int getPoints() const;                // Очки = победы*3 + ничьи
    int getGoalDifference() const;        // Разница голов = забитые - пропущенные

    // Геттеры статистики
    int getGamesPlayed() const;           // Количество сыгранных матчей
    int getWins() const;                  // Количество побед
    int getDraws() const;                 // Количество ничьих
    int getLosses() const;                // Количество поражений
    int getGoalsFor() const;              // Количество забитых голов
    int getGoalsAgainst() const;          // Количество пропущенных голов
    int getCurrentForm() const;           // Текущая форма команды

    // Переопределение виртуального метода из DataEntity
    // Выводит полную информацию о команде
    void displayInfo() const override;
};

#endif