inline void zapisz_do_pliku(std::string nazwa_pliku, std::string tekst_do_zapisu){
 std::ofstream plik;
 plik.open(nazwa_pliku.c_str());
 plik<<tekst_do_zapisu;
 plik.close();
}

inline void zapisz_tab2d_do_pliku(std::string nazwa_pliku,double **mapa_inter, int il_wierszy, int il_kolumn){
  std::ofstream plik;
  plik.open(nazwa_pliku.c_str());
  for(int i=1;i<il_wierszy+1;i++){
    for(int j=1;j<il_kolumn+1;j++){
	  if(j==1){plik<<(ceil(mapa_inter[i][j]*1000))/1000;}
	  else{plik<<" ";plik<<(ceil(mapa_inter[i][j]*1000))/1000;}
	 }
    if(i<il_wierszy){plik<<std::endl;}
   }
  plik.close();
 }

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,