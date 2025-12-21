// main.cpp
#include <iostream>
#include "BacktrackingSolver.h"

using namespace std;

void printBoard(const Board& board) {
    for (const auto& row : board) {
        for (int cell : row) {
            cout << (cell ? "Q " : ". ");
        }
        cout << endl;
    }
    cout << "----------------\n";
}

int main() {
    int n = 8; // Ví dụ n = 4
    BacktrackingSolver solver;
    vector<Board> results = solver.solve(n);

    cout << "Tim thay " << results.size() << " loi giai cho ban co " << n << "x" << n << ":\n";
    for (const auto& board : results) {
        printBoard(board);
    }

    return 0;
}