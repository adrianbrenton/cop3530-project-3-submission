//
// Created by adria on 7/31/2021.
//

#ifndef COP3530_PROJECT_3_SONGCONTAINER_H
#define COP3530_PROJECT_3_SONGCONTAINER_H

#include <vector>

/**
 * Abstract base class for the SplayTree and MaxHeap classes.
 * Enables use of polymorphism in the main method of the program.
 */
class SongContainer {
public:
    // All method are pure virtual and should be overridden by subclasses:
    virtual void build(std::vector<Song>& inputSongs) = 0;
    virtual Song extractMax() = 0;
    virtual Song search(int targetScore) = 0;
    virtual void insert(Song song) = 0;
    virtual bool remove(string songName) = 0;
    virtual int size() = 0;
};


#endif //COP3530_PROJECT_3_SONGCONTAINER_H
