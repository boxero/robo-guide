#include "map_reader.cpp"

//odczyt rozmiaru mapy
inline void size_reader(int* rows, int* cols, int* x_goal, int* y_goal, std::string file_name);

//odczyt mapy
inline void map_reader(int rows_temp,int cols_temp,double ***dist_to_obstacles,int ***dist_to_field_bondary,int **new_north_direction_all,double **mapa,std::string file_name);

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,