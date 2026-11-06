// Check whether two lowercase strings are anagrams (same letter frequencies).
#include <stdio.h>
#include <string.h>
int main(void) {
    static char s[100005], t[100005];
    if (scanf("%100000s %100000s", s, t) != 2) return 1;
    int cnt[256] = {0};
    if (strlen(s) != strlen(t)) { printf("Not Anagram\n"); return 0; }
    for (int i = 0; s[i]; i++) { cnt[(unsigned char)s[i]]++; cnt[(unsigned char)t[i]]--; }
    for (int i = 0; i < 256; i++) if (cnt[i]) { printf("Not Anagram\n"); return 0; }
    printf("Anagram\n");
    return 0;
}
