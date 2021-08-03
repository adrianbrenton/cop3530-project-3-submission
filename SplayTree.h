//
// Created by adria on 7/27/2021.
//

#ifndef COP3530_PROJECT_3_SPLAYTREE_H
#define COP3530_PROJECT_3_SPLAYTREE_H

#include "song.h"
#include "SongContainer.h"
#include <vector>

class SplayTree : public SongContainer {
private:

    /**
     * Inner class to represent a node in the SplayTree.
     */
    struct Node {
        Song val;
        Node* left;
        Node* right;
        Node(const Song& song, int height = 1, Node* left = nullptr, Node* right = nullptr) : val(song), left(left), right(right) {}
    };

    Node* root; // Always points to the root Node of the SplayTree.
    int numElements; // Keeps track of the number of elements in the SplayTree.

    // Zig/Zag methods to operate on subtrees. Very similar to AVL rotations:
    Node* zigLeft(Node* node);
    Node* zigRight(Node* node);
    Node* zigZigLeft(Node* node);
    Node* zigZigRight(Node* node);
    Node* zigZagLeftRight(Node* node);
    Node* zigZagRightLeft(Node* node);

    // Tree helper methods:
    Node* splay(Node* node, Node* target);
    Node* insertSong(Song song);
    void removeNode(Node* node);
    Node* searchNode(int targetScore); // Plan: usual splay tree search
    Node* getMaxNode(Node* node);
    Node* getMinNode(Node* node);
    int getMaxScore(Node* node);

    vector<Node*> preorderNodes(Node* node); // Iteratively obtains a preorder traversal of the nodes in the tree.
public:
    SplayTree(); // ctr

    // Overridden functions. See the .cpp implementation file:
    virtual void build(vector<Song>& songs);
    virtual Song extractMax();
    virtual Song search(int targetScore);
    virtual void insert(Song song);
    virtual bool remove(string songName);
    virtual int size();
};


#endif //COP3530_PROJECT_3_SPLAYTREE_H
