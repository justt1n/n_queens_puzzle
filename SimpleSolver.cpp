//
// Created by justt1n on 21/12/2025.
//

#include "SimpleSolver.h"

using namespace std;

void SimpleSolver::solveRecursive(int row, Board& currentBoard) {
    if (row == N) {
        solutions.push_back(currentBoard);
        return;
    }

    for (int col = 0; col < N; col++) {
        int id1 = row - col + N - 1;
        int id2 = row + col;

        if (cols[col] || d1[id1] || d2[id2]) {
            continue;
        }

        cols[col] = true;
        d1[id1] = true;
        d2[id2] = true;
        currentBoard[row][col] = 1;

        solveRecursive(row + 1, currentBoard);

        cols[col] = false;
        d1[id1] = false;
        d2[id2] = false;
        currentBoard[row][col] = 0;
    }
}

vector<Board> SimpleSolver::solve(int n) {
    this->N = n;
    this->solutions.clear();

    cols.assign(n, false);
    d1.assign(2 * n, false);
    d2.assign(2 * n, false);

    Board board(n, vector<int>(n, 0));
    solveRecursive(0, board);

    return solutions;
}