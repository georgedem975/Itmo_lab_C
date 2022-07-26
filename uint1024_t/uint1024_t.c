#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>

const int BASE_OF_UINT = 1e9;

typedef struct
{
    int32_t* data;
    size_t size;
}uint1024_t;

uint1024_t from_uint(unsigned int x);
uint1024_t add_op(uint1024_t x, uint1024_t y);
uint1024_t subtr_op(uint1024_t x, uint1024_t y);
uint1024_t mult_op(uint1024_t x, uint1024_t y);
void printf_value(uint1024_t x);
void scanf_value(uint1024_t* x);

int main()
{
    uint1024_t firstElement, secondElement, resultOfOperation;
    char buf[310];
    char operation;
    memset(buf, 0, 310);
    scanf_value(&firstElement);
    scanf_value(&secondElement);
    scanf("%c", operation);
    if (!strcmp(operation, "*"))
    {
        resultOfOperation = mult_op(firstElement, secondElement);
    }
    else if (!strcmp(operation, "-"))
    {
        resultOfOperation = subtr_op(firstElement, secondElement);
    }
    else
    {
        resultOfOperation = add_op(firstElement, secondElement);
    }
    printf_value(resultOfOperation);

    return 0;
}

uint1024_t from_uint(unsigned int x)
{
    uint1024_t  newX;
    if (x >= BASE_OF_UINT)
    {
        newX.size = 2;
        newX.data = malloc(2 * sizeof(int32_t));
        newX.data[0] = x % BASE_OF_UINT;
        newX.data[1] = x % BASE_OF_UINT;
        return newX;
    }
    else
    {
        newX.size = 1;
        newX.data = malloc(1 * sizeof(int32_t));
        newX.data[0] = x % BASE_OF_UINT;
        return newX;
    }
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t resultOfAdd_op;
    int sizeOfUint;
    if (x.size > y.size)
    {
        sizeOfUint = x.size;
    }
    else
    {
        sizeOfUint = y.size;
    }
    resultOfAdd_op.data = calloc(sizeOfUint, sizeof(int32_t));
    resultOfAdd_op.size = sizeOfUint;
    int32_t over = 0;
    for (int i = 0; i < sizeOfUint || over; i++)
    {
        if (i == sizeOfUint)
        {
            resultOfAdd_op.data = realloc(resultOfAdd_op.data, (sizeOfUint + 1) * sizeof(int32_t));
            resultOfAdd_op.data[sizeOfUint] = 0;
            resultOfAdd_op.size++;
        }
        if (i < x.size)
        {
            if (i < y. size)
            {
                resultOfAdd_op.data[i] = over + x.data[i] + y.data[i];
            }
            else
            {
                resultOfAdd_op.data[i] = over + x.data[i];
            }
        }
        else
        {
            if (i < y.size)
            {
                resultOfAdd_op.data[i] = over + y.data[i];
            }
            else
            {
                resultOfAdd_op.data[i] = over;
            }
        }
        if (resultOfAdd_op.data[i] >= BASE_OF_UINT)
        {
            over = 1;
            resultOfAdd_op.data[i] -= BASE_OF_UINT;
        }
        else
        {
            over = 0;
        }
    }
    return resultOfAdd_op;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t resultOfSutr_op;
    int sizeOfUint;
    if (x.size > y.size)
    {
        sizeOfUint = x.size;
    }
    else
    {
        sizeOfUint = y.size;
    }
    resultOfSutr_op.data = calloc(sizeOfUint, sizeof(int32_t));
    resultOfSutr_op.size = sizeOfUint;
    int over = 0;
    for (int i = 0; i < sizeOfUint || over; i++)
    {
        if (i == sizeOfUint)
        {
            break;
        }
        if (i < y.size)
        {
            resultOfSutr_op.data[i] = x.data[i] - y.data[i] - over;
        }
        else
        {
            resultOfSutr_op.data[i] = x.data[i] - over;
        }
        if (resultOfSutr_op.data[i] < 0)
        {
            over = 1;
            resultOfSutr_op.data[i] += BASE_OF_UINT;
        }
        else
        {
            over = 0;
        }
    }
    while (resultOfSutr_op.data[resultOfSutr_op.size - 1] == 0 && resultOfSutr_op.size > 1)
    {
        resultOfSutr_op.size--;
    }
    resultOfSutr_op.data = realloc(resultOfSutr_op.data, resultOfSutr_op.size * sizeof(int32_t));
    return resultOfSutr_op;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    uint1024_t resultOfMult_op;
    int sizeOfUint = x.size + y.size;
    resultOfMult_op.data = calloc(sizeOfUint, sizeof(int32_t));
    resultOfMult_op.size = sizeOfUint;
    for (int i = 0; i < x.size; i++)
    {
        for (int j = 0, over = 0; j < y.size || over; j++)
        {
            long long temporary;
            if (j < y.size)
            {
                temporary = resultOfMult_op.data[i + j] + x.data[i] * 1ll * y.data[j] + over;
            }
            else
            {
                temporary = resultOfMult_op.data[i + j] + over;
            }
            resultOfMult_op.data[i + j] = (int)(temporary % BASE_OF_UINT);
            over = (int)(temporary / BASE_OF_UINT);
        }
        while(resultOfMult_op.data[sizeOfUint - 1] == 0 && sizeOfUint > 1)
        {
            sizeOfUint--;
        }
        resultOfMult_op.data = realloc(resultOfMult_op.data, sizeOfUint * sizeof(int32_t));
        resultOfMult_op.size = sizeOfUint;
        return resultOfMult_op;
    }
}

void printf_value(uint1024_t x)
{
    int sizeOfUint = x.size;
    if (!sizeOfUint)
    {
        printf("%d", 0);
    }
    else
    {
        printf("%d", x.data[sizeOfUint - 1]);
    }
    for (int i = sizeOfUint - 2; i >= 0; --i)
    {
        printf("%d", x.data[i]);
    }
}

void scanf_value(uint1024_t* x)
{
    char string[1024];
    scanf("%s", string);
    int lengh = strlen(string);
    int size;
    if (lengh % 9 == 0)
    {
        size = lengh / 9;
    }
    else
    {
        size = lengh / 9 + 1;
    }
    uint1024_t result;
    result.data = malloc(size * sizeof(int32_t));
    result.size = size;
    for (int i = lengh, j = 0; i > 0; i -= 9, j++)
    {
        string[i] = '\0';
        if (i >= 9)
        {
            result.data[j] = atoi(string + i - 9);
        }
        else
        {
            result.data[j] = atoi(string);
        }
    }
    *x = result;
}