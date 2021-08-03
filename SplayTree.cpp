//
// Created by adria on 7/27/2021.
//

#include <stack>
#include <stdexcept>

#include "SplayTree.h"

using namespace std;


/**
 * Default Constructor.
 */
SplayTree::SplayTree() : root(nullptr), numElements(0)  {}


/**
 * Build the SplayTree from Songs in a vector.
 * @param songs A vector of songs from which to populate the SplayTree.
 */
void SplayTree::build(vector<Song>& songs) {
    for (unsigned int i = 0; i < songs.size(); i++) {
        Node* newNode = insertSong(songs.at(i)); // Insert the song and obtain pointer to the node in which it is stored.
        numElements++;
        root = splay(root, newNode); // Move the node that was found to the root.
    }
}


/**
 * The rotation to use when the Node being splayed to the top is the right child of the root node.
 * @param node The parent node of the target node.
 * @return The root of the resulting subtree
 */
SplayTree::Node* SplayTree::zigLeft(Node* node) {
    // 'Zig' is effectively a single rotation:
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    return newRoot;
}


/**
 * The rotation to use when the Node being splayed to the top is the left child of the root node.
 * @param node The parent node of the target node.
 * @return The root of the resulting subtree
 */
SplayTree::Node* SplayTree::zigRight(Node* node) {
    // 'Zig' is effectively a single rotation:
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    return newRoot;
}


/**
 * The rotation to use when the Node being splayed to the top is the right child of a right child.
 * @param node The grandparent node of the target node.
 * @return The root of the resulting subtree
 */
SplayTree::Node* SplayTree::zigZigLeft(Node* node) {
    // Prevent misuse of this helper method:
    if (node->right->right == nullptr) {
        throw std::invalid_argument("Invalid use of zigZigLeft. Required grandchild does not exist.");
    }

    // 'Zig-zig' is a rotation about the grandparent followed by a rotation about the parent:
    Node* newRoot = zigLeft(node);
    newRoot = zigLeft(newRoot);
    return newRoot;
}


/**
 * The rotation to use when the Node being splayed to the top is the left child of a left child.
 * @param node The grandparent node of the target node.
 * @return The root of the resulting subtree
 */
SplayTree::Node* SplayTree::zigZigRight(Node* node) {
    // Prevent misuse of this helper method:
    if (node->left->left == nullptr) {
        throw std::invalid_argument("Invalid use of zigZigRight. Required grandchild does not exist.");
    }

    // 'Zig-zig' is a rotation about the grandparent followed by a rotation about the parent:
    Node* newRoot = zigRight(node);
    newRoot = zigRight(newRoot);
    return newRoot;
}


/**
 * The rotation to use when the Node being splayed to the top is the right child of a left child.
 * @param node The grandparent node of the target node.
 * @return The root of the resulting subtree
 */
SplayTree::Node* SplayTree::zigZagLeftRight(Node* node) {
    // Prevent misuse of this helper method:
    if (node->left->right == nullptr) {
        throw std::invalid_argument("Invalid use of zigZagLeftRight. Required grandchild does not exist.");
    }

    // 'Zig-zag' is a rotation about the parent followed by an opposite rotation about the grandparent:
    node->left = zigLeft(node->left);
    Node* newRoot = zigRight(node);
    return newRoot;
}


/**
 * The rotation to use when the Node being splayed to the top is the left child of a right child.
 * @param node The grandparent node of the target node.
 * @return The root of the resulting subtree
 */
SplayTree::Node* SplayTree::zigZagRightLeft(Node* node) {
// Prevent misuse of this helper method:
    if (node->right->left == nullptr) {
        throw std::invalid_argument("Invalid use of zigZagRightLeft. Required grandchild does not exist.");
    }

    // 'Zig-zag' is a rotation about the parent followed by an opposite rotation about the grandparent:
    node->right = zigRight(node->right);
    Node* newRoot = zigLeft(node);
    return newRoot;
}


/**
 * Rearranges (A.K.A. 'splays') a tree/subtree to move a given node to the root position of the subtree.
 * @param node The root of the tree/subtree.
 * @param target The node to be moved to the root position.
 * @return A pointer to the new root node after splaying has been completed, or nullptr if the node does not exist.
 */
SplayTree::Node* SplayTree::splay(Node *node, Node *target) {
    if (node == nullptr) { // If the entire tree/subtree is empty
        return nullptr;
    }

    if (node == target) { // This should only occur if node is the root node of the entire Splay Tree
        return node;
    }

    stack<Node*> path; // This will keep track of the path from node to the target node.
    path.push(node);

    // populate the 'path' stack with the path to the target node:
    while(path.top() != target) {
        if(target->val < path.top()->val) {
            path.push(path.top()->left);
        }
        else { // if target->val > node->val:
            path.push(path.top()->right);
        }
        if (path.top() == nullptr) {
            return nullptr;
        }
    }

    path.pop(); // Remove the target from the stack. Our iteration needs to start from its parent.

    // Work down the stack with ZigZig, ZigZag and Zig operations to get the target node to the root of the subtree:
    while(path.size() > 1) {
        // Determine the direction of the pointer to target.
        Node* parent = path.top();
        bool lowerDirectionLeft = (parent->left == target);
        path.pop();

        // Determine the direction of the pointer to target.
        Node* grandParent = path.top();
        bool upperDirectionLeft = (grandParent->left == parent);
        path.pop();

        // Perform zig/zag operation to rearrange the subtree so that target is at the root of the current subtree:
        Node* newSubtree = nullptr;
        if(lowerDirectionLeft && upperDirectionLeft) {
            newSubtree = zigZigRight(grandParent);
        }
        else if (!lowerDirectionLeft && !upperDirectionLeft) {
            newSubtree = zigZigLeft(grandParent);
        }
        else if (lowerDirectionLeft && !upperDirectionLeft) {
            newSubtree = zigZagRightLeft(grandParent);
        }
        else { // !lowerDirectionLeft && upperDirectionLeft
            newSubtree = zigZagLeftRight(grandParent);
        }

        // Attach the newly rearranged subtree:
        if (!path.empty()) {
            if (path.top()->left == grandParent) {
                path.top()->left = newSubtree;
            }
            else { //path.top()->right == grandparent
                path.top()->right = newSubtree;
            }
        }
    }

    // Perform final Zig operation if necessary:
    if (!path.empty()) {
        if (path.top()->left == target) {
            zigRight(path.top());
        }
        else { //path.top()->right == target
            zigLeft(path.top());
        }
    }

    return target;
}


/**
 * Helper method to find a node by score. The result is NOT guaranteed to be unique.
 * Does NOT splay the found node to the root position. Splay must be called separately if needed.
 * @param targetScore The score to be searched for.
 * @return A pointer to the node that has been found.
 */
SplayTree::Node* SplayTree::searchNode(int targetScore) {
    Node* curr = root;
    while (curr != nullptr) {
        if (targetScore < curr->val.getScore()) {
            curr = curr->left; // target node can only be in the left subtree.
        }
        else if (targetScore > curr->val.getScore()) {
            curr = curr->right; // target node can only be in the right subtree.
        }
        else { // curr->val == targetScore
            return curr; // We have found the target node.
        }
    }
    return curr; // this will always be nullptr because of the loop condition above.
}


/**
 * Helper method that inserts a song in a valid location to maintain BST ordering.
 * If the song already exists, then this method simply updates its score field.
 * Does NOT splay the newly inserted node to the root position. Splay must be called separately if needed.
 * @param song The new song object to be inserted.
 * @return The node that was updated or created and inserted.
 */
SplayTree::Node* SplayTree::insertSong(Song song) {
    // Corner case: Tree is empty:
    if (root == nullptr) { // newNode can simply be placed at root of SplayTree.
        root = new Node(song);
        return root;
    }
    // We must find the correct leaf and insert the new node as a child of that leaf.

    // Variables to keep track of the iteration:
    Node* curr = root;
    Node* leaf = nullptr;

    // Find the appropriate leaf node:
    while (curr != nullptr) {
        // First, check for edge case where song already exists:
        if(curr->val.getName() == song.getName()) {
            curr->val.setScore(song.getScore()); // Update the score because this song already exists.
            return curr; // return the updated node so it can be splayed by the caller if necessary.
        }

        leaf = curr; // 'leaf' will keep being updated each iteration until we have found a real leaf.

        if (song < curr->val) {
            curr = curr->left; // new node must be attached in left subtree.
        }
        else if (song >= curr->val) {
            curr = curr->right; // new node must be attached in right subtree.
        }
    }

    Node* newNode = new Node(song); // Nodes always live in heap memory.

    // Add the new node as a child of the leaf node:
    if (song < leaf->val) {
        leaf->left = newNode;
    }
    else { // newNode->val >= leaf->val
        leaf->right = newNode;
    }

    return newNode;
}


/**
 * Removes a node by splaying it to the root position and then deleting it.
 * Under most circumstances, the new root will be the inorder predecessor of the node that was deleted.
 * The inorder predecessor must be splayed
 * If the deleted node does not have an inorder successor, then the inorder predecessor will be the new root instead.
 * @param node The node to be removed.
 */
void SplayTree::removeNode(Node* node) {
    root = splay(root, node); // Bring the node that needs to be removed to the root position of the tree.

    // Splay the inorder predecessor to the root of the left subtree. If left subtree is nullptr, nothing will change:
    Node* inorderPredecessor = getMaxNode(root->left);
    Node* leftSubtree = splay(root->left, inorderPredecessor);

    Node* rightSubtree = root->right;

    delete root; // deallocate the memory for the element being deleted. A new value will be assigned to root below.

    // Edge case:
    if (leftSubtree == nullptr) { // Root is the minimum element based on the given ordering:
        root = rightSubtree;
    }
    else { // Join the Left and Right Subtrees together:
        leftSubtree->right = rightSubtree; // We now leftSubtree has no right child because it is the inorderPredecessor of the old root.
        root = leftSubtree;
    }
}


/**
 * Find the highest node in a subtree (based on the ordering of the nodes)
 * @param node The root of the subtree.
 * @return The highest node in the subtree.
 */
SplayTree::Node* SplayTree::getMaxNode(Node* node) {
    Node* maxNode = node; // Initialize maxNode to the root of the tree/subtree.

    // If maxNode has a right child, that right child must be higher than maxNode, so reassign it:
    while(maxNode->right != nullptr) {
        maxNode = maxNode->right;
    }
    return maxNode;
}


/**
 * Find the lowest node in a subtree (based on the ordering of the nodes)
 * @param node The root of the subtree.
 * @return The lowest node in the subtree.
 */
SplayTree::Node* SplayTree::getMinNode(Node* node) {
    Node* minNode = node; // Initialize maxNode to the root of the tree/subtree.

    // If minNode has a left child, that left child must be lower than minNode, so reassign it:
    while(minNode->left != nullptr) {
        minNode = minNode->left;
    }
    return minNode;
}


/**
 * Recursive method that populates a vector with the preorder traversal of the nodes in a tree.
 * @param node Root node of tree or subtree to be traversed.
 * @param output The vector to which the traversal should be appended.
 */
vector<SplayTree::Node*> SplayTree::preorderNodes(Node* node) {
    vector<Node*> output;

    // Edge case - empty tree/subtree:
    if (node == nullptr) {
        return output;
    }

    // Iterative traversal needs a stack for a depth-first approach:
    stack<Node*> stk;
    stk.push(node);

    // Use stack to keep track of traversal. Store already-visited nodes in output:
    while(!stk.empty()) {
        Node* top = stk.top();
        stk.pop();

        output.push_back(top);

        if(top->right != nullptr) {
            stk.push(top->right);
        }

        if(top->left != nullptr) {
            stk.push(top->left);
        }
    }

    return output; // output has been populated - return it.
}


// Public methods:
// ===============
Song SplayTree::extractMax() {
    // Handle 'empty tree' edge case:
    if (root == nullptr) {
        return Song();
    }

    Node* maxNode = getMaxNode(root);
    Song output = maxNode->val;
    removeNode(maxNode);
    numElements--;
    return output;
}

Song SplayTree::search(int targetScore) {
    Node* target = searchNode(targetScore); // Obtain ptr to the highest node in the tree with the target score.
    if (target == nullptr) {
        return Song();
    }
    root = splay(root, target); // Move the node that was found to the root
    return target->val;
}


void SplayTree::insert(Song song) {
    Node* newNode = insertSong(song); // Insert the song and obtain pointer to the node in which it is stored.
    numElements++;
    root = splay(root, newNode); // Move the node that was found to the root.
}

bool SplayTree::remove(string songName) {
    // Get a preorder traversal of the Splay tree.
    // We are searching by songName, but the tree is not ordered by songName, so we have to check every node.
    vector<Node*> preorderTraversal;
    preorderTraversal = preorderNodes(root);

    // Search for the song name in the preorder traversal linearly:
    for (unsigned int i = 0; i < preorderTraversal.size(); i++) {
        if (preorderTraversal.at(i)->val.getName() == songName) {
            removeNode(preorderTraversal.at(i));
            numElements--;
            return true;
        }
    }

    return false; // The song was not in the Splay Tree.
}

int SplayTree::size() {
    return numElements;
}
