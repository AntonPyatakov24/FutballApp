#ifndef LEAGUE_HPP
#define LEAGUE_HPP

#include "DataEntity.hpp"
#include "Team.hpp"
#include "Match.hpp"
#include <vector>
#include <string>

// Уровни лиг для классификации
const int LEAGUE_PREMIER = 0;   // Премьер-лига (высший уровень)
const int LEAGUE_FIRST = 1;     // Первая лига (средний уровень)
const int LEAGUE_SECOND = 2;    // Вторая лига (низший уровень)

// Класс футбольной лиги, наследуется от DataEntity
class League : public DataEntity {
private:
    std::string country;
    std::string currentSeason;
    int level;
    std::vector<Team> teams;
    std::vector<Match> matches;
    int nextTeamId;
    int nextMatchId;

public:
    League(int leagueId, const std::string& leagueName, const std::string& leagueCountry,
        const std::string& season, int leagueLevel = LEAGUE_PREMIER);

    // Управление командами
    void addTeam(const std::string& name, const std::string& shortName,
        const std::string& stadium, int capacity, int founded);
    bool removeTeam(int teamId);
    Team* findTeamById(int id);
    const Team* findTeamById(int id) const;
    std::vector<Team*> getAllTeams();
    std::vector<const Team*> getAllTeams() const;

    // Расписание матчей
    void generateSchedule();
    void addMatchResult(int homeTeamId, int awayTeamId, int homeGoals, int awayGoals, int matchDay);
    std::vector<Match*> getTeamMatches(int teamId);
    std::vector<Match*> getPlayedMatches();

    // Турнирная таблица
    std::vector<Team*> getStandings();
    std::vector<const Team*> getStandings() const;
    void displayStandings() const;

    // Статистика
    void displayLeagueStats() const;
    void displayTeamStats(int teamId) const;
    Team* getTopScorer();
    const Team* getTopScorer() const;
    Team* getBestDefense();
    const Team* getBestDefense() const;
    double getAverageGoalsPerGame() const;
    int getTotalMatchesPlayed() const;
    int getTotalGoals() const;

    // Геттеры
    std::string getCountry() const;
    std::string getCurrentSeason() const;
    int getLevel() const;
    int getTeamsCount() const;
    int getMaxMatchDay() const;

    // Виртуальные методы
    void displayInfo() const override;

    // Работа с файлами
    void saveMatchesToFile();
    void loadMatchesFromFile();
    void saveTeamsToFile();
    void loadTeamsFromFile();

private:
    void updateTeamStats();
};

#endif