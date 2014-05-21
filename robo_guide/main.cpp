//Oprogramowanie pozwalajace na lokalizacje w budynku opisanym mapa w formacie .bmf (building map format)

//podstawowe
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <conio.h>

//dodatkowe
#include "NXT++.h"
#include "PID-controler.h"
#include "radar.h"
#include "drive.h"
#include "move_policy.h"
#include "magnetic_anomalies.h"
#include "obstacle_avoidance.h"
#include "histogram_filter.h"
#include "additions.h"
#include "map_reader.h"

//przestrzenie nazw
using namespace std;
using namespace NXT;

Comm::NXTComm comm;

int main(){
  std::cout << "\nSzukanie sygnalu ... ";
  //inicjalizacja NXT
  if(NXT::OpenBT(&comm)){
    std::cout<< "Sygnal odnaleziony";
    std::cout<<"\nPoziom baterii = "<<NXT::BatteryLevel(&comm)/100<<" percent";
    std::cout<<"\nDostepna pamiec Flash = "<<NXT::GetAvailableFlash(&comm);

	//zmienne pomocnicze
	int temp;
	int direction_calib_temp=0;//zmienna potrzebna w kalibracji do mapy
	int new_north_direction;//po kalibracji pamietana jest inicjacyjna polnoc
	int x=30;//inicjacyjna predkasc jazdy w przod i tyl
	int xtemp=30;//wartosc do weryfikacji zmiany predkosci
	int y=20;//inicjacyjna predkosc obracania sie w lewo lub prawo
	int ytemp=20;//wartosc do weryfikacji zmiany predkosci
	char last_value;//przydatna w odswiezaniu predkosci
	char decyzja;//wartosc uzywana do sterowania
	int infinity_loop_pointer=1;//zmienna uzywana w glownej, nieskonczonej petli do while,

	//ustawinia sprzetowe
	NXT::Sensor::SetSonar(&comm, IN_4);//inicjacja kompasu
    NXT::Sensor::SetTouch(&comm, IN_3);//inicjacja czujnika dotyku

	NXT::StartProgram(&comm,"Lokalizacja_Silver_Shadow");

    //the beginning of infinity filtration,
	do
	 {//glowna petla nieskonczona
      //obsluga przedniego czujnika dotyku,
	  if(NXT::Sensor::GetValue(&comm, IN_3)==1){
	    NXT::Motor::Stop(&comm, OUT_B, 0);
	    NXT::Motor::Stop(&comm, OUT_C, 0);
	    std::cout<< "\n\nWykryta przeszkoda - silniki zatrzymane";
	    NXT::PlaySoundFile(&comm,"R2D2c.rso",1);
	    Wait(1500);
	    NXT::StopSound(&comm);
	   }
    if(kbhit()==true){
	  system("CLS");
	  cout<<"\n ___Podstawowe sterowanie___";
	  cout<<"\n A - Skrecanie w lewo"; 
	  cout<<"\n D - Skrecanie w prawo"; 
	  cout<<"\n W - Jazda na wprost"; 
	  cout<<"\n S - Jazda do tylu"; 
	  cout<<"\n M - Przyspieszenie jazdy na wprost";
	  cout<<"\n N - Zwolnienie jazdy na wprost";
	  cout<<"\n H - Zatrzymanie silnikow";
	  cout<<"\n K - Koniec Programu";			
	  cout<<"\n\n ___Zaawansowane sterowanie___";
	  cout<<"\n O, ], [ - R2D2 dzwieki";
	  cout<<"\n Z - Lokalizacja na mapie";

	  decyzja=getch();

	  if((decyzja=='N' || decyzja=='n') && x>0){
        x-=10; 
		std::cout<< "\nPredkosc jazdy = "<<x;
	   }
	  
	  if((decyzja=='M' || decyzja=='m') && x<100){
        x+=10; 
		std::cout<< "\nPredkosc jazdy = "<<x;
	   }

	  if((decyzja=='V' || decyzja=='v') && x>0){
        y--; 
		std::cout<< "\nPredkosc obrotu = "<<y;
	   }

	   if((decyzja=='B' || decyzja=='b') && x<100){
         y++; 
		 std::cout<< "\nPredkosc obrotu = "<<y;
		}

	   if(decyzja=='A' || decyzja=='a' || (ytemp!=y && last_value=='a')){
	     ytemp=y;
		 last_value='a';
		 NXT::Motor::SetForward(&comm, OUT_B, y);
		 NXT::Motor::SetReverse(&comm, OUT_C, y);
		}

	   if(decyzja=='o' || decyzja=='O'){
	     NXT::PlaySoundFile(&comm,"R2D2c.rso",1);
		 Wait(1500);
		 NXT::StopSound(&comm);
		}

	   if(decyzja=='['){
	     NXT::PlaySoundFile(&comm,"sadr2d2.rso",1);
		 Wait(700);
		 NXT::StopSound(&comm);
		}

	   if(decyzja==']'){
		 NXT::PlaySoundFile(&comm,"R2D2e.rso",1);
		 Wait(900);
		 NXT::StopSound(&comm);
		}

	   if(decyzja=='D' || decyzja=='d' || (ytemp!=y && last_value=='d')){
		 ytemp=y;
		 last_value='d';
		 NXT::Motor::SetReverse(&comm, OUT_B, y);
		 NXT::Motor::SetForward(&comm, OUT_C, y);
		}

	   if(decyzja=='W' || decyzja=='w' || (xtemp!=x && last_value=='w')){
	     xtemp=x;
		 last_value='w';
		 NXT::Motor::SetForward(&comm, OUT_B, x);
		 NXT::Motor::SetForward(&comm, OUT_C, x);
		}

	   if(decyzja=='S' || decyzja=='s'  || (xtemp!=x && last_value=='s')){
		 xtemp=x;
		 last_value='s';
		 NXT::Motor::SetReverse(&comm, OUT_B, x);
		 NXT::Motor::SetReverse(&comm, OUT_C, x);
		}

	   if(decyzja=='H' || decyzja=='h'){
		 NXT::Motor::SetForward(&comm, OUT_B, 0);
		 NXT::Motor::SetForward(&comm, OUT_C, 0);
		 NXT::Motor::Stop(&comm, OUT_B, 0);
		 NXT::Motor::Stop(&comm, OUT_C, 0);
		}

	   if(decyzja=='K' || decyzja=='k'){
		 NXT::Motor::Stop(&comm, OUT_B, 0);
		 NXT::Motor::Stop(&comm, OUT_C, 0);
		 NXT::Motor::Stop(&comm, OUT_A, 0);
		 NXT::Sensor::SetSonarOff(&comm, IN_4);
		 NXT::Sensor::SetSonarOff(&comm, IN_1);
		 break;
		}

	   //poczatek lokalizacji
	   if((decyzja=='Z' || decyzja=='z')){ 
		 //string name="E08-grid.bmf";//the whole information about environment,	
		 string name="E0-east.bmf";//the whole information about environment,	

		 //inicjacja tablicy odczytu z radaru,
		 double ktemp[4]; ktemp[0]=255; ktemp[1]=255; ktemp[2]=255; ktemp[3]=255;

		 //kalibracja kierunku polnocnego na mapie, oraz wlaczenie multiplekserow,
		 if(direction_calib_temp==0){
		   //wlaczenie multipleksera czujnikow,
		   NXT::Sensor::SetSMUX(&comm, IN_2);
		   //wlaczenie multipleksera czujnikow dotyku,
		   //NXT::Sensor::SetTMUX(&comm, IN_1);
		   cout<<"\n\nUstaw robota w kierunku polnocnym mapy i wcisnij ENTER";
		   getchar();
		   new_north_direction=2*NXT::Sensor::GetSonarValue(&comm, IN_4);
		   direction_calib_temp=1;
		   cout<<"\n new_north_direction = "<<new_north_direction;
		   cout<<"\n...kalibracja ukonczona pomyslnie nacisnij Z aby wlaczyc lokalizacje\n";
		   continue;
		  }

		 //dane do systemu wizualizacji
		 //aktualna pozycja inicjacyjna 0 0, na poczatku nie wiemy gdzie jest robot,
		 zapisz_do_pliku("actualpos.txt", "0 0");
		 //status 0 oznacza brak czynnosci, robot czeka na polecenia,
		 zapisz_do_pliku("status.txt", "0");
		 //danych do systemu wizualizacji

		 int current_direction=0;//inicjacyjny kierunek (north)

		 //Pobieranie rozmiaru mapy oraz inicjacyjnej pozycji docelowej z pliku .bmf
		 int il_wiermapy, il_kolmapy;
		 int * rows_temp = new int;
		 int * cols_temp = new int;
 		 int x_goal, y_goal;
		 int * x_goal_temp = new int;
		 int * y_goal_temp = new int;
		 size_reader(rows_temp,cols_temp,x_goal_temp,y_goal_temp,name.c_str());
		 il_wiermapy=*rows_temp;
		 il_kolmapy=*cols_temp;
		 delete rows_temp;
		 delete cols_temp;
 		 x_goal=*x_goal_temp;
		 y_goal=*y_goal_temp;
		 delete x_goal_temp;
		 delete y_goal_temp;

		 cout<<"\n Wczytywanie mapy";
		 cout<<"\n liczba_wiermapy mapy = "<<il_wiermapy;
		 cout<<"\n liczba kolumn mapy = "<<il_kolmapy;
		 cout<<"\n inicjacyjny x_goal = "<<x_goal;
		 cout<<"\n inicjacyjny y_goal = "<<y_goal; 

		 //deklaracja mapy
		 int rows = il_wiermapy+1;
		 double **mapa;
		 mapa = new double*[rows];
		 while(rows--) mapa[rows] = new double[il_kolmapy+1];

		 //deklaracja tablicy polityki poruszania sie do celu
		 rows = il_wiermapy+1;
		 int **policy_to_get_goal;
		 policy_to_get_goal = new int*[rows];
		 while(rows--) policy_to_get_goal[rows] = new int[il_kolmapy+1];

		 //deklaracja tablicy kierunkow polnocnych wszystkich pol mapy,
		 rows = il_wiermapy+1;
		 int **new_north_direction_all;
		 new_north_direction_all = new int*[rows];
		 while(rows--) new_north_direction_all[rows] = new int[il_kolmapy+1];

		 //inicjujemy tablice odleglosci do przeszkod,
		 rows = il_wiermapy+1;
		 double ***dist_to_obstacles = new double **[rows];
		 for(int i = 0; i < rows; i++){
		   dist_to_obstacles[i] = new double *[il_kolmapy+1];
		   for(int j = 0; j < il_kolmapy+1; j++)
		     dist_to_obstacles[i][j] = new double[5];
		  }

		 //inicjujemy tablice ograniczen dostepnych pol mapy,
		 rows = il_wiermapy+1;
		 int ***dist_to_field_bondary = new int **[rows];
		 for(int i = 0; i < rows; i++){
		   dist_to_field_bondary[i] = new int *[il_kolmapy+1];
		   for(int j = 0; j < il_kolmapy+1; j++)
		     dist_to_field_bondary[i][j] = new int[5];
		  }

		 //wypelnianie tablic informacjami o budynku potrzebnymi do nawigacji
		 map_reader(il_wiermapy,il_kolmapy,dist_to_obstacles,dist_to_field_bondary,new_north_direction_all,mapa,name.c_str());				
		
		 cout<<"\n\nDystans do przeszkod";
		 cout<<"\nNORTH:\n";
		 for(int i=1;i<il_wiermapy+1;i++){
		   for(int j=1;j<il_kolmapy+1;j++){
			 cout<<" "<<dist_to_obstacles[i][j][0];
		    }
		   cout<<"\n";
		  }
		 cout<<"\nEAST:\n";
		 for(int i=1;i<il_wiermapy+1;i++){
		   for(int j=1;j<il_kolmapy+1;j++){
			 cout<<" "<<dist_to_obstacles[i][j][1];
		    }
		   cout<<"\n";
		  }
		 cout<<"\nSOUTH:\n";
		 for(int i=1;i<il_wiermapy+1;i++){
		   for(int j=1;j<il_kolmapy+1;j++){
			 cout<<" "<<dist_to_obstacles[i][j][2];
		    }
		   cout<<"\n";
		  }
		 cout<<"\nWEST:\n";
		 for(int i=1;i<il_wiermapy+1;i++){
		   for(int j=1;j<il_kolmapy+1;j++){
			 cout<<" "<<dist_to_obstacles[i][j][3];
		    }
		   cout<<"\n";
		  }

		 cout<<"\n\nRozmiar pol mapy";
		 cout<<"\nNORTH:\n";
		 for(int i=1;i<il_wiermapy+1;i++){
		   for(int j=1;j<il_kolmapy+1;j++){
			 cout<<" "<<dist_to_field_bondary[i][j][0];
		    }
		   cout<<"\n";
		  }
		 cout<<"\nEAST:\n";
		 for(int i=1;i<il_wiermapy+1;i++){
		   for(int j=1;j<il_kolmapy+1;j++){
			 cout<<" "<<dist_to_field_bondary[i][j][1];
		    }
		   cout<<"\n";
		  }
		 cout<<"\nSOUTH:\n";
		 for(int i=1;i<il_wiermapy+1;i++){
		   for(int j=1;j<il_kolmapy+1;j++){
	 		 cout<<" "<<dist_to_field_bondary[i][j][2];
		    }
		   cout<<"\n";
		  }
		 cout<<"\nWEST:\n";
		 for(int i=1;i<il_wiermapy+1;i++){
		   for(int j=1;j<il_kolmapy+1;j++){
		 	 cout<<" "<<dist_to_field_bondary[i][j][3];
		    }
		   cout<<"\n";
		  }

		 cout<<"\nkierunek polnocny w polach mapy:\n";
		 for(int i=1;i<il_wiermapy+1;i++){
		   for(int j=1;j<il_kolmapy+1;j++){
			 cout<<" "<<new_north_direction_all[i][j];
		    }
		   cout<<"\n";
		  }

		 cout<<"\npozycje przeszkod:\n";
		 for(int i=1;i<il_wiermapy+1;i++){
		   for(int j=1;j<il_kolmapy+1;j++){
		 	 cout<<" "<<mapa[i][j];
		    }
		   cout<<"\n";
		  }

		  //inicjujemy wszystkie dostepne pola mapy tym samym prawdopodobienstwem,
		  mapa_init_prob(mapa,il_wiermapy,il_kolmapy);

		 //dane do systemu wizualizacji
		  zapisz_tab2d_do_pliku("data_tab.txt", mapa,il_wiermapy,il_kolmapy);
		 //dane do systemu wizualizacji

		 //ustalam awaryjny kierunek polnocny
		 //moze byc uzyty gdy robot zostanie przeniesiony w nieznane pole mapy i ustawiony w nienzanym kierunku,
		 int average_north_direction=emergency_north_direction(mapa,new_north_direction_all,il_wiermapy,il_kolmapy);
				 
		 //0-north, 1-east, 2-south, 3-west
		 int direction;

		 int actual_x_pos=0;//wsp x pozycji robota
		 int actual_y_pos=0;//wsp y pozycji robota

		 double actual_pos_max_prob;//maksymalne prawdopodobienstwo polozenia

		 int pointer_inf_loop;

		 int ilosc_krokow; //wartosc potrzebna do splotu

		 double move_precision=0.6;//potrzebna do splotu
		
	     begfiltr:

		 //policy_to_get_goal - tablica polityki dotarcia do celu, x_goal - wsp x celu, y_goal - wsp y celu;
		 move_policy(il_wiermapy,il_kolmapy,policy_to_get_goal,mapa,x_goal,y_goal);

		 do
		 {//petla nieskonczona w srodku filtru
	       pointer_inf_loop=1;

		  //uaktualniam p-stwo polozenia robota po pomiarze
		  p_update_after_sense(comm,current_direction,new_north_direction,direction,ktemp,il_wiermapy,il_kolmapy,mapa,dist_to_obstacles);

		  //ustalam pozycje robota:
		  //tu jest miejsce na granulacje, w przypadku niejednoznacznosci, trzeba napisac strategie wyboru ruchu,
		  actual_pos_max_prob=0;//maksymalne prawdopodobienstwo polozenia
		  for(int i=1;i<il_wiermapy+1;i++){
		    for(int j=1;j<il_kolmapy+1;j++){
			  if(mapa[i][j]!=5){
			    if(mapa[i][j]>actual_pos_max_prob){
			      actual_pos_max_prob=mapa[i][j];
				  actual_x_pos=i;
				  actual_y_pos=j;
			     }
			   }
			 }
		   } 

		 //dane do systemu wizualizacji
		 zapisz_tab2d_do_pliku("data_tab.txt",mapa,il_wiermapy,il_kolmapy);
		 ofstream plik2;
		 plik2.open("actualpos.txt");
		 plik2<<actual_x_pos;
		 plik2<<" ";
		 plik2<<actual_y_pos;
		 plik2.close();
		 //dane do systemu wizualizacji

		 cout<<"\nmoja aktualna pozycja na mapie to ("<<actual_x_pos<<","<<actual_y_pos<<")";

		 //wykonujemy gdy cel jest osiagniety
		 if(actual_x_pos==x_goal && actual_y_pos==y_goal){
		   //dane do systemu wizualizacji
 	 	   zapisz_do_pliku("status.txt", "0");
		   //dane do systemu wizualizacji
		   cout<<"\n\nPozycja docelowa osiagnieta, wylaczamy filtracje";
		   NXT::PlaySoundFile(&comm,"R2D2c.rso",1);
		   Wait(1500);
		   NXT::PlaySoundFile(&comm,"R2D2b.rso",1);
		   Wait(1450);
		   NXT::StopSound(&comm);
		   new_north_direction=new_north_direction_all[x_goal][y_goal];
		   turn_to_specified_map_direction(comm,IN_4,0,new_north_direction);
		   current_direction=0;
		   char dec1;
		   dokonajWyboru:
		   cout<<"\n\npress k to exit navigation";
		   cout<<"\npress n to specify another destination coordinates";
		   dec1=getch();
		   if(dec1=='k' || dec1=='K'){
			 cout<<"\n\nLokalizacja zakonczona - kontrola odzyskana\n\n";
			 //pointer_inf_loop=2;
			 goto koniec;
		    }
		   if(dec1=='n' || dec1=='N'){
		     do
		      {
			   cout<<"\npodaj x_goal and press enter:";
			   std::cin >> x_goal;
			   cout<<"x_goal = "<<x_goal;
			   cout<<"\npodaj y_goal and press enter:";
			   std::cin >> y_goal;
			   cout<<"y_goal = "<<y_goal;
			  }
			  while(x_goal<=0 || x_goal>il_wiermapy || y_goal<=0 || y_goal>il_kolmapy);
			  goto begfiltr; //goto beggining of filtration
			 }
			if(dec1!='k' && dec1!='K' && dec1!='n' && dec1!='N'){
			  cout<<"\nnieznana komenda - wybierz jeszcze raz";
			  goto dokonajWyboru;
			 }
          }

		 cout<<"\nMoim zadaniem jest dotarcie do celu o wspolrzednych ("<<x_goal<<","<<y_goal<<")";
		 new_north_direction=new_north_direction_all[actual_x_pos][actual_y_pos];
		 cout<<"\nnew_north_direction = "<<new_north_direction;
		 direction=policy_to_get_goal[actual_x_pos][actual_y_pos];
		 turn_to_specified_map_direction(comm,IN_4,direction,new_north_direction);
		 current_direction=direction;
			   
		 ofstream plikx;
		 plikx.open("status.txt");
		 plikx<<current_direction+1;
		 plikx.close();

		 int ilosc_taktow=0;//bedzie zawierala inforacje ile taktow jazdy nalezy wykonac, do momentu kolejnej lokalizacji,
		 ilosc_taktow=il_takt(comm,x_goal,y_goal,actual_x_pos,actual_y_pos,dist_to_field_bondary,direction);
		 ilosc_taktow=ilosc_taktow;//-(0.15*ilosc_taktow);
		 cout<<"\nplanowana ilosc taktow = "<<ilosc_taktow;
		
		 current_direction=policy_to_get_goal[actual_x_pos][actual_y_pos];
		 drive_PD_in_specified_map_direction_between_walls(comm,IN_4,IN_4,policy_to_get_goal[actual_x_pos][actual_y_pos],new_north_direction,ilosc_taktow); 

 	 	 zapisz_do_pliku("status.txt", "0");

		 //ustalam ilosc krokow, ktore teoretycznie wykonal robot (w ustalonym kierunku)
		 if(current_direction==0 || current_direction==2){ilosc_krokow=abs(actual_x_pos-x_goal);				}
		 if(current_direction==1 || current_direction==3){ilosc_krokow=abs(actual_y_pos-y_goal);}

		 //uaktualniam p-stwo polozenia robota po ruchu - dokonuje splotu prawdopodobienstwa,
		 p_update_after_move(comm,current_direction,ilosc_krokow,il_wiermapy,il_kolmapy,mapa,move_precision);
			 
		 zapisz_tab2d_do_pliku("data_tab.txt",mapa,il_wiermapy,il_kolmapy);

		 //ustalam pozycje robota:
		 //tu jest miejsce na granulacje, w przypadku niejednoznacznosci, trzeba napisac strategie wyboru ruchu,
		 actual_pos_max_prob=0;//maksymalne prawdopodobienstwo polozenia
		 for(int i=1;i<il_wiermapy+1;i++){
		   for(int j=1;j<il_kolmapy+1;j++) {
			 if(mapa[i][j]!=5){
			   if(mapa[i][j]>actual_pos_max_prob){
				 actual_pos_max_prob=mapa[i][j];
				 actual_x_pos=i;
				 actual_y_pos=j;
				}
			  }
			}
		  } 

		 //ofstream plik5;
		 plik2.open("actualpos.txt");
		 plik2<<actual_x_pos;
		 plik2<<" ";
		 plik2<<actual_y_pos;
		 plik2.close();

		 cout<<"\nmoja aktualna pozycja na mapie to ("<<actual_x_pos<<","<<actual_y_pos<<")";
			 
		 //ustalam nowy kierunek polnocny na ten, ktory wystepuje w kratce docelowej, lub posredniej.
		 new_north_direction=next_north_direction(comm,x_goal,y_goal,actual_x_pos,actual_y_pos,direction,new_north_direction_all);
	
	   }//petla nieskonczona w srodku filtru
	    while(pointer_inf_loop==1);

	    koniec:

		//czyszczenie pamieci
		for(int i=0;i<il_wiermapy+1;i++){
		  delete[] new_north_direction_all[i];
		 }//czyscimy wiersze
		delete[] new_north_direction_all;//zwalniamy tablice wskaznikow do wierszy

		//czyszczenie pamieci
		for(int i=0;i<il_wiermapy+1;i++){
		  delete[] policy_to_get_goal[i];
		 }//czyscimy wiersze
		delete[] policy_to_get_goal;//zwalniamy tablice wskaznikow do wierszy

		//czyszczenie pamieci
		for(int i=0;i<il_wiermapy+1;i++){
		  delete[] mapa[i];
		 }//czyscimy wiersze
		delete[] mapa;//zwalniamy tablice wskaznikow do wierszy

		//czyszczenie pamieci
	   	for(int i=0;i<il_wiermapy+1;i++){
		  for(int j=0;j<il_kolmapy+1;j++){
		    delete[] dist_to_obstacles[i][j];
		   }
		 }
		for(int i=0;i<il_wiermapy+1;i++){
		  delete[] dist_to_obstacles[i];
		 }//czyscimy wiersze
		delete[] dist_to_obstacles;//zwalniamy tablice wskaznikow do wierszy

		//czyszczenie pamieci
	   	for(int i=0;i<il_wiermapy+1;i++){
		  for(int j=0;j<il_kolmapy+1;j++){
		    delete[] dist_to_field_bondary[i][j];
		   }
		 }
		for(int i=0;i<il_wiermapy+1;i++){
		  delete[] dist_to_field_bondary[i];
		 }//czyscimy wiersze
		delete[] dist_to_field_bondary;//zwalniamy tablice wskaznikow do wierszy
	 }//koniec filtracji

//	koniec:

    continue;
   }
  }//glowna petla nieskonczona
  while(infinity_loop_pointer!=2);

  NXT::StopProgram(&comm);
  }
  NXT::Close(&comm); //close the NXT
  std::cout<<"\n\nNacisnij ENTER aby zakonczyc Program";
  getchar();
  return 0;
 }

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,