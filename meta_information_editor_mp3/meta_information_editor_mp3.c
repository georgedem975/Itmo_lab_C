#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Header
{
    unsigned char markerID3v2[3];
    unsigned char versionOfHeaderID3v2[2];
    unsigned char flag;
    unsigned char lengthOfHeaderID3v2[4];
}Header;

Header gHeader;

typedef struct Frame
{
    unsigned char IDframe[4];
    unsigned char sizeOfFrame[4];
    unsigned char flag[2];
    unsigned char unicodeFlag;
}Frame;

Frame gFrame;

int conversionBytesToInt(const unsigned int lineSegment[4])
{
    return (int)lineSegment[0] << 21 | (int)lineSegment[1] << 14 | (int)lineSegment[2] << 7 | (int)lineSegment[3] << 0;
}

void conversionIntsToBytes(int x, unsigned char *lineSegment)
{
    if (gHeader.versionOfHeaderID3v2[0] == 2 || gHeader.versionOfHeaderID3v2[0] == 3)
    {
        lineSegment[3] = (unsigned char)x & 127;
        lineSegment[2] = (unsigned char)(x >> 8) & 127;
        lineSegment[1] = (unsigned char)(x >> 16) & 127;
        lineSegment[0] = (unsigned char)(x >> 24) & 127;
    }
    else
    {
        lineSegment[3] = (unsigned char)x & 127;
        lineSegment[2] = (unsigned char)(x >> 7) & 127;
        lineSegment[1] = (unsigned char)(x >> 14) & 127;
        lineSegment[0] = (unsigned char)(x >> 21) & 127;
    }
}

bool stringComparison(const char firstLineToBeCompared[], const char secondLineToBeCompared[], unsigned int length);

int DotTowrite(char *fileName, unsigned char *name, int *oldSize, bool flag)
{
    FILE *file = fopen(fileName, "rb");

    if (file == NULL)
    {
        printf("    Файл пуст или его невозможно открыть");
        exit(1);
    }

    fread(&gHeader, 1, 10, file);

    int readingPosition = 0;
    int sizeOfTag = conversionBytesToInt((const unsigned int *) gHeader.lengthOfHeaderID3v2);

    while (fread(&gFrame, 1, 11, file))
    {
        if ((ftell(file) >= sizeOfTag || gFrame.IDframe[0] == 0) && (name != NULL))
        {
            printf("   В данном файле не сущечтвует такого поля метаинформации");
            exit(1);
        }

        else if (ftell(file) >= sizeOfTag || gFrame.IDframe[0] == 0)
        {
            break;
        }

        int dotInTheLine = conversionBytesToInt((const unsigned int *) gFrame.sizeOfFrame);

        unsigned char *lineSegment = (unsigned char *)calloc(dotInTheLine, 1);
        fread(lineSegment, 1, dotInTheLine - 1, file);
        lineSegment[dotInTheLine - 1] = 0;

        if (name == NULL)
        {
            printf("%s --  ", gFrame.IDframe, dotInTheLine);
            if (gFrame.unicodeFlag)
            {
                wprintf(L"%ls\n", lineSegment);
            }
            else
            {
                printf("%s\n", lineSegment);
            }
        }
        else if (!strcmp(gFrame.IDframe, name))
        {
            if (flag == 0)
            {
                printf("%s  --  ", gFrame.IDframe);
                if (gFrame.unicodeFlag)
                {
                    wprintf(L"%ls\n", lineSegment);
                }
                else
                {
                    printf("%s\n", lineSegment);
                }
                exit(0);
            }
            else
            {
                *oldSize = dotInTheLine;
                readingPosition = ftell(file) - 10 - dotInTheLine;
                break;
            }
        }
        free(lineSegment);
    }
    fclose(file);
    return readingPosition;
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");
    unsigned char *filePath = NULL;
    unsigned char *propName = NULL;
    unsigned char *propValue = NULL;


    if (stringComparison(argv[1], "--filePath", 10))
    {
        filePath = strpbrk(argv[1], "=") + 1;
    }
    else
    {
        printf("    Некорректный ввод названия файла.....");
        exit(1);
    }

    if (argc == 3)
    {
        if (stringComparison(argv[2], "--show", 6))
        {
            printf("Вывод всей метаинформации, содержащийся в файле %s: \n", filePath);
            FILE *file = fopen(filePath, "rb");
            if (file == NULL)
            {
                printf("    Файл пуст или его невозможно открыть");
                exit(1);
            }

            fread(&gHeader, 1, 10, file);
            int sizeOfTag = conversionBytesToInt((const unsigned int *) gHeader.lengthOfHeaderID3v2);
            while (fread(&gFrame, 1, 11, file))
            {
                if (ftell(file) >= sizeOfTag || gFrame.IDframe[0] == 0)
                {
                    break;
                }
                int dotInTheLine = conversionBytesToInt((const unsigned int *) gFrame.sizeOfFrame);
                unsigned char *lineSegment = (unsigned char *) calloc(dotInTheLine, 1);
                fread(lineSegment, 1, dotInTheLine - 1, file);
                lineSegment[dotInTheLine - 1] = 0;

                printf("%s -- ", gFrame.IDframe);

                if (gFrame.unicodeFlag)
                {
                    wprintf(L"%ls\n", lineSegment);
                }
                else
                {
                    printf("%s\n", lineSegment);
                }
            }
        }
        else if (stringComparison(argv[2], "--get", 5))
        {
            printf("Вывод определенного поля метаинформации, содержащегося в файле %s:\n", filePath);
            propName = strpbrk(argv[2], "=") + 1;
            bool flag = 0;
            FILE *file = fopen(filePath, "rb");

            if (file == NULL)
            {
                printf("    Файл пуст или его невозможно открыть");
                exit(1);
            }

            fread(&gHeader, 1, 10, file);

            int sizeOfTag = conversionBytesToInt((const unsigned int *) gHeader.lengthOfHeaderID3v2);

            while (fread(&gFrame, 1, 11, file))
            {

                if (ftell(file) >= sizeOfTag || gFrame.IDframe[0] == 0)
                {
                    break;
                }

                int dotInTheLine = conversionBytesToInt((const unsigned int *) gFrame.sizeOfFrame);

                unsigned char *lineSegment = (unsigned char *)calloc(dotInTheLine, 1);
                fread(lineSegment, 1, dotInTheLine - 1, file);
                lineSegment[dotInTheLine - 1] = 0;

                if (strcmp(gFrame.IDframe, propName) == 0)
                {
                    if (flag == 0)
                    {
                        printf("%s  --  ", gFrame.IDframe);
                        if (gFrame.unicodeFlag)
                        {
                            wprintf(L"%ls\n", lineSegment);
                        }
                        else
                        {
                            printf("%s\n", lineSegment);
                        }
                        exit(0);
                    }
                }
            }
        }

        else
        {
            printf("    Неверный ввод команды");
            exit(1);
        }
    }

    if (argc == 4)
    {
        if (stringComparison(argv[2], "--set", 5) == true && stringComparison(argv[3], "--value", 7) == true)
        {
            propName = strpbrk(argv[2], "=") + 1;
            propValue = strpbrk(argv[3], "=") + 1;
            printf("Выставить значение %s в поле %s \n", propValue, propName);

            FILE *fileOld = fopen(filePath, "rb");
            if (fileOld == NULL)
            {
                printf("    Файл пуст или его невозможно открыть");
                exit(1);
            }

            FILE *fileNew = fopen("temp", "wb");

            int *sizeOld = calloc(1, sizeof(int));
            int writingPosition = DotTowrite(filePath, propName, sizeOld, 1);
            int frameMagnification = strlen(propValue) - (*sizeOld - 1);
            unsigned char sizeFrame[4];

            fread(&gHeader, 1, 10, fileOld);
            int sizeOfTheUpdatedStructure = conversionBytesToInt((const unsigned int *) gHeader.lengthOfHeaderID3v2);
            sizeOfTheUpdatedStructure += frameMagnification;

            conversionIntsToBytes(sizeOfTheUpdatedStructure, gHeader.lengthOfHeaderID3v2);
            fwrite(&gHeader, 1, 10, fileNew);

            unsigned char *lineSegment = (unsigned char *)malloc(writingPosition - 10);
            fread(lineSegment, 1, writingPosition - 10, fileOld);
            fwrite(lineSegment, 1, writingPosition - 10, fileNew);
            free(lineSegment);

            memcpy(gFrame.IDframe, propName, 4);

            int sizeTag = conversionBytesToInt((const unsigned int *) gHeader.lengthOfHeaderID3v2);
            sizeTag += frameMagnification;

            conversionIntsToBytes(sizeTag, gHeader.lengthOfHeaderID3v2);
            conversionIntsToBytes(strlen(propValue) + 1, sizeFrame);
            memcpy(gFrame.sizeOfFrame, sizeFrame, 4);

            fwrite(&gFrame, 1, 11, fileNew);
            fwrite(propValue, 1, strlen(propValue), fileNew);

            fseek(fileOld, 0, SEEK_END);

            int readOff = *sizeOld ? writingPosition + 10 + *sizeOld : writingPosition;
            int readSize = ftell(fileOld) - readOff;

            lineSegment = (unsigned char *)malloc(readSize);
            fseek(fileOld, readOff, SEEK_SET);
            fread(lineSegment, 1, readSize, fileOld);
            fwrite(lineSegment, 1, readSize, fileNew);
            free(lineSegment);

            fclose(fileOld);
            fclose(fileNew);
            remove(filePath);
            rename("temp", filePath);
        }

        else
        {
            printf("    Неверно введены команды");
            exit(1);
        }
    }
}

bool stringComparison(const char firstLineToBeCompared[], const char secondLineToBeCompared[], unsigned int length)
{
    unsigned int counter = 0;
    while (counter < length)
    {
        if (firstLineToBeCompared[counter] != secondLineToBeCompared[counter])
        {
            return false;
        }
        counter++;
    }
    return true;
}