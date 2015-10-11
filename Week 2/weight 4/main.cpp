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

/** ------------------------------ DOES NOT WORK !!! -----------------------------------**/

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
        default:

            break;
    }

    return 0;
}

long int Input (const char name[])
{
    char cBuff[nMaxElementLen] = {};

    scanf ("%s", cBuff);

    long int nNumber = strtol (cBuff, NULL, nMaxElementLen - 1);
    if (errno == ERANGE) nNumber = ANGRY;

    return nNumber;
}

int mod (long int *nNumber, long int nMaxMultiplier) {
    long int nWeight = 1;

    while (*nNumber >= 4 * nWeight) {
        nWeight *= 4;

        if (nWeight >= nMaxMultiplier) {
            if (nWeight == 1) return SAD;

            nWeight = nWeight / 4;
            *nNumber = *nNumber - nWeight;
            nMaxMultiplier = nWeight;
            printf("# w = %ld, m = %ld \n", nWeight, nMaxMultiplier);
            if (nMaxMultiplier == 1) {
                return SAD;
            }
            mod (nNumber, nMaxMultiplier);
        }
    }

    if (nWeight < nMaxMultiplier) *nNumber = *nNumber - nWeight;
    if (nWeight >= nMaxMultiplier) nMaxMultiplier = nWeight;

    if (*nNumber == 0) return HAPPY;
    if (*nNumber != 0) mod (nNumber, nMaxMultiplier);
}
