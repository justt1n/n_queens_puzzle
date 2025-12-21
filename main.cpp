#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

#include "BacktrackingSolver.h"
#include "SimpleSolver.h"

using namespace std;
using namespace std::chrono;


void printBoard(const Board& board) {
    for (const auto& row : board) {
        for (int cell : row) cout << (cell ? "Q " : ". ");
        cout << endl;
    }
    cout << "----------------\n";
}

void benchmark(NQueensSolver* solver, int n, string solverName) {
    int isPrint = 0;
    cout << solverName << " voi N = " << n << "..." << endl;

    auto start = high_resolution_clock::now();

    vector<Board> results = solver->solve(n);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "=> Ket qua " << solverName << ":\n";
    cout << "Cac loi giai tim duoc:\n";
    if (isPrint == 1) {
        for (const auto& board : results) {
            printBoard(board);
        }
    }
    cout << "   - So loi giai: " << results.size() << endl;
    cout << "   - Thoi gian:   " << duration.count() << " ms (" << duration.count() / 1000.0 << " giay)" << endl;
    cout << "----------------------------------------\n";
}

int main() {
    int n = 13;

    BacktrackingSolver backtrackingSolver;
    benchmark(&backtrackingSolver, n, "BacktrackingSolver");

    SimpleSolver simpleSolver;
    benchmark(&simpleSolver, n, "SimpleSolver");


    return 0;
}