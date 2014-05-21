//turn_to_specified_map_direction(&comm,IN_1,direct_temp,new_north_direction), direct_temp=0-N,1-E,2-S,3-W 
inline int turn_to_specified_map_direction(Comm::NXTComm comm, int port, int direct_temp, int new_north_direction){
  int orient_temp;
  double alpha=0.3;
  int dokladnosc=50;
  int speed=15;
  int prec=2;
  int orientation;
  int cte;
  int k1,k2;
  int end_pointer=0;
  if(direct_temp==0){orient_temp=new_north_direction;}
  if(direct_temp==1){orient_temp=(new_north_direction+90)%360;}
  if(direct_temp==2){orient_temp=(new_north_direction+180)%360;}
  if(direct_temp==3){orient_temp=(new_north_direction+270)%360;}
  do
   {// the begging of do while loop
    orientation=2*NXT::Sensor::GetSonarValue(&comm, port);
	std::cout<<"\nOrientation = "<<orientation; 
	orientation=convert_orientation(orient_temp,orientation);

	//sterowanie robotem na podstawie skonwertowanej orientacji
	cte=orientation;		
	k1=speed;
	k2=speed;

	if(cte<=0){//if cte1
	  if(abs(cte)>dokladnosc){
		k1=speed;//new
		k2=-speed;//(speed-(alpha*cte));
	   }
	  if(abs(cte)<dokladnosc && abs(cte)>prec){
		//k1=-((speed)+(alpha*cte));
		//k2=(speed)+(alpha*cte);
		k1=-1*(alpha*cte);
		k2=(alpha*cte);
		//k2=speed*(alpha*cte);
	   }
	  if(abs(cte)<=prec){
		k1=0;
		k2=0;
		//k2=speed+(alpha*cte);
	   }
	 }//if cte1
	if(cte>0){//if cte2
	  if(cte>dokladnosc){
	    k1=-speed;//(speed+(alpha*cte));
		k2=speed;//new
	   }
	  if(abs(cte)<dokladnosc && abs(cte)>prec){
		k1=-1*(alpha*cte);
		k2=((alpha*cte));
		//k1=speed*(alpha*cte);
	   }
	  if(abs(cte)<=prec){
		k2=0;
		k1=0;
		//k1=speed-(alpha*cte);
	   }
	 }//if cte2  
	NXT::Motor::SetForward(&comm, OUT_C, k1);//lewe
    NXT::Motor::SetForward(&comm, OUT_B, k2);//prawe
	Wait(50);
	if(k1==0 && k2==0){end_pointer=1;}
   }// the end of do while loop
   while(end_pointer!=1);
   return 0;
 } 

//drive_PD_in_specified_map_direction(&comm,IN_1,IN_4,direct_temp,new_north_direction), direct_temp=0-N,1-E,2-S,3-W, IN_1 port kompasu, IN_4 port sonaru,
inline int drive_PD_in_specified_map_direction(Comm::NXTComm comm, int port, int port2, int direct_temp, int new_north_direction){
  int orient_temp;
  double alpha=0.2;
  int dokladnosc=80;
  int speed=30;
  int pointer007=0;
  int orientation;
  int cte;
  int k1,k2;
  int end_pointer=0;
  char decyzja;

  if(direct_temp==0){orient_temp=new_north_direction;}
  if(direct_temp==1){orient_temp=(new_north_direction+90)%360;}
  if(direct_temp==2){orient_temp=(new_north_direction+180)%360;}
  if(direct_temp==3){orient_temp=(new_north_direction+270)%360;}

  do
   {// the begging of do while loop
   if(NXT::Sensor::GetSonarValue(&comm, port2)<=70){
	 NXT::Motor::SetForward(&comm, OUT_C, 0);//lewe
	 NXT::Motor::SetForward(&comm, OUT_B, 0);//prawe
	 break;
	}
   //kalibracja do ustalonego kierunku direct_temp
   if(pointer007==0){
	 turn_to_specified_map_direction(comm,port,direct_temp,new_north_direction);
	 pointer007=1;
    }
   //koniec kalibracji  do ustalonego kierunku direct_temp

   orientation=2*NXT::Sensor::GetSonarValue(&comm, port);
   std::cout<<"\nOrientation = "<<orientation;
   orientation=convert_orientation(orient_temp,orientation);

   //sterowanie robotem na podstawie skonwertowanej orientacji

   cte=orientation;
				
   k1=speed;
   k2=speed;

   if(abs(cte)>45){
	 pointer007=0;
    }
   if(cte<=0){//if cte1
     if(abs(cte)>dokladnosc){
	   k2=0;
	  }
	 else{
	   k2=speed+(alpha*cte);
	  }
	}//if cte1
   if(cte>0){//if cte2
     if(cte>dokladnosc){
	   k1=0;
	  }
	 else{
	   k1=speed-(alpha*cte);
	  }
    }//if cte2
				  
   NXT::Motor::SetForward(&comm, OUT_C, k1);//lewe
   NXT::Motor::SetForward(&comm, OUT_B, k2);//prawe
   Wait(50);

   std::cout<<"\nk1 = "<<k1;
   std::cout<<"\nk2 = "<<k2;

   if(kbhit()==true){// if kbhit
     decyzja=getch();
	 if(decyzja=='k' || decyzja=='K'){
	   end_pointer=1;
	  }
	 if(decyzja=='n' || decyzja=='N'){
	   orient_temp=new_north_direction;
	   direct_temp=0;
	   pointer007=0;    
	  }
	 if(decyzja=='s' || decyzja=='S'){   
	   orient_temp=(new_north_direction+180)%360;;
	   direct_temp=2;
	   pointer007=0;
	  }
	 if(decyzja=='e' || decyzja=='E'){
	   orient_temp=(new_north_direction+90)%360;;
	   direct_temp=1;
	   pointer007=0;
	  }
 	 if(decyzja=='w' || decyzja=='W'){
	   orient_temp=(new_north_direction+270)%360;;
	   direct_temp=3;
	   pointer007=0;
	  }
	}// if kbhit
  }// the end of do while loop
 while(end_pointer!=1);
 return 0;
} 

//drive_PD_in_specified_map_direction_between_walls(&comm,IN_1,IN_4,direct_temp,new_north_direction), direct_temp=0-N,1-E,2-S,3-W, IN_1 port kompasu, IN_4 port sonaru przod,
inline int drive_PD_in_specified_map_direction_between_walls(Comm::NXTComm comm, int port, int port2, int direct_temp, int new_north_direction, int ilosc_taktow){
  int orient_temp;
  //int old_orient_temp;
  //old_orient_temp=2*NXT::Sensor::GetSonarValue(&comm, port);
  double alpha=0.2;
  int dokladnosc=80;
  int speed=30;
  int pointer007=0;
  int orientation;
  int cte;
  int k1,k2;
  int end_pointer=0;
  char decyzja;

  if(direct_temp==0){orient_temp=new_north_direction;}
  if(direct_temp==1){orient_temp=(new_north_direction+90)%360;}
  if(direct_temp==2){orient_temp=(new_north_direction+180)%360;}
  if(direct_temp==3){orient_temp=(new_north_direction+270)%360;}

  int temp1,temp2;
  int old_temp1,old_temp2;
  int numer_taktu=0;
  do
   {// the begging of do while loop
	numer_taktu++;
	if(numer_taktu==ilosc_taktow){
	  std::cout<<"\nilosc taktow = "<<numer_taktu;
	  NXT::Motor::SetForward(&comm, OUT_C, 0);//lewe
	  NXT::Motor::SetForward(&comm, OUT_B, 0);//prawe
	  NXT::PlaySoundFile(&comm,"R2D2a.rso",1);
	  Wait(1300);
	  NXT::StopSound(&comm);
	  break;
	 }
   //cout<<"\n\nPoczatkowy orient_temp = "<<orient_temp<<"\n\n";
   //temp1=NXT::Sensor::GetSonarValue(&comm,IN_3); 
   temp1=NXT::Sensor::GetSMUXvalue(&comm, IN_2, 4, 1); 
   //left
   if(temp1<80 && old_temp1-1>=temp1){
	 orient_temp=(orient_temp+3)%360;
	 std::cout<<"\n\nz lewej przeszkoda odl "<<temp1;
	 std::cout<<"  orient_temp = "<<orient_temp;  
	 //if(abs(old_orient_temp-NXT::Sensor::GetSonarValue(&comm, IN_1))>50)
	 // {
	 //orient_temp=(orient_temp+6)%360;
	 //cout<<"\n\nEKSTRA ODDALENIE ZE WZGLEDU NA ANOMALIE MAGNETYCZNE";
	 //}
	}
   old_temp1=temp1;
   //temp2=NXT::Sensor::GetSonarValue(&comm, IN_2);
   temp2=NXT::Sensor::GetSMUXvalue(&comm, IN_2, 2, 1);
   //right
   if(temp2<80 && old_temp2-1>=temp2){
	 orient_temp=(orient_temp+(360-3))%360;
	 std::cout<<"\n\nz prawej przeszkoda odl "<<temp2;
	 std::cout<<"  orient_temp = "<<orient_temp;
	 //if(abs(old_orient_temp-NXT::Sensor::GetSonarValue(&comm, IN_1))>50)
	 //{
	 //orient_temp=(orient_temp+(360-6))%360;
	 //cout<<"\n\nEKSTRA ODDALENIE ZE WZGLEDU NA ANOMALIE MAGNETYCZNE";
	 //}
	}
   old_temp2=temp2;  
   //if(NXT::Sensor::GetSonarValue(&comm, IN_4)<=20)
   if(NXT::Sensor::GetSMUXvalue(&comm, IN_2, 1, 1)<=20){
	 NXT::Motor::SetForward(&comm, OUT_C, 0);//lewe
	 NXT::Motor::SetForward(&comm, OUT_B, 0);//prawe
	 std::cout<<"napotkalem przeszkode, dokonuje ponownej lokalizacji";
	 NXT::PlaySoundFile(&comm,"R2D2a.rso",1);
	 Wait(1300);
	 NXT::StopSound(&comm);
	 break;
	}
			 
   //kalibracja do ustalonego kierunku direct_temp
   if(pointer007==0){
	 turn_to_specified_map_direction(comm,port,direct_temp,new_north_direction);
	 pointer007=1;
    }
   //koniec kalibracji  do ustalonego kierunku direct_temp

	orientation=2*NXT::Sensor::GetSonarValue(&comm, port);
	//old_orient_temp=orientation;
	std::cout<<"\nOrientation = "<<orientation;
	orientation=convert_orientation(orient_temp,orientation);
			  
	//sterowanie robotem na podstawie skonwertowanej orientacji

	cte=orientation;
				
	k1=speed;
	k2=speed;

	if(abs(cte)>45){
	  pointer007=0;
	 }
	if(cte<=0){//if cte1
	  if(abs(cte)>dokladnosc){
	    k2=0;
	   }
	  else{
	    k2=speed+(alpha*cte);
	   }
	 }//if cte1
	if(cte>0){//if cte2
	  if(cte>dokladnosc){
		k1=0;
	   }
	  else{
		k1=speed-(alpha*cte);
	   }
	 }//if cte2
				  
	NXT::Motor::SetForward(&comm, OUT_C, k1);//lewe
	NXT::Motor::SetForward(&comm, OUT_B, k2);//prawe
	Wait(50);

	std::cout<<"\nk1 = "<<k1;
	std::cout<<"\nk2 = "<<k2;

	if(kbhit()==true){// if kbhit
	  decyzja=getch();
	  if(decyzja=='k' || decyzja=='K'){
		end_pointer=1;
	   }
	  if(decyzja=='n' || decyzja=='N'){
		orient_temp=new_north_direction;
		direct_temp=0;
		pointer007=0;    
	   }
	  if(decyzja=='s' || decyzja=='S'){   
		orient_temp=(new_north_direction+180)%360;;
		direct_temp=2;
		pointer007=0;
	   }
	  if(decyzja=='e' || decyzja=='E'){
		orient_temp=(new_north_direction+90)%360;;
		direct_temp=1;
		pointer007=0;
	   }
	  if(decyzja=='w' || decyzja=='W'){
		orient_temp=(new_north_direction+270)%360;;
		direct_temp=3;
		pointer007=0;
	   }
	 }// if kbhit
   }// the end of do while loop
  while(end_pointer!=1);
 return 0;
 } 

 inline int drive_PD_left_wall(Comm::NXTComm comm, int port, int port2, int direct_temp, int new_north_direction){
    int orient_temp;
	double alpha=0.2;
	int dokladnosc=80;
	int speed=30;
	int pointer007=0;
	int orientation;
	int cte;
	int k1,k2;
	int end_pointer=0;
	char decyzja;

	if(direct_temp==0){orient_temp=new_north_direction;}
	if(direct_temp==1){orient_temp=(new_north_direction+90)%360;}
	if(direct_temp==2){orient_temp=(new_north_direction+180)%360;}
	if(direct_temp==3){orient_temp=(new_north_direction+270)%360;}

	int temp1,temp2,temp_average;
	int temp1_old, temp2_old;
	do
	{// the begging of do while loop
      //cout<<"\n\nPoczatkowy orient_temp = "<<orient_temp<<"\n\n";
	  temp1=NXT::Sensor::GetSonarValue(&comm,IN_3); 
	  temp2=NXT::Sensor::GetSonarValue(&comm, IN_2);
	  temp_average=(temp1+temp2)/2;
	  std::cout<<"\ntemp_average = "<<temp_average;
	  //left
	  if(temp_average>temp1 && temp1_old-1>=temp1){
		orient_temp=(orient_temp+3)%360;
		std::cout<<"\n\nz lewej przeszkoda odl "<<temp1;
		std::cout<<" orient_temp = "<<orient_temp;  
	   }
	  temp1_old=temp1;
	  //right
	  if(temp_average>temp2 && temp2_old-1>=temp2){
		orient_temp=(orient_temp+(360-3))%360;
		std::cout<<"\n\nz prawej przeszkoda odl "<<temp2;
		std::cout<<"  orient_temp = "<<orient_temp;
	   }
	  temp2_old=temp2;
      //kalibracja do ustalonego kierunku direct_temp
	  if(pointer007==0){
		turn_to_specified_map_direction(comm,port,direct_temp,new_north_direction);
		pointer007=1;
	   }
 	  //koniec kalibracji  do ustalonego kierunku direct_temp

	   orientation=2*NXT::Sensor::GetSonarValue(&comm, port);
	   std::cout<<"\nOrientation = "<<orientation;
	   orientation=convert_orientation(orient_temp,orientation);

	   //sterowanie robotem na podstawie skonwertowanej orientacji

	   cte=orientation;
				
	    k1=speed;
	    k2=speed;

		if(abs(cte)>45){
		  pointer007=0;
		 }

		if(cte<=0){//if cte1
		  if(abs(cte)>dokladnosc){
				k2=0;
			}
			else{
			  k2=speed+(alpha*cte);
			}
		 }//if cte1
		if(cte>0){//if cte2
		  if(cte>dokladnosc){
			k1=0;
		   }
		  else{
		    k1=speed-(alpha*cte);
		   }
		  }//if cte2
				  
		  NXT::Motor::SetForward(&comm, OUT_C, k1);//lewe
	  	  NXT::Motor::SetForward(&comm, OUT_B, k2);//prawe
		  Wait(50);

		  std::cout<<"\nk1 = "<<k1;
		  std::cout<<"\nk2 = "<<k2;

		  if(kbhit()==true){// if kbhit
			decyzja=getch();
			if(decyzja=='k' || decyzja=='K'){
			  end_pointer=1;
			 }
			if(decyzja=='n' || decyzja=='N'){
			  orient_temp=new_north_direction;
			  direct_temp=0;
			  pointer007=0;    
			 }
			if(decyzja=='s' || decyzja=='S'){   
			  orient_temp=(new_north_direction+180)%360;;
			  direct_temp=2;
			  pointer007=0;
			 }
			if(decyzja=='e' || decyzja=='E'){
			  orient_temp=(new_north_direction+90)%360;;
			  direct_temp=1;
			  pointer007=0;
			 }
			if(decyzja=='w' || decyzja=='W'){
			  orient_temp=(new_north_direction+270)%360;;
			  direct_temp=3;
			  pointer007=0;
			 }
		   }// if kbhit
	     }// the end of do while loop
	  while(end_pointer!=1);
   return 0;
 }

 //funkcja szacuje ilosc taktow do przejechania, w celu zblizenia sie do ustalonego celu,
 inline int il_takt(Comm::NXTComm comm,int x_goal,int y_goal,int actual_x_pos,int actual_y_pos, int ***dist_to_field_bondary, int direction){
    //ustalam ile taktow mam jechac w strone celu, za pomoca taktow mozliwy jest splot,
	// mocno dopasowana wartosc do ksztaltu mapy, nalezy przeformatowac w przypadku innej mapy
	int temp_val;
	int ilosc_taktow;
	if(direction==0){
	  if(actual_y_pos==y_goal){
	    temp_val=0;
	    for(int i=0;i<abs(actual_x_pos-x_goal);i++){
		  temp_val=temp_val+dist_to_field_bondary[actual_x_pos-i][actual_y_pos][direction]+dist_to_field_bondary[actual_x_pos-i-1][actual_y_pos][(direction+2)%4];
		 }
	    ilosc_taktow=temp_val/7;
	   }
	  else{
	    temp_val=0;
	    for(int i=0;i<abs(actual_x_pos-1);i++){
		  temp_val=temp_val+dist_to_field_bondary[actual_x_pos-i][actual_y_pos][direction]+dist_to_field_bondary[actual_x_pos-i-1][actual_y_pos][(direction+2)%4];
		 }
	    ilosc_taktow=temp_val/7;
	    //ilosc_taktow=(dist_to_field_bondary[actual_x_pos][actual_y_pos][direction]+dist_to_field_bondary[actual_x_pos-1][actual_y_pos][(direction+2)%4])/7;
	   }
	 }
	if(direction==1){
	  if(actual_x_pos==x_goal){
	    temp_val=0;
	    for(int i=0;i<abs(actual_y_pos-y_goal);i++){
		  temp_val=temp_val+dist_to_field_bondary[actual_x_pos][actual_y_pos+i][direction]+dist_to_field_bondary[actual_x_pos][actual_y_pos+i+1][(direction+2)%4];
		 }
	    ilosc_taktow=temp_val/7;
	   }
	  else{
	   temp_val=0;
	   for(int i=0;i<abs(actual_y_pos-6);i++){
		 temp_val=temp_val+dist_to_field_bondary[actual_x_pos][actual_y_pos+i][direction]+dist_to_field_bondary[actual_x_pos][actual_y_pos+i+1][(direction+2)%4];
		}
	    ilosc_taktow=temp_val/7;
	    //ilosc_taktow=(dist_to_field_bondary[actual_x_pos][actual_y_pos][direction]+dist_to_field_bondary[actual_x_pos][actual_y_pos+1][(direction+2)%4])/7;
	   }
	 }
	if(direction==2){
	  if(actual_y_pos==y_goal){
	    temp_val=0;
	    for(int i=0;i<abs(actual_x_pos-x_goal);i++){
		  temp_val=temp_val+dist_to_field_bondary[actual_x_pos+i][actual_y_pos][direction]+dist_to_field_bondary[actual_x_pos+i+1][actual_y_pos][(direction+2)%4];
		 }
	    ilosc_taktow=temp_val/7;
	   }
	  else{
	    temp_val=0;
	    for(int i=0;i<abs(actual_x_pos-3);i++){
		  temp_val=temp_val+dist_to_field_bondary[actual_x_pos+i][actual_y_pos][direction]+dist_to_field_bondary[actual_x_pos+i+1][actual_y_pos][(direction+2)%4];
		 }
	    ilosc_taktow=temp_val/7;
	    //ilosc_taktow=(dist_to_field_bondary[actual_x_pos][actual_y_pos][direction]+dist_to_field_bondary[actual_x_pos+1][actual_y_pos][(direction+2)%4])/7;
	   }
	 }
	if(direction==3){
	  if(actual_x_pos==x_goal){
	    temp_val=0;
	    for(int i=0;i<abs(actual_y_pos-y_goal);i++){
		  temp_val=temp_val+dist_to_field_bondary[actual_x_pos][actual_y_pos-i][direction]+dist_to_field_bondary[actual_x_pos][actual_y_pos-i-1][(direction+2)%4];
		 }
	    ilosc_taktow=temp_val/7;  
	   }
	  else{
	    temp_val=0;
	    for(int i=0;i<abs(actual_y_pos-1);i++){
		  temp_val=temp_val+dist_to_field_bondary[actual_x_pos][actual_y_pos-i][direction]+dist_to_field_bondary[actual_x_pos][actual_y_pos-i-1][(direction+2)%4];
		 }
	    ilosc_taktow=temp_val/7;
	    //ilosc_taktow=(dist_to_field_bondary[actual_x_pos][actual_y_pos][direction]+dist_to_field_bondary[actual_x_pos][actual_y_pos-1][(direction+2)%4])/7;
	   }
	 }
  return ilosc_taktow;
 } 

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,