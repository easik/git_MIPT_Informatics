#include <stdio.h> /* printf */
#include <stdlib.h> /* atof */
#include <float.h> /* DBL_EPSILON */
#include <math.h> /* fabs, pow, sqrt */

const int INFINITE_ROOTS = -1;
//const int NO_ROOTS = 0; removed by easik for structured text (positive
//                        numbers could be described as themselves)

double Input (const char name[]);
int SolveSquare (double a, double b, double c,
                double* x1, double* x2);

int main()
{
    printf ("# SolveSquare v.1 (c) %s %s. by easik\n", __TIME__, __DATE__);

    printf ("# Enter a, b, c for equation ax^2 + bx + c = 0 :\n");
    double a = Input ("a"), b = Input ("b"), c = Input ("c");

    double x1 = 0, x2 = 0;
    int nRoots = SolveSquare (a, b, c, &x1, &x2);

    /** building temporary part **/
    printf ("\n");
    printf ("# a = %lg\n", a);
    printf ("# b = %lg\n", b);
    printf ("# c = %lg\n", c);
    printf ("# if chars then nums or chars are on input then 0 value will occure !\n");
    printf ("\n");

    printf ("Equation %g * x^2 + %g * x + %g = 0 has ", a, b, c); //find and place * symbol code between coefficients and roots !
    switch (nRoots)
    {
        case (-1):
            printf ("an infinite number of roots.\n");
            break;
        case (0):
            printf ("%d roots.\n", nRoots);
            break;
        case (1):
            printf ("%d root:\n", nRoots);
            printf ("x1 = %lg\n", x1);
            break;
        case (2):
            printf ("%d roots:\n", nRoots);
            printf ("x1 = %lg\n", x1);
            printf ("x2 = %lg\n", x2);
            break;
        case (-2):
            printf("# SYSTEM ERROR: SolveSquare() failed to identify roots !\n");
            break;
    }

    return 0;
}

double Input (const char name[])
    {
    printf ("Input %s > ", name);

    char cBuff[256]; // need to improve ! how ?
    scanf ("%s", cBuff);

    double val = atof (cBuff);

    return val;
    }

int SolveSquare (double a, double b, double c,
                double* x1, double* x2)
{

    if (fabs (b) == 0)
    {
        if (fabs (a) == 0 && fabs (c) == 0) return INFINITE_ROOTS;
        return 0;
    }
    else //if (fabs(b) > DBL_EPSILON) removed by easik for reaching end in every time.
    {
        if (fabs (a) == 0)
        {
            *x1 = -c / b;
            return 1;
            //ok
        }
        else //if (fabs(a) > DBL_EPSILON) removed by easik for reaching end in every time.
        {
            if (c == 0)
            {
                *x1 = 0;
                *x2 = -b / a;
                return 2;
            }
            else //if (fabs(c) > DBL_EPSILON) removed by easik for reaching end in every time.
            {
                int nDiscriminant = pow (b, 2) - 4 * a * c;

                if (nDiscriminant < 0) return 0;
                if (nDiscriminant == 0)
                {
                    *x1 = -b / (2 * a);
                    return 1;
                }
                if (nDiscriminant > DBL_EPSILON)
                {
                    *x1 = (-b - sqrt (nDiscriminant)) / (2 * a);
                    *x2 = (-b + sqrt (nDiscriminant)) / (2 * a);
                    return 2;
                }
                return -2;
            }
        }
    }
}
