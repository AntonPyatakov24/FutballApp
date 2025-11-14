#include "Team.hpp"
#include <iostream>

// Конструктор команды инициализирует все поля
// Использует список инициализации для эффективности
Team::Team(int teamId, const std::string& teamName, const std::string& teamShortName,
    const std::string& teamStadium, int capacity, int founded)
    : DataEntity(teamId, teamName),           // Вызов конструктора базового класса
    shortName(teamShortName),               // Инициализация короткого названия
    stadium(teamStadium),                   // Инициализация названия стадиона
    stadiumCapacity(capacity),              // Инициализация вместимости
    foundedYear(founded),                   // Инициализация года основания
    gamesPlayed(0), wins(0), draws(0), losses(0), // Статистика начинается с нуля
    goalsFor(0), goalsAgainst(0),           // Голы обнуляются
    currentForm(1) {                        // Форма по умолчанию - средняя
}

// Обновляет статистику команды после матча
void Team::updateStats(int gf, int ga, bool isWin, bool isDraw) {
    gamesPlayed++;                    // Увеличиваем счетчик сыгранных матчей
    goalsFor += gf;                   // Добавляем забитые голы
    goalsAgainst += ga;               // Добавляем пропущенные голы

    if (isWin) {
        wins++;                       // Увеличиваем счетчик побед
        currentForm = 0;              // Устанавливаем хорошую форму
    }
    else if (isDraw) {
        draws++;                      // Увеличиваем счетчик ничьих
        currentForm = 1;              // Устанавливаем среднюю форму
    }
    else {
        losses++;                     // Увеличиваем счетчик поражений
        currentForm = 2;              // Устанавливаем плохую форму
    }
}

// Сбрасывает всю статистику команды
// Используется при пересчете статистики из актуальных матчей
void Team::resetStats() {
    gamesPlayed = 0;
    wins = 0;
    draws = 0;
    losses = 0;
    goalsFor = 0;
    goalsAgainst = 0;
    currentForm = 1;  // Возвращаем форму к среднему значению
}

// Геттер для короткого названия команды
std::string Team::getShortName() const {
    return shortName;
}

// Геттер для названия стадиона
std::string Team::getStadium() const {
    return stadium;
}

// Геттер для вместимости стадиона
int Team::getStadiumCapacity() const {
    return stadiumCapacity;
}

// Геттер для года основания
int Team::getFoundedYear() const {
    return foundedYear;
}

// Вычисляет количество очков команды
// Победа = 3 очка, ничья = 1 очко
int Team::getPoints() const {
    return wins * 3 + draws;
}

// Вычисляет разницу забитых и пропущенных голов
int Team::getGoalDifference() const {
    return goalsFor - goalsAgainst;
}

// Геттер для количества сыгранных матчей
int Team::getGamesPlayed() const {
    return gamesPlayed;
}

// Геттер для количества побед
int Team::getWins() const {
    return wins;
}

// Геттер для количества ничьих
int Team::getDraws() const {
    return draws;
}

// Геттер для количества поражений
int Team::getLosses() const {
    return losses;
}

// Геттер для количества забитых голов
int Team::getGoalsFor() const {
    return goalsFor;
}

// Геттер для количества пропущенных голов
int Team::getGoalsAgainst() const {
    return goalsAgainst;
}

// Геттер для текущей формы команды
int Team::getCurrentForm() const {
    return currentForm;
}

// Выводит полную информацию о команде в консоль
// Реализация чисто виртуального метода из DataEntity
void Team::displayInfo() const {
    std::cout << "\n=== " << name << " (" << shortName << ") ===\n";
    std::cout << "Стадион: " << stadium << " (вместимость: " << stadiumCapacity << ")\n";
    std::cout << "Год основания: " << foundedYear << "\n";
    std::cout << "Статистика: " << gamesPlayed << " игр, " << wins << " побед, "
        << draws << " ничьих, " << losses << " поражений\n";
    std::cout << "Голы: " << goalsFor << "-" << goalsAgainst
        << " (разница: " << getGoalDifference() << ")\n";
    std::cout << "Очки: " << getPoints() << "\n";
}