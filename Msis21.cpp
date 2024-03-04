#include "Msis21.h"
#include <physical_constants.h>

namespace AndoLab {

int Msis21::doy(const int year, const int month, const int day){
  int days_of_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  /* DOYの計算 */
  int vdoy = 0;
  for(int m = 1; m <= month-1; m++){ /* 1月から前月(month-1)まで */
    vdoy += days_of_month[m-1];
  }
  vdoy += day;

  /* 閏年の判定 (不要かも知れないが) 
   *  ※2001年〜2099年のみに適用可
   */
  if ( ( year%4 == 0 ) && ( month >= 3 ) ){ /* 閏年で3月以降 */
    vdoy++; /* 1日追加 */
  }

  return vdoy;
}

void Msis21::date(const int Year, const int Month, const int Day, const int minute_day){

  pDoy = doy(Year, Month, Day);
  year_day = ( Year - 2000 ) * 1000 + pDoy; /* MSIS2.1の形式。2017年2月2日なら 17033*/
  second_of_day = minute_day * 60.0f;
}

constexpr float Meter2Killometer { 1.0e-3f };

void Msis21::position(const float alt, const float lat, const float longi){
  altitude_in_m = alt;
  altitude_in_km = alt * Meter2Killometer;
  latitude = lat;
  longitude = longi;
}

constexpr float CM_3toM_3 { 1.0e6f };

void Msis21::calc_msis(void){
  float *d = new float [10];
  float *t = new float [2];

  msistest_(&year_day, &second_of_day, &altitude_in_km, &latitude, &longitude, &f107a, &f107, &apd, d, t);

  /* 各組成の密度の合計をとる */
  density = 0.0f;
  for(int n = 0; n <= 4; n++){
    density += d[n];
  }
  for(int n = 6; n <= 9; n++){
    density += d[n];
  }
  density *= CM_3toM_3;
  
  temperature = t[1];

  pressure = density * BOLTZMANN_CONSTANT * temperature;
  
  delete [] d;
  delete [] t;
    
}

void Msis21::calc_at(const float alt, const float lat, const float longi){
  position(alt, lat, longi);
  calc_msis();
}

}
