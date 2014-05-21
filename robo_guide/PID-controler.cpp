inline int convert_orientation(int orient_temp, int orientation){
  //konwersja kierunku robota
  for(int i=1;i<2;i++){// for temp
    if(orient_temp>=180){//if 1
	  if(orientation>=orient_temp-180){ 
	    orientation=orientation-orient_temp;
		continue;
	   }
	  if(orientation<orient_temp-180){
	    orientation=360-(orient_temp-orientation);  
	   }
	 }//if1
	if(orient_temp<180){//if2
	  if(orientation<=orient_temp+180){
		orientation=orientation-orient_temp;
		continue;
	   }
	  if(orientation>orient_temp+180){
		orientation=(orientation-orient_temp)-360;
	   }
	 }//if2
  }// for temp
 return orientation;
}

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,