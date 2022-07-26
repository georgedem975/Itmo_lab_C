#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SpaceX 3070
#define SpaceY 3070

#pragma once

int X = SpaceX + 2;
int Y = SpaceY + 2;

const int NUMBER_OF_EPOCHS = 1000000;

typedef struct Life
{
    int space[3070][3070];
}Life;

Life gGame;

int checkRule(int i, int j)
{
    int lifeCount = 0;
    for (int k = i - 1; k < i + 2; k++)
    {
        for (int n = j - 1; n < j + 2; n++)
        {
            lifeCount = lifeCount + gGame.space[i][j];
        }
    }
    if (lifeCount == 3 && gGame.space[i][j] == 0)
    {
        gGame.space[i][j] = 1;
    }
    if ((lifeCount < 2 || lifeCount > 3) && gGame.space[i][j] == 1)
    {
        gGame.space[i][j] = 0;
    }
    return gGame.space[i][j];
}

void printSpace()
{
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (gGame.space[i][j])
            {
                printf("*");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void generateLife()
{
    for (int i = 0; i < 12; i++)
    {
        for(int j = 0; j < 12; j++)
        {
            gGame.space[i][j] = rand() % 2;
        }
    }
}

void runLife()
{
    generateLife(gGame);
    printSpace(gGame);
    system("cls");
    int i = -NUMBER_OF_EPOCHS;
    
    while(i < NUMBER_OF_EPOCHS)
    {
        for (int i = 0; i < X - 1; i++)
        {
            for (int j = 0; j < Y - 1; j++)
            {
                checkRule(i, j);
            }
        }
        
        printSpace(gGame);
        system("cls");
        i++;
    }
}

struct BITMAPHeader
{
    char name[2];
    unsigned int size;
    int garbage;
    unsigned int imageOffset;
};

struct DIBHeader
{
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short int colorPlanes;
    unsigned short int bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
};

struct RGB
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct Image
{
    int height;
    int width;
    struct RGB **rgb;
};

struct Image readImage(FILE *file, int height, int width);

int createBWImage(struct BITMAPHeader header, struct DIBHeader dibHeader, struct Image pic, char *outputFile);

int createBWImageMN(struct BITMAPHeader header, struct DIBHeader dibHeader, struct Image pic, int iteration, char* outputFile);

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Few arguments for main\n");
        exit(1);
    }

    char inputFile[100];
    strncpy(inputFile, argv[1], 100);

    char inputSecondFile[strlen(argv[1]) - 8];

    if (strcmp(inputFile, argv[1]) != 0)
    {
        printf("Coping string error...\n");
        exit(2);
    }

    for (int i = 8; i < strlen(inputFile); i++)
    {
        inputSecondFile[i - 8] = inputFile[i];
    }
    printf("%s\n", inputSecondFile);


    char outputFile[1000];
    strncpy(outputFile, argv[2], 1000);
    printf("%s \n", outputFile);

    char outputSecondFile[strlen(argv[2]) - 9];

    if(strcmp(outputFile, argv[2]) != 0)
    {
        printf("Coping string error...");
        exit(6);
    }

    for (int i = 9; i < strlen(outputFile); i++)
    {
        outputSecondFile[i - 9] = outputFile[i];
    }
    printf("%s\n", outputSecondFile);


    if (argc > 3)
    {
        if (argc == 4)
        {
            char temp[1000];
            strncpy(temp, argv[3], 1000);
            if (temp[0] == '-' && temp[1] == '-')
            {
                if (temp[2] == 'm')
                {
                    char maxI[strlen(argv[3]) - 11];

                    if(strcmp(temp, argv[3]) != 0)
                    {
                        printf("Coping string error...");
                        exit(7);
                    }

                    for (int i = 11; i < strlen(temp); i++)
                    {
                        maxI[i - 11] = temp[i];
                    }

                    printf("%d\n", (int)maxI[0] - (int)'0');

                    int maxIter = (int)maxI[0] - (int)'0';

                    FILE *fileOfTheFirstPositionInGame = fopen(inputSecondFile, "rb");

                    if (fileOfTheFirstPositionInGame == NULL)
                    {
                        printf("Error: File is empty...");
                        exit(3);
                    }

                    struct BITMAPHeader header;
                    struct DIBHeader dibHeader;

                    fread(header.name, 2, 1, fileOfTheFirstPositionInGame);
                    fread(&header.size, 3*sizeof(int), 1, fileOfTheFirstPositionInGame);
                    printf("First two characters: %c%c\n", header.name[0], header.name[1]);

                    if ((header.name[0] != 'B') || (header.name[1] != 'M'))
                    {
                        fclose(fileOfTheFirstPositionInGame);
                        printf("Error: File is not BMP");
                        exit(4);
                    }

                    printf("Size: %d\n", header.size);
                    printf("Offset:%d\n", header.imageOffset);
                    fread(&dibHeader, sizeof(struct DIBHeader), 1, fileOfTheFirstPositionInGame);
                    printf("Header size: %d\n", dibHeader.headerSize);
                    printf("Width: %d\n", dibHeader.width);
                    printf("Hight: %d\n", dibHeader.height);
                    printf("Color planes: %d\n", dibHeader.colorPlanes);
                    printf("Bits per pixel: %d\n", dibHeader.bitsPerPixel);
                    printf("Comparison: %d\n", dibHeader.compression);
                    printf("Image size: %d\n", dibHeader.imageSize);

                    if ((dibHeader.headerSize != 40) || (dibHeader.compression != 0) || (dibHeader.bitsPerPixel != 24))
                    {
                        printf("Error. Wrong format...");
                        fclose(fileOfTheFirstPositionInGame);
                        exit(5);
                    }

                    fseek(fileOfTheFirstPositionInGame, header.imageOffset, SEEK_SET);
                    struct Image image = readImage(fileOfTheFirstPositionInGame, dibHeader.height, dibHeader.width);

                    for (int i = 0; i < maxIter; i++)
                    {
                        runLife();
                        createBWImageMN(header, dibHeader, image, i, (char *) &outputSecondFile);
                    }

                    fclose(fileOfTheFirstPositionInGame);
                    return 0;
                }
                else
                {
                    char dumpF[strlen(argv[3]) - 12];

                    if(strcmp(temp, argv[3]) != 0)
                    {
                        printf("Coping string error...");
                        exit(8);
                    }

                    for (int i = 12; i < strlen(temp); i++)
                    {
                        dumpF[i - 12] = temp[i];
                    }

                    printf("%d\n", (int)dumpF[0] - (int)'0');

                    FILE *fileOfTheFirstPositionInGame = fopen(inputSecondFile, "rb");

                    if (fileOfTheFirstPositionInGame == NULL)
                    {
                        printf("Error: File is empty...");
                        exit(3);
                    }

                    struct BITMAPHeader header;
                    struct DIBHeader dibHeader;

                    fread(header.name, 2, 1, fileOfTheFirstPositionInGame);
                    fread(&header.size, 3*sizeof(int), 1, fileOfTheFirstPositionInGame);
                    printf("First two characters: %c%c\n", header.name[0], header.name[1]);

                    if ((header.name[0] != 'B') || (header.name[1] != 'M'))
                    {
                        fclose(fileOfTheFirstPositionInGame);
                        printf("Error: File is not BMP");
                        exit(4);
                    }

                    printf("Size: %d\n", header.size);
                    printf("Offset:%d\n", header.imageOffset);
                    fread(&dibHeader, sizeof(struct DIBHeader), 1, fileOfTheFirstPositionInGame);
                    printf("Header size: %d\n", dibHeader.headerSize);
                    printf("Width: %d\n", dibHeader.width);
                    printf("Height: %d\n", dibHeader.height);
                    printf("Color planes: %d\n", dibHeader.colorPlanes);
                    printf("Bits per pixel: %d\n", dibHeader.bitsPerPixel);
                    printf("Comparison: %d\n", dibHeader.compression);
                    printf("Image size: %d\n", dibHeader.imageSize);

                    if ((dibHeader.headerSize != 40) || (dibHeader.compression != 0) || (dibHeader.bitsPerPixel != 24))
                    {
                        printf("Error. Wrong format...");
                        fclose(fileOfTheFirstPositionInGame);
                        exit(5);
                    }

                    fseek(fileOfTheFirstPositionInGame, header.imageOffset, SEEK_SET);
                    struct Image image;
                    image = readImage(fileOfTheFirstPositionInGame, dibHeader.height, dibHeader.width);

                    createBWImage(header, dibHeader, image, NULL);

                    fclose(fileOfTheFirstPositionInGame);
                    return 0;
                }
            }
        }
    }


    if (argc == 3)
    {
        FILE *fileOfTheFirstPositionInGame = fopen(inputSecondFile, "rb");

        if (fileOfTheFirstPositionInGame == NULL)
        {
            printf("Error: File is empty...");
            exit(3);
        }

        struct BITMAPHeader header;
        struct DIBHeader dibHeader;

        fread(header.name, 2, 1, fileOfTheFirstPositionInGame);
        fread(&header.size, 3*sizeof(int), 1, fileOfTheFirstPositionInGame);
        printf("First two characters: %c%c\n", header.name[0], header.name[1]);

        if ((header.name[0] != 'B') || (header.name[1] != 'M'))
        {
            fclose(fileOfTheFirstPositionInGame);
            printf("Error: File is not BMP");
            exit(4);
        }

        printf("Size: %d\n", header.size);
        printf("Offset:%d\n", header.imageOffset);
        fread(&dibHeader, sizeof(struct DIBHeader), 1, fileOfTheFirstPositionInGame);
        printf("Header size: %d\n", dibHeader.headerSize);
        printf("Width: %d\n", dibHeader.width);
        printf("Hight: %d\n", dibHeader.height);
        printf("Color planes: %d\n", dibHeader.colorPlanes);
        printf("Bits per pixel: %d\n", dibHeader.bitsPerPixel);
        printf("Comparison: %d\n", dibHeader.compression);
        printf("Image size: %d\n", dibHeader.imageSize);

        if ((dibHeader.headerSize != 40) || (dibHeader.compression != 0) || (dibHeader.bitsPerPixel != 24))
        {
            printf("Error. Wrong format...");
            fclose(fileOfTheFirstPositionInGame);
            exit(5);
        }

        fseek(fileOfTheFirstPositionInGame, header.imageOffset, SEEK_SET);
        struct Image image = readImage(fileOfTheFirstPositionInGame, dibHeader.height, dibHeader.width);
        createBWImage(header, dibHeader, image, NULL);


        fclose(fileOfTheFirstPositionInGame);

        return 0;
    }
}



int createBWImageMN(struct BITMAPHeader header, struct DIBHeader dibHeader, struct Image pic, int iteration, char* outputFile)
{
    if (iteration == 1)
    {
        createBWImage(header, dibHeader, pic, outputFile);
        exit(0);
    }
    int i;
    int iter = strlen(outputFile);
    iter = iter + 9;
    printf("o\n");
    char over = iteration;
    printf("%d", over);
    char outputFileIter[iter];

    for (int i = 0; i < strlen(outputFile) + 1; i++)
    {
        if (i < strlen(outputFile))
        {
            outputFileIter[i] = '0';
        }
        else if (i == strlen(outputFile))
        {
            outputFileIter[i + 1] = over;
        }
    }
    printf("%s\n", outputFileIter);

    printf("p\n");

    for (int i = 0; i < strlen(outputFile); i++)
    {
        outputFileIter[i] = outputFile[i];
    }
    printf("%s\n", outputFileIter);
    printf("+");
    outputFileIter[iter + 1] = over;
    printf("y\n");
    outputFileIter[iter + 2] = 'n';
    outputFileIter[iter + 3] = 'e';
    outputFileIter[iter + 4] = 'w';
    outputFileIter[iter + 5] = '.';
    outputFileIter[iter + 6] = 'b';
    outputFileIter[iter + 7] = 'm';
    outputFileIter[iter + 7] = 'p';
    printf("%s\n", outputFileIter);


    FILE *fpw = fopen(outputFileIter, "wb");
    if (fpw == NULL)
        return 1;


    fwrite(header.name, 2, 1, fpw);
    fwrite(&header.size, 3*sizeof(int), 1, fpw);
    fwrite(&dibHeader, sizeof(struct DIBHeader), 1, fpw);

    for (i = pic.height - 1; i >= 0; i--)
    {

        fwrite(gGame.space, pic.width, sizeof(struct RGB), fpw);
    }

    fclose(fpw);
    return 0;

    int createBWImage(struct BITMAPHeader header, struct DIBHeader dibHeader, struct Image pic, char* outputFile)

    int i;
    int iter = strlen(outputFile);
    iter = iter + 9;
    printf("o\n");

    char outputFileIter[iter];

    printf("%s\n", outputFileIter);

    printf("p\n");

    for (i = 0; i < strlen(outputFile); i++)
    {
        outputFileIter[i] = outputFile[i];
    }

    printf("%s\n", outputFileIter);
    printf("+");
    printf("y\n");
    outputFileIter[iter + 2] = 'n';
    outputFileIter[iter + 3] = 'e';
    outputFileIter[iter + 4] = 'w';
    outputFileIter[iter + 5] = '.';
    outputFileIter[iter + 6] = 'b';
    outputFileIter[iter + 7] = 'm';
    outputFileIter[iter + 7] = 'p';
    printf("%s\n", outputFileIter);

    FILE *fpw = fopen(outputFileIter, "wb");

    if (fpw == NULL)
        return 1;

    fwrite(header.name, 2, 1, fpw);
    fwrite(&header.size, 3*sizeof(int), 1, fpw);
    fwrite(&dibHeader, sizeof(struct DIBHeader), 1, fpw);

    for (i = pic.height - 1; i >= 0; i--)
    {
        fwrite(gGame.space, pic.width, sizeof(struct RGB), fpw);
    }

    fclose(fpw);
    return 0;
}

struct Image readImage(FILE *file, int height, int width)
{
    struct Image pic;
    int i;
    pic.rgb = (struct RGB**) malloc(height * sizeof(void*));
    pic.height = height;
    pic.width = width;
    int elementCount = ((24 * width + 31) / 32) * 4;
    for (i =  height - 1; i >= 0; i--)
    {
        pic.rgb[i] = (struct RGB*) malloc(width*sizeof(struct RGB));
        fread(pic.rgb[i], 1, elementCount, file);
    }
    return pic;
}

