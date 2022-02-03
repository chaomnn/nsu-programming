#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define M 255

int main() {
    int common, i, j, w, h, a[M][M], b[M][M];
    //ввод
    scanf("%d %d", &w, &h);
    common = 0;
    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++) {
            scanf("%d", &a[i][j]);
            b[i][j] = a[i][j];
        }
    //заполняем матрицу b
    for (i = h - 2; i >= 0; i--)
        for (j = w - 1; j >= 0; j--) {
                b[i][j] = max(b[i][j], b[i + 1][j]);
            if (j < w - 1)
                b[i][j] = max(b[i][j], b[i + 1][j+1]);
            if (j > 0)
                b[i][j] = max(b[i][j], b[i + 1][j-1]);
    }
    //считаем общие элементы
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            if (a[i][j] == b[i][j])
                common++;
        }
    }
    //вывод
    printf("%d \n", common);
    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++) {
            printf("%d ", b[i][j]);
            if (j == w - 1)
                printf("\n");
        }
    return 0;
}