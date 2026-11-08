// Find the repeated element in ONE pass using an open-addressing hash set.
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) return 1;
    unsigned size = 2;
    while (size < (unsigned)n * 2) size <<= 1;
    int *key = malloc(size * sizeof *key);
    unsigned char *used = calloc(size, 1);
    for (int i = 0; i < n; i++) {
        int x;
        if (scanf("%d", &x) != 1) return 1;
        unsigned h = ((unsigned)x * 2654435761u) & (size - 1);
        while (used[h]) {
            if (key[h] == x) { printf("%d\n", x); return 0; }   // seen before -> repeated
            h = (h + 1) & (size - 1);
        }
        used[h] = 1; key[h] = x;
    }
    printf("-1\n");
    return 0;
}
