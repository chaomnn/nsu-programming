#include <iostream>
#include <vector>
using namespace std;

vector<double> tridiag_matrix(vector<double>& a, vector<double>& b, vector<double>& c, vector<double>& d, int n) {
    n--;
    c[0] = c[0] / b[0];
    d[0] /= b[0];

    for (int i = 1; i < n; i++) {
        c[i] /= b[i] - a[i]*c[i-1];
        d[i] = (d[i] - a[i]*d[i-1]) / (b[i] - a[i]*c[i-1]);
    }

    d[n] = (d[n] - a[n]*d[n-1]) / (b[n] - a[n]*c[n-1]);

    for (int i = n; i-- > 0;) {
        d[i] -= c[i]*d[i+1];
    }
    return d;
}

int main() {
    int n;
    cin >> n;

    vector<vector<double>> a(4);
    //ввод матрицы
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < n; j++) {
            double elem;
            cin >> elem;
                a[i].push_back(elem);
        }
    }

    vector<double> x = tridiag_matrix(a[0], a[1], a[2], a[3], n);

    // вывод решения
    for (int i = 0; i < n; i++) {
        cout << "x[" << i+1 << "] = " << x[i] << "; " << endl;
    }
    return 0;
}