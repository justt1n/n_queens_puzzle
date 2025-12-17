public class Board {
    private final int N;
    private final int[][] cells;

    public Board(int n) {
        this.N = n;
        this.cells = new int[n][n];
    }

    public Board(int n, int[] queens) {
        this.N = n;
        this.cells = new int[n][n];
        for (int r = 0; r < n; r++) {
            this.cells[r][queens[r]] = 1; // Đặt hậu
        }
    }

    public Board(Board other) {
        this.N = other.N;
        this.cells = new int[N][N];
        for (int i = 0; i < N; i++) {
            this.cells[i] = other.cells[i].clone();
        }
    }

    public int getN() { return N; }

    public void setPiece(int r, int c, int val) { cells[r][c] = val; }
    public int getPiece(int r, int c) { return cells[r][c]; }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int[] row : cells) {
            sb.append("  ");
            for (int val : row) {
                sb.append(val == 1 ? "Q " : ". ");
            }
            sb.append("\n");
        }
        return sb.toString();
    }

    public Board copy() { return new Board(this); }
}