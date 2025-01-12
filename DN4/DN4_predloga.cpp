#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <omp.h>
#include <ctime>
//#include <unistd.h>

using namespace std;

int main() {

  // inicializiramo matriko A
  vector<vector<double>> A;

  // inicializiramo vektor b;
  vector<double> b;
  
  // podamo ime datoteke
  std::string filename = "./datoteka_A_b.txt";

  // preberemo datoteko
  std::ifstream infile;
  infile.open(filename);

  // preberemo prvo vrstico, v kateri imamo podano velikost matrike A 
  std::string string_first_line;
  std::getline(infile, string_first_line);
  
  // string_first_line je enak 'A: n=256'
  // vemo, da je delimiter pri '=', lahko zamenjamo npr. z ' '
  std::replace(string_first_line.begin(), string_first_line.end(), '=', ' ');

  // definiramo stringstream, s katerim je nekoliko lazje shranjevati
  // podatke iz string-a, ce so loceni z ' '
  std::istringstream iss(string_first_line);
  std::string nepomemben_del1; // sem bomo dali string 'A:' iz prve vrstice
  std::string nepomemben_del2; // sem bomo dali string 'n' iz prve vrstice
  int n; // sem bomo dali velikost matrike A

  // istringstream loci glede na ' ', trenutno imamo 'A: n 256'
  iss >> nepomemben_del1;
  iss >> nepomemben_del2;
  iss >> n;

  std::cout << "Velikost matrike A: " << n << "x" << n << std::endl;; 

  // V naslednjih n vrsticah imamo elemente matrike A, zato naredimo
  // iteracijo, da preberemo in zafilamo matriko A
  for (int iiA=0; iiA < n; iiA++)
    {
      // preberemo vrstico
      std::string line;
      std::getline(infile, line);
      // zamenjamo ';' s  ' ', saj istringstream loci glede na ' '
      std::replace(line.begin(), line.end(), ';', ' ');

      // z istringstream ponovno pretvorimo string
      std::istringstream iss_column(line);

      // definiramo nov vektor, da shranimo vrstico 
      vector<double> row;

      // sedaj lahko iteriramo po elementih v iss_column
      for (int column=0; column<n; column++)
	{
	  double element_a = 0;
	  iss_column >> element_a;
	  row.push_back(element_a);
	}

      // ko imamo vektor za vrstico, ga damo v A
      A.push_back(row);
    }

  // sedaj imamo sestavljeno matriko A. Naslednja vrstica je prazna,
  // zato jo samo preberemo in ne naredimo nicesar. Poglejte v
  // datoteko datoteka_A_b.txt
  std::string empty_line;
  std::getline(infile, empty_line);

  // prebrati moramo se vektor b
  std::string string_line_b;
  std::getline(infile, string_line_b);

  // lahko uporabimo podoben trik kot pri prvi vrstici
  std::replace(string_line_b.begin(), string_line_b.end(), '>', ' ');
  std::istringstream iss_b(string_line_b);
  int n_b; // sem bomo dali velikost vektorja b (ki je identicna velikosti A)

  iss_b >> nepomemben_del1;
  iss_b >> nepomemben_del2;
  iss_b >> n_b;

  std::cout << "Velikost vektorja b: " << n_b << std::endl;; 

  // naredimo iteracijo po naslednjem n_b stevilu vrstic
  for (int iib=0; iib<n_b; iib++)
    {
      // preberemo vrstico in shranimo element v vrstici v vektor b
      std::string line_b_element;
      std::getline(infile, line_b_element);
      std::istringstream iss_b_element(line_b_element);
      
      double b_element=0;
      iss_b_element >> b_element;

      b.push_back(b_element);
    }

  // Sedaj imamo A in b. Lahko napisemo Gauss-Seidel metodo. Najprej
  // inicializiramo vektor resitve T, npr. na 100 stopinj.
  vector<double> T;
  for (int iiT=0; iiT<n_b; iiT++)
    {
      T.push_back(100);
    }

  auto start_time = std::chrono::high_resolution_clock::now();
  for (int ii=0; ii<2000; ii++) {
      // zanka čez vse neznanke
    for (int j = 0; j < n; j++) {
      double d = b[j];
      // zanka za odštevanje prispevkov znanih spremenljivk
      for (int i = 0; i < n; i++)
      {
        if (i != j)
        {
          d -= A[j][i] * T[i];
        }
      }
      // posodobimo T[j]
      T[j] = d / A[j][j];
    }

  } 

  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> time_duration = end_time - start_time;
  std::cout << "Time of Gauss-Seidel: " << time_duration.count() << " seconds" << std::endl;
  
  // Za izpis maksimalne vrednosti
  double max_T = 0;
  for (int iiT=0; iiT<n_b; iiT++)
    {
      cout << T[iiT] << endl;
      
      if (T[iiT] > max_T){
	max_T =T[iiT];
      }
    }
  std::cout << "Serial Time: " << time_duration.count() << " seconds" << std::endl;
  cout << "Max. temperature: " << max_T << " degree C." << endl;
  
  return 0;

	/*
   KOMENTAR NA PARALELIZACIJO GAUSS SEIDLOVE METODE: Metoda Gauss Seidl je iterativna metoda. Glavni pomen iterativnih metod je, da vsako
   novo iteracijo izračunajo nov približek rezultata, ki je nekoliko boljši od prejšnjega. Metoda se konča po max iteracijah, ali pa kadar je
   razlika med novim in starim rezultatom znotraj meje, ki jo določimo sami. Pri tem opisu še ne naletimo na probleme pri paralelizaciji.
   Nadaljno moramo razmisliti o lastnostih metode Gauss-Seidl. Glavna značilnost je, da pri tej iterativni metodi uporabimo za izračun
   novih vrednosti, vrednosti iz stare in iz TRENUTNE ITERACIJE računanja. Pri drugem delu te definicije naletimo na problem. Brez težav
   lahko paraleliziramo metodo, ki upošteva le vrednosti iz prejšnje iteracije. Lep primer te je npr. Jacobijeva metoda reševanja,
   ki je sicer zelo podobna Gauss-Seidlovi, ampak zaradi upoštevanja le prejšnje iteracije nima problema s paralelizacijo (vir: https://arxiv.org/pdf/1403.5805.). 
   Ker pri Gauss-Seidlu uporabimo vrendnosti iz te paralelizacije, pa je logično, da paraleliziranja ne moremo izvest(threadi si ne morejo razdeliti dela in podajati
   novih vrednosti drugemu threadu, prav tistemu, ki te vrednosti potrebuje...). Po nekaj brskanja po spletu sem sicer našel načine, da se tudi
   metodo Gauss-Seidl da paralelizirati, vendar so za to uporabljene neke precej napredne metode kot npr. graph coloring(vir: https://www.osti.gov/servlets/purl/1246285.).
   Torej po mojem mnenju se metode Gauss-Seidl ne da enostavno paralelizirati. 
*/
