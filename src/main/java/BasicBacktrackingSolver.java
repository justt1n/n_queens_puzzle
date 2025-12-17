import java.util.ArrayList;
import java.util.List;

public class BasicBacktrackingSolver implements NQueensSolver {

    private List<Board> solutions;
    private Board tempBoard;
    private int N;

    @Override
    public List<Board> solve(int n) {
        this.N = n;
        this.solutions = new ArrayList<>();
        this.tempBoard = new Board(n);
        backtrack(0);
        return solutions;
    }

    private void backtrack(int row) {
        if (row == N) {
            solutions.add(tempBoard.copy());
            return;
        }

        for (int col = 0; col < N; col++) {
            if (isSafe(row, col)) {
                tempBoard.setPiece(row, col, 1); // Đặt
                backtrack(row + 1);
                tempBoard.setPiece(row, col, 0); // Gỡ (Backtrack)
            }
        }
    }

    private boolean isSafe(int row, int col) {
        for (int i = 0; i < row; i++) if (tempBoard.getPiece(i, col) == 1) return false;
        for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) if (tempBoard.getPiece(i, j) == 1) return false;
        for (int i = row, j = col; i >= 0 && j < N; i--, j++) if (tempBoard.getPiece(i, j) == 1) return false;
        return true;
    }
}