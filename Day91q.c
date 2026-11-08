// Merge two sorted arrays. Input: m, m ints, n, n ints.
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int m, n;
    if (scanf("%d", &m) != 1 || m < 0) return 1;
    int *a = malloc((m + 1) * sizeof *a);
    for (int i = 0; i < m; i++) if (scanf("%d", &a[i]) != 1) return 1;
    if (scanf("%d", &n) != 1 || n < 0) return 1;
    int *b = malloc((n + 1) * sizeof *b);
    for (int i = 0; i < n; i++) if (scanf("%d", &b[i]) != 1) return 1;
    int i = 0, j = 0, first = 1;
    while (i < m || j < n) {
        int v = (j >= n || (i < m && a[i] <= b[j])) ? a[i++] : b[j++];
        printf(first ? "%d" : " %d", v);
        first = 0;
    }
    printf("\n");
    free(a); free(b);
    return 0;
}
