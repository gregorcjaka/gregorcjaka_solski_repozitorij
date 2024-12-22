#include <iostream>
#include <cmath>

// funkcija za izračun acrtan s taylorjevo vrsto
double calcarctan(double x, int st_clenov) {
    double arctan = 0.0;
    for (int n = 0; n < st_clenov; ++n) {
        double vrednost = pow(-1, n) * pow(x, 2 * n + 1) / (2 * n + 1);
        arctan += vrednost;
    }
    return arctan;
}

// funkcija, ki izračuna vrednost izraza v integralu
double vrednostIzraza(double x, int st_clenov) {
    double arctan = calcarctan(x / 2.0, st_clenov);
    return exp(3 * x) * arctan;
}

// funkcija, ki izračuna vrednost integrala
double trapeznaMetoda(double a, double b, int n, int st_clenov) {
    double h = (b - a) / n;
    double integral; // samo inicializiramo spremenljivko

    // posebej izračun prvega in zadnjega člena
    integral += vrednostIzraza(a, st_clenov) / 2.0;
    integral += vrednostIzraza(b, st_clenov) / 2.0;

    for (int i = 1; i < n; ++i) {
        double x_i = a + i * h;
        integral += vrednostIzraza(x_i, st_clenov);
    }
    integral *= h;

    return integral;
}

// glavna funkcija
int main() {
    double a = 0.0; // spodnja meja integrala
    double b = M_PI / 4.0;  //zgornja meja integrala
    int n = 500;  // stevilo podintervalov, na katere razdelimo naše območje, večja je cifra, manjšo napako imamo
    int st_clenov= 3; // število členov, ki jih uporabimo v taylorjevi vrsti(torej kateri odvod še upoštevamo)

    double rezultat = trapeznaMetoda(a, b, n, st_clenov);

    std::cout << "Izračunana vrednost integrala je: " << rezultat << std::endl;

    return 0;
}
