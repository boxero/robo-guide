#include "drive.cpp"

//turn_to_specified_map_direction(&comm,IN_1,direct_temp,new_north_direction), direct_temp=0-N,1-E,2-S,3-W 
inline int turn_to_specified_map_direction(Comm::NXTComm comm, int port, int direct_temp, int new_north_direction);

//drive_PD_in_specified_map_direction(&comm,IN_1,IN_4,direct_temp,new_north_direction), direct_temp=0-N,1-E,2-S,3-W, IN_1 port kompasu, IN_4 port sonaru,
inline int drive_PD_in_specified_map_direction(Comm::NXTComm comm, int port, int port2, int direct_temp, int new_north_direction);

//drive_PD_in_specified_map_direction_between_walls(&comm,IN_1,IN_4,direct_temp,new_north_direction), direct_temp=0-N,1-E,2-S,3-W, IN_1 port kompasu, IN_4 port sonaru przod,
inline int drive_PD_in_specified_map_direction_between_walls(Comm::NXTComm comm, int port, int port2, int direct_temp, int new_north_direction, int ilosc_taktow);

inline int drive_PD_left_wall(Comm::NXTComm comm, int port, int port2, int direct_temp, int new_north_direction);

//funkcja szacuje ilosc taktow do przejechania, w celu zblizenia sie do ustalonego celu,
inline int il_takt(Comm::NXTComm comm,int x_goal,int y_goal,int actual_x_pos,int actual_y_pos, int ***dist_to_field_bondary, int direction); 

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,
