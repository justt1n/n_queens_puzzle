import java.util.List;

public interface NQueensSolver {
    /**
     * @param n Size of the board (n x n)
     * @return A list of all possible solutions, each represented as a Board object
     */
    List<Board> solve(int n);

    default String getAlgorithmName() {
        return this.getClass().getSimpleName();
    }
}