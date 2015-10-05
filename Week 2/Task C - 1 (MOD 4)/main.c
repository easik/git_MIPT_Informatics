#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

const int HAPPY = 1;
const int SAD = 0;
const int ANGRY = -1;
const int nMaxElementLen = 10 + 1;

long int Input (const char name[]);
int mod (long int *nNumber, long int nMaxMultiplier);

/* ---------------------------------------------------------------------------------------------- */

/** ---------------------------- DOES NOT WORKING !!! ----------------------------------**/

/* -----------------------------------------------------------------------------------------------*/


int main()
{
    long int nNumber = Input ("Number");

    if (nNumber == ANGRY)
    {
        perror("ERROR");
        return 0;
    }

    switch (mod (&nNumber, nNumber))
    {
        case (1):
            printf("YES");
            break;
        case (0):
            printf("NO");
            break;
    }

    return 0;
}

long int Input (const char name[])
{
    int i;
    char cBuff[nMaxElementLen];

    for (i = 0; i < nMaxElementLen - 1; i++) cBuff[i] = ' ';
    scanf ("%s", cBuff);

    long int nNumber = strtol (cBuff, NULL, nMaxElementLen - 1);
    if (errno == ERANGE) nNumber = ANGRY;

    return nNumber;
}

int mod (long int *nNumber, long int nMaxMultiplier)
{
    long int nWeight = 1;

    if (*nNumber < nWeight) return SAD;

    printf("# %ld \n", *nNumber);
    while (*nNumber > 4 * nWeight)
    {
        nWeight = nWeight * 4;

        if (nWeight >= nMaxMultiplier)
        {
            printf("# n = %ld, w = %ld, m = %ld \n", *nNumber, nWeight, nMaxMultiplier); //developer mode
            if (nWeight == 1) return SAD;

            nWeight = nWeight / 4;
            *nNumber = *nNumber - nWeight;
            nMaxMultiplier = nWeight;
            printf("# w = %ld, m = %ld \n", nWeight, nMaxMultiplier);
            if (nMaxMultiplier == 1) {
                //printf("# here!\n");
                return SAD; //doesn't exit to main (input nNumber = 58). Why?
                printf("# metka\n"); // but doesn't show this printf.
            }
            mod (nNumber, nMaxMultiplier);
        }
    }

    *nNumber = *nNumber - nWeight;
    nMaxMultiplier = nWeight;

    if (*nNumber == 0) return HAPPY;

    mod (nNumber, nMaxMultiplier);
    return 0;
}
