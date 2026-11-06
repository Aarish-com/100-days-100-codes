/ Length of the longest substring without repeating characters (sliding window).
#include <stdio.h>
#include <string.h>
int main(void) {
    static char s[100005];
    if (!fgets(s, sizeof s, stdin)) { printf("0\n"); return 0; }
    s[strcspn(s, "\r\n")] = '\0';
    int last[256], start = 0, best = 0;
    for (int i = 0; i < 256; i++) last[i] = -1;
    for (int i = 0; s[i]; i++) {
        unsigned char c = (unsigned char)s[i];
        if (last[c] >= start) start = last[c] + 1;           // jump past previous occurrence
        last[c] = i;
        if (i - start + 1 > best) best = i - start + 1;
    }
    printf("%d\n", best);
    return 0;
}
 
