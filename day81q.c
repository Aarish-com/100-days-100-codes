// Pivot integer: find x with 1+2+...+x == x+(x+1)+...+n.
// Math: x(x+1)/2 = n(n+1)/2 - x(x-1)/2  =>  x^2 = n(n+1)/2.
// So x exists only if n(n+1)/2 is a perfect square.
#include <stdio.h>

int main(void) {
    unsigned long long n;
    printf("Enter n: ");
    if (scanf("%llu", &n) != 1 || n == 0) { printf("-1\n"); return 0; }

    unsigned long long target = n * (n + 1) / 2;   // x*x must equal this

    // Integer binary search for sqrt(target): avoids floating-point errors
    unsigned long long lo = 1, hi = n, x = 0;
    while (lo <= hi) {
        unsigned long long mid = lo + (hi - lo) / 2;
        unsigned long long sq = mid * mid;
        if (sq == target) { x = mid; break; }
        if (sq < target) lo = mid + 1;
        else hi = mid - 1;
    }

    if (x) printf("%llu\n", x);
    else   printf("-1\n");
    return 0;
}
