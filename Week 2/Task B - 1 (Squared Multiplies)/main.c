#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

const int HAPPY = 1;
const int SAD = 0;
const int nLenOfMaxInteger = 10 + 1;

long int Input (const char name[]);

int main()
{
    printf ("# SolveSquare v.1 (c) %s %s. by easik\n", __TIME__, __DATE__);

    int i = 1;
    long int nNumber = Input ("# Enter integer number");
    if (nNumber == SAD)
    {
        perror("ERROR");
        return 0;
    }

    int nMultiplier = sqrt (nNumber);

    while (nMultiplier > i)
    {
        if (nNumber % (int) pow (nMultiplier, 2) == 0)
        {
            printf("YES");
            return 0;
        }
        else nMultiplier = nMultiplier - 1;
    }

    printf("NO");
    return 0;
}

long int Input (const char name[])
{
    int i;
    char cBuff[nLenOfMaxInteger];
    for (i = 0; i < nLenOfMaxInteger - 1; i++) cBuff[i] = ' ';
    scanf ("%s", cBuff);

    long int nResNumber = strtol (cBuff, NULL, nLenOfMaxInteger - 1);
    if (errno == ERANGE) nResNumber = SAD;

    return nResNumber;
}
