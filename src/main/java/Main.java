import java.util.List;
import java.util.Scanner;

public class Main {

    public static void runSolver(NQueensSolver solver, int n) {
        System.out.println("------------------------------------------------");
        System.out.println("Running: " + solver.getAlgorithmName() + " with N=" + n);

        long start = System.currentTimeMillis();
        List<Board> results = solver.solve(n);
        long end = System.currentTimeMillis();

        System.out.println("Time: " + (end - start) + "ms");
        System.out.println("Solutions found: " + results.size());

        if (!results.isEmpty()) {
            System.out.println("First solution preview:");
            System.out.println(results.getFirst().toString());
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("N (8, 12, 14): ");
        int n = scanner.nextInt();

        runSolver(new BasicBacktrackingSolver(), n);

        scanner.close();
    }
}