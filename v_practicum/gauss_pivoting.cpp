#include<iostream>
#include<iomanip>
#include<math.h>
#include<stdlib.h>

#define   SIZE   10

using namespace std;

int main()
{
    float a[SIZE][SIZE], b[SIZE][SIZE] = {0}, x[SIZE], c[SIZE], ratio, pivot,  det = 1;
    int i,j,k,n,l, p_row, m;

    /* Установка количества знаков после запятой и записи чисел в форме с фиксированной запятой */
    cout<< setprecision(3)<< fixed;

    /* Ввод */
    cout<<"N: ";
    cin>>n;

    for(i=1;i<=n;i++) {
        for(j=1;j<=n+1;j++) {
            cout<<"a["<< i<<"]["<< j<<"]= ";
            cin>>a[i][j];
        }
        b[i][i] = 1;
    }
    /* Применение метода Гаусса */
    for(i=1;i<=n-1;i++) {
        pivot = 0;
        for (m = i; m<= n; m++) { /*находим главный элемент в столбце*/
            if (pivot < abs(a[m][i])) {
                pivot = a[m][i];
                p_row = m;
            }
        }
        for(l=1;l<=n+1;l++) {
            swap(a[i][l], a[p_row][l]); /* меняем строки */
            swap(b[i][l], b[p_row][l]); /* и меняем строки в матрице перестановки */
        }
        for(j=i+1;j<=n;j++) { /* линейные преобразования */
            ratio = a[j][i]/pivot;
            for(k=1;k<=n+1;k++) {
                a[j][k] -= ratio*a[i][k];
                b[j][k] -= ratio*b[i][k];
            }
        }
    }

    /* Получение решения */
    for(k=n;k>=1;k--) {
        x[k] = a[k][n+1];
        for(j=k+1;j<=n;j++) {
            x[k] = x[k] - a[k][j]*x[j];
        }
        x[k] = x[k]/a[k][k];
    }

    /* Вывод решения */
    for(i=1;i<=n;i++) {
        det *= a[i][i]; /* подсчет определителя */
        cout<<"x["<< i<<"] = "<< x[i] << " " << endl;
    }

    /* линейные преобразования */
    for(i=1;i<=n;i++) {
        c[i] = a[i][i];
        for (j = 1; j <= n; j++) {
            if (i != j) {
                ratio = a[j][i] / a[i][i];
                for (k = 1; k <= n; k++) {
                    a[j][k] -= ratio * a[i][k];
                    b[j][k] -= ratio * b[i][k];
               }
            }
        }
    }

    /* находим обратную матрицу b элементарными преобразованиями */
    for (i=1;i<=n;i++) {
        for (j=1;j<=n;j++) {
            b[i][j] /= c[i];
            a[i][j] /= a[i][i];
        }
    }

    cout<<"det = "<< det<< endl; /* вывод определителя */

    for(i=1;i<=n;i++) { /* вывод обратной матрицы */
        for (j=1;j<=n;j++) {
            cout<<"b["<< i <<"]["<< j <<"] = "<<b[i][j]<<" ";
        }
        cout<<endl;
    }

    return(0);
}
