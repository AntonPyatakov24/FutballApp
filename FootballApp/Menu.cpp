#include "Menu.hpp"
#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <Windows.h>
#include <fstream>

using namespace std;

// Устанавливает русскую кодировку для консоли Windows
void setRussianLocale() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
}

// Конструктор меню инициализирует состояние и загружает данные
Menu::Menu() : currentLeague(nullptr), isRunning(true), currentState(MENU_MAIN) {
    setRussianLocale();
    initializeSampleData();
}

// Основной цикл работы меню
void Menu::run() {
    loadData();  // Загружаем сохраненные данные

    // Главный цикл программы
    while (isRunning) {
        switch (currentState) {
        case MENU_MAIN:
            showMainMenu();        // Показываем главное меню
            break;
        case MENU_LEAGUE:
            showLeagueMenu();      // Показываем меню лиги
            break;
        case MENU_TEAMS:
            showTeamsMenu();       // Показываем меню команд
            break;
        case MENU_TEAM_DETAILS:
            // Обрабатывается внутри showTeamsMenu
            break;
        }
    }

    saveData();  // Сохраняем данные перед выходом
}

// Отображает главное меню с выбором лиги
void Menu::showMainMenu() {
    cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
    cout << "1. Английская Премьер-Лига\n";
    cout << "2. Выйти\n";
    cout << "Выберите опцию: ";

    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
        if (!leagues.empty()) {
            currentLeague = &leagues[0];    // Устанавливаем текущую лигу
            currentState = MENU_LEAGUE;     // Переходим в меню лиги
            cout << "Переход в меню АПЛ...\n";
        }
        break;
    case 2:
        isRunning = false;                  // Завершаем программу
        cout << "До свидания!\n";
        break;
    default:
        cout << "Неверный выбор!\n";
        cin.clear();
        cin.ignore(10000, '\n');           // Очищаем буфер ввода
    }
}

// Отображает меню управления лигой
void Menu::showLeagueMenu() {
    int choice;
    do {
        cout << "\n=== " << currentLeague->getName() << " ===\n";
        cout << "1. Показать турнирную таблицу\n";
        cout << "2. Провести матч\n";
        cout << "3. Управление командами\n";
        cout << "4. Статистика лиги\n";
        cout << "5. Сохранить данные\n";
        cout << "6. Очистить базу данных\n";
        cout << "7. Назад в главное меню\n";
        cout << "Выберите действие: ";

        cin >> choice;

        switch (choice) {
        case 1:
            currentLeague->displayStandings();  // Показываем таблицу
            break;
        case 2:
            playMatch();                        // Проводим матч
            break;
        case 3:
            currentState = MENU_TEAMS;          // Переходим к управлению командами
            showTeamsMenu();
            break;
        case 4:
            currentLeague->displayLeagueStats(); // Показываем статистику
            break;
        case 5:
            saveData();                         // Сохраняем данные
            break;
        case 6:
            clearDatabase();                    // Очищаем базу
            break;
        case 7:
            currentState = MENU_MAIN;           // Возвращаемся в главное меню
            cout << "Возврат в главное меню...\n";
            break;
        default:
            cout << "Неверный выбор!\n";
        }
    } while (choice != 7 && currentState == MENU_LEAGUE);
}

// Отображает меню управления командами
void Menu::showTeamsMenu() {
    int choice;
    do {
        cout << "\n=== УПРАВЛЕНИЕ КОМАНДАМИ ===\n";
        cout << "1. Добавить команду\n";
        cout << "2. Список команд\n";
        cout << "3. Назад в меню лиги\n";
        cout << "Выберите действие: ";

        cin >> choice;

        switch (choice) {
        case 1:
            addTeam();      // Добавляем новую команду
            break;
        case 2:
        {
            auto teams = currentLeague->getAllTeams();
            if (teams.empty()) {
                cout << "Нет команд в лиге.\n";
                break;
            }

            // Показываем список всех команд
            cout << "\n=== СПИСОК КОМАНД ===\n";
            for (size_t i = 0; i < teams.size(); ++i) {
                cout << i + 1 << ". " << teams[i]->getName()
                    << " (" << teams[i]->getShortName() << ")\n";
            }

            // Предлагаем выбрать команду для детального просмотра
            cout << "\nВыберите команду (0 - назад): ";
            int teamChoice;
            cin >> teamChoice;

            if (teamChoice > 0 && teamChoice <= static_cast<int>(teams.size())) {
                currentState = MENU_TEAM_DETAILS;
                showTeamDetailsMenu(teams[teamChoice - 1]);
            }
            else if (teamChoice != 0) {
                cout << "Неверный выбор!\n";
            }
        }
        break;
        case 3:
            currentState = MENU_LEAGUE;  // Возвращаемся в меню лиги
            cout << "Возврат в меню лиги...\n";
            break;
        default:
            cout << "Неверный выбор!\n";
        }
    } while (choice != 3 && currentState == MENU_TEAMS);
}

// Отображает меню деталей конкретной команды
void Menu::showTeamDetailsMenu(Team* team) {
    int choice;
    do {
        cout << "\n=== " << team->getName() << " ===\n";
        cout << "1. Показать информацию\n";
        cout << "2. Показать статистику\n";
        cout << "3. Удалить\n";
        cout << "4. Назад к списку команд\n";
        cout << "Выберите действие: ";

        cin >> choice;

        switch (choice) {
        case 1:
            displayTeamInfo(team);            // Показываем информацию
            break;
        case 2:
            currentLeague->displayTeamStats(team->getId());  // Показываем статистику
            break;
        case 3:
            if (deleteTeam(team)) {           // Удаляем команду
                currentState = MENU_TEAMS;
                return;
            }
            break;
        case 4:
            currentState = MENU_TEAMS;        // Возвращаемся к списку команд
            break;
        default:
            cout << "Неверный выбор!\n";
        }
    } while (choice != 4 && currentState == MENU_TEAM_DETAILS);
}

// Добавляет новую команду в лигу
void Menu::addTeam() {
    // Проверяем максимальное количество команд
    if (currentLeague->getTeamsCount() >= 20) {
        cout << "Ошибка: невозможно добавить больше команд. Максимум 20 команд в лиге.\n";
        return;
    }

    string name, shortName, stadium;
    int capacity, founded;

    cin.ignore();  // Очищаем буфер ввода
    cout << "Название команды: ";
    getline(cin, name);
    cout << "Короткое название: ";
    getline(cin, shortName);
    cout << "Стадион: ";
    getline(cin, stadium);
    cout << "Вместимость стадиона: ";
    cin >> capacity;
    cout << "Год основания: ";
    cin >> founded;

    // Сохраняем сыгранные матчи перед генерацией нового расписания
    auto playedMatches = currentLeague->getPlayedMatches();

    // Добавляем команду и генерируем новое расписание
    currentLeague->addTeam(name, shortName, stadium, capacity, founded);
    cout << "Команда '" << name << "' добавлена успешно!\n";

    currentLeague->generateSchedule();

    // Восстанавливаем результаты сыгранных матчей
    for (auto match : playedMatches) {
        currentLeague->addMatchResult(
            match->getHomeTeamId(),
            match->getAwayTeamId(),
            match->getHomeScore(),
            match->getAwayScore(),
            match->getMatchDay()
        );
    }

    currentLeague->saveTeamsToFile();
    cout << "Расписание обновлено с сохранением результатов матчей.\n";
}

// Удаляет команду из лиги
bool Menu::deleteTeam(Team* team) {
    cout << "Вы уверены, что хотите удалить команду '" << team->getName() << "'? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        if (currentLeague->removeTeam(team->getId())) {
            cout << "Команда удалена успешно!\n";
            currentLeague->saveTeamsToFile();
            return true;
        }
        else {
            cout << "Ошибка при удалении команды.\n";
        }
    }
    else {
        cout << "Удаление отменено.\n";
    }
    return false;
}

// Очищает базу данных и создает базовые команды
void Menu::clearDatabase() {
    cout << "\n=== ОЧИСТКА БАЗЫ ДАННЫХ ===\n";
    cout << "ВНИМАНИЕ: Это действие удалит все команды, матчи и статистику!\n";
    cout << "После очистки будут созданы 4 базовые команды.\n";
    cout << "Вы уверены, что хотите продолжить? (y/n): ";

    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        // Удаляем файлы с данными
        remove("teams.txt");
        remove("matches.txt");

        cout << "Файлы данных удалены.\n";

        // Пересоздаем лигу с базовыми командами
        leagues.clear();

        League pl(1, "Английская Премьер-Лига", "Англия", "2024/25", LEAGUE_PREMIER);
        leagues.push_back(pl);
        currentLeague = &leagues[0];

        // Создаем базовые команды
        currentLeague->addTeam("Арсенал", "ARS", "Эмирейтс Стэдиум", 60704, 1886);
        currentLeague->addTeam("Манчестер Сити", "MCI", "Этихад Стэдиум", 53400, 1880);
        currentLeague->addTeam("Ливерпуль", "LIV", "Энфилд", 53394, 1892);
        currentLeague->addTeam("Челси", "CHE", "Стэмфорд Бридж", 40343, 1905);
        currentLeague->generateSchedule();

        cout << "База данных очищена. Созданы 4 базовые команды.\n";
    }
    else {
        cout << "Очистка отменена.\n";
    }
}

// Проводит матч между двумя командами
void Menu::playMatch() {
    auto teams = currentLeague->getAllTeams();

    // Проверяем, что в лиге есть хотя бы 2 команды
    if (teams.size() < 2) {
        cout << "Для проведения матча нужно как минимум 2 команды.\n";
        return;
    }

    cout << "\n=== ПРОВЕДЕНИЕ МАТЧА ===\n";
    cout << "Выберите домашнюю команду:\n";

    // Показываем список команд для выбора
    for (size_t i = 0; i < teams.size(); ++i) {
        cout << i + 1 << ". " << teams[i]->getName() << " (" << teams[i]->getShortName() << ")\n";
    }

    int homeChoice, awayChoice, homeGoals, awayGoals, matchDay;
    int maxMatchDay = currentLeague->getMaxMatchDay();

    cout << "Домашняя команда: ";
    cin >> homeChoice;
    cout << "Гостевая команда: ";
    cin >> awayChoice;

    // Проверяем корректность выбора команд
    if (homeChoice < 1 || homeChoice > static_cast<int>(teams.size()) ||
        awayChoice < 1 || awayChoice > static_cast<int>(teams.size()) ||
        homeChoice == awayChoice) {
        cout << "Неверный выбор команд!\n";
        return;
    }

    Team* homeTeam = teams[homeChoice - 1];
    Team* awayTeam = teams[awayChoice - 1];

    cout << "Номер тура (1-" << maxMatchDay << "): ";
    cin >> matchDay;

    // Проверяем корректность номера тура
    if (matchDay < 1 || matchDay > maxMatchDay) {
        cout << "Ошибка: номер тура должен быть от 1 до " << maxMatchDay << "\n";
        return;
    }

    // Запрашиваем счет матча
    cout << "Голы домашней команды (" << homeTeam->getShortName() << "): ";
    cin >> homeGoals;
    cout << "Голы гостевой команды (" << awayTeam->getShortName() << "): ";
    cin >> awayGoals;

    // Проверяем корректность счета
    if (homeGoals < 0 || awayGoals < 0) {
        cout << "Ошибка: количество голов не может быть отрицательным!\n";
        return;
    }

    // Добавляем результат матча
    currentLeague->addMatchResult(
        homeTeam->getId(),
        awayTeam->getId(),
        homeGoals,
        awayGoals,
        matchDay
    );

    cout << "Матч добавлен: " << homeTeam->getShortName()
        << " " << homeGoals << " : " << awayGoals << " "
        << awayTeam->getShortName() << "\n";

    currentLeague->saveMatchesToFile();

    // Показываем обновленную таблицу
    cout << "\nОбновленная таблица:\n";
    currentLeague->displayStandings();
}

// Отображает детальную информацию о команде
void Menu::displayTeamInfo(Team* team) {
    team->displayInfo();  // Основная информация о команде

    auto matches = currentLeague->getTeamMatches(team->getId());

    cout << "\n--- МАТЧИ КОМАНДЫ ---\n";

    // Показываем сыгранные матчи
    cout << "Сыгранные матчи:\n";
    bool hasPlayedMatches = false;
    for (const auto& match : matches) {
        if (match->getStatus() == MATCH_PLAYED) {
            Team* homeTeam = currentLeague->findTeamById(match->getHomeTeamId());
            Team* awayTeam = currentLeague->findTeamById(match->getAwayTeamId());
            if (homeTeam && awayTeam) {
                cout << "  Тур " << match->getMatchDay() << ": "
                    << homeTeam->getShortName() << " " << match->getHomeScore()
                    << ":" << match->getAwayScore() << " "
                    << awayTeam->getShortName() << "\n";
                hasPlayedMatches = true;
            }
        }
    }
    if (!hasPlayedMatches) {
        cout << "  Нет сыгранных матчей\n";
    }

    // Показываем предстоящие матчи
    cout << "Предстоящие матчи:\n";
    bool hasUpcomingMatches = false;
    for (const auto& match : matches) {
        if (match->getStatus() == MATCH_SCHEDULED) {
            Team* homeTeam = currentLeague->findTeamById(match->getHomeTeamId());
            Team* awayTeam = currentLeague->findTeamById(match->getAwayTeamId());
            if (homeTeam && awayTeam) {
                cout << "  Тур " << match->getMatchDay() << ": "
                    << homeTeam->getShortName() << " vs "
                    << awayTeam->getShortName() << "\n";
                hasUpcomingMatches = true;
            }
        }
    }
    if (!hasUpcomingMatches) {
        cout << "  Нет предстоящих матчей\n";
    }
}

// Сохраняет все данные в файлы
void Menu::saveData() {
    if (currentLeague) {
        currentLeague->saveTeamsToFile();
        currentLeague->saveMatchesToFile();
        cout << ">>> Все данные сохранены!\n";
    }
    else {
        cout << "Ошибка: нет активной лиги для сохранения.\n";
    }
}

// Загружает данные из файлов
void Menu::loadData() {
    if (currentLeague) {
        currentLeague->loadTeamsFromFile();
        currentLeague->loadMatchesFromFile();
        cout << ">>> Данные загружены!\n";
    }
}

// Инициализирует начальные данные при запуске
void Menu::initializeSampleData() {
    League pl(1, "Английская Премьер-Лига", "Англия", "2024/25", LEAGUE_PREMIER);

    // Проверяем существование файла с командами
    ifstream teamsFile("teams.txt");
    if (!teamsFile.is_open()) {
        // Создаем базовые команды если файла нет
        pl.addTeam("Арсенал", "ARS", "Эмирейтс Стэдиум", 60704, 1886);
        pl.addTeam("Манчестер Сити", "MCI", "Этихад Стэдиум", 53400, 1880);
        pl.addTeam("Ливерпуль", "LIV", "Энфилд", 53394, 1892);
        pl.addTeam("Челси", "CHE", "Стэмфорд Бридж", 40343, 1905);
        pl.generateSchedule();
        cout << "Созданы базовые команды АПЛ.\n";
    }
    else {
        teamsFile.close();
        // Загружаем команды из файла
        pl.loadTeamsFromFile();
        pl.loadMatchesFromFile();
    }

    leagues.push_back(pl);
    currentLeague = &leagues[0];
}