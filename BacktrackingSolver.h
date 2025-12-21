#include "NQueensSolver.h"

class BacktrackingSolver : public NQueensSolver {
private:
    int N;
    std::vector<Board> solutions;

    bool isSafe(const Board& board, int row, int col);
    void solveRecursive(Board& board, int row);

public:
    std::vector<Board> solve(int n) override;
};