 Maximum contiguous subarray sum (Kadane). Works when all elements are negative.
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) return 1;
    long long x, cur, best;
    if (scanf("%lld", &x) != 1) return 1;
    cur = best = x;
    for (int i = 1; i < n; i++) {
        if (scanf("%lld", &x) != 1) return 1;
        cur = (x > cur + x) ? x : cur + x;
        if (cur > best) best = cur;
    }
    printf("%lld\n", best);
    return 0;
}
 
