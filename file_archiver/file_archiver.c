#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    argc = 6;

    char* tempFileWithDataFile = "";


    if (argc < 4)
    {
        exit(1);
    }

    if ((strcmp(argv[1], "--FILE") != 0))
    {
        exit(2);
    }


    char* command = argv[3];

    if (!(strcmp(command, "--create")))
    {
        int counterNamesOfFiles = argc - 4;
        for (int i = 0; i < counterNamesOfFiles; i++)
        {
            char masOfData;
            FILE* fileFinInCreate = fopen(argv[i + 4], "rb");
            char *newNameOfArchFile = malloc(strlen(argv[2]) + 1 + 1);
            strcpy(newNameOfArchFile, argv[2]);
            
            if (strstr(argv[i + 4], "txt"))
            {
                switch (i)
                {
                    case 0:
                        strcat(newNameOfArchFile, ".txt");
                        break;
                    case 1:
                        strcat(newNameOfArchFile, ".txt");
                        break;
                    case 2:
                        strcat(newNameOfArchFile, ".txt");
                        break;
                    case 3:
                        strcat(newNameOfArchFile, ".txt");
                        break;
                    case 4:
                        strcat(newNameOfArchFile, ".txt");
                        break;
                }
            }

            if (strstr(argv[i + 4], "bmp"))
            {
                switch (i)
                {
                    case 0:
                        strcat(newNameOfArchFile, ".bmp");
                        break;
                    case 1:
                        strcat(newNameOfArchFile, ".bmp");
                        break;
                    case 2:
                        strcat(newNameOfArchFile, ".bmp");
                        break;
                    case 3:
                        strcat(newNameOfArchFile, ".bmp");
                        break;
                    case 4:
                        strcat(newNameOfArchFile, ".bmp");
                        break;
                }
            }
            
            FILE* fileFoutInCreate = fopen(newNameOfArchFile, "wb");

            while (!feof(fileFinInCreate))
            {
                fread(&masOfData, 1, sizeof(masOfData), fileFinInCreate);
                fwrite(&masOfData, 1, sizeof(masOfData), fileFoutInCreate);
            }
            
            fwrite(&masOfData, 1, sizeof(masOfData), fileFoutInCreate);

            fclose(fileFinInCreate);
            fclose(fileFoutInCreate);

            FILE* TempFile = fopen(tempFileWithDataFile, "a");
            fprintf(TempFile, "%s", newNameOfArchFile);
            fprintf(TempFile, "%c", '\n');
            fclose(TempFile);
        }
    }

    else if (!(strcmp(command, "--extract")))
    {
        int counterNamesOfFiles = argc - 4;
        for (int i = 0; i < counterNamesOfFiles; i++)
        {
            char masOfData;
            FILE* fileFinInCreate = fopen(argv[i + 4], "rb");
            char *newNameOfArchFile = malloc(strlen(argv[2]) + 1 + 1);
            strcpy(newNameOfArchFile, argv[2]);
            
            if (strstr(argv[i + 4], "txt"))
            {
                strcat(newNameOfArchFile, ".txt");
            }

            if (strstr(argv[i + 4], "bmp"))
            {
                strcat(newNameOfArchFile, ".bmp");
            }

            FILE* fileFoutInCreate = fopen(newNameOfArchFile, "wb");

            while (!feof(fileFinInCreate))
            {
                fread(&masOfData, 1, sizeof(masOfData), fileFinInCreate);
                fwrite(&masOfData, 1, sizeof(masOfData), fileFoutInCreate);
            }

            fwrite(&masOfData, 1, sizeof(masOfData), fileFoutInCreate);

            fclose(fileFinInCreate);
            fclose(fileFoutInCreate);

            FILE* temp = fopen(tempFileWithDataFile, "a");
            fprintf(temp,"%s", "(decompresed)");
            fprintf(temp, "%s", argv[i + 4]);
            fprintf(temp, "%c", '\n');
            fclose(temp);
        }
    }

    else if (!(strcmp(command, "--list")))
    {
        FILE* tempFile = fopen(tempFileWithDataFile, "r");
        char* temp;
        
        while (!feof(tempFile))
        {
            fread(&temp, 1, 1, tempFile);
            printf("%s",  temp);
        }
        
        fclose(tempFile);
    }

    else
    {
        exit(2);
    }
    
    return 0;
}
