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

    // Finds the next empty cell.
    bool findEmptyCell(int &row, int &col) const {
        for (row = 0; row < SIZE; row++) {
            for (col = 0; col < SIZE; col++) {
                if (grid[row][col] == 0) {
                    return true;
                }
            }
        }

        return false;
    }

    // Recursively searches for every solution.
    long long solveRecursive() {
        int row, col;

        // If there are no empty cells, a solution has been found.
        if (!findEmptyCell(row, col)) {
            printGrid();
            return 1;
        }

        long long solutions = 0;

        // Try every possible digit from 1 through 9.
        for (int num = 1; num <= 9; num++) {
            if (isValid(row, col, num)) {
                // Place the candidate number.
                grid[row][col] = num;

                // Recursively solve the remaining puzzle.
                solutions += solveRecursive();

                // Backtrack and try another candidate.
                grid[row][col] = 0;
            }
        }

        return solutions;
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
        string filename = "puzzle" + to_string(i) + ".txt";

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