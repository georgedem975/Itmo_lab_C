#include <stdio.h>
#include <stdlib.h>

int main()
{
    char* nameFile = "";

    FILE* userFile = fopen(nameFile, "r");

    if (userFile == NULL)
    {
        perror("Error opening the source file");
        return EXIT_FAILURE;
    }

    size_t counter = 0;
    size_t symbols = 0;
    size_t words = 0;

    int ch, pre = EOF;

    while ((ch = fgetc(userFile)) != EOF)
    {
        pre = ch;
        ++symbols;

        if (ch == '\n')
        {
            ++counter;
            ++words;
        }

        if (ch == ' ')
        {
            ++words;
        }
    }

    if (pre == EOF)
    {
        puts("The file is empty!");
    }

    else if(pre != '\n')
    {
        ++counter;
    }



    char userCommand [7];

    scanf("%c %c %c", &userCommand[0], &userCommand[1], &userCommand[2], &userCommand[3], &userCommand[4], &userCommand[5], &userCommand[6]);

    int fileBytes;

    if (symbols % 8 == 0)
    {
        fileBytes = symbols / 8;
    }
    else
    {
        fileBytes = symbols / 8 + 1;
    }

    char str1 [7] = {'-','-','l','i','n','e','s'};
    char str2 [7] = {'-','-','b','y','t','e','s'};
    char str3 [7] = {'-','-','w','o','r','d','s'};

    if (userCommand[0] == str1[0] && userCommand[1] == str1[1] && userCommand[2] == str1[2])
    {
        printf( "%s %d %s", "Lines in the file", counter, "; ");
    }
    else if (userCommand[0] == str1[0] && userCommand[1] == str1[1] && userCommand[2] == str1[2] && userCommand[3] == str1[3] && userCommand[4] == str1[4] && userCommand[5] == str1[5] && userCommand[6] == str1[6])
    {
        printf( "%s %d %s", "Lines in the file", counter, "; ");
    }
    else if (userCommand[0] == str2[0] && userCommand[1] == str2[1] && userCommand[2] == str2[2])
    {
        printf("%s %d %s", "file size in bytes = ", fileBytes, "; ");
    }
    else if (userCommand[0] == str2[0] && userCommand[1] == str2[1] && userCommand[2] == str2[2] && userCommand[3] == str2[3] && userCommand[4] == str2[4] && userCommand[5] == str2[5] && userCommand[6] == str2[6])
    {
        printf("%s %d %s", "file size in bytes = ", fileBytes, "; ");
    }
    else if (userCommand[0] == str3[0] && userCommand[1] == str3[1] && userCommand[2] == str3[2])
    {
        printf("%s %d %s", "words in file", words + 1, "; ");
    }
    else if (userCommand[0] == str3[0] && userCommand[1] == str3[1] && userCommand[2] == str3[2] && userCommand[3] == str3[3] && userCommand[4] == str3[4] && userCommand[5] == str3[5] && userCommand[6] == str3[6])
    {
        printf("%s %d %s", "words in file", words + 1, "; ");
    }
    else
    {
        printf("%s","WRONG COMMAND!!!");
    }
}