#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <vector>
#include "Macierz.h"
#include "Solver.h"

class Reader{
  private: 
  bool isDouble;
  int mSize;
  std::string filePath;
  Macierz<double>* dMacierz;
  Macierz<float>* fMacierz;
  std::vector<char> varNames;

  public:
  Reader(): isDouble(false), mSize(0){}

  void setDMacierz(Macierz<double>& macierz){
    dMacierz = &macierz;
  }

  void setFMacierz(Macierz<float>& macierz){
    fMacierz = &macierz;
  }

  bool getIsDouble() const{
    return isDouble;
  }

  std::vector<char>& getVarNames(){
    return varNames;
  }

  void setType(){ //typ z pierwszej linijki
    std::ifstream file;
    file.open(filePath);
    if(!file.is_open()){
        std::cerr << "ERROR: Nie udalo sie otworzyc" << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    //std::cout << "f path = " << filePath << std::endl;
    //std::cout << "line = " << line << std::endl;
    if(line.at(0) == 'D'){
      isDouble = true;
    }
    else{
      isDouble = false;
    }
  }

  //metoda do czytania macierzy z pliku
  template <typename T>
  void readMtype(Macierz<T>& macierz){
    std::string line;    
    std::vector<T> values; 
    T tmp;
    int lineCount = 0;
    std::ifstream file(filePath); 
    while(std::getline(file, line)){
      lineCount++;
      if(lineCount == 1){
        continue;
      }

      std::stringstream lineStream(line);
      while(lineStream >> tmp){
        values.push_back(tmp);
      }
      if(lineCount == 2){ //allokowanie pamieci na podstawie ilosci liczb w linijce
        mSize = values.size() - 1;
        macierz.setSize(mSize);        
      }
     
      for(int i = 0; i < mSize; i++){
        macierz.element(lineCount - 2 , i) = values.at(i);
      }
      macierz.wolny(lineCount - 2) = values.at(mSize);    
      values.clear();
    }
  }

  void readM(const std::string& fileName){
    filePath.clear();
    filePath.append("./przykladowe_macierze/"); //otwiera pliki w folderze
    filePath.append(fileName);
    setType();
    if(isDouble){
      readMtype<double>(*dMacierz);
    }
    else{
      readMtype<float>(*fMacierz);
    }    
  }

  //metoda do czytania rownan z pliku
  template <typename T>
  void readRtype(Macierz<T>& macierz){
    std::string line;    
    std::vector<T> values; 
    varNames.clear();
    T tmp;
    int lineCount = 0;
    std::ifstream file(filePath); 
    while(std::getline(file, line)){
      lineCount++;
      if(lineCount == 1){
        continue;
      }
      for(int i = 0; i < (int)line.length(); i++){
        if(isspace(line.at(i)) || line.at(i) == '*'){
          line.erase(line.begin() + i);
          i--;
        }
        if(line.at(i) == '='){
          line.at(i) = ' ';
        }
        if(!(isdigit(line.at(i)) || line.at(i) == '.' || line.at(i) == '-' || line.at(i) == '+' || isspace(line.at(i)))){          
          varNames.push_back(line.at(i));
          line.erase(line.begin() + i);
          i--;
        }
        //std::cout << "line = " << line << std::endl;
      }

      if(varNames.size() == 0){
          throw std::logic_error("Zly typ pliku!");
      }
      
      std::stringstream lineStream(line);
      while(lineStream >> tmp){
        values.push_back(tmp);
        //std::cout << "pushback: " << tmp << std::endl;
      }
      if(lineCount == 2){ //allokowanie pamieci na podstawie ilosci liczb w linijce
        mSize = values.size() - 1;
        macierz.setSize(mSize);        
      }
     
      for(int i = 0; i < mSize; i++){
        macierz.element(lineCount - 2 , i) = values.at(i);
      }
      macierz.wolny(lineCount - 2) = values.at(mSize);    
      values.clear();
    }
  }

  void readR(const std::string& fileName){
    filePath.clear();
    filePath.append("./przykladowe_macierze/");
    filePath.append(fileName);
    setType();
    if(isDouble){
      readRtype<double>(*dMacierz);
    }
    else{
      readRtype<float>(*fMacierz);
    }
  }

long int dajInt() {
    using namespace std;
    bool isGood = false;
    const int sizeLimit = 5;
    string str;
    while (!isGood) {
        getline(cin, str);
        //cout<< "podano: " << str << endl;
        if ((int)str.size() > sizeLimit) {
            cerr << "Liczba jest za dluga. Podaj liczbe calkowita: " << endl;
            continue;
        }
        for (int i = 0; i < (int)str.size(); i++) {
            if (!(isdigit(str[i]) || str[i] == '-')) {
                cerr << "Podaj liczbe calkowita: \n";
                isGood = false;
                break;
            }
            isGood = true;
        }        
    }
    cin.sync();
    return stoi(str);
}

    void menu(){ 
        using namespace std;
        Macierz<float> fm;
        Macierz<double> dm;
        string fileName;
        setFMacierz(fm);
        setDMacierz(dm);
        int wybor = 0;
        while(true){
            switch(wybor){
                case 0:
                cout << "1) Wczytaj macierz z pliku" << endl
                << "2) Wczytaj rownanie z pliku" << endl
                << "3) Zakoncz" << endl;
                wybor = (int)dajInt();
                break;

                case 1:
                try{
                    cout << "Podaj nazwe pliku macierzy do odczytu z folderu 'przykladowe_macierze': " << endl;
                    cin >> fileName;
                    readM(fileName); //wybierz plik z macierza z folderu 'przykladowe_macierze'     
                    if(getIsDouble()){
                        dm.pokaz();
                        std::cout << std::endl << std::endl;
                        Solver<double> ds = Solver<double>(dm);        
                        ds.rozwiaz();
                        dm.pokaz();
                    }
                    else{
                        fm.pokaz();
                        std::cout << std::endl << std::endl;
                        Solver<float> fs = Solver<float>(fm);
                        fs.rozwiaz();
                        fm.pokaz();
                    }
                }
                catch (const std::out_of_range& e){ //wyjscie poza rozmiar macierzy
                    std::cerr << "ERROR: " << e.what() << std::endl;
                }

                catch(const std::invalid_argument& e){ //dzielenie przez 0
                    std::cerr << "ERROR: " << e.what() << std::endl;
                }

                catch(const std::exception& e){
                    std::cerr << "Ojoj cos sie popsulo :( -  " << e.what() << std::endl;
                }
                wybor = 0;
                break;
                

                case 2:
                try{
                    cout << "Podaj nazwe pliku ukladu do odczytu z folderu 'przykladowe_macierze': " << endl;
                    cin >> fileName;
                    readR(fileName); //wybierz plik z macierza z folderu 'przykladowe_macierze'       
                    if(getIsDouble()){
                        dm.pokaz();
                        std::cout << std::endl << std::endl;
                        Solver<double> ds = Solver<double>(dm);        
                        ds.rozwiaz();
                        dm.pokaz(getVarNames());
                    }
                    else{
                        fm.pokaz();
                        std::cout << std::endl << std::endl;
                        Solver<float> fs = Solver<float>(fm);
                        fs.rozwiaz();
                        fm.pokaz(getVarNames());
                    }
                }
                catch (const std::out_of_range& e){ //wyjscie poza rozmiar macierzy
                    std::cerr << "ERROR: " << e.what() << std::endl;
                }

                catch(const std::invalid_argument& e){ //dzielenie przez 0
                    std::cerr << "ERROR: " << e.what() << std::endl;
                }

                catch(const std::exception& e){
                    std::cerr << "Ojoj cos sie popsulo :(  -  " << e.what() << std::endl;
                }
                wybor = 0;
                break;


                case 3:
                return;

                default:
                cout << "Podaj liczbe od 1 do 3!" << endl;
                wybor = 0;
                break;
            }
        }    
    }
};