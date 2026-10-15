#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Sudoku {
private:
    char board[9][9];

    // Check if placing 'num' at board[row][col] is valid
    bool isValid(int row, int col, char num) const {
        for (int i = 0; i < 9; ++i) {
            // Check row and column constraints
            if (board[row][i] == num) return false;
            if (board[i][col] == num) return false;
        }

        // Check 3x3 block constraints
        int startRow = row - (row % 3);
        int startCol = col - (col % 3);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[startRow + i][startCol + j] == num) return false;
            }
        }

        return true;
    }

    // Recursive backtracking function to find and print all solutions
    void solveRecursive(int row, int col, int& solutionCount) {
        // Base case: if we've moved past the last row, a solution is found
        if (row == 9) {
            solutionCount++;
            std::cout << "Solution " << solutionCount << ":\n";
            printBoard();
            std::cout << "\n";
            return;
        }

        // Determine coordinates for the next cell
        int nextRow = (col == 8) ? row + 1 : row;
        int nextCol = (col == 8) ? 0 : col + 1;

        // If the current cell is already filled, move to the next cell
        if (board[row][col] != '_') {
            solveRecursive(nextRow, nextCol, solutionCount);
            return;
        }

        // Try candidates from '1' to '9'
        for (char num = '1'; num <= '9'; ++num) {
            if (isValid(row, col, num)) {
                board[row][col] = num;                      // Place candidate
                solveRecursive(nextRow, nextCol, solutionCount); // Recurse
                board[row][col] = '_';                      // Backtrack
            }
        }
    }

public:
    Sudoku() {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                board[i][j] = '_';
            }
        }
    }

    // Load puzzle from file (handles whitespace- and underscore-delimited grid formatting)
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                file >> board[i][j];
            }
        }
        return true;
    }

    // Print the 9x9 grid
    void printBoard() const {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                std::cout << board[i][j] << (j == 8 ? "" : " ");
            }
            std::cout << "\n";
        }
    }

    // Public method to initiate solving
    void solve() {
        int solutionCount = 0;
        solveRecursive(0, 0, solutionCount);
        if (solutionCount == 0) {
            std::cout << "No solution found\n";
        }
    }
};

int main() {
    std::string puzzleFiles[] = {"puzzle1.txt", "puzzle2.txt", "puzzle3.txt", "puzzle4.txt", "puzzle5.txt"};

    for (const auto& filename : puzzleFiles) {
        std::cout << "========================================\n";
        std::cout << "Puzzle File: " << filename << "\n";
        std::cout << "----------------------------------------\n";

        Sudoku puzzle;
        if (puzzle.loadFromFile(filename)) {
            std::cout << "Initial Puzzle:\n";
            puzzle.printBoard();
            std::cout << "\nSolving...\n\n";
            puzzle.solve();
        } else {
            std::cout << "Error: Could not open file " << filename << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}