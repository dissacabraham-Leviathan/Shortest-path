#include <stdio.h>
#include <stdlib.h>

#define INF 999999999

int main() {
    int N;
    if (scanf("%d", &N) != 1) return 1;

    int **fw = (int **)malloc(N * sizeof(int *));
    int **nxt = (int **)malloc(N * sizeof(int *));
    
    for (int i = 0; i < N; i++) {
        fw[i] = (int *)malloc(N * sizeof(int));
        nxt[i] = (int *)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            if (scanf("%d", &fw[i][j]) != 1) return 1;
            if (fw[i][j] != INF && i != j) {
                nxt[i][j] = j;
            } else {
                nxt[i][j] = -1;
            }
        }
    }

    int start, end;
    if (scanf("%d %d", &start, &end) != 2) return 1;

    // Floyd-Warshall Algorithm
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            if (fw[i][k] >= INF) continue;
            for (int j = 0; j < N; j++) {
                if (fw[k][j] >= INF) continue;
                if (fw[i][k] + fw[k][j] < fw[i][j]) {
                    fw[i][j] = fw[i][k] + fw[k][j];
                    nxt[i][j] = nxt[i][k];
                }
            }
        }
    }

    // Path Reconstruction
    if (fw[start][end] >= INF) {
        printf("-1\n"); // No path
    } else {
        printf("%d\n", fw[start][end]);
        if (start == end) {
            printf("1\n%d\n", start);
        } else {
            // Count path length first
            int len = 1;
            int curr = start;
            while (curr != end) {
                curr = nxt[curr][end];
                if (curr == -1) break;
                len++;
            }
            if (curr != end) {
                printf("-1\n");
            } else {
                printf("%d\n", len);
                curr = start;
                printf("%d", curr);
                while (curr != end) {
                    curr = nxt[curr][end];
                    printf(" %d", curr);
                }
                printf("\n");
            }
        }
    }

    for (int i = 0; i < N; i++) {
        free(fw[i]);
        free(nxt[i]);
    }
    free(fw);
    free(nxt);

    return 0;
}
