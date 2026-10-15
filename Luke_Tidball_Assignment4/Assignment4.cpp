/* EECS 348 Assignment 4
 *  This program is a sodoku solver meant to brute-force solutions for the timeless
 *  puzzle type. 
 *  Collaborators: Luke Tidball, ChatGPT 6
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Sudoku {
private:
    static const int SIZE = 9;
    int grid[SIZE][SIZE];

    // Checks whether a number can be placed in a cell.
    bool isValid(int row, int col, int num) const {
        // Check the row.
        for (int c = 0; c < SIZE; c++) {
            if (grid[row][c] == num) {
                return false;
            }
        }

        // Check the column.
        for (int r = 0; r < SIZE; r++) {
            if (grid[r][col] == num) {
                return false;
            }
        }

        // Find the top-left corner of the 3x3 block.
        int blockRow = (row / 3) * 3;
        int blockCol = (col / 3) * 3;

        // Check the 3x3 block.
        for (int r = blockRow; r < blockRow + 3; r++) {
            for (int c = blockCol; c < blockCol + 3; c++) {
                if (grid[r][c] == num) {
                    return false;
                }
            }
        }

        return true;
    }
    
    // Counts how many valid numbers can be placed in an empty cell.
    int countCandidates(int row, int col) const {
        int count = 0; // Stores the number of valid candidates found.

        // Try every possible Sudoku digit, from 1 through 9.
        for (int num = 1; num <= 9; num++) {

            // Check whether the current digit follows Sudoku's rules.
            if (isValid(row, col, num)) {

                count++; // Increase the count if the digit is valid.
            }
        }

        return count; // Return the total number of valid candidates.
    }

    // Finds the empty cell with the fewest valid candidate numbers.
    bool findBestEmptyCell(int &bestRow, int &bestCol) const {
        int minCandidates = 10; // Start above the maximum of nine candidates.
        bool foundEmpty = false; // Tracks whether an empty cell exists.
        // Examine each row of the Sudoku grid.
        for (int row = 0; row < SIZE; row++) {
            // Examine each column in the current row.
            for (int col = 0; col < SIZE; col++) {
                // Check whether the current cell is empty.
                if (grid[row][col] == 0) { //Cheks if the value at grid[row][col] is zero, then:
                    foundEmpty = true; // Record that an empty cell exists.
                    // Count the valid numbers for this empty cell.
                    int candidates = countCandidates(row, col);
                    // A cell with zero candidates cannot be solved.
                    if (candidates == 0) { //Cheks if canidates is zero, then:
                        bestRow = row; // Save the impossible cell's row.
                        bestCol = col; // Save the impossible cell's column.
                        return true; // Return so the solver can detect failure.
                    }//Closes if statement for if canidates == 0
                    // Check whether this cell has fewer candidates than before.
                    if (candidates < minCandidates) { // Checks if canidates is smaller than minCanidates, then:
                        minCandidates = candidates; // Update the smallest count.
                        bestRow = row; // Save the selected cell's row.
                        bestCol = col; // Save the selected cell's column.
                    } // closes the if statement for if canidates is smaller than minCanidates
                    // Stop searching if a cell has only one candidate.
                    if (minCandidates == 1) {
                        return true; // This is the smallest possible positive count.
                    }
                }
            }
        }

        // Return true if an empty cell was found, or false if the grid is full.
        return foundEmpty;
    }

// Recursively solves the puzzle using depth-first search and backtracking.
long long solveRecursive() {

    int row, col; // Store the coordinates of the selected empty cell.

    // Find the empty cell with the fewest valid candidates.
    if (!findBestEmptyCell(row, col)) {

        printGrid(); // Print the completed Sudoku solution.

        return 1; // Report that one valid solution has been found.
    }

    // Check whether the selected cell has any valid candidates.
    if (countCandidates(row, col) == 0) {

        return 0; // This search path cannot produce a solution.
    }

    long long solutions = 0; // Count the solutions found in this search.

    // Try every possible digit from 1 through 9.
    for (int num = 1; num <= 9; num++) {

        // Only attempt to place a digit if it follows Sudoku's rules.
        if (isValid(row, col, num)) {

            grid[row][col] = num; // Place the candidate in the selected cell.

            // Recursively search for solutions with this digit in place.
            solutions += solveRecursive();

            // Remove the digit so another candidate can be tested.
            grid[row][col] = 0;
        }
    }

    return solutions; // Return the total solutions found in this search.
}

    // Checks whether the original puzzle has conflicting clues.
    bool hasValidStartingGrid() {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                int num = grid[row][col];

                if (num == 0) {
                    continue;
                }

                // Temporarily remove the clue to check for conflicts.
                grid[row][col] = 0;

                bool valid = isValid(row, col, num);

                // Restore the original clue.
                grid[row][col] = num;

                if (!valid) {
                    return false;
                }
            }
        }

        return true;
    }

public:
    // Initializes an empty Sudoku grid.
    Sudoku() {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                grid[row][col] = 0;
            }
        }
    }

    // Reads a Sudoku puzzle from a file.
    bool loadPuzzle(const string &filename) {
        ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            cout << "Error: Could not open " << filename << endl;
            return false;
        }

        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                string cell;

                if (!(inputFile >> cell)) {
                    cout << "Error: Invalid puzzle format in "
                         << filename << endl;
                    inputFile.close();
                    return false;
                }

                if (cell == "_") {
                    grid[row][col] = 0;
                } else if (cell.length() == 1 &&
                           cell[0] >= '1' && cell[0] <= '9') {
                    grid[row][col] = cell[0] - '0';
                } else {
                    cout << "Error: Invalid cell in "
                         << filename << endl;
                    inputFile.close();
                    return false;
                }
            }
        }

        // Ensure the file contains exactly 81 cells.
        string extra;

        if (inputFile >> extra) {
            cout << "Error: Too many cells in "
                 << filename << endl;
            inputFile.close();
            return false;
        }

        inputFile.close();
        return true;
    }

    // Prints the Sudoku grid.
    void printGrid() const {
        for (int row = 0; row < SIZE; row++) {
            if (row % 3 == 0) {
                cout << "+-------+-------+-------+" << endl;
            }

            for (int col = 0; col < SIZE; col++) {
                if (col % 3 == 0) {
                    cout << "| ";
                }

                if (grid[row][col] == 0) {
                    cout << "_ ";
                } else {
                    cout << grid[row][col] << ' ';
                }
            }

            cout << "|" << endl;
        }

        cout << "+-------+-------+-------+" << endl;
    }

    // Solves the puzzle and prints all solutions.
    void solve() {
        if (!hasValidStartingGrid()) {
            cout << "No solution found" << endl;
            return;
        }

        long long solutions = solveRecursive();

        if (solutions == 0) {
            cout << "No solution found" << endl;
        } else {
            cout << "Total solutions found: "
                 << solutions << endl;
        }
    }
};

int main() {
    // Process all five puzzle files.
    for (int i = 1; i <= 5; i++) {
        string filename = "../puzzle" + to_string(i) + ".txt";

        cout << "\n====================================" << endl;
        cout << "Puzzle file: " << filename << endl;
        cout << "====================================" << endl;

        Sudoku sudoku;

        if (sudoku.loadPuzzle(filename)) {
            cout << "\nOriginal puzzle:" << endl;
            sudoku.printGrid();

            cout << "\nSolution(s):" << endl;
            sudoku.solve();
        }

        cout << endl;
    }

    return 0;
}