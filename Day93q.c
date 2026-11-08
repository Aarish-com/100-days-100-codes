// Read info.txt line by line with fgets() until EOF.
#include <stdio.h>
int main(void) {
    FILE *f = fopen("info.txt", "r");
    if (!f) { printf("Error: could not open info.txt\n"); return 1; }
    char line[256];
    while (fgets(line, sizeof line, f)) printf("%s", line);
    fclose(f);
    return 0;
}
