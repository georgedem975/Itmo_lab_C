#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        FILE *f;
        f = fopen(argv[1], "r");

        if (f == NULL)
        {
            perror("Error opening the source file");
            return EXIT_FAILURE;
        }

        size_t counter = 0;
        size_t symbols = 0;
        size_t words = 0;

        int ch, pre = EOF;

        while ((ch = fgetc(f)) != EOF)
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

        else if (pre != '\n')
        {
            ++counter;
        }

        if ((strcmp(argv[2], "--l") == 0) || (strcmp(argv[2], "--lines") == 0))
        {
            printf("%d", counter);
        }
        if ((strcmp(argv[2], "--w") == 0) || (strcmp(argv[2], "--words") == 0))
        {
            printf("%d", words + 1);
        }
        if ((strcmp(argv[2], "--b") == 0) || (strcmp(argv[2], "--bytes") == 0))
        {
            printf("%d", sizeof(argv[1]));
        }
    }

    if (argc != 3)
    {
        printf("%s", "Wrong operation");
        return 1;
    }
}