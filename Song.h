//
// Created by adria on 7/26/2021.
//

#ifndef COP3530_PROJECT_3_SONG_H
#define COP3530_PROJECT_3_SONG_H

#include <string>

using namespace std;

/**
 * Data class to store Student data.
 */
class Song {
private:
    string name;
    int score;

public:
    // Accessors:
    string getName() const;
    int getScore() const;

    // Mutators:
    void setName(string name);
    void setScore(int score);

    // Constructor:
    Song(const string& name = "", int score = 0);

    // Operator overloads:
    bool operator>(const Song& song);
    bool operator<(const Song& song);
    bool operator==(const Song& song);
    bool operator<=(const Song& song);
    bool operator>=(const Song& song);
};


#endif //COP3530_PROJECT_3_SONG_H
