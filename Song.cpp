//
// Created by adria on 7/26/2021.
//

#include "Song.h"

// Accessors:
string Song::getName() const {
    return name;
}

int Song::getScore() const {
    return score;
}

// Mutators:
void Song::setName(string name) {
    this->name = name;
}

void Song::setScore(int score) {
    this->score = score;
}

// Constructor:
Song::Song(const string& name, int score) {
    this->name = name;
    this->score = score;
}

// Operator overloads:
bool Song::operator==(const Song& song) {
    return this->score == song.getScore();
}

bool Song::operator>(const Song& song){
    return this->score > song.getScore();
}

bool Song::operator<(const Song& song) {
    return this->score < song.getScore();
}

bool Song::operator<=(const Song& song) {
    return this->score <= song.getScore();
}

bool Song::operator>=(const Song& song) {
    return this->score >= song.getScore();
}
