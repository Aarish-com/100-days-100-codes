// Write the user's name and age to info.txt using fprintf().
#include <stdio.h>
#include <string.h>
int main(void) {
    char name[100]; int age;
    printf("Enter name: ");
    if (!fgets(name, sizeof name, stdin)) return 1;
    name[strcspn(name, "\r\n")] = '\0';
    printf("Enter age: ");
    if (scanf("%d", &age) != 1) return 1;
    FILE *f = fopen("info.txt", "w");
    if (!f) { perror("Error"); return 1; }
    fprintf(f, "Name: %s\nAge: %d\n", name, age);
    fclose(f);
    printf("Data successfully saved to info.txt\n");
    return 0;
}
