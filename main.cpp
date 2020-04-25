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

  void setType(){ //typ z pierwszej linijki
    std::ifstream file;
    file.open(filePath);
    if(!file.is_open()){
        std::cerr << "ERROR: Nie udalo sie otworzyc" << std::endl;
        return;
    }
    std::string line;
    while(std::getline(file, line)){
        if(line.length() > 0){
            break;
        }
    }    
    //std::cout << "f path = " << filePath << std::endl;
    //std::cout << "line = " << line << std::endl;
    if(line.at(0) == 'D'){
      isDouble = true;
    }
    else{
      isDouble = false;
    }
  }

  template <typename T>
  void readM(Macierz<T>& macierz){
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

  void readMDialog(const std::string& fileName){
    filePath.clear();
    filePath.append("./przykladowe_macierze/");
    filePath.append(fileName);
    setType();
    if(isDouble){
      readM<double>(*dMacierz);
    }
    else{
      readM<float>(*fMacierz);
    }    
  }
};



int main(){
    Reader r;
    Macierz<float> fm;
    Macierz<double> dm;   
    r.setFMacierz(fm);
    r.setDMacierz(dm);
    r.readMDialog("macierz_5.txt");
    if(r.getIsDouble()){
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