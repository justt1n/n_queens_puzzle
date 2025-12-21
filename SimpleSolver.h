#pragma once
#include "NQueensSolver.h"
#include <vector>

class SimpleSolver : public NQueensSolver {
private:
    int N;
    std::vector<Board> solutions;
    
    // 3 mảng đánh dấu quan trọng
    std::vector<bool> cols;     // Kiểm soát cột
    std::vector<bool> d1;       // Kiểm soát đường chéo chính (row - col)
    std::vector<bool> d2;       // Kiểm soát đường chéo phụ (row + col)

    void solveRecursive(int row, Board& currentBoard);

public:
    std::vector<Board> solve(int n) override;
};