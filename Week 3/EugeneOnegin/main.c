#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//#define NDEBUG
#include <assert.h>
#include <string.h> // strcmp()
#include <ctype.h>  // isalpha()

#define TRUE    1
#define FALSE   0

#define SUCCESS 1
#define FAIL    -1
#define CHECKERROR(a)   if(a == FAIL) { \
                            printf ("%s\n", sErrMsg); \
                            if (cBuffer) free (cBuffer); \
                            if (pLines) free (pLines); \
                            return FAIL; \
                        }
#define ERRMSG_LEN  1024
char sErrMsg[ERRMSG_LEN];

const int nBlockSize = 256;
char sInputFile[] = "onegin.txt";
char sOutputFileOrg[] = "original.txt";
char sOutputFileSrtBeg[] = "sorted_beg.txt";
char sOutputFileSrtEnd[] = "sorted_end.txt";

long int FileLen(FILE *pFile);
long int ReadFile(char *sInputFile, char **cBuffer);
long int WriteFile(char *sOutputFile, char **pLines, long int nLines);
long int SplitBuffer(char *cBuffer, long int nBuffSize, char ***pLines);
int CompareBegin(const void *a, const void *b);
int CompareEnd(const void *a, const void *b);
int IsCharAlpha(unsigned char ch);
unsigned char ToLower(unsigned char ch);

int main() {
    long int nRes;
    char *cBuffer = NULL;
    char **pLines = NULL;
    long int nBuffSize ,nLines;

    nBuffSize = ReadFile(sInputFile, &cBuffer);
    CHECKERROR(nBuffSize);

    nLines = SplitBuffer(cBuffer, nBuffSize, &pLines);
    CHECKERROR(nLines);

    nRes = WriteFile(sOutputFileOrg, pLines, nLines);
    CHECKERROR(nRes);

    qsort(pLines, nLines, sizeof(*pLines), CompareBegin);

    nRes = WriteFile(sOutputFileSrtBeg, pLines, nLines);
    CHECKERROR(nRes);

    qsort(pLines, nLines, sizeof(*pLines), CompareEnd);

    nRes = WriteFile(sOutputFileSrtEnd, pLines, nLines);
    CHECKERROR(nRes);

    if (cBuffer) free(cBuffer);
    if (pLines) free(pLines);

    return 0;
}


long int FileLen(FILE *pFile) {
    if (fseek(pFile, 0, SEEK_END) != 0) {
        snprintf(sErrMsg, ERRMSG_LEN, "ERROR: Can't set position in file.  [FileLen]");
        return FAIL;
    }

    long int nFileLen = ftell(pFile);
    if (nFileLen < 0) {
        snprintf(sErrMsg, ERRMSG_LEN, "ERROR: Can't get position in file.  [FileLen]");
        return FAIL;
    }

    if (fseek(pFile, 0, SEEK_SET) != 0) {
        snprintf(sErrMsg, ERRMSG_LEN, "ERROR: Can't reset position in file.  [FileLen]");
        return FAIL;
    }

    return nFileLen;
}


long int ReadFile(char *sInputFile, char **cBuffer) {
    FILE *pFile = fopen(sInputFile, "rb");
    if (pFile == NULL) {
        snprintf(sErrMsg, ERRMSG_LEN, "ERROR: Can't open file '%s'.  [ReadFile]", sInputFile);
        return FAIL;
    }

    long int nDataLen;
    if ((nDataLen = FileLen (pFile)) < 0) return FAIL;

    char *cBuff = (char *) calloc(nDataLen + 1, sizeof(*cBuff));
    if (cBuff == NULL) {
        snprintf(sErrMsg, ERRMSG_LEN, "ERROR: Can't allocate memory buffer.  [ReadFile]");
        fclose(pFile);
        return FAIL;
    }

    long int nBytesRead = fread(cBuff, sizeof (*cBuff), nDataLen, pFile);
    if (nBytesRead != nDataLen) {
        snprintf(sErrMsg, ERRMSG_LEN, "ERROR: Can't read file '%s'.  [ReadFile]", sInputFile);
        printf("ERROR: Can't read file '%s': ReadSize = %ld, FileSize = %ld.  [ReadFile]", sInputFile, nBytesRead, nDataLen);
        free(cBuff);
        fclose(pFile);
        return FAIL;
    }
    cBuff[nDataLen] = '\0';

    fclose(pFile);
    *cBuffer = cBuff;

    return nBytesRead;
}

long int SplitBuffer(char *cBuffer, long int nBuffSize, char ***pLines) {
    char **pBuffer = NULL, **pTemp, *p;
    long int nLine = 0, nLinesAllocated = -1, i = 0;
    int bNewLine = TRUE;

    for (p = cBuffer; *p != '\0'; p++) {
        assert(i++ < nBuffSize);
        if (*p == '\r' || *p == '\n') {
            *p = '\0';
            bNewLine = TRUE;
        } else if (bNewLine) {
            if (nLine >= nLinesAllocated) {
                nLinesAllocated += nBlockSize;
                pTemp = (char **) realloc(pBuffer, nLinesAllocated * sizeof (*pBuffer));
                if (pTemp == NULL) {
                    if (pBuffer != NULL) free(pBuffer);
                    snprintf(sErrMsg, ERRMSG_LEN, "ERROR: Can't reallocate memory buffer.  [SplitBuffer]");
                    return FAIL;
                }
                pBuffer = pTemp;
            }
            pBuffer[nLine++] = p;
            bNewLine = FALSE;
        }
    }

    pTemp = (char **) realloc(pBuffer, nLine * sizeof (*pBuffer));
    if (pTemp == NULL) {
        if (pBuffer != NULL) free(pBuffer);
        snprintf(sErrMsg, ERRMSG_LEN, "ERROR: Can't shrink memory buffer.  [SplitBuffer]");
        return FAIL;
    }
    *pLines = pTemp;

    return nLine;
}

long int WriteFile(char *sOutputFile, char **pLines, long int nLines) {
    FILE *pFile = fopen(sOutputFile, "w");
    if (pFile == NULL) {
        snprintf(sErrMsg, ERRMSG_LEN, "ERROR: Can't open output file '%s'.  [WriteFile]", sOutputFile);
        return FAIL;
    }

    int i;
    for (i = 0; i < nLines; i++) {
        if (fputs(pLines[i], pFile) < 0) {
            snprintf(sErrMsg, ERRMSG_LEN, "ERROR: Can't write output file '%s'.  [WriteFile]", sOutputFile);
            fclose(pFile);
            return FAIL;
        }
        if (fputs("\r\n", pFile) < 0) {
            snprintf(sErrMsg, ERRMSG_LEN, "ERROR: Can't write output file '%s'.  [WriteFile]", sOutputFile);
            fclose(pFile);
            return FAIL;
        }
    }
    fclose(pFile);

    return SUCCESS;
}

int CompareBegin(const void *a, const void *b) {
    unsigned char *str1 = *(unsigned char **)a;
    unsigned char *str2 = *(unsigned char **)b;
    int len1 = strlen(str1), i1;
    int len2 = strlen(str2), i2;

    for (i1 = 0; i1 < len1 && str1[i1] == ' '; i1++);
    if (i1 < len1 && !IsCharAlpha(str1[i1])) i1++;
    for (i2 = 0; i2 < len2 && str2[i2] == ' '; i2++);
    if (i2 < len2 && !IsCharAlpha(str2[i2])) i2++;

    assert(i1 <= len1);
    assert(i2 <= len2);

    int len = (len1 - i1) < (len2 - i2) ? (len1 - i1) : (len2 - i2);
    int i;
    for (i = 0; i < len; i++) {
        assert(i1 + i < len1);
        assert(i2 + i < len2);
        if (ToLower(str1[i1 + i]) < ToLower(str2[i2 + i])) return -1;
        if (ToLower(str1[i1 + i]) > ToLower(str2[i2 + i])) return 1;
        // order caps first
        if (str1[i1 + i] < str2[i2 + i]) return -1;
        if (str1[i1 + i] > str2[i2 + i]) return 1;
    }

    return ((len1 - i1) - (len2 - i2));
}

int CompareEnd(const void *a, const void *b) {
    unsigned char *str1 = *(unsigned char**)a;
    unsigned char *str2 = *(unsigned char**)b;
    unsigned char *p;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    for (p = str1 + len1 - 1; *p == ' ' && p >= str1; p--) len1--;
    if (len1 > 0 && !IsCharAlpha(str1[len1 - 1])) len1--;
    for (p = str2 + len2 - 1; *p == ' ' && p >= str2; p--) len2--;
    if (len2 > 0 && !IsCharAlpha(str2[len2 - 1])) len2--;

    assert(len1 >= 0);
    assert(len2 >= 0);

    int len = len1 < len2 ? len1 : len2;
    int i;
    for (i = 1; i <= len; i++) {
        assert(len1 - i >= 0);
        assert(len2 - i >= 0);
        if (ToLower(str1[len1 - i]) < ToLower(str2[len2 - i])) return -1;
        if (ToLower(str1[len1 - i]) > ToLower(str2[len2 - i])) return 1;
        // order caps first
        if (str1[len1 - i] < str2[len2 - i]) return -1;
        if (str1[len1 - i] > str2[len2 - i]) return 1;
    }

    return (len1 - len2);
}

int IsCharAlpha(unsigned char c) {
    // Windows 1251 character table
    return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 192 && c <= 255) || c == 168 || c == 184) ? TRUE : FALSE;
}
unsigned char ToLower(unsigned char c) {
    // Windows 1251 character table
    return ((c >= 65 && c <= 90) || (c >= 192 && c <= 223)) ? c + 32 : (c == 168 ? c + 16 : c);
}
