#ifndef MENU_HPP
#define MENU_HPP

#include "League.hpp"
#include <vector>
#include <fstream>

// Состояния меню для навигации по интерфейсу
const int MENU_MAIN = 0;        // Главное меню
const int MENU_LEAGUE = 1;      // Меню лиги
const int MENU_TEAMS = 2;       // Меню управления командами
const int MENU_TEAM_DETAILS = 3;// Меню деталей команды

// Класс управления пользовательским интерфейсом
// Обеспечивает навигацию по всем функциям системы
class Menu {
private:
    std::vector<League> leagues;    // Вектор доступных лиг
    League* currentLeague;          // Указатель на текущую активную лигу
    bool isRunning;                 // Флаг работы программы
    int currentState;               // Текущее состояние меню

public:
    // Конструктор инициализирует меню и загружает данные
    Menu();

    // Основной метод запуска меню
    void run();

private:
    // Методы отображения различных экранов меню
    void showMainMenu();                    // Главное меню
    void showLeagueMenu();                  // Меню лиги
    void showTeamsMenu();                   // Меню команд
    void showTeamDetailsMenu(Team* team);   // Детали команды

    // Методы управления командами
    void addTeam();                         // Добавление новой команды
    void editTeam(Team* team);              // Редактирование команды
    bool deleteTeam(Team* team);            // Удаление команды

    // Методы работы с матчами
    void playMatch();                       // Проведение матча

    // Методы отображения информации
    void displayTeamInfo(Team* team);       // Информация о команде

    // Методы управления данными
    void clearDatabase();                   // Очистка базы данных
    void saveData();                        // Сохранение данных
    void loadData();                        // Загрузка данных

    // Вспомогательные методы
    void initializeSampleData();            // Инициализация тестовых данных
};

#endif