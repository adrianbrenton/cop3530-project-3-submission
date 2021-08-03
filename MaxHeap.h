//
// Created by adria on 7/30/2021.
//

#ifndef COP3530_PROJECT_3_MAXHEAP_H
#define COP3530_PROJECT_3_MAXHEAP_H

#include <vector>

#include "Song.h"
#include "SongContainer.h"

using namespace std;

class MaxHeap : public SongContainer {
private:
    vector<Song> songs; // Dynamic array representation of heap is used.
    int numElements; // Keep track of the number of elements.

    // Helper methods to correct the heap layout during insertion and removal operations:
    void adjustHeapDown(int startPos);
    void adjustHeapUp(int startPos);
public:
    MaxHeap(); // ctr:

    // Overridden functions. See the .cpp implementation file:
    virtual void build(vector<Song>& inputSongs);
    virtual Song extractMax();
    virtual Song search(int targetScore);
    virtual void insert(Song song);
    virtual bool remove(string songName);
    virtual int size();

    // Method not used in program, but is implemented in the .cpp file:
    void print();
};


#endif //COP3530_PROJECT_3_MAXHEAP_H
