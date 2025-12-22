#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;

// =========================================================================
// GLOBAL SETTINGS
// =========================================================================
const double TIMEOUT_SECONDS = 180; // Thời gian timeout (giây) cho Benchmark
const long long MAX_STEPS_MULTIPLIER = 10;
const int SAMPLE_SIZE = 20;

// Các trường hợp test cho Benchmark
const vector<int> TEST_CASES = {8, 12, 15, 20, 29, 30, 50, 100, 1000, 1000000};

// Exception cho Timeout
class SolverTimeout : public exception {
public:
    const char* what() const noexcept override {
        return "Solver Timeout";
    }
};

class NQueensSolver {
private:
    chrono::time_point<chrono::high_resolution_clock> start_time;
    mt19937 rng; // Random number generator

    // Helper: Check timeout
    void check_timeout() {
        auto now = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = now - start_time;
        if (elapsed.count() > TIMEOUT_SECONDS) {
            throw SolverTimeout();
        }
    }

    // Helper: Random int [0, max_val]
    int random_int(int max_val) {
        uniform_int_distribution<int> dist(0, max_val);
        return dist(rng);
    }

public:
    NQueensSolver() {
        rng.seed(time(0));
    }

    // =========================================================================
    // 1. BASIC BACKTRACKING
    // =========================================================================
    bool solve_backtracking_basic(int n, vector<int>& board) {
        start_time = chrono::high_resolution_clock::now();
        board.assign(n, -1);
        try {
            return _solve_basic_util(board, 0, n);
        } catch (const SolverTimeout&) {
            throw;
        }
    }

    bool _solve_basic_util(vector<int>& board, int col, int n) {
        check_timeout();

        if (col >= n) return true;

        for (int row = 0; row < n; ++row) {
            if (_is_safe_basic(board, row, col)) {
                board[col] = row;
                if (_solve_basic_util(board, col + 1, n)) return true;
                board[col] = -1; // Backtrack
            }
        }
        return false;
    }

    bool _is_safe_basic(const vector<int>& board, int row, int col) {
        for (int i = 0; i < col; ++i) {
            if (board[i] == row || abs(board[i] - row) == abs(i - col))
                return false;
        }
        return true;
    }

    // =========================================================================
    // 2. OPTIMIZED BACKTRACKING
    // =========================================================================
    bool solve_backtracking_optimized(int n, vector<int>& board) {
        start_time = chrono::high_resolution_clock::now();
        board.assign(n, -1);

        vector<bool> cols(n, false);
        vector<bool> pos_diag(2 * n, false);
        vector<bool> neg_diag(2 * n, false);

        try {
            return _solve_optimized_util(0, n, board, cols, pos_diag, neg_diag);
        } catch (const SolverTimeout&) {
            throw;
        }
    }

    bool _solve_optimized_util(int col, int n, vector<int>& board,
                               vector<bool>& cols, vector<bool>& pos_diag, vector<bool>& neg_diag) {
        check_timeout();

        if (col == n) return true;

        for (int row = 0; row < n; ++row) {
            if (!cols[row] && !pos_diag[row + col] && !neg_diag[row - col + n - 1]) {
                board[col] = row;
                cols[row] = true;
                pos_diag[row + col] = true;
                neg_diag[row - col + n - 1] = true;

                if (_solve_optimized_util(col + 1, n, board, cols, pos_diag, neg_diag))
                    return true;

                // Backtrack
                cols[row] = false;
                pos_diag[row + col] = false;
                neg_diag[row - col + n - 1] = false;
                board[col] = -1;
            }
        }
        return false;
    }

    // =========================================================================
    // 3. MIN-CONFLICTS (Standard Local Search)
    // =========================================================================
    bool solve_min_conflicts(int n, vector<int>& board, long long max_steps = -1) {
        if (max_steps == -1) max_steps = n * MAX_STEPS_MULTIPLIER;
        start_time = chrono::high_resolution_clock::now();

        board.assign(n, -1);
        vector<int> rows_cnt(n, 0);
        vector<int> d1_cnt(2 * n, 0);
        vector<int> d2_cnt(2 * n, 0);

        // 3.1 Greedy Init
        for (int col = 0; col < n; ++col) {
            check_timeout();
            int min_conf = 1e9;
            vector<int> best_rows;

            // Duyệt hết N hàng (O(N^2) total)
            for (int row = 0; row < n; ++row) {
                int c = rows_cnt[row] + d1_cnt[row + col] + d2_cnt[row - col + n - 1];
                if (c < min_conf) {
                    min_conf = c;
                    best_rows.clear();
                    best_rows.push_back(row);
                } else if (c == min_conf) {
                    best_rows.push_back(row);
                }
            }

            int chosen_row = best_rows[random_int(best_rows.size() - 1)];
            board[col] = chosen_row;
            rows_cnt[chosen_row]++;
            d1_cnt[chosen_row + col]++;
            d2_cnt[chosen_row - col + n - 1]++;
        }

        // 3.2 Repair
        for (long long step = 0; step < max_steps; ++step) {
            if (step % 1000 == 0) check_timeout();

            vector<int> conflicted_cols;
            for (int col = 0; col < n; ++col) {
                int r = board[col];
                if (rows_cnt[r] + d1_cnt[r + col] + d2_cnt[r - col + n - 1] > 3) {
                    conflicted_cols.push_back(col);
                }
            }

            if (conflicted_cols.empty()) return true;

            int col = conflicted_cols[random_int(conflicted_cols.size() - 1)];
            int curr_row = board[col];

            // Remove old
            rows_cnt[curr_row]--;
            d1_cnt[curr_row + col]--;
            d2_cnt[curr_row - col + n - 1]--;

            // Find new
            int min_conf = 1e9;
            vector<int> best_rows;
            for (int row = 0; row < n; ++row) {
                int c = rows_cnt[row] + d1_cnt[row + col] + d2_cnt[row - col + n - 1];
                if (c < min_conf) {
                    min_conf = c;
                    best_rows.clear();
                    best_rows.push_back(row);
                } else if (c == min_conf) {
                    best_rows.push_back(row);
                }
            }

            int new_row = best_rows[random_int(best_rows.size() - 1)];
            board[col] = new_row;

            rows_cnt[new_row]++;
            d1_cnt[new_row + col]++;
            d2_cnt[new_row - col + n - 1]++;
        }

        return false;
    }

    // =========================================================================
    // 4. PERMUTATION SWAP SOLVER (Thay thế logic cũ để giải N cực lớn)
    // =========================================================================
    // Sử dụng hoán vị để đảm bảo không trùng hàng/cột, chỉ giảm xung đột chéo.
    bool solve_permutation_swap(int n, vector<int>& board, long long max_steps = 20000000) {
        start_time = chrono::high_resolution_clock::now();
        board.resize(n);

        // Mảng đếm đường chéo
        vector<int> d1(2 * n, 0); // row + col
        vector<int> d2(2 * n, 0); // row - col + n - 1

        // 1. Khởi tạo hoán vị ngẫu nhiên (0..n-1)
        for(int i = 0; i < n; ++i) board[i] = i;
        shuffle(board.begin(), board.end(), rng);

        // Tính xung đột ban đầu
        long long current_conflicts = 0;
        for(int c = 0; c < n; ++c) {
            int r = board[c];
            d1[r + c]++;
            d2[r - c + n - 1]++;
        }

        // Hàm lambda tính conflict của 1 quân hậu
        auto get_coll = [&](int r, int c) {
            return (d1[r + c] - 1) + (d2[r - c + n - 1] - 1);
        };

        for(int c = 0; c < n; ++c) current_conflicts += get_coll(board[c], c);
        current_conflicts /= 2;

        const int MAX_CANDIDATES = 50;

        // 2. Vòng lặp Swap
        for (long long step = 0; step < max_steps; ++step) {
            // Check timeout định kỳ
            if (step % 50000 == 0) check_timeout();

            if (current_conflicts == 0) return true;

            // A. Chọn quân hậu nguồn i (ngẫu nhiên)
            // Có thể tối ưu bằng cách lưu danh sách bad queens, nhưng với N lớn, random thuần cũng đủ tốt
            int i = random_int(n - 1);
            int row_i = board[i];

            // Nếu quân này không xung đột, skip với xác suất cao (để tập trung vào quân lỗi)
            if (get_coll(row_i, i) == 0 && random_int(100) > 5) continue;

            // B. Tìm quân hậu đích j để swap
            int best_j = -1;
            int max_improv = -100000;

            for (int k = 0; k < MAX_CANDIDATES; ++k) {
                int j = random_int(n - 1);
                if (i == j) continue;
                int row_j = board[j];

                // Tính delta conflict (nhanh)
                // 1. Gỡ tạm
                d1[row_i + i]--; d2[row_i - i + n - 1]--;
                d1[row_j + j]--; d2[row_j - j + n - 1]--;


                // Vì đã gỡ ra khỏi d1, d2 nên get_coll lúc này trả về số lượng quân KHÁC đang tấn công ô đó
                int conf_new_i = (d1[row_j + i]) + (d2[row_j - i + n - 1]); // Đặt row_j vào cột i
                int conf_new_j = (d1[row_i + j]) + (d2[row_i - j + n - 1]); // Đặt row_i vào cột j

                // Trả lại
                d1[row_i + i]++; d2[row_i - i + n - 1]++;
                d1[row_j + j]++; d2[row_j - j + n - 1]++;

                int current_score = get_coll(row_i, i) + get_coll(row_j, j);
                int new_score = conf_new_i + conf_new_j;

                // Hiệu chỉnh nhỏ nếu i và j tấn công nhau, nhưng với N lớn xác suất cực thấp -> Bỏ qua

                int improv = current_score - new_score;

                if (improv > max_improv) {
                    max_improv = improv;
                    best_j = j;
                }
            }

            // C. Swap
            if (best_j != -1) {
                // Chấp nhận bước đi ngang (improv=0) với xác suất nhỏ để thoát local optima
                if (max_improv < 0) continue;
                if (max_improv == 0 && random_int(10) > 0) continue;

                int j = best_j;
                int row_i = board[i];
                int row_j = board[j];

                // Update mảng đếm
                d1[row_i + i]--; d2[row_i - i + n - 1]--;
                d1[row_j + j]--; d2[row_j - j + n - 1]--;

                swap(board[i], board[j]);

                d1[row_j + i]++; d2[row_j - i + n - 1]++;
                d1[row_i + j]++; d2[row_i - j + n - 1]++;

                current_conflicts -= max_improv;

                // Đồng bộ lại current_conflicts thỉnh thoảng để tránh sai số
                if (step % 10000 == 0) {
                     long long real_conf = 0;
                     for(int c = 0; c < n; ++c) real_conf += get_coll(board[c], c);
                     current_conflicts = real_conf / 2;
                }
            }
        }
        return false;
    }
};

// =========================================================================
// RUNNER & BENCHMARK
// =========================================================================

string format_time(double seconds) {
    if (seconds < 0) return "Error";
    char buffer[50];
    sprintf(buffer, "%.4fs", seconds);
    return string(buffer);
}

void run_benchmark() {
    NQueensSolver solver;
    vector<int> board;

    cout << "Settings: Timeout=" << TIMEOUT_SECONDS << "s" << endl;
    cout << left << setw(8) << "N"
         << " | " << setw(15) << "Basic BT"
         << " | " << setw(15) << "Optimized BT"
         << " | " << setw(15) << "Min-Conflicts"
         << " | " << setw(15) << "Optimize Min-Conflicts" << endl;
    cout << string(80, '-') << endl;

    for (int n : TEST_CASES) {
        string t1, t2, t3, t4;

        // 1. Basic BT
        if (n <= 15) {
            try {
                auto start = chrono::high_resolution_clock::now();
                solver.solve_backtracking_basic(n, board);
                auto end = chrono::high_resolution_clock::now();
                t1 = format_time(chrono::duration<double>(end - start).count());
            } catch (SolverTimeout&) { t1 = "Timeout"; }
        } else t1 = "Skip (>15)";

        // 2. Optimized BT
        if (n <= 35) {
            try {
                auto start = chrono::high_resolution_clock::now();
                solver.solve_backtracking_optimized(n, board);
                auto end = chrono::high_resolution_clock::now();
                t2 = format_time(chrono::duration<double>(end - start).count());
            } catch (SolverTimeout&) { t2 = "Timeout"; }
        } else t2 = "Skip (>35)";

        // 3. Min-Conflicts (Standard)
        if (n <= 1000) { // O(N^2) init nên chậm nếu N quá lớn
            try {
                auto start = chrono::high_resolution_clock::now();
                if (solver.solve_min_conflicts(n, board)) {
                    auto end = chrono::high_resolution_clock::now();
                    t3 = format_time(chrono::duration<double>(end - start).count());
                } else t3 = "Not Found";
            } catch (SolverTimeout&) { t3 = "Timeout"; }
        } else t3 = "Skip (>1000)";

        // 4. Permutation Swap (Mode 4 - Optimized for N=1,000,000)
        try {
            auto start = chrono::high_resolution_clock::now();
            if (solver.solve_permutation_swap(n, board)) {
                auto end = chrono::high_resolution_clock::now();
                t4 = format_time(chrono::duration<double>(end - start).count());
            } else t4 = "Not Found";
        } catch (SolverTimeout&) { t4 = "Timeout"; }

        cout << left << setw(8) << n
             << " | " << setw(15) << t1
             << " | " << setw(15) << t2
             << " | " << setw(15) << t3
             << " | " << setw(15) << t4 << endl;
    }
}

int main(int argc, char* argv[]) {
    // Nếu có tham số dòng lệnh, chạy chế độ đơn lẻ
    // Ví dụ: ./nqueens 4 1000000 (Mode 4, N=1M)
    if (argc == 3) {
        int mode = atoi(argv[1]);
        int n = atoi(argv[2]);
        NQueensSolver solver;
        vector<int> board;
        bool found = false;

        auto start = chrono::high_resolution_clock::now();
        try {
            if (mode == 1) found = solver.solve_backtracking_basic(n, board);
            else if (mode == 2) found = solver.solve_backtracking_optimized(n, board);
            else if (mode == 3) found = solver.solve_min_conflicts(n, board);
            else if (mode == 4) found = solver.solve_permutation_swap(n, board);

            auto end = chrono::high_resolution_clock::now();
            double dur = chrono::duration<double>(end - start).count();

            if (found) cout << "Found solution for N=" << n << " in " << dur << "s" << endl;
            else cout << "Not found within steps." << endl;
        } catch (SolverTimeout&) {
            cout << "Timeout reached!" << endl;
        }
    } else {
        run_benchmark();
    }

    return 0;
}