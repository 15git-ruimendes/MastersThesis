#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <time.h>

#define PI 3.142857
#define MaxIte 10000000

double Temperature = 0.075;
double CoolingRate = 0.25;

double Rs = 0.03552, L = 0.000335, Rr = 0.02092, Lm = 0.0151, w;

double complex Target = 0.1737 + I * 0.5422;
double complex Zopt;
double complex Zcurrent;
double complex Zpossible;
double RealError = 100, ImagError = 100;

void SelectCurrentValue(double Eval, int Ite)
{
    Temperature = Temperature * (1 - (Ite + 1) / MaxIte);
    double Prob = (double)(rand() % 101) * 0.01;

    if (Prob < Temperature / Eval)
        Zcurrent = Zpossible;
    else
        return;

    return;
}

void CalculateError(int Ite)
{
    double Real = abs(creal(Target) - creal(Zpossible));
    double Imag = abs(cimag(Target) - cimag(Zpossible));
    double Eval = Real * 0.5 + Imag * 0.5;
    double Error = Eval - (RealError * 0.5 + ImagError * 0.5);
    if (Eval < (RealError * 0.5 + ImagError * 0.5))
    {
        RealError = Real;
        ImagError = Imag;

        Zopt = Zpossible;
        Zcurrent = Zpossible;

        printf("The target value is = %.8f %+.8fi\n", creal(Target), cimag(Target));
        printf("The new optimal value is = %.8f %+.8fi\n", creal(Zopt), cimag(Zopt));
        printf("Current parameters Rs = %.8f, Rr = %.8f, L = %.8f, Lm = %.8f\n", Rs, Rr, L, Lm);
    }
    else
    {
        // SelectCurrentValue(Error, Ite);
    }
    return;
}

void CreateNeighbour()
{
    Rs += (double)((rand() % 201) - 100) * 0.001;
    Rr += (double)((rand() % 201) - 100) * 0.001;
    L += (double)((rand() % 201) - 100) * 0.001;
    Lm += (double)((rand() % 201) - 100) * 0.001;

    Zpossible = (Rs + (pow(w, 2) * (L + Lm) * Lm * Rr - pow(w, 2) * L * Lm * Rr) / (pow(Rr, 2) - pow((w * (L + Lm)), 2))) + I * (w * L + (w * Lm * pow(Rr, 2) + pow(w, 3) * L * Lm * (L + Lm)) / (pow(Rr, 2) - pow((w * (L + Lm)), 2)));
}

int main()
{
    int Ite = 0;
    w = 2 * PI * 50;
    Zopt = (Rs + (pow(w, 2) * (L + Lm) * Lm * Rr - pow(w, 2) * L * Lm * Rr) / (pow(Rr, 2) - pow((w * (L + Lm)), 2))) + I * (w * L + (w * Lm * pow(Rr, 2) + pow(w, 3) * L * Lm * (L + Lm)) / (pow(Rr, 2) - pow((w * (L + Lm)), 2)));
    Zcurrent = Zopt;
    printf("The target value is = %.2f %+.2fi\n", creal(Target), cimag(Target));
    printf("The inital value is = %.2f %+.2fi\n", creal(Zopt), cimag(Zopt));
    printf("Current parameters Rs = %.8f, Rr = %.8f, L = %.8f, Lm = %.8f\n", Rs, Rr, L, Lm);

    srand(time(NULL));

    while (Ite < MaxIte)
    {
        CreateNeighbour();
        CalculateError(Ite);
        Ite++;
        srand(time(NULL));
    }

    return 1;
}