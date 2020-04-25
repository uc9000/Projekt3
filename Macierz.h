#pragma once
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>

template<typename T>
class Macierz {
  private:
  int ile_wierszy;
  int dlugosc_wiersza;
  T* dane;
  T** wiersze;

  void alloc(){
    if(ile_wierszy == 0 || dlugosc_wiersza == 0){
      std::cerr << "ERROR: Nie przypisano wielkosci macierzy" << std::endl;
      return;
    }
    if(dane != NULL && wiersze != NULL){
      delete[] dane;
      delete[] wiersze;
    }
    dane = new T[ile_wierszy * dlugosc_wiersza];
    wiersze = new T*[ile_wierszy];
    for (int i = 0; i < ile_wierszy; ++i){
      wiersze[i] = dane + i*dlugosc_wiersza;
    }
  }  

 public:
  void setSize(int n){
    if(n > 1){
      ile_wierszy = n;
      dlugosc_wiersza = n + 1;
    }
    alloc();
  }
  
  Macierz() : ile_wierszy(0), dlugosc_wiersza(0) {
    dane = NULL;
    wiersze = NULL;
  }

  ~Macierz(){
    if(dane != NULL && wiersze != NULL){
      delete[] dane;
      delete[] wiersze;
    }
  }
  // Usuniety konstruktor kopiujacy i operator przypisania powoduje,
  // ze obiektow tej klasy nie da sie przekazywac przez wartosc.
  // Jest to rowniez sposob dotrzymania zasady trzech.
  void operator=(Macierz& other_) = delete;
  Macierz(const Macierz& other_) = delete;

  void zeruj() {
    for (int i = 0; i < ile_wierszy*dlugosc_wiersza; ++i) {
      dane[i] = 0;
    }
  }

  void pokaz() const {
    using std::cout;
    using std::endl;
    using std::setw;
    using std::fixed;
    using std::setprecision;
    for (int i = 0; i < ile_wierszy; ++i) {
      for (int j = 0; j < dlugosc_wiersza - 1; ++j) {
        cout << setw(8) << setprecision(4) << fixed << wiersze[i][j];
      }
      cout << "  |" << setw(8) << setprecision(4) << wiersze[i][dlugosc_wiersza - 1] << endl;
    }
  }

  int getIleWierszy() { return ile_wierszy; }

  T& element(int i, int j) {
    if(i >= ile_wierszy || j >= dlugosc_wiersza){
      std::stringstream ss;
      ss << "Proba dostepu poza wymiarami (n = " << ile_wierszy << ") macierzy dla indeksow [" << i << "][" << j << "]"; 
      throw std::out_of_range(ss.str());
    }
    return wiersze[i][j];    
  }

  T& wolny(int i) {
    return element(i, dlugosc_wiersza - 1);
  }
};