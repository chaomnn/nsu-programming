#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define INF 9999
#define M 100

int main() {
    int k, vert, edges, i, j, a, b, v[M];
    int Graph[M][M], start = 0, cost[M][M], distance[M], pred[M], visited[M], count, mindistance, nextnode;

    scanf("%d %d %d\n", &k, &vert, &edges);

    for (i = 0; i < vert; i++) {
        for (j = 0; j < vert; j++) {
            Graph[i][j] = 0;
        }
    }

    for (i = 0; i < edges; i++) {
        scanf("%d %d ", &a, &b);
        scanf("%d", &Graph[a][b]);
    }

    for (i = 0; i < vert; i++)
        for (j = 0; j < vert; j++)
            if (Graph[i][j] == 0)
                cost[i][j] = INF;
            else
                cost[i][j] = Graph[i][j];

    for (i = 0; i < vert; i++) {
        distance[i] = cost[start][i];
        pred[i] = start;
        visited[i] = 0;
    }

    distance[start] = 0;
    visited[start] = 1;
    count = 1;
    v[start] = 0; a = 1;

    while (count < vert - 1) {
        mindistance = INF;

        for (i = 0; i < vert; i++)
            if (distance[i] < mindistance && !visited[i]) {
                mindistance = distance[i];
                nextnode = i;
            }

        visited[nextnode] = 1;
        for (i = 0; i < vert; i++)
            if (!visited[i])
                if (mindistance + cost[nextnode][i] < distance[i]) {
                    distance[i] = mindistance + cost[nextnode][i];
                    pred[i] = nextnode;
                    if (v[a-1] != 0) {
                        v[a-1] = nextnode;
                    }
                    else v[a] = nextnode;
                    a++;
                }
        count++;
    }

    printf("%d \n", distance[k]);
    for (i = 0; i < vert; i++) {
        if ((v[i] > -1) && (v[i] != v[i-1]))
            printf("%d ", v[i]);
    }
    printf("%d", k);
    return 0;
}
