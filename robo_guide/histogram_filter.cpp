inline void mapa_init_prob(double **mapa, int il_wierszy, int il_kolumn){
  std::cout<< "\nInicjujemy ka¿dy obszar mapy tym samym prawdopodobienstwem\n";

  //zliczamy ilosc dostepnych pol mapy,
  int il_pol=0;
  for(int i=1;i<il_wierszy+1;i++){
	for(int j=1;j<il_kolumn+1;j++){
	  if(mapa[i][j]==0){
		il_pol++;
	   }
	 }
   }
  //wypelniam mape rownym prawdopodobienstwem,
  for(int i=1;i<il_wierszy+1;i++){
	for(int j=1;j<il_kolumn+1;j++) {
	  if(mapa[i][j]==0){mapa[i][j]=floor(((double)1/il_pol)*1000)/1000;}
	 }
   }
  std::cout<<"\n Prawopodobienstwo inicjacyjne \n";
  for(int i=1;i<il_wierszy+1;i++){for(int j=1;j<il_kolumn+1;j++){if(mapa[i][j]==5){std::cout<<" xxxxx";}else{std::cout<<" "<<mapa[i][j];}}std::cout<<"\n";} 
 }

//odswiezam prawdopodobienstwo polozenia robota po pomiarze z sensorow - regula bayesa
inline void p_update_after_sense(Comm::NXTComm comm, int current_direction,int new_north_direction,int direction,double *ktemp,int il_wiermapy,int il_kolmapy, double **mapa,double ***dist_to_obstacles){
  double suma_mapa;
  //double mapa_temp;
  double mapa_temp1;
  double mapa_temp2;
  for(int i1=0;i1<4;i1++){
    //funkcja pozwala na oddalenie sie od scian w celu umozliwienia obrotu robota,
    odjedz_od_scian(comm,current_direction,new_north_direction);
    direction=i1;
    current_direction=direction;
    turn_to_specified_map_direction(comm,IN_4,direction,new_north_direction);	
	suma_mapa=0;			
	std::ofstream plikx;
	plikx.open("status.txt");
    plikx<<direction+5;
	plikx.close();
	sense2(comm,IN_3,IN_2,IN_4,direction,ktemp);

	//aktualizacja p-stwa po odczycie sensorycznym,
    for(int i=1;i<il_wiermapy+1;i++){
	  for(int j=1;j<il_kolmapy+1;j++) {
		if(mapa[i][j]!=5){
		  //porownywanie odleglosci na wprost z oczekiwana na mapie
		  mapa_temp1=(1-(abs(dist_to_obstacles[i][j][direction]-ktemp[direction])/(dist_to_obstacles[i][j][direction]+ktemp[direction])));
		  //porownywanie odleglosci lewej i prawej strony robota, z oczekiwanymi na mapie
		  mapa_temp2=	1-(abs((dist_to_obstacles[i][j][(direction+3)%4]+dist_to_obstacles[i][j][(direction+1)%4])-(ktemp[(direction+3)%4]+ktemp[(direction+1)%4]))/((dist_to_obstacles[i][j][(direction+3)%4]+dist_to_obstacles[i][j][(direction+1)%4])+(ktemp[(direction+3)%4]+ktemp[(direction+1)%4])));
		  mapa[i][j]=mapa[i][j]*((mapa_temp1+mapa_temp2)/2);
		  suma_mapa=suma_mapa+mapa[i][j];
		  /*
		  mapa_temp=0;
		  for(int k=0;k<4;k++){
			if(k!=(direction+2)%4){
		      mapa_temp=mapa_temp+(1-(abs(dist_to_obstacles[i][j][k]-ktemp[k])/(dist_to_obstacles[i][j][k]+ktemp[k])));
			 }
		   }
		  mapa[i][j]=mapa[i][j]*(mapa_temp/3);
		  suma_mapa=suma_mapa+mapa[i][j];
		  */

		 }
	   }
     }
    std::cout<<"\n Prawopodobienstwo przed normalizacja \n";
	for(int i=1;i<il_wiermapy+1;i++){for(int j=1;j<il_kolmapy+1;j++) {if(mapa[i][j]==5){std::cout<<" xxxxx";}else{std::cout<<" "<<mapa[i][j];}}std::cout<<"\n";} 
	  //normalizacja prawdopodobienstwa polozenia,
	  for(int i=1;i<il_wiermapy+1;i++){
		for(int j=1;j<il_kolmapy+1;j++){
		  if(mapa[i][j]!=5){
			mapa[i][j]=mapa[i][j]/suma_mapa;
		   }
		 }
	   }  
	  std::cout<<"\n Prawopodobienstwo po normalizacji \n";
	  for(int i=1;i<il_wiermapy+1;i++){for(int j=1;j<il_kolmapy+1;j++) {if(mapa[i][j]==5){std::cout<<" xxxxx";}else{std::cout<<" "<<mapa[i][j];}}std::cout<<"\n";} 
	}
  }

inline void p_update_after_move(Comm::NXTComm comm, int current_direction,int ilosc_krokow,int il_wiermapy,int il_kolmapy, double **mapa, double move_precision){
  //deklaracja mapy tymczasowej
  int rows = il_wiermapy+1;
  double **mapa_temp;
  mapa_temp = new double*[rows];
  while(rows--) mapa_temp[rows] = new double[il_kolmapy+1];
  //petla liczby splotow,
  for(int k=1;k<ilosc_krokow+1;k++){
    for(int i=1;i<il_wiermapy+1;i++){
      for(int j=1;j<il_kolmapy+1;j++){
	    if(mapa[i][j]!=5){
	      mapa_temp[i][j]=0;
	     }
		else{
	      mapa_temp[i][j]=mapa[i][j];
	     }
	   }
     }
    if(current_direction==0){
      for(int i=1;i<il_wiermapy+1;i++){
        for(int j=1;j<il_kolmapy+1;j++){
	      if(mapa_temp[i][j]!=5){
		    if(i+2<=il_wiermapy && mapa[i+2][j]!=5){
		      mapa_temp[i][j]+=((1-move_precision)/2)*mapa[i+2][j];
		     }
		    if(i+1<=il_wiermapy && mapa[i+1][j]!=5){
		      mapa_temp[i][j]+=move_precision*mapa[i+1][j];
		     }
		    mapa_temp[i][j]+=((1-move_precision)/2)*mapa[i][j];
	       }
	     }
       }
     }
    if(current_direction==1){
      for(int i=1;i<il_wiermapy+1;i++){
        for(int j=1;j<il_kolmapy+1;j++){
	      if(mapa_temp[i][j]!=5){
		    if(j-2>=1 && mapa[i][j-2]!=5){
		      mapa_temp[i][j]+=((1-move_precision)/2)*mapa[i][j-2];
		     }
		    if(j-1>=1 && mapa[i][j-1]!=5){
		      mapa_temp[i][j]+=move_precision*mapa[i][j-1];
		     }
		    mapa_temp[i][j]+=((1-move_precision)/2)*mapa[i][j];
	       }
	     }
       }
     }
    if(current_direction==2){
      for(int i=1;i<il_wiermapy+1;i++){
        for(int j=1;j<il_kolmapy+1;j++){
	      if(mapa_temp[i][j]!=5){
		    if(i-2>=1 && mapa[i-2][j]!=5){
		      mapa_temp[i][j]+=((1-move_precision)/2)*mapa[i-2][j];
		     }
		    if(i-1>=1 && mapa[i-1][j]!=5){
		      mapa_temp[i][j]+=move_precision*mapa[i-1][j];
		     }
		    mapa_temp[i][j]+=((1-move_precision)/2)*mapa[i][j];
	       }
	     }
       }
     }
    if(current_direction==3){
      for(int i=1;i<il_wiermapy+1;i++){
        for(int j=1;j<il_kolmapy+1;j++){
	      if(mapa_temp[i][j]!=5){
		    if(j+2<=il_kolmapy && mapa[i][j+2]!=5){
		      mapa_temp[i][j]+=((1-move_precision)/2)*mapa[i][j+2];
		     }
		    if(j+1<=il_kolmapy && mapa[i][j+1]!=5){
		      mapa_temp[i][j]+=move_precision*mapa[i][j+1];
		     }
		    mapa_temp[i][j]+=((1-move_precision)/2)*mapa[i][j];
	       }
	     }
       }
     }
    //wpisuje do mapy wartosci pstwa po splocie,
    for(int i=1;i<il_wiermapy+1;i++){
      for(int j=1;j<il_kolmapy+1;j++){
	    mapa[i][j]=mapa_temp[i][j];
	   }
     } 
   }//petla liczby splotow,

  //czyszczenie pamieci
  for(int i=0;i<il_wiermapy+1;i++){
    delete[] mapa_temp[i];}//czyscimy wiersze
    delete[] mapa_temp;//zwalniamy tablice wskaznikow do wierszy
   }

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,