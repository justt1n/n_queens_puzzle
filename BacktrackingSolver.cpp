// BacktrackingSolver.cpp
#include "BacktrackingSolver.h"

using namespace std;

// Định nghĩa hàm isSafe
bool BacktrackingSolver::isSafe(const Board& board, int row, int col) {
    // 1. Kiểm tra cột dọc phía trên
    for (int i = 0; i < row; i++)
        if (board[i][col] == 1) return false;
    // 2. Kiểm tra đường chéo trên bên trái
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j] == 1) return false;
    // 3. Kiểm tra đường chéo trên bên phải
    for (int i = row, j = col; i >= 0 && j < N; i--, j++)
        if (board[i][j] == 1) return false;
    return true;
}

void BacktrackingSolver::solveRecursive(Board& board, int row) {
    if (row == N) {
        solutions.push_back(board);
        return;
    }
    for (int col = 0; col < N; col++) {
        if (isSafe(board, row, col)) {
            board[row][col] = 1;
            solveRecursive(board, row + 1);
            board[row][col] = 0;
        }
    }
}

vector<Board> BacktrackingSolver::solve(int n) {
    this->N = n;
    this->solutions.clear();

    Board board(n, vector<int>(n, 0));
    solveRecursive(board, 0);

    return solutions;
}