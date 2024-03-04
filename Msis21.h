#ifndef __MSIS21_H__
#define __MSIS21_H__

/*** F10.7パラメタ関係 (とりあえず使わない) ***/
constexpr int NumberOfF107 { 3323 }; /* data number from 20141201 to 20240107 */
constexpr int NumberOfAveraging { 81 }; /* averaging number */
constexpr int NumberOfAveragedF107 { NumberOfF107 - NumberOfAveraging + 1 }; /* averaged data number */


/*
 * [ MSIS 2.1 を扱い易くするためのクラス ]
 * 
 * 基本的には、変数を設定し、密度、温度、気圧を得やすくしているだけのWrapper的クラス
 * 大気圏のみを扱うため、Ap index, F107, 平均F107 は適当な値の固定値とする
 *
 */

namespace AndoLab{
  
class Msis21{
private:
  int year_day; /* MSIS形式の年(YY)とDOY(DDD)で、YYDDD。2016年2月1日なら 16032 */
  int pDoy; /* 1 <= x <= 365 or 366 */
  float second_of_day; /* 0 <= x < 86400 */
  float altitude_in_m;
  float altitude_in_km;
  float latitude, longitude;
  float f107a, f107, apd;

  float density;     /* Number density [m^-3] */
  float temperature; /* [K] */
  float pressure;    /* [Pa] */

  int doy(const int Year, const int Month, const int Day);

  
public:
  /* 大気圏を扱うため、F107やAp指数は適当な値で構わない */
  Msis21(void): f107a( 145.0f ), f107( 145.0f ), apd( 6.0f ){ };

  /* 日付と 1日における分から、MSIS用の iyd (年+DOY) と その日の秒を設定する */
  void date
  (
   const int Year,
   const int Month, /* 1〜12 */
   const int Day,
   const int minute_day /* 0≦ x < 720 */
   );
  
  
  void position /* MSISで計算する位置を設定するのみ */
  (const float alt,  /* [m]   高度 */
   const float lat,  /* [deg] 緯度 */
   const float longi /* [deg] 経度 */
   ); /* 高度[km]、緯度[deg]、経度[deg] を設定 */

  void calc_msis(void); /* 設定されたパラメタを元に MSIS にて計算、クラスのメンバ変数に値を入れる */

  void calc_at /* 位置の設定とMSISの計算を、一度に行う */
  (const float alt,  /* [m]   高度 */
   const float lat,  /* [deg] 緯度 */
   const float longi /* [deg] 経度 */
   );

  /* アクセサ */
  float N(void){ return density; } /* [m^-3] 密度 */
  float T(void){ return temperature; } /* [K] 温度 */
  float p(void){ return pressure; } /* [Pa]  気圧 */
  int doy(){ return pDoy; } /* Day of Year 設定された DOY */
};

}

extern "C" {

  /* === 高度プロファイルとして求める ===
   * 求める高度の数 Nalt 、最低高度 alt_min と、高度ステップ alt_step を指定
   * alt_min 〜 alt_min + (Nalt-1)*alt_step までの各パラメタを出力
   */
  void msis_profile_
  (
   int *iyd,    /* Year and day as YYDDD (Year is ignored in current model) */
   float *sec,  /* Universal time (seconds) */
   int *Nalt,   /* Number of altitude */
   float *alt_min, /* minimum altitude (km) */
   float *alt_step,  /* step of altitude (km) */
   float *glat, /* Geodetic latitude (deg) */
   float *glong,/* Geodetic longitude (deg) */
   float *f107a,/* 81 day average, centered on input time, of F10.7 solar activity index */
   float *f107, /* Daily F10.7 for previous day */
   float *apd,  /* aily Ap */
   /*
OUTPUT VARIABLES:
!       float D[Nalt*10]
!       float T[Nalt*2]
!
!       D(ialt*10 + 0)  He number density (cm-3) at altitude of (alt_min + i*alt_step)
!       D(ialt*10 + 1)  O number density (cm-3) at altitude of (alt_min + i*alt_step)
!       D(ialt*10 + 2)  N2 number density (cm-3) at altitude of (alt_min + i*alt_step)
!       D(ialt*10 + 3)  O2 number density (cm-3) at altitude of (alt_min + i*alt_step)
!       D(ialt*10 + 4)  Ar number density (cm-3) at altitude of (alt_min + i*alt_step)
!       D(ialt*10 + 5)  Total mass density (g/cm3) at altitude of (alt_min + i*alt_step)
!       D(ialt*10 + 6)  H number density (cm-3) at altitude of (alt_min + i*alt_step)
!       D(ialt*10 + 7)  N number density (cm-3) at altitude of (alt_min + i*alt_step)
!       D(ialt*10 + 8)  Anomalous oxygen number density (cm-3) at altitude of (alt_min + i*alt_step)
!       D(ialt*10 + 9) NO number density (cm-3) at altitude of (alt_min + i*alt_step)
!       T(ialt*2 + 0)  Exospheric temperature (K) at altitude of (alt_min + i*alt_step)
!       T(ialt*2 + 1)  Temperature at altitude (K) at altitude of (alt_min + i*alt_step)
*/

   float *d,
   float *t
   );


  /*
   * === 単一点の大気数密度、温度を求めるプログラム ===
   */
  void msistest_
  (
   int *iyd,    /* Year and day as YYDDD (Year is ignored in current model) */
   float *sec,  /* Universal time (seconds) */
   float *alt,  /* Geodetic altitude (km) */
   float *glat, /* Geodetic latitude (deg) */
   float *glong,/* Geodetic longitude (deg) */
   float *f107a,/* 81 day average, centered on input time, of F10.7 solar activity index */
   float *f107, /* Daily F10.7 for previous day */
   float *apd,  /* aily Ap */
   /*
OUTPUT VARIABLES:
!       float d[10]
!       float t[2]
!
!       d[0]  He number density (cm-3)
!       d[1]  O number density (cm-3)
!       d[2]  N2 number density (cm-3)
!       d[3]  O2 number density (cm-3)
!       d[4]  Ar number density (cm-3)
!       d[5]  Total mass density (g/cm3)
!       d[6]  H number density (cm-3)
!       d[7]  N number density (cm-3)
!       d[8]  Anomalous oxygen number density (cm-3)
!       d[9] NO number density (cm-3)
!       t[0]  Exospheric temperature (K)
!       t[1]  Temperature at altitude (K)
*/
   float *d,
   float *t
  );

  /*
   * === 上記 MSIS の初期化Wrapper。初回に一回呼び出す必要がある。 ===
   */
  void wrap_msisinit_();
}

#endif
