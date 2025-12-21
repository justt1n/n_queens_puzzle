#include <vector>

// Định nghĩa Board dùng chung
using Board = std::vector<std::vector<int>>;

class NQueensSolver {
public:
    virtual ~NQueensSolver() = default;

    virtual std::vector<Board> solve(int n) = 0;
};