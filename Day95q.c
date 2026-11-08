// Array of size n holds 0..n except one number. Missing = n(n+1)/2 - sum.
#include <stdio.h>
int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n < 0) return 1;
    long long sum = 0, x;
    for (int i = 0; i < n; i++) { if (scanf("%lld", &x) != 1) return 1; sum += x; }
    printf("%lld\n", (long long)n * (n + 1) / 2 - sum);
    return 0;
}
