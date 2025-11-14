#include "League.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

// Конструктор
League::League(int leagueId, const std::string& leagueName, const std::string& leagueCountry,
    const std::string& season, int leagueLevel)
    : DataEntity(leagueId, leagueName),
    country(leagueCountry),
    currentSeason(season),
    level(leagueLevel),
    nextTeamId(1),
    nextMatchId(1) {
}

// Добавление команды
void League::addTeam(const std::string& teamName, const std::string& shortName,
    const std::string& stadium, int capacity, int founded) {
    Team newTeam(nextTeamId++, teamName, shortName, stadium, capacity, founded);
    teams.push_back(newTeam);
}

// Удаление команды
bool League::removeTeam(int teamId) {
    auto it = std::remove_if(teams.begin(), teams.end(),
        [teamId](const Team& team) { return team.getId() == teamId; });

    if (it != teams.end()) {
        teams.erase(it, teams.end());

        // Удаляем матчи с участием команды
        auto matchIt = std::remove_if(matches.begin(), matches.end(),
            [teamId](const Match& match) { return match.involvesTeam(teamId); });
        matches.erase(matchIt, matches.end());

        updateTeamStats();
        return true;
    }
    return false;
}

// Поиск команды по ID
Team* League::findTeamById(int id) {
    for (auto& team : teams) {
        if (team.getId() == id) {
            return &team;
        }
    }
    return nullptr;
}

const Team* League::findTeamById(int id) const {
    for (const auto& team : teams) {
        if (team.getId() == id) {
            return &team;
        }
    }
    return nullptr;
}

// Получение всех команд
std::vector<Team*> League::getAllTeams() {
    std::vector<Team*> teamPtrs;
    for (auto& team : teams) {
        teamPtrs.push_back(&team);
    }
    return teamPtrs;
}

std::vector<const Team*> League::getAllTeams() const {
    std::vector<const Team*> teamPtrs;
    for (const auto& team : teams) {
        teamPtrs.push_back(&team);
    }
    return teamPtrs;
}

// Генерация расписания
void League::generateSchedule() {
    matches.clear();
    int totalTeams = static_cast<int>(teams.size());
    if (totalTeams < 2) return;

    int totalRounds = (totalTeams - 1) * 2;
    int matchId = nextMatchId;

    std::vector<int> teamIds;
    for (const auto& team : teams) {
        teamIds.push_back(team.getId());
    }

    // Первый круг
    for (int round = 1; round <= totalTeams - 1; round++) {
        for (int i = 0; i < totalTeams / 2; i++) {
            int homeIdx = i;
            int awayIdx = totalTeams - 1 - i;

            if (round % 2 == 0) {
                std::swap(homeIdx, awayIdx);
            }

            matches.push_back(Match(matchId++, teamIds[homeIdx], teamIds[awayIdx], round));
        }

        // Циклический сдвиг
        int firstTeamId = teamIds[0];
        for (int i = 0; i < totalTeams - 1; i++) {
            teamIds[i] = teamIds[i + 1];
        }
        teamIds[totalTeams - 1] = firstTeamId;
    }

    // Второй круг
    for (int round = totalTeams; round <= totalRounds; round++) {
        int firstRoundMatch = round - totalTeams + 1;

        for (const auto& match : matches) {
            if (match.getMatchDay() == firstRoundMatch) {
                matches.push_back(Match(matchId++, match.getAwayTeamId(), match.getHomeTeamId(), round));
            }
        }
    }

    nextMatchId = matchId;
}

// Добавление результата матча
void League::addMatchResult(int homeTeamId, int awayTeamId, int homeGoals, int awayGoals, int matchDay) {
    int maxMatchDay = getMaxMatchDay();
    if (matchDay < 1 || matchDay > maxMatchDay) {
        std::cout << "Ошибка: номер тура должен быть от 1 до " << maxMatchDay << "\n";
        return;
    }

    bool matchFound = false;

    for (auto& match : matches) {
        if (match.getHomeTeamId() == homeTeamId &&
            match.getAwayTeamId() == awayTeamId &&
            match.getMatchDay() == matchDay) {

            match.playMatch(homeGoals, awayGoals);
            matchFound = true;

            Team* homeTeam = findTeamById(homeTeamId);
            Team* awayTeam = findTeamById(awayTeamId);

            if (homeTeam && awayTeam) {
                bool homeWin = homeGoals > awayGoals;
                bool draw = homeGoals == awayGoals;

                homeTeam->updateStats(homeGoals, awayGoals, homeWin, draw);
                awayTeam->updateStats(awayGoals, homeGoals, !homeWin && !draw, draw);
            }
            break;
        }
    }

    if (!matchFound) {
        Match newMatch(nextMatchId++, homeTeamId, awayTeamId, matchDay);
        newMatch.playMatch(homeGoals, awayGoals);
        matches.push_back(newMatch);

        Team* homeTeam = findTeamById(homeTeamId);
        Team* awayTeam = findTeamById(awayTeamId);

        if (homeTeam && awayTeam) {
            bool homeWin = homeGoals > awayGoals;
            bool draw = homeGoals == awayGoals;

            homeTeam->updateStats(homeGoals, awayGoals, homeWin, draw);
            awayTeam->updateStats(awayGoals, homeGoals, !homeWin && !draw, draw);
        }
    }

    std::cout << "Матч добавлен и статистика обновлена!\n";
}

// Получение матчей команды
std::vector<Match*> League::getTeamMatches(int teamId) {
    std::vector<Match*> teamMatches;
    for (auto& match : matches) {
        if (match.involvesTeam(teamId)) {
            teamMatches.push_back(&match);
        }
    }
    return teamMatches;
}

// Получение сыгранных матчей
std::vector<Match*> League::getPlayedMatches() {
    std::vector<Match*> played;
    for (auto& match : matches) {
        if (match.getStatus() == MATCH_PLAYED) {
            played.push_back(&match);
        }
    }
    return played;
}

// Турнирная таблица
std::vector<Team*> League::getStandings() {
    updateTeamStats();
    std::vector<Team*> standings = getAllTeams();

    std::sort(standings.begin(), standings.end(),
        [](Team* a, Team* b) {
            if (a->getPoints() != b->getPoints()) {
                return a->getPoints() > b->getPoints();
            }
            if (a->getGoalDifference() != b->getGoalDifference()) {
                return a->getGoalDifference() > b->getGoalDifference();
            }
            return a->getGoalsFor() > b->getGoalsFor();
        });
    return standings;
}

std::vector<const Team*> League::getStandings() const {
    // Создаем неконстантную копию для обновления статистики
    League* nonConstThis = const_cast<League*>(this);
    nonConstThis->updateTeamStats();

    std::vector<const Team*> constStandings;
    auto allTeams = getAllTeams();

    // Создаем временный вектор для сортировки
    std::vector<Team*> tempStandings;
    for (auto team : allTeams) {
        tempStandings.push_back(const_cast<Team*>(team));
    }

    std::sort(tempStandings.begin(), tempStandings.end(),
        [](Team* a, Team* b) {
            if (a->getPoints() != b->getPoints()) {
                return a->getPoints() > b->getPoints();
            }
            if (a->getGoalDifference() != b->getGoalDifference()) {
                return a->getGoalDifference() > b->getGoalDifference();
            }
            return a->getGoalsFor() > b->getGoalsFor();
        });

    // Конвертируем обратно в const
    for (auto team : tempStandings) {
        constStandings.push_back(team);
    }

    return constStandings;
}

// Отображение таблицы
void League::displayStandings() const {
    auto standings = getStandings();

    std::cout << "\n=== ТУРНИРНАЯ ТАБЛИЦА " << name << " ===\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "№  Команда            И   В   Н   П   ГЗ   ГП   РМ   О\n";
    std::cout << "------------------------------------------------------------\n";

    int position = 1;
    for (const auto& team : standings) {
        printf("%-2d %-17s %-3d %-3d %-3d %-3d %-4d %-4d %-4d %-3d\n",
            position++, team->getShortName().c_str(), team->getGamesPlayed(),
            team->getWins(), team->getDraws(), team->getLosses(), team->getGoalsFor(),
            team->getGoalsAgainst(), team->getGoalDifference(), team->getPoints());
    }
    std::cout << "------------------------------------------------------------\n";
}

// Статистика лиги
void League::displayLeagueStats() const {
    std::cout << "\n=== СТАТИСТИКА ЛИГИ " << name << " ===\n";
    std::cout << "============================================\n";

    int totalMatches = getTotalMatchesPlayed();
    int totalGoals = getTotalGoals();
    double avgGoals = getAverageGoalsPerGame();

    std::cout << "Общее количество матчей: " << totalMatches << "\n";
    std::cout << "Общее количество голов: " << totalGoals << "\n";
    std::cout << "Среднее количество голов за матч: " << std::fixed << std::setprecision(2) << avgGoals << "\n";

    const Team* topScorer = getTopScorer();
    const Team* bestDefense = getBestDefense();

    if (topScorer) {
        std::cout << "Лучшая атака: " << topScorer->getName()
            << " (" << topScorer->getGoalsFor() << " голов)\n";
    }

    if (bestDefense) {
        std::cout << "Лучшая защита: " << bestDefense->getName()
            << " (" << bestDefense->getGoalsAgainst() << " пропущенных)\n";
    }

    std::cout << "Количество команд: " << teams.size() << "\n";
    std::cout << "Всего туров в сезоне: " << getMaxMatchDay() << "\n";
    std::cout << "============================================\n";
}

// Статистика команды
void League::displayTeamStats(int teamId) const {
    const Team* team = findTeamById(teamId);
    if (!team) {
        std::cout << "Команда не найдена!\n";
        return;
    }

    std::cout << "\n=== СТАТИСТИКА КОМАНДЫ " << team->getName() << " ===\n";
    std::cout << "----------------------------------------\n";

    team->displayInfo();

    std::cout << "\nТУРНИРНАЯ СТАТИСТИКА:\n";
    std::cout << "Сыграно матчей: " << team->getGamesPlayed() << "\n";
    std::cout << "Победы/Ничьи/Поражения: " << team->getWins() << "/"
        << team->getDraws() << "/" << team->getLosses() << "\n";
    std::cout << "Забито голов: " << team->getGoalsFor() << "\n";
    std::cout << "Пропущено голов: " << team->getGoalsAgainst() << "\n";
    std::cout << "Разница голов: " << team->getGoalDifference() << "\n";
    std::cout << "Очки: " << team->getPoints() << "\n";

    double winPercentage = (team->getGamesPlayed() > 0) ?
        (static_cast<double>(team->getWins()) / team->getGamesPlayed() * 100) : 0;

    std::cout << "Процент побед: " << std::fixed << std::setprecision(1) << winPercentage << "%\n";
    std::cout << "----------------------------------------\n";
}

// Лучшая атака
Team* League::getTopScorer() {
    auto allTeams = getAllTeams();
    if (allTeams.empty()) return nullptr;

    Team* topScorer = allTeams[0];
    for (auto team : allTeams) {
        if (team->getGoalsFor() > topScorer->getGoalsFor()) {
            topScorer = team;
        }
    }
    return topScorer;
}

const Team* League::getTopScorer() const {
    auto allTeams = getAllTeams();
    if (allTeams.empty()) return nullptr;

    const Team* topScorer = allTeams[0];
    for (auto team : allTeams) {
        if (team->getGoalsFor() > topScorer->getGoalsFor()) {
            topScorer = team;
        }
    }
    return topScorer;
}

// Лучшая защита
Team* League::getBestDefense() {
    auto allTeams = getAllTeams();
    if (allTeams.empty()) return nullptr;

    Team* bestDefense = allTeams[0];
    for (auto team : allTeams) {
        if (team->getGoalsAgainst() < bestDefense->getGoalsAgainst()) {
            bestDefense = team;
        }
    }
    return bestDefense;
}

const Team* League::getBestDefense() const {
    auto allTeams = getAllTeams();
    if (allTeams.empty()) return nullptr;

    const Team* bestDefense = allTeams[0];
    for (auto team : allTeams) {
        if (team->getGoalsAgainst() < bestDefense->getGoalsAgainst()) {
            bestDefense = team;
        }
    }
    return bestDefense;
}

// Среднее количество голов
double League::getAverageGoalsPerGame() const {
    int totalMatches = getTotalMatchesPlayed();
    if (totalMatches == 0) return 0.0;
    return static_cast<double>(getTotalGoals()) / totalMatches;
}

// Количество сыгранных матчей
int League::getTotalMatchesPlayed() const {
    int count = 0;
    for (const auto& match : matches) {
        if (match.getStatus() == MATCH_PLAYED) {
            count++;
        }
    }
    return count;
}

// Общее количество голов
int League::getTotalGoals() const {
    int total = 0;
    for (const auto& match : matches) {
        if (match.getStatus() == MATCH_PLAYED) {
            total += match.getHomeScore() + match.getAwayScore();
        }
    }
    return total;
}

// Максимальное количество туров
int League::getMaxMatchDay() const {
    if (teams.size() < 2) return 0;
    return static_cast<int>((teams.size() - 1) * 2);
}

// Геттеры
std::string League::getCountry() const { return country; }
std::string League::getCurrentSeason() const { return currentSeason; }
int League::getLevel() const { return level; }
int League::getTeamsCount() const { return static_cast<int>(teams.size()); }

// Информация о лиге
void League::displayInfo() const {
    std::cout << "\n=== " << name << " ===\n";
    std::cout << "Страна: " << country << "\n";
    std::cout << "Сезон: " << currentSeason << "\n";

    std::string levelStr;
    switch (level) {
    case LEAGUE_PREMIER: levelStr = "Премьер-лига"; break;
    case LEAGUE_FIRST: levelStr = "Первая лига"; break;
    case LEAGUE_SECOND: levelStr = "Вторая лига"; break;
    default: levelStr = "Неизвестный уровень";
    }
    std::cout << "Уровень: " << levelStr << "\n";

    std::cout << "Количество команд: " << teams.size() << "\n";
    std::cout << "Количество матчей: " << matches.size() << "\n";
    std::cout << "Сыграно матчей: " << getTotalMatchesPlayed() << "\n";
}

// Обновление статистики
void League::updateTeamStats() {
    for (auto& team : teams) {
        team.resetStats();
    }

    for (const auto& match : matches) {
        if (match.getStatus() == MATCH_PLAYED) {
            Team* homeTeam = findTeamById(match.getHomeTeamId());
            Team* awayTeam = findTeamById(match.getAwayTeamId());

            if (homeTeam && awayTeam) {
                int homeGoals = match.getHomeScore();
                int awayGoals = match.getAwayScore();
                bool homeWin = homeGoals > awayGoals;
                bool draw = homeGoals == awayGoals;

                homeTeam->updateStats(homeGoals, awayGoals, homeWin, draw);
                awayTeam->updateStats(awayGoals, homeGoals, !homeWin && !draw, draw);
            }
        }
    }
}

// Сохранение матчей
void League::saveMatchesToFile() {
    std::ofstream file("matches.txt");
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось создать файл matches.txt\n";
        return;
    }

    file << matches.size() << "\n";
    for (const auto& match : matches) {
        file << match.getId() << " "
            << match.getHomeTeamId() << " "
            << match.getAwayTeamId() << " "
            << match.getHomeScore() << " "
            << match.getAwayScore() << " "
            << match.getMatchDay() << " "
            << match.getStatus() << "\n";
    }

    file.close();
    std::cout << "Матчи сохранены в файл matches.txt\n";
}

// Загрузка матчей
void League::loadMatchesFromFile() {
    std::ifstream file("matches.txt");
    if (!file.is_open()) {
        std::cout << "Файл matches.txt не найден. Будет создан новый.\n";
        return;
    }

    matches.clear();
    int matchCount;
    file >> matchCount;

    for (int i = 0; i < matchCount; ++i) {
        int id, homeId, awayId, homeScore, awayScore, matchDay, status;
        file >> id >> homeId >> awayId >> homeScore >> awayScore >> matchDay >> status;

        Match match(id, homeId, awayId, matchDay);
        if (status == MATCH_PLAYED) {
            match.playMatch(homeScore, awayScore);
        }
        matches.push_back(match);

        if (id >= nextMatchId) {
            nextMatchId = id + 1;
        }
    }

    file.close();
    updateTeamStats();
    std::cout << "Матчи загружены из файла matches.txt\n";
}

// Сохранение команд
void League::saveTeamsToFile() {
    std::ofstream file("teams.txt");
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось создать файл teams.txt\n";
        return;
    }

    file << teams.size() << "\n";
    for (const auto& team : teams) {
        file << team.getId() << "\n"
            << team.getName() << "\n"
            << team.getShortName() << "\n"
            << team.getStadium() << "\n"
            << team.getStadiumCapacity() << "\n"
            << team.getFoundedYear() << "\n";
    }

    file.close();
    std::cout << "Команды сохранены в файл teams.txt\n";
}

// Загрузка команд
void League::loadTeamsFromFile() {
    std::ifstream file("teams.txt");
    if (!file.is_open()) {
        std::cout << "Файл teams.txt не найден. Будет создан новый.\n";
        return;
    }

    teams.clear();
    int teamCount;
    file >> teamCount;
    file.ignore();

    for (int i = 0; i < teamCount; ++i) {
        int id, capacity, founded;
        std::string name, shortName, stadium;

        file >> id;
        file.ignore();
        std::getline(file, name);
        std::getline(file, shortName);
        std::getline(file, stadium);
        file >> capacity >> founded;
        file.ignore();

        Team team(id, name, shortName, stadium, capacity, founded);
        teams.push_back(team);

        if (id >= nextTeamId) {
            nextTeamId = id + 1;
        }
    }

    file.close();
    std::cout << "Команды загружены из файла teams.txt\n";
}