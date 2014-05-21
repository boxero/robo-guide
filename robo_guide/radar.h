#include "radar.cpp"

//pobieranie dystansu od przeszkod na lewo, prawo i wprost za pomoc¹ radaru obrotowego,
//wymaga odrebnej konstrukcji radaru, sonaru umieszczonego na servo w OUT_A,
inline int sense(Comm::NXTComm comm, int port1,int direct_temp, double *ktemp);

//sense distance to obstacles on the left, right and forward direction - static radar, port1-left,port2-right,port3-forward based on SMUX (multiplekser czujnikow cyfrowych)
inline int sense2(Comm::NXTComm comm,int port1,int port2,int port3,int direct_temp, double *ktemp);

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,