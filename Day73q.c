#include <stdio.h>

int main()
{
    FILE *fp;
    char ch;

    int characters = 0;
    int words = 0;
    int lines = 0;
    int inWord = 0;

    fp = fopen("info.txt", "r");

    if(fp == NULL)
    {
        printf("File could not be opened.");
        return 1;
    }

    while((ch = fgetc(fp)) != EOF)
    {
        characters++;

        if(ch == '\n')
        {
            lines++;
        }

        if(ch == ' ' || ch == '\n' || ch == '\t')
        {
            inWord = 0;
        }
        else if(inWord == 0)
        {
            words++;
            inWord = 1;
        }
    }

    fclose(fp);

    printf("Total characters = %d\n", characters);
    printf("Total words = %d\n", words);
    printf("Total lines = %d\n", lines);

    return 0;
}
