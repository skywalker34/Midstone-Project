#include "Leaderboard.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Debug.h"

Leaderboard::Leaderboard(float t, int s, std::string n) : time(t), score(s), name(n)
{
}

//Use ifstream to read from text file
void readLeaderboard(const std::string& filename, std::vector<Leaderboard>& leaderboard) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        Debug::Info("Can't open leaderboard text file ", __FILE__, __LINE__);
        return;
    }

    float time;
    int score;
    std::string name = "";
    while (file >> time >> score >> name) {
        leaderboard.push_back(Leaderboard(time, score, name));
    }

    file.close();
}

// Sort the entries from the text file
void sortLeaderboardByScore(std::vector<Leaderboard>& leaderboard) {
    std::sort(leaderboard.begin(), leaderboard.end(), [](const Leaderboard& a, const Leaderboard& b) 
    {
        return a.score > b.score; // Sort score in descending order
    });
}

