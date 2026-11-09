// Print initials of a name, with the surname (last word) shown in full.
// "mohandas karamchand gandhi" -> "M. K. Gandhi"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    char name[200];
    printf("Enter full name: ");
    if (!fgets(name, sizeof name, stdin)) return 1;
    name[strcspn(name, "\n")] = '\0';

    // Trim trailing spaces so the last word is really the surname
    size_t len = strlen(name);
    while (len > 0 && isspace((unsigned char)name[len - 1])) name[--len] = '\0';

    // Find where the surname starts (just after the last space)
    char *last_space = strrchr(name, ' ');
    char *surname = last_space ? last_space + 1 : name;

    // Initials for every word before the surname
    for (char *p = name; p < surname; p++) {
        if (isalpha((unsigned char)*p) && (p == name || *(p - 1) == ' '))
            printf("%c. ", toupper((unsigned char)*p));
    }

    // Surname: first letter capital, rest lowercase
    printf("%c", toupper((unsigned char)surname[0]));
    for (char *p = surname + 1; *p; p++) putchar(tolower((unsigned char)*p));
    putchar('\n');
    return 0;
}
