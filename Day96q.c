// Append a new line of text to an existing file without overwriting it.
#include <stdio.h>
#include <string.h>
int main(void) {
    char name[256], text[1024];
    if (scanf("%255s", name) != 1) return 1;
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}         // discard rest of the filename line
    if (!fgets(text, sizeof text, stdin)) return 1;
    text[strcspn(text, "\r\n")] = '\0';
    FILE *f = fopen(name, "a");
    if (!f) { printf("Error: could not open %s\n", name); return 1; }
    fprintf(f, "%s\n", text);
    fclose(f);
    printf("Text appended successfully\n");
    return 0;
}
