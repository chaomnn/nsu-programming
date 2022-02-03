#include<iostream>
#include<iomanip>
#include<math.h>
#include<stdlib.h>

#define   SIZE   10

using namespace std;

int main()
{
    float a[SIZE][SIZE], x[SIZE], ratio;
    int i,j,k,n;

    /* Установка количества знаков после запятой и записи чисел в форме с фиксированной запятой */
    cout<< setprecision(3)<< fixed;

    /* Ввод */
    cout<<"N: ";
    cin>>n;

    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n+1;j++)
        {
            cout<<"a["<< i<<"]["<< j<<"]= ";
            cin>>a[i][j];
        }
    }
    /* Применение метода Гаусса, приводим матрицу к треугольному виду */
    for(i=1;i<=n-1;i++)
    {
        if(a[i][i] == 0.0)
        {
            cout<<"Error"; /* на ноль делить нельзя */
            exit(0);
        }
        for(j=i+1;j<=n;j++)
        {
            ratio = a[j][i]/a[i][i];

            for(k=1;k<=n+1;k++)
            {
                a[j][k] = a[j][k] - ratio*a[i][k];
            }
        }
    }
    /* Получение решения */
    x[n] = a[n][n+1]/a[n][n]; /* найдем последнюю неизвестную */

    for(i=n-1;i>=1;i--)
    {
        x[i] = a[i][n+1];
        for(j=i+1;j<=n;j++)
        {
            x[i] = x[i] - a[i][j]*x[j];
        }
        x[i] = x[i]/a[i][i];
    }

    /* Вывод решения */
    for(i=1;i<=n;i++)
    {
        cout<<"x["<< i<<"] = "<< x[i]<< endl;
    }

    return(0);
}