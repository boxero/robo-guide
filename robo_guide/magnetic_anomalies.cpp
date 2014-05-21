//ustalamy nowy kierunek polnocny po przejechaniu ustalonej ilosci taktow,
inline int next_north_direction(Comm::NXTComm comm,int x_goal,int y_goal,int actual_x_pos,int actual_y_pos, int direction,int **new_north_direction_all){
  // mocno dopasowana wartosc do ksztaltu mapy, nalezy przeformatowac w prypadku innej mapy
  int poiner2=0;//daje sygnal ze cel jest osiagalny i pozwala na lepsza lokalizacje w poblizu celu,
  if(direction==0){
    if(actual_y_pos==y_goal){
      poiner2=1;
     }
    else{
      poiner2=2;
     }
     }
    if(direction==1){
      if(actual_x_pos==x_goal){
        poiner2=1;
       }
      else{
        poiner2=3;
       }
     }
	if(direction==2){
	  if(actual_y_pos==y_goal){
	    poiner2=1;
	   }
	  else{
	    poiner2=4;
	   }
	 }
	if(direction==3){
	  if(actual_x_pos==x_goal){ 
	    poiner2=1;
	   }
	  else{
	    poiner2=5;
	   }
	 }

	int new_north_direction_internal;

	if(poiner2==1){
	  new_north_direction_internal=new_north_direction_all[x_goal][y_goal];
	 }
	if(poiner2==2){
	  new_north_direction_internal=new_north_direction_all[1][y_goal];
	 }
	if(poiner2==3){
	  new_north_direction_internal=new_north_direction_all[x_goal][6];
	 }
	if(poiner2==4){
	  new_north_direction_internal=new_north_direction_all[3][y_goal];
	 }
	if(poiner2==5){
	  new_north_direction_internal=new_north_direction_all[x_goal][1];
	 }
	return new_north_direction_internal;
  } 

inline int emergency_north_direction(double **mapa, int **new_north_direction_all, int il_wierszy, int il_kolumn){
  int average_north_direction=0;
  int max_north=0;
  int min_north=360;
  int il_pol=0;
  for(int i=1;i<il_wierszy+1;i++){
	for(int j=1;j<il_kolumn+1;j++){
	  if(mapa[i][j]!=5){
	    il_pol++;
		average_north_direction+=new_north_direction_all[i][j];
		if(max_north<=new_north_direction_all[i][j]){
		  max_north=new_north_direction_all[i][j];
		 }
		if(min_north>=new_north_direction_all[i][j]){
		  min_north=new_north_direction_all[i][j];
		 }
	   }
	 }
	}
   //warunek zapobiegajacy dzzieleniu przez zero
   if(il_pol>2){
	 average_north_direction-=min_north;
	 average_north_direction-=max_north;
	 average_north_direction=average_north_direction/(il_pol-2);
	}
   //cout<<"\n\nAverage north direction = "<<average_north_direction;
   //cout<<"\nmin_north = "<<min_north;
   //cout<<"\nmax_north = "<<max_north;
   //cout<<"\nil_pol = "<<il_pol;
  return average_north_direction;
 }

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,