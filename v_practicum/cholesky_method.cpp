#include<iostream>
#include <vector>

using namespace std;
using ld = long double;

// вычисление элементов матрицы по формуле
vector<vector<ld>> cholesky(vector<vector<ld>> a)
{
    int n = a.size();
    vector<vector<ld>> l(n, vector<ld>(n, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            ld sum = 0;
            for (int k = 0; k < j; k++)
                sum += l[i][k] * l[j][k];

            if (i == j)
                l[i][j] = sqrt(a[i][i] - sum);
            else
                l[i][j] = (1.0 / l[j][j] * (a[i][j] - sum));
        }
    }
    return l;
}
// нахождение L-матрицы
vector<ld> solve_L(vector<vector<ld>> l, vector<ld> b)
{
    int n = l.size();
    vector<ld> ans(n, 0);
    for (int t = 0; t < n; t++)
    {
        for (int row = t + 1; row < n; row++)
        {
            b[row] -= b[t] * l[row][t] / l[t][t];
        }
        ans[t] = b[t] / l[t][t];
    }
    return ans;
}
//нахождение U-матрицы
vector<ld> solve_U(vector<vector<ld>> u, vector<ld> b)
{
    int n = u.size();
    vector<ld> ans(n, 0);
    for (int t = n - 1; t >= 0; t--)
    {
        for (int row = t - 1; row >= 0; row--)
        {
            b[row] -= b[t] * u[t][row] / u[t][t];
        }
        ans[t] = b[t] / u[t][t];
    }
    return ans;
}

int main() {
    int n;
    cin >> n;

    vector<vector<ld>> a(n), l;
    vector<ld> b;
    //ввод матрицы
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n + 1; j++)
        {
            ld elem;
            cin >> elem;
            if (j != n)
            {
                a[i].push_back(elem);
            }
            else
            {
                b.push_back(elem);
            }
        }
    }

    l = cholesky(a);
    vector<ld> prom = solve_L(l, b);
    vector<ld> ans = solve_U(l, prom);
    // вывод решения
    for (int i = 0; i < n; i++) {
        cout << "x[" << i+1 << "] = " << ans[i] << "; " << endl;
    }
    cout << "\n";

    return 0;
}