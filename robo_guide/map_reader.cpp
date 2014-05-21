#include <iostream>
#include "convert.h"

inline void size_reader(int* rows, int* cols, int* x_goal, int* y_goal,std::string file_name){
  std::ifstream plik(file_name.c_str());
  
  //sprawdzanie czy otwierany plik istnieje
  if (!plik){
    std::cerr << "Plik o podaniej nazwie nie istnieje w katalogu programu";
   }
  //koniec sprawdzania czy otwierany plik istnieje;

   if(!plik.is_open())
     throw-1;  
	 
   std::cout <<"\nOtwieranie pliku "<<file_name<<"\n";

   std::string temp;
   std::string temp2;	
   
   //odczyt wielkosci mapy   
   for(int i=1;i<5;i++){
  	 plik >> temp;
  	 std::cout<<"\nodczyt = "<<temp;
  	 temp2="";
	 if(i==1){
	   for(int j=9;j<temp.length()+1;j++){
	     temp2+=temp[j];	
	    }
	   *rows=string_to_int(temp2);
	  }
	 if(i==2){
	   for(int j=9;j<temp.length()+1;j++){
	     temp2+=temp[j];	
	    }
	   *cols=string_to_int(temp2);
	  }
	 if(i==3){
	   for(int j=7;j<temp.length()+1;j++){
	     temp2+=temp[j];	
	    }
	   *x_goal=string_to_int(temp2);
	  }
	 if(i==4){
	   for(int j=7;j<temp.length()+1;j++){
	     temp2+=temp[j];	
	    }
	   *y_goal=string_to_int(temp2);
	  }	  
	}
   plik.close();
 }

inline void map_reader(int rows_temp,int cols_temp,double ***dist_to_obstacles,int ***dist_to_field_bondary,int **new_north_direction_all,double **mapa,std::string file_name){
  std::ifstream plik(file_name.c_str());
  
  //sprawdzanie czy otwierany plik istnieje
  if (!plik){
    std::cerr << "File does not exist - check the filename ";
   }
  //koniec sprawdzania czy otwierany plik istnieje;
    
  if(!plik.is_open())
    throw-1;  
	 
  std::cout <<'\n'<<"Otwieranie pliku o nazwie "<<file_name<<"\n";
  std::cout <<"\npo nacisnieciu entera\n";
  getchar();
 
  std::string temp;
  std::string temp2;	

 for(int i=1;i<6+1;i++){
   plik >> temp;
   std::cout<<"\ntemp = "<<temp;
  }

  //ustalam tablice odleglosci od scian w ustalonych kierunkach
  //dist_to_obstacles[x][y][z], (x,y) to wspolrzedne rozwazanej pozycji na mapie
  //z to kierunek w ktorym mierzymy odleglosc do przeszkod mapy, 0-north, 1-east, 2-south, 3-west
  //double dist_to_obstacles[il_wiermapy+1][il_kolmapy+1][4];
  //read DISTANCE TO OBSTACLES
  //NORTH
  for(int i=1;i<rows_temp+1;i++){
 	for(int j=1;j<cols_temp+1;j++){ 
  	  plik >> temp;
  	  std::cout<<"\ntemp = "<<temp;
  	  temp2="";
  	  for(int k=(int_to_string(i).length()+int_to_string(j).length())+4;k<temp.length();k++){
  	    temp2+=temp[k];
	   }
  	  dist_to_obstacles[i][j][0]=string_to_double(temp2);
     }
   }
  plik >> temp;
  //EAST
  for(int i=1;i<rows_temp+1;i++){
 	for(int j=1;j<cols_temp+1;j++){ 
  	  plik >> temp;
  	  std::cout<<"\ntemp = "<<temp;
  	  temp2="";
  	  for(int k=(int_to_string(i).length()+int_to_string(j).length())+4;k<temp.length();k++){
  	    temp2+=temp[k];
	   }
  	  dist_to_obstacles[i][j][1]=string_to_double(temp2);
     }
   }
  plik >> temp;	
  //SOUTH
  for(int i=1;i<rows_temp+1;i++){
 	for(int j=1;j<cols_temp+1;j++){ 
  	  plik >> temp;
  	  std::cout<<"\ntemp = "<<temp;
  	  temp2="";
  	  for(int k=(int_to_string(i).length()+int_to_string(j).length())+4;k<temp.length();k++){
  	    temp2+=temp[k];
	   }
  	  dist_to_obstacles[i][j][2]=string_to_double(temp2);
     }
   }
  plik >> temp;	
  //WEST
  for(int i=1;i<rows_temp+1;i++){
 	for(int j=1;j<cols_temp+1;j++){ 
  	  plik >> temp;
  	  std::cout<<"\ntemp = "<<temp;
  	  temp2="";
  	  for(int k=(int_to_string(i).length()+int_to_string(j).length())+4;k<temp.length();k++){
  	    temp2+=temp[k];
	   }
  	  dist_to_obstacles[i][j][3]=string_to_double(temp2);
     }
   }
  plik >> temp;	
  plik >> temp;		

  //distance to field boundaries, define boundaries of all area boxes, it is useful for proper driving between boxes, 
  //int dist_to_field_bondary[il_wiermapy+1][il_kolmapy+1][4];
  //read FIELDS SIZE
  //NORTH
  for(int i=1;i<rows_temp+1;i++){
 	for(int j=1;j<cols_temp+1;j++){ 
  	  plik >> temp;
  	  std::cout<<"\ntemp = "<<temp;
  	  temp2="";
  	  for(int k=(int_to_string(i).length()+int_to_string(j).length())+4;k<temp.length();k++){
  	    temp2+=temp[k];
	   }
  	  dist_to_field_bondary[i][j][0]=string_to_double(temp2);
     }
   }
  plik >> temp;
  //EAST
  for(int i=1;i<rows_temp+1;i++){
 	for(int j=1;j<cols_temp+1;j++){ 
  	  plik >> temp;
  	  std::cout<<"\ntemp = "<<temp;
  	  temp2="";
  	  for(int k=(int_to_string(i).length()+int_to_string(j).length())+4;k<temp.length();k++){
  	    temp2+=temp[k];
	   }
  	  dist_to_field_bondary[i][j][1]=string_to_double(temp2);
     }
   }
  plik >> temp;	
  //SOUTH
  for(int i=1;i<rows_temp+1;i++){
 	for(int j=1;j<cols_temp+1;j++){ 
  	  plik >> temp;
  	  std::cout<<"\ntemp = "<<temp;
  	  temp2="";
  	  for(int k=(int_to_string(i).length()+int_to_string(j).length())+4;k<temp.length();k++){
  	    temp2+=temp[k];
	   }
  	  dist_to_field_bondary[i][j][2]=string_to_double(temp2);
     }
   }
  plik >> temp;	
  //WEST
  for(int i=1;i<rows_temp+1;i++){
 	for(int j=1;j<cols_temp+1;j++){ 
  	  plik >> temp;
  	  std::cout<<"\ntemp = "<<temp;
  	  temp2="";
  	  for(int k=(int_to_string(i).length()+int_to_string(j).length())+4;k<temp.length();k++){
  	    temp2+=temp[k];
	   }
  	  dist_to_field_bondary[i][j][3]=string_to_double(temp2);
     }
   }
  plik >> temp;	

  //to compensate magnetic anomalies, we use separated north directions for all fields
  //NORTH DIRECTION READER
  for(int i=1;i<rows_temp+1;i++){
 	for(int j=1;j<cols_temp+1;j++){ 
  	  plik >> temp;
  	  std::cout<<"\ntemp = "<<temp;
  	  temp2="";
  	  for(int k=(int_to_string(i).length()+int_to_string(j).length())+4;k<temp.length();k++){
  	    temp2+=temp[k];
	   }
  	  new_north_direction_all[i][j]=string_to_double(temp2);
     }
   }
  plik >> temp;

  //OBSTACLES READER
  for(int i=1;i<rows_temp+1;i++){
 	for(int j=1;j<cols_temp+1;j++){ 
  	  plik >> temp;
  	  std::cout<<"\ntemp = "<<temp;
  	  temp2="";
  	  for(int k=(int_to_string(i).length()+int_to_string(j).length())+4;k<temp.length();k++){
  	    temp2+=temp[k];
	   }
  	  mapa[i][j]=string_to_double(temp2);
     }
   }
  plik.close();
 }

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,