//
// Created by adria on 7/30/2021.
//

#include <iostream>
#include "MaxHeap.h"

/**
 * Swap elements down the heap to move the Song at startPos down to its correct position.
 * @param startPos The initial position of the Song that might need to be moved.
 */
void MaxHeap::adjustHeapDown(int startPos) {
    // Initialize index trackers:
    int current = startPos;
    int left = (current * 2) + 1;
    int right = (current * 2) + 2;

    // Iterate through the heap from the top until either we reach the end, or the right and left children are both smaller than current.
    while( (left < songs.size() && songs.at(current) < songs.at(left) ) ||
           (right < songs.size() && songs.at(current) < songs.at(right) )) {
        if (right < songs.size() && songs.at(left) < songs.at(right)) { // If right child exists and is the larger value:

            // Swap Song at current with its right child:
            Song temp = songs.at(right);
            songs.at(right) = songs.at(current);
            songs.at(current) = temp;

            // Update index trackers:
            current = right;
            left = current * 2 + 1;
            right = current * 2 + 2;
        }
        else { // if right child does not exist of if left child is larger than right child:

            // Swap Song at current with its left child:
            Song temp = songs.at(left);
            songs.at(left) = songs.at(current);
            songs.at(current) = temp;

            // Update index trackers:
            current = left;
            left = current * 2 + 1;
            right = current * 2 + 2;
        }
    }
}

/**
 * Swap elements up the heap to move the Song at startPos up to its correct position.
 * @param startPos The initial position of the Song that might need to be moved.
 */
void MaxHeap::adjustHeapUp(int startPos) {
    // Initialize index trackers:
    int current = startPos;
    int parent = (current - 1) / 2; // integer division ensures that this index is correct.

    while (current > 0 && songs.at(parent) < songs.at(current)) { // while current is not at top and heap layout is invalid:

        // Swap Song at current with its parent:
        Song temp = songs.at(parent);
        songs.at(parent) = songs.at(current);
        songs.at(current) = temp;

        // Update index trackers:
        current = parent;
        parent = (current - 1) / 2;
    }
}

/**
 * Default constructor initializes the MaxHeap as an empty heap.
 */
MaxHeap::MaxHeap() : numElements(0) {}

/**
 * Builds the MaxHeap from the songs in a vector.
 * @param songs A vector containing the songs to be used for building the MaxHeap.
 */
void MaxHeap::build(vector<Song>& inputSongs) {
    for (int i = inputSongs.size() - 1; i > -1; i--) {
        this->insert(inputSongs.at(i));
    }
}

/**
 * Remove the highest scoring song from the heap.
 * @return A copy of the Song object that was removed.
 */
Song MaxHeap::extractMax() {
    Song output = songs.front();
    songs.front() = songs.back();
    songs.pop_back();
    numElements--;
    adjustHeapDown(0);
    return output;
}


/**
 * Find the first song in the heap with a given score when searching by level order traversal.
 * @param targetScore The song score we are looking for in the heap.
 * @return A copy of the song object found by the search.
 */
Song MaxHeap::search(int targetScore) {
    for (unsigned int i = 0; i < songs.size(); i++) {
        if (songs.at(i).getScore() == targetScore) {
            return songs.at(i);
        }
    }
    return Song(); // If song not found with the target value, then return an empty Song object.
}


/**
 * Insert a new Song into the heap.
 * @param song The new Song object to be inserted
 */
void MaxHeap::insert(Song song) {
    songs.push_back(song); // Add song to last position of heap.
    numElements++;
    adjustHeapUp(songs.size() - 1);
}

/**
 * Remove a Song by name.
 * @param songName The name of the Song object to be removed.
 * @return true if the song was found and removed, false otherwise.
 */
bool MaxHeap::remove(string songName) {
    for(unsigned int i = 0; i < songs.size(); i++) {
        if(songs.at(i).getName() == songName) { // If we have found the song with name 'songName':
            // Move the last element of the heap to replace the element being removed and then adjust downwards as needed:
            songs.at(i) = songs.back();
            songs.pop_back();
            adjustHeapDown(i);
            numElements--;
            return true;
        }
    }
    return false; // If execution reaches here, then the song was not found by the iteration, so return false.
}

int MaxHeap::size() {
    return numElements;
}

// DEBUG:
void MaxHeap::print() {
    for (int i = 0; i < songs.size(); i++) {
        std::cout << songs.at(i).getName() << ": " << songs.at(i).getScore() << ", ";
    }
    std::cout << std::endl;
}