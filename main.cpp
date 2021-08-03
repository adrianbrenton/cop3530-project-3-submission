#include <chrono>
#include <iostream>
#include <queue>
#include <unordered_map>

#include "Song.h"
#include "SongContainer.h"
#include "SplayTree.h"
// Please note: the below import sqlite3.h is not my code. It is a header file required for using the sqlite3 dll. Source: https://www.sqlite.org/download.html (taken from the amalgamation file).
#include "sqlite3.h"
#include "MaxHeap.h"

using namespace std;

// Prototypes:
// ===========
void readSqliteDb(const char* dbFilepath, unordered_map<string, int>& resultsMap);
void printMainMenu();
void printOperationsMenu();


// Implementations:
// ================

/**
 * Entry point for the program. Handles user input and general flow of the program.
 * @return 0 if program finishes without error.
 */
int main() {
    SongContainer* container = nullptr; // Pointer to the abstract base class. Polymorphism will allow this to contain either of our data structures.

    // Print initial text:
    cout << "Welcome to LyricsPsy!" << endl << endl;
    printMainMenu();

    // Get user's choice of data structure (either SplayTree or MaxHeap).
    int dataStuctureChoice = 0;
    cin >> dataStuctureChoice;
    while (dataStuctureChoice != 1 && dataStuctureChoice != 2) {
        cout << "Invalid data structure choice. Please select on of the option numbers from the menu." << endl << endl;
        printMainMenu();
        cin >> dataStuctureChoice;
    }

    // Initialize container.
    // Polymorphism is being used through an abstract base class, so the variable 'container' can handle either data structure:
    if (dataStuctureChoice == 1) {
        container = new MaxHeap();
    }
    else {
        container = new SplayTree();
    }

    bool isDataLoaded = false; // keeps track of whether the container has data yet. Determines whether 'build' should be called.

    // Print first appearance of the Operations Menu:
    cout << endl;
    cout << endl;
    printOperationsMenu();

    // Get user's choice of operation based on the Operations Menu options:
    int operationChoice;
    cin >> operationChoice;

    // Keep following user instructions until user chooses option 9 (Quit):
    while(operationChoice != 9) {

        if (operationChoice == 1) { // Load data from SQLite database file:
            if (!isDataLoaded) {
                cout << "Please specify the path of your SQLite database file: ";
                string filepath;
                cin >> filepath;
                unordered_map<string, int> songScores;
                readSqliteDb(filepath.c_str(), songScores);
                vector<Song> songs; // This will be used to initialize the SplayTree or MaxHeap.

                // Populate 'songs' with the songs from the database.
                for (auto iter = songScores.begin(); iter != songScores.end(); ++iter) {
                    songs.push_back(Song(iter->first, iter->second));
                }

                // Build the data structure with the newly read data and time how long it takes:
                chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now(); // start timing.
                container->build(songs); // build the underlying data structure for the program.
                chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now(); // stop timing.
                auto timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

                // Print the result and how long it took:
                cout << "Success! Data structure has been built and populated with values from the database!" << endl;
                cout << "Time taken: " << timeTaken.count() << "ns" << endl << endl << endl;
            }
            else {
                // If container has already been built, the program prevents user from building it again and assumes this is a mistake.
                // User should restart the program if they wish to reinitialize data:
                cout << "Container has already been initialized with data." << endl;
                cout << "If you wish to use a different dataset, please restart the program." << endl;
            }
            cout << endl << endl;
        }

        else if (operationChoice == 2) { // Insert a song by providing the name/ID-string and narcissism score:
            string songId;
            cout << "Please provide the song ID: ";
            cin >> songId;

            int score;
            cout << "Please specify the narcissism score for the song: ";
            cin >> score;

            // Insert the song and time how long it takes:
            chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
            container->insert(Song(songId, score));
            chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
            auto timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

            // Print result and how long it took:
            cout << "Success! " << songId << " has been added with narcissism index " << score << "." << endl;
            cout << "Time taken: " << timeTaken.count() << "ns" << endl << endl << endl;
            isDataLoaded = true;
        }

        else if (operationChoice == 3) { // Insert a song by providing the name/ID-string and word frequencies:
            string songId;
            cout << "Please provide the song ID: ";
            cin >> songId;

            // To be populated by user input. User for calculating song's score:
            int iCount;
            int meCount;
            int myCount;

            // Get information about the song from the user:
            cout << "How many times does the word 'I' appear in the lyrics? ";
            cin >> iCount;
            cout << "How many times does the word 'me' appear in the lyrics? ";
            cin >> meCount;
            cout << "How many times does the word 'my' appear in the lyrics? ";
            cin >> myCount;

            int score = iCount + meCount + myCount; // Calculate the narcissism score

            // Insert the song and time how long it takes:
            chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
            container->insert(Song(songId, score));
            chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
            auto timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

            // Print result and how long it took:
            cout << "Success! " << songId << " has been added with narcissism index " << score << "." << endl;
            cout << "Time taken: " << timeTaken.count() << "ns" << endl << endl << endl;
            isDataLoaded = true;
        }

        else if (operationChoice == 4) { // Remove a song by songname/ID-string:
            string songId;
            cout << "Please provide the song ID to remove: ";
            cin >> songId;

            // Remove the song and time how long it takes:
            chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
            bool success = container->remove(songId);
            chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
            auto timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

            // Print result:
            if (success) {
                cout << songId << " successfully removed!" << endl;
            }
            else {
                cout << songId << " could not be found. Nothing was removed!" << endl;
            }

            // Print the time taken:
            cout << "Time taken: " << timeTaken.count() << "ns" << endl << endl << endl;
        }

        else if (operationChoice == 5) { // Search for a song with a specific narcissism score:

            // Get the score that needs to be searched for. The user should provide this:
            int targetScore;
            cout << "Please provide the score to search for: ";
            cin >> targetScore;

            // Search for the song and time how long it takes:
            chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
            Song result = container->search(targetScore);
            chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
            auto timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

            // Print result:
            if (result.getName().empty()) {
                cout << "No songs found with narcissism score of " << targetScore << endl;
            }
            else {
                cout << "Found song " << result.getName() << " with score of " << result.getScore() << endl;
            }

            // Print the time taken:
            cout << "Time taken: " << timeTaken.count() << "ns" << endl << endl << endl;
        }

        else if (operationChoice == 6) { // Search for all songs within a range of narcissism scores:

            // Get the lower bound of the range from the user:
            int lowerBound;
            cout << "Please provide the minimum score to search for in the range: ";
            cin >> lowerBound;

            // Get the upper bound of the range from the user:
            int upperBound;
            cout << "Please provide the maximum score to search for in the range: ";
            cin >> upperBound;

            queue<Song> results; // This will keep track of the results of the search.

            // Search for all songs in the given range and time how long it takes:
            chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
            for (int searchCounter = lowerBound; searchCounter < upperBound + 1; searchCounter++) {
                Song result = container->search(searchCounter);
                results.push(result);
            }
            chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
            auto timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

            // Print results:
            if (results.empty()) {
                cout << "Found no songs in the given narcissism score range!" << endl;
            }
            else {
                while(!results.empty()) {
                    string name = results.front().getName();
                    int score = results.front().getScore();

                    if (!name.empty()) { // If the name is empty, then it was a placeholder result for a score that does not exist.
                        cout << "Found song " << name << " with score of " << score << endl;
                    }
                    results.pop();
                }
            }

            // Print time taken:
            cout << endl;
            cout << "Time taken for " << (upperBound - lowerBound + 1) << " searches: " << timeTaken.count() << "ns" << endl << endl << endl;
        }

        else if (operationChoice == 7) { // Print on-screen the number of songs currently stored in the data structure:

            // Get the size() of the data structure and time how long the method call takes to execute:
            chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
            int size = container->size();
            chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
            auto timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

            // Print the result and the time taken:
            cout << "There are " << size << " songs in the program." << endl;
            cout << "Time taken: " << timeTaken.count() << "ns" << endl << endl << endl;
        }

        else if (operationChoice == 8) { // extract the N songs with the highest narcissism scores from the data structure:

            // User needs to specify how many songs they want to extract:
            int N;
            cout << "Please specify N: ";
            cin >> N;

            // Extract N songs and time how long it takes:
            chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
            for (int i = 0; i < N; i++) {
                Song song = container->extractMax();
                cout << song.getName() << " has score of " << song.getScore() << endl;
            }
            chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
            auto timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

            // Print the time taken:
            cout << "Time taken: " << timeTaken.count() << "ms" << endl << endl << endl;
        }

        else if (operationChoice == 9) { // Quit the program:
            cout << "Goodbye!" << endl;
            return 0;
        }

        else { // User made an invalid selection:
            cout << "Invalid selection! Please enter an option number from the menu." << endl << endl;
        }

        // Operation complete! Print the Operations Menu again and prompt the user to choose another operation:
        printOperationsMenu();
        cin >> operationChoice;
    }


    return 0;
}


/**
 * Reads a Million Song Dataset 'bag of words'-style SQLite database.
 * @param resultsMap A map to contain each song name and its calculated narcissism score.
 * @param dbFilepath The path to the SQLite database file.
 */
void readSqliteDb(const char* dbFilepath, unordered_map<string, int>& resultsMap) {
    sqlite3* connection;
    int retCode = sqlite3_open(dbFilepath, &connection);
    if (retCode != 0) {
        cout << "Error reading database. Canceling DB operation." << endl;
    }
    else {
        sqlite3_stmt* selectStmt; // Stores a prepared statement. Will be populatef by sqlite3_prepare_v2

        // Library documentation specifies to use v2 method because the original method is a deprecated legacy function:
        const char* selectQuery = "SELECT track_id, word, count FROM lyrics WHERE word='i' OR word='me' OR word='my'";
        sqlite3_prepare_v2(connection, selectQuery, -1, &selectStmt, nullptr);

        int counter = 0;
        cout << endl;

        chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now(); // Start timing

        // Loop through every row that resulted from the SELECT SQL statement to create the Songs.
        while(sqlite3_step(selectStmt) != SQLITE_DONE) { // While there are still rows in the result set:
            cout << "\rreading database result row: " << counter;
            counter++;
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt, 0));
            string nameStr = name;
            int score = sqlite3_column_int(selectStmt, 2);
            auto resultsMapIter = resultsMap.find(name);

            if(resultsMapIter == resultsMap.end()) { // insert into map if song has not yet been encountered:
                resultsMap[nameStr] = score;
            }
            else { // If song already in the mapping, then update it with a higher score:
                resultsMap.at(nameStr) = resultsMap.at(nameStr) + score;
            }
        }

        chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now(); // Stop timing

        // Print the time taken for the data import step to complete:
        auto timeTaken = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
        cout << endl << "Database import into program took " << timeTaken.count() << " seconds." << endl << endl << endl;

        // Close all database connections and statements to free up memory:
        sqlite3_finalize(selectStmt);
        sqlite3_close(connection);
    }
}


/**
 * Prints the main menu in the console.
 */
void printMainMenu() {
    cout << "==========" << endl;
    cout << "Main menu:" << endl;
    cout << "==========" << endl;
    cout << "1. Priority queue (MaxHeap)" << endl;
    cout << "2. Splay tree" << endl;
    cout << endl;
    cout << "Please select a data structure: ";
}


/**
 * Prints the operations menu in the console.
 */
void printOperationsMenu() {
    cout << "================" << endl;
    cout << "Operations menu:" << endl;
    cout << "================" << endl;
    cout << "1. Read from SQLite data source" << endl;
    cout << "2. Insert a single song with known narcissism metric" << endl;
    cout << "3. Insert a single song with unknown narcissism metric" << endl;
    cout << "4. Remove a song by MXM ID string" << endl;
    cout << "5. Search for a song by score" << endl;
    cout << "6. Search for songs with a range of scores" << endl;
    cout << "7. Count songs in program" << endl;
    cout << "8. Print and remove top N results." << endl;
    cout << "9. Quit" << endl;
    cout << endl;
    cout << "Please select an operation: ";
}