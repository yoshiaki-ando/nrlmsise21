#include <iostream>
#include <fstream>
#include <string>

#include <chrono>

#include "Msis21.h"

int main(){
  
  int iyd = 16178;
  float sec = 64800.0f;
  float alt = 0.0f;
  float glat = 50.0f;
  float glong = 55.0f;
  float f107a = 145.0f;
  float f107 = 145.0f;
  float apd = 6.0f;
  /*
  float *d = new float [10];
  float *t = new float [2];
  */

  wrap_msisinit_();

  /*
  std::ofstream ofs("result.dat");
  for(int j = 0; j < 10; j++){
    alt = float(j*10);
    msistest_(&iyd, &sec, &alt, &glat, &glong, &f107a, &f107, &apd, d, t);

    ofs << alt << " ";
    for(int i = 0; i < 10; i++){
      ofs << d[i] << " ";
    }
    ofs << " " << t[0] << " " << t[1] << "\n";
  }
  ofs.close();
  */

  float *d = new float [10];
  float *t = new float [2];
  float alt_min = 0.0;
  float alt_step = 10.0;
  int Nalt = 10;

  
  float *dp = new float [10*10];
  float *tp = new float [2*10];
  msis_profile_(&iyd, &sec, &Nalt, &alt_min, &alt_step, &glat, &glong, &f107a, &f107, &apd, dp, tp);
  //msis_profile_(&iyd, &sec, &Nalt, &alt_min, &alt_step, &glat, &glong, &f107a, &f107, &apd, d, t);
  msistest_(&iyd, &sec, &alt, &glat, &glong, &f107a, &f107, &apd, d, t);
  for(int i = 0; i < 10; i++){
    std::cout << "d[" << i << "] = " << d[i] << std::endl;
  }
  for(int i = 0; i < 2; i++){
    std::cout << "t[" << i << "] = " << t[i] << std::endl;
  }

  std::ofstream ofs("result2.dat");
  for(int j = 0; j < Nalt; j++){
    ofs << alt_min + alt_step*j << " ";
    for(int i = 0; i < 10; i++){
      ofs << d[j*10 + i] << " ";
    }
    ofs << " " << t[j*2 + 0] << " " << t[j*2 + 1] << "\n";
  }
  ofs.close();

  AndoLab::Msis21 a;
  a.date(2016, 6, 26, 1080);
  a.position(0.0f, 50.0f, 55.0f);
  a.calc_msis();

  float dens = 0.0f;
  for(int i = 0; i <= 4; i++){
    dens += d[i];
  }
  for(int i = 6; i <= 9; i++){
    dens += d[i];
  }
  std::cout << dens << " & class(D) = " << a.N() << std::endl;
  std::cout << "class(T) = " << a.T() << std::endl;
  std::cout << "class(pressure [hPa]) = " << a.p() * 1e-2 << std::endl;

  delete [] d;
  delete [] t;
}
