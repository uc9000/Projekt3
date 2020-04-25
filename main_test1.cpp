//TEST ZABEZPIECZEN

#include <iostream>
#include <stdexcept>
#include "Macierz.h"
#include "Solver.h"


void demo3(Macierz<double>& m) {
  m.element(0, 0) =  2.2;
  m.element(0, 1) = -1.5;
  m.element(0, 2) =  0.6;
  m.wolny(0) = 1.3;
  m.element(1, 0) = -0.7;
  m.element(1, 1) =  1.2;
  m.element(1, 2) =  1.8;
  m.wolny(1) = 1.85;
  m.element(2, 0) =  1.2;
  m.element(2, 1) =  0.8;
  m.element(2, 2) = -2.1;
  m.wolny(2) = -4.55;
}

void demo4(Macierz<float>& m) {
  m.element(0, 0) = -0.2;
  m.element(0, 1) =  1.1;
  m.element(0, 2) =  0.1;
  m.element(0, 3) =  0.9;
  m.wolny(0) = 1.95;
  m.element(1, 0) = -1.2;
  m.element(1, 1) =  3.5;
  m.element(1, 2) = -2.6;
  m.element(1, 3) =  1.9;
  m.wolny(1) = 5.43;
  m.element(2, 0) =  0.8;
  m.element(2, 1) = -2.3;
  m.element(2, 2) =  1.2;
  m.element(2, 3) = -0.8;
  m.wolny(2) = -3.28;
  m.element(3, 0) =  0.2;
  m.element(3, 1) = -1.7;
  m.element(3, 2) =  1.9;
  m.element(3, 3) =  1.3;
  m.wolny(3) = -0.05;
}

int main() {
  Macierz<float> m;
  m.setSize(4); //aby zobaczyc jak dziala rzucanie wyjatku i blad mozna ustawic tutaj wielkosc na 3
  try{
    demo4(m);
  }
  catch (const std::out_of_range& e){ //wyjscie poza rozmiar macierzy
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 0;
  }
  m.pokaz();
  std::cout << std::endl;

  Solver<float> s = Solver<float>(m);
  try{
    s.rozwiaz();
  }
  catch(const std::invalid_argument& e){ //dzielenie przez 0
    std::cerr << "ERROR: " << e.what() << std::endl;
  }
  m.pokaz();
  return 0;
}