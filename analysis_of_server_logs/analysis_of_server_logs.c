#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int SECONDS_IN_MIN = 60;
const int SECONDS_IN_HOUR = 3600;
const int SECONDS_IN_DAY = 86400;
const int SECONDS_IN_YEAR = 31536000;
const int SECONDS_IN_V_YEAR = 31622400;

const char* MONTHS[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const int DAYS_IN_MONTHS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int determiningTheLeapYear(int year);
long long convertingMonthAndYearToSeconds(int year, char* month);

typedef struct
{
    char day[3];
    char month[4];
    char year[5];
    char hour[3];
    char minute[3];
    char second[3];
} Time;

void nullTerminator(Time* time);
long long convertingTimeToSeconds(Time* time);

int main(int argc, char* argv[])
{
    char* fileName = NULL;
    long long window;

    fileName = argv[1];
    window = atoll(argv[2]);

    FILE* fin = fopen(fileName, "r");

    long long temporarySize = 1000;
    long long countLines = 1000;
    char** temporary = (char**)malloc(temporarySize * sizeof(char*));
    long long* times = (long long*)malloc(countLines * sizeof(long long));
    char buf[1000];
    long long count = 0;
    long long auxiliaryCounter = 0;
    int flag = 0;


    while (fgets(buf, sizeof(buf), fin) != NULL)
    {
        Time time;
        int i = 0;

        while (buf[i++] != '[');

        memcpy(&time, buf+i, 20);
        nullTerminator(&time);

        if (auxiliaryCounter == countLines)
        {
            countLines *= 2;
            times = realloc(times, countLines * sizeof(long long));
        }

        times[auxiliaryCounter++] = convertingTimeToSeconds(&time);

        for (i = strlen(buf) - 1; buf[i] != ' '; i--);

        if (argc >= 4)
        {
            char storage[3];
            strcpy(storage, argv[3]);

            if (buf[i - 3] == storage[0] && storage[0] == '5' && storage[1] == 'x' && storage[2] == 'x')
            {
                if (flag == 0)
                {
                    printf("server errors!!!\n");
                    printf("Response status codes beginning with the digit 5 indicate cases in which the server is aware that it has encountered an error or is otherwise incapable of performing the request. \n");
                    printf("Except when responding to a HEAD request, the server should include an entity containing an explanation of the error situation, and indicate whether it is a temporary or permanent condition.\n");
                    printf("Likewise, user agents should display any included entity to the user. These response codes are applicable to any request method.\n");
                    flag += 1;
                    printf("server errors:\n");
                }

                int y = 0;
                int p = 0;
                while (buf[y] != ']' )
                {
                    y++;
                }

                for (y = y + 3; p == 0; y++)
                {
                    printf("%c", buf[y]);

                    if (buf[y + 1] == '"')
                    {
                        p += 1;
                    }
                }

                printf("\n");

                if (count == temporarySize)
                {
                    temporarySize *= 2;
                    temporary = realloc(temporary, temporarySize * sizeof(char*));
                }

                temporary[count] = malloc(strlen(buf)+1);
                strcpy(temporary[count++], buf);
            }
        }
    }

    long long indicator = 0;
    long long t = 0;
    long long maximumTimeInterval = 0;
    long long newTimeLine = 1;
    long long beginningOfTheInterval = 0;
    long long endOfTheInterval = 0;

    for (long long i = 1; i < auxiliaryCounter; i++)
    {
        t += times[i] - times[i - 1];
        newTimeLine++;
        while (t > window && indicator + 1 < auxiliaryCounter)
        {
            t -= times[indicator + 1] - times[indicator];
            indicator++;
            newTimeLine--;
        }
        if (newTimeLine > maximumTimeInterval)
        {
            maximumTimeInterval = newTimeLine;
            beginningOfTheInterval = indicator;
            endOfTheInterval = i;
        }
    }

    fclose(fin);
    printf("max requests in window = %lld \n", maximumTimeInterval);
    printf("%s  count: %lld\n", argv[3], count);

    if (argc == 4)
    {
        long long masOfDiogrammaInterval[10] = {0};
        long long diogrammaInterval;

        if (window % 10 == 0)
        {
            diogrammaInterval = window / 10;
        }

        else
        {
            diogrammaInterval = ((10 - (window % 10)) + window) / 10;
        }

        long long j = 0;
        long long g = 0;

        if (times[beginningOfTheInterval] != 0)
        {
            masOfDiogrammaInterval[0] += 1;
        }

        else
        {
            j += 1;
        }

        for (long long i = beginningOfTheInterval + 1; i < endOfTheInterval + 1; i++)
        {
            int checkInterval = times[i] - times[i - 1];

            if (checkInterval == 0)
            {
                masOfDiogrammaInterval[j] += 1;
                if (times[i] == times[i - 1] && g == 0)
                {
                    masOfDiogrammaInterval[j] +=1;
                    g += 1;
                }
            }

            else if (checkInterval > 0 && checkInterval < diogrammaInterval)
            {
                masOfDiogrammaInterval[j] += 1;
            }

            else if(checkInterval > diogrammaInterval)
            {
                j++;
                j+= checkInterval - diogrammaInterval;
                g = 0;
            }

            else
            {
                j += 1;
                g = 0;
            }

            if (i == endOfTheInterval && times[i] - times[i - 1] > 0)
            {
                masOfDiogrammaInterval[j] += 1;
            }

        }
        printf("\n");

        long long masOf2dDiogramm[10][10] = {0};
        int newKey = 0;

        for (int i = 0; i < 10; i++)
        {
            masOf2dDiogramm[masOfDiogrammaInterval[newKey]][i] = 1;
            newKey++;
        }

        printf(" ^\n");

        for (int i = 9; i > 0; i--)
        {
            printf("%d|", i);

            for(int k = 0; k < 10; k++)
            {
                if(masOf2dDiogramm[i][k] == 1)
                {
                    printf(" * ");
                }
                else
                {
                    printf("   ");
                }
            }

            printf("\n");
        }

        printf(" .");

        for (int i = 0; i < 10; i++)
        {
            printf("---");
        }

        printf("> \n");
        printf("  ");

        for (int i = 0; i < 10; i++)
        {
            printf(" %d ", i);
        }

    }

    return 0;
}

int determiningTheLeapYear(int year)
{
    if (year % 4 == 0 && year % 100 != 0)
    {
        return 0;
    }

    else if (year % 400 == 0)
    {
        return 0;
    }

    else
    {
        return 1;
    }
}

long long convertingMonthAndYearToSeconds(int year, char* month)
{
    long long seconds = 0;

    for (int i = 1; i < year; i++)
    {
        if (determiningTheLeapYear(i))
        {
            seconds += SECONDS_IN_V_YEAR;
        }

        else
        {
            seconds += SECONDS_IN_YEAR;
        }
    }

    int n = 0;

    while(strcmp(month, MONTHS[n]) != 0)
    {
        n++;
    }

    for (int i = 0; i < n; i++)
    {
        seconds += DAYS_IN_MONTHS[i] * SECONDS_IN_DAY;

        if (i == 1 && determiningTheLeapYear(year))
        {
            seconds += SECONDS_IN_DAY;
        }
    }

    return seconds;
}

void nullTerminator(Time* time)
{
    time -> day[2] = '\0';
    time -> month[3] = '\0';
    time -> year[4] = '\0';
    time -> hour[2] = '\0';
    time -> minute[2] = '\0';
    time -> second[2] = '\0';
}

long long convertingTimeToSeconds(Time* time)
{
    long long seconds = 0;
    seconds += atoi(time -> second);
    seconds += atoi(time -> minute) * SECONDS_IN_MIN;
    seconds += atoi(time -> hour) * SECONDS_IN_HOUR;
    seconds += (atoi(time -> day) - 1) * SECONDS_IN_DAY;
    seconds += convertingMonthAndYearToSeconds(atoi(time -> year), time -> month);
    return seconds;
}
