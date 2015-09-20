#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//#define NDEBUG
#include <assert.h>
#include <math.h>

const int HAPPY = 1;
const int SAD = -1;
const int nNumOfElements = 5 + 1;
const int MaxElementLen = 6 + 1;

long int Input (const char name[]);
long int * InputDynamic (int nArraySize);
int MultipliersOfInteger (long int * ptrArrayofIntegers);

int main()
{
    printf ("# SolveSquare v.1 (c) %s %s. by easik\n", __TIME__, __DATE__);

    int i, nCount = 0;
    int nBool = -1;

    long int nArraySize = Input ("nArraySize");
    if (nArraySize == SAD)
    {
        perror ("ERROR");
        return 0;
    }

    long int * ptrArrayofIntegers = InputDynamic (nArraySize);
    if (ptrArrayofIntegers == NULL)
    {
        perror ("ERROR");

        free(ptrArrayofIntegers);
        ptrArrayofIntegers = NULL;

        return 0;
    }

    for (i = 0; i < nArraySize; i++)
    {
        nBool = MultipliersOfInteger (ptrArrayofIntegers + i);
        assert (nBool != -1);

        if (nCount != 0)
        {
            if (nBool == HAPPY) printf (" %ld", *(ptrArrayofIntegers + i));
        }
        else if (nCount == 0)
        {
            if (nBool == HAPPY)
            {
                printf ("%ld", *(ptrArrayofIntegers + i));
                nCount ++;
            }
        }
        nBool = 0;
    }

    free(ptrArrayofIntegers);
    ptrArrayofIntegers = NULL;

    return 0;
}

long int Input (const char name[])
{
    char cBuff[nNumOfElements];
    int i;
    for (i = 0; i < nNumOfElements - 1; i++) cBuff[i] = ' ';
    scanf ("%s", cBuff);

    long int nArraySize = strtol (cBuff, NULL, nNumOfElements - 1);
    if (errno == ERANGE) nArraySize = SAD;

    return nArraySize;
}

long int * InputDynamic (int nArraySize)
{
    long int * nArrayOfIntegers = (long int *) calloc (nArraySize, sizeof (*nArrayOfIntegers));
    if (nArrayOfIntegers == NULL) return NULL;

    int i;

    for (i = 0; i < nArraySize; i++)
    {
        if (!(scanf ("%ld", nArrayOfIntegers + i)))
        {
            free(nArrayOfIntegers);
            nArrayOfIntegers = NULL;
            return NULL;
        }
    }

    return nArrayOfIntegers;
}

int MultipliersOfInteger (long int * ptrArrayofIntegers)
{
    long int Num = *ptrArrayofIntegers;
    assert (Num >= 0);
    long int nMultiplier = sqrt (Num) + 1;
    if (nMultiplier >= Num) nMultiplier = Num - 1;

    while (nMultiplier > 1)
    {
        if ((Num % nMultiplier) == 0) return 0;
        nMultiplier = nMultiplier - 1;
    }

    return HAPPY;
}
