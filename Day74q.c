#include <stdio.h>

int main()
{
    FILE *source, *destination;
    char sourceName[100], destinationName[100];
    char ch;

    printf("Enter source file name: ");
    scanf("%s", sourceName);

    printf("Enter destination file name: ");
    scanf("%s", destinationName);

    source = fopen(sourceName, "r");

    if(source == NULL)
    {
        printf("Source file could not be opened.");
        return 1;
    }

    destination = fopen(destinationName, "w");

    if(destination == NULL)
    {
        printf("Destination file could not be opened.");
        fclose(source);
        return 1;
    }
