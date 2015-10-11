#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define TRUE    1
#define FALSE   0
#define SUCCESS    1
#define FAIL   -1

const int nMaxElementLen = 10 + 1; // len(2^31) + 1 ('1' for '\0' element)

long int Input(const char name[]);
int mod(long int nNumber);

int main() {
    long int nNumber = Input("Number");

    if (nNumber == FAIL) {
        perror("ERROR");
        return 0;
    }

    switch (mod(nNumber)) {
        case TRUE:
            printf("YES");
            break;
        case FALSE:
            printf("NO");
            break;
        default:
            printf("ERROR");
    }

    return 0;
}

long int Input(const char name[]) {
    int i;
    char cBuff[nMaxElementLen];

    for (i = 0; i < nMaxElementLen - 1; i++) cBuff[i] = ' ';
    scanf("%s", cBuff);

    long int nNumber = strtol(cBuff, NULL, nMaxElementLen - 1);
    if (errno == ERANGE) nNumber = FAIL;

    return nNumber;
}

int mod(long int nNumber) {
    if ((nNumber + 2) % 4 == 0) return FALSE;
    else if ((nNumber + 1) % 4 == 0) nNumber = (nNumber + 1) / 4;
    else if ((nNumber + 0) % 4 == 0) nNumber = (nNumber + 0) / 4;
    else if ((nNumber - 1) % 4 == 0) nNumber = (nNumber - 1) / 4;
/*
    if ((nNumber + 1) % 4 == 0) {
        nNumber = (nNumber + 1) / 4;
    } else if ((nNumber + 0) % 4 == 0) {
        nNumber = (nNumber + 0) / 4;
    } else if ((nNumber - 1) % 4 == 0) {
        nNumber = (nNumber - 1) / 4;
    }
*/
/*
    for (i = -1; i <= 1; i++) {
        if ((nNumber + i) % 4 == 0) {
            nNumber = (nNumber + i) / 4;
            break;
        }
    }
*/
    if (nNumber < 1) return FALSE;
    if (nNumber == 1) return TRUE;

    return mod(nNumber);
}
