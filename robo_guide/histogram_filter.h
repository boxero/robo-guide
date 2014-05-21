#include "histogram_filter.cpp"

inline void mapa_init_prob(double **mapa, int il_wierszy, int il_kolumn);

//odswiezam prawdopodobienstwo polozenia robota po pomiarze z sensorow - regula bayesa
inline void p_update_after_sense(Comm::NXTComm comm, int current_direction,int new_north_direction,int direction,double *ktemp,int il_wiermapy,int il_kolmapy, double **mapa,double ***dist_to_obstacles);

inline void p_update_after_move(Comm::NXTComm comm, int current_direction,int ilosc_krokow,int il_wiermapy,int il_kolmapy, double **mapa, double move_precision);

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,