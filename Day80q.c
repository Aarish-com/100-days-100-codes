// Leftmost pivot index: sum of elements left of i == sum of elements right of i.
// Input:  n, then n integers.   Output: leftmost pivot index, or -1.
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("-1\n"); return 0; }

    int *a = malloc(n * sizeof *a);
    if (!a) return 1;

    long long total = 0;
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &a[i]) != 1) { free(a); return 1; }
        total += a[i];
    }

    long long left = 0;
    int pivot = -1;
    for (int i = 0; i < n; i++) {
        long long right = total - left - a[i];   // sum strictly right of i
        if (left == right) { pivot = i; break; } // first hit = leftmost
        left += a[i];
    }

    printf("%d\n", pivot);
    free(a);
    return 0;
}
