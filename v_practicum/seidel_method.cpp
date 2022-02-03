#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Условие окончания //
bool end_condition(vector<double> xk, vector<double> xkp, int n, double eps) {
    double norm = 0;
    for (int i = 0; i < n; i++)
        norm += (xk[i] - xkp[i]) * (xk[i] - xkp[i]);
    return (sqrt(norm) < eps);
}

double solve(double x, double eps) {
    int i = 0;
    while (eps < 1) {
        i++;
        eps *= 10;
    }
    double solution = pow(double(10), i);
    solution = (x * solution + 0.5) /solution;
    return solution;
}

int main() {
    int n, num, iterations_num = 0;
    cin >> n;
    vector<double> row, vect_b, solution, prev_solution;
    vector<vector<double>> matrix;
    double eps;
    cin >> eps;
    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < n; j++) {
            cin >> num;
            row.push_back(num);
        }
        matrix.push_back(row);
        row.clear();
    }

    for (auto i = 0; i < n; i++) {
        cin >> num;
        vect_b.push_back(num);
    }

    /* в массив solution[n] следует поместить начальное
    приближение столбца решений (например, все нули)
    */

    for (auto i = 0; i < n; i++) {
        solution.push_back(0);
    }

    while (true) {
        prev_solution.clear();
        for (int i = 0; i < n; i++) {
            prev_solution.push_back(solution[i]);
        }
        for (int i = 0; i < n; i++) {
            double var = 0;
            for (int j = 0; j < n; j++) {
                if (j!=i) {
                    var += (matrix[i][j] * solution[j]);
                }
            }
            solution[i] = (vect_b[i] - var) / matrix[i][i];
        }
        iterations_num++;
        if (end_condition(solution, prev_solution, n, eps)) {
            break;
        }
    }

    for (auto i = 0; i < n; i++) {
        cout << "x[" << i << "] = " << solve(solution[i], eps) << endl;
    }
    cout << "Iterations: " << iterations_num << endl;

    return 0;
}
