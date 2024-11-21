#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <vector>
#include <string>

// struct to store leader board variables
struct Leaderboard {
    float time; // Time in seconds
    int score;  // Score of the player
    std::string name;

    Leaderboard(float t, int s, std::string n);
};

// Function declarations
void readLeaderboard(const std::string& filename, std::vector<Leaderboard>& leaderboard);
void sortLeaderboardByScore(std::vector<Leaderboard>& leaderboard);
//TO DO
//void writeLeaderboard(const std::string& filename, std::vector<Leaderboard>& leaderboard);

#endif // LEADERBOARD_H
