// First negative integer in every window of size k (0 if none). O(n).
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int n, k;
    if (scanf("%d", &n) != 1 || n <= 0) return 1;
    int *a = malloc(n * sizeof *a), *q = malloc(n * sizeof *q);
    for (int i = 0; i < n; i++) if (scanf("%d", &a[i]) != 1) return 1;
    if (scanf("%d", &k) != 1 || k <= 0 || k > n) return 1;
    int h = 0, t = 0, first = 1;
    for (int i = 0; i < n; i++) {
        if (a[i] < 0) q[t++] = i;                            // queue of negative indices
        if (i >= k - 1) {
            while (h < t && q[h] <= i - k) h++;              // expired indices
            printf(first ? "%d" : " %d", h < t ? a[q[h]] : 0);
            first = 0;
        }
    }
    printf("\n");
    free(a); free(q);
    return 0;
}
 
