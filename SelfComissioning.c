#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

float **FileDate;

void CSVParser()
{
    FILE *pFile;
    pFile = fopen("", 'r');

    long LineNo = 0L;
    char sBufffer[BUFFER_SIZE];

    while (!feof(pFile))
    {
        if (fgets(sBufffer, BUFFER_SIZE - 1, pFile) == NULL)
            break;
        if (LineNo == 0)
        {
            printf("%s\n", sBufffer);
        }
        else
        {
        }
    }
}

void RsCalculation()
{
    if (CurrentError <= 0.001 &&CurrentRef = 0.75 * In)
    {
        RsCalculation = VDC * Duty / (2 * Ia);
    }
}

void LmRrCalculation()
{
}

void LCalculation()
{
}