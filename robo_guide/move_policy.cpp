inline int move_policy(int il_wiermapy,int il_kolmapy,int **policy_to_get_goal,double **mapa, int x_cel, int y_cel){
  int rows = il_wiermapy+1;
  int **distance_to_goal;
  distance_to_goal = new int*[rows];
  while(rows--) distance_to_goal[rows] = new int[il_kolmapy+1];

  for(int i=1;i<il_wiermapy+1;i++){
    for(int j=1;j<il_kolmapy+1;j++) {
      if(mapa[i][j]!=5){
	    distance_to_goal[i][j]=0;
		policy_to_get_goal[i][j]=0;
	   }
     }
   }
  //cout<<"\nMapa:\n";     
  //for(int i=1;i<il_wiermapy+1;i++){for(int j=1;j<il_kolmapy+1;j++) {if(mapa[i][j]==5){cout<<" xxxxx";}else{cout<<" "<<mapa[i][j];}}cout<<"\n";} 
  //ustalam wspolrzedne celu, uwazajac, zeby nie ustalic na scianie;
  int x_wsp_of_goal=x_cel;// ze zbioru {1,2,...,liczba wierszy}
  int y_wsp_of_goal=y_cel;// ze zbioru {1,2,...,liczba kolumn}
  distance_to_goal[x_wsp_of_goal][y_wsp_of_goal]=0;
  //cout<<"\nKoszt dotarcia do celu:\n";
  //for(int i=1;i<il_wiermapy+1;i++){for(int j=1;j<il_kolmapy+1;j++) {if(mapa[i][j]==5){cout<<" xxxxx";}else{cout<<" "<<distance_to_goal[i][j];}}cout<<"\n";} 
  for(int i=1;i<il_wiermapy+1;i++){
    for(int j=1;j<il_kolmapy+1;j++){
      if(mapa[i][j]==5){
        distance_to_goal[i][j]=100;
       }
      else{
        distance_to_goal[i][j]=abs(i-x_wsp_of_goal)+abs(j-y_wsp_of_goal);
       }
     }
   }
  //cout<<"\nKoszt dotarcia do celu wypelniony:\n";
  //for(int i=1;i<il_wiermapy+1;i++){for(int j=1;j<il_kolmapy+1;j++) {cout<<" "<<distance_to_goal[i][j];}cout<<"\n";} 
  //cout<<"\n\nUstalam polityke poruszania sie:\n";
  int min_temp=0;
  int temp_ind_x=0;
  int temp_ind_y=0;
  for(int i=1;i<il_wiermapy+1;i++){
    for(int j=1;j<il_kolmapy+1;j++){
      min_temp=100;
      temp_ind_x=0;
      temp_ind_y=0; 
      if(mapa[i][j]==5){
        policy_to_get_goal[i][j]=5;
       }
      else{
        if(i==x_wsp_of_goal && j==y_wsp_of_goal){
          continue;
         }
        if(i-1>0){
          if(distance_to_goal[i-1][j]<min_temp){
            min_temp=distance_to_goal[i-1][j];
            temp_ind_x=i-1;
            temp_ind_y=j;
           }
         }
        if(j+1<=il_kolmapy){
          if(distance_to_goal[i][j+1]<min_temp){
            min_temp=distance_to_goal[i][j+1];
            temp_ind_x=i;
            temp_ind_y=j+1;
           }
         }
        if(i+1<=il_wiermapy){
          if(distance_to_goal[i+1][j]<min_temp){
            min_temp=distance_to_goal[i+1][j];
            temp_ind_x=i+1;
            temp_ind_y=j;
           }
         }
        if(j-1>0){
          if(distance_to_goal[i][j-1]<min_temp){
            min_temp=distance_to_goal[i][j-1];
            temp_ind_x=i;
            temp_ind_y=j-1;
           }
         }
        if(temp_ind_x==i-1 && temp_ind_y==j)policy_to_get_goal[i][j]=0;//"n";
        if(temp_ind_x==i && temp_ind_y==j+1)policy_to_get_goal[i][j]=1;//"e";
        if(temp_ind_x==i+1 && temp_ind_y==j)policy_to_get_goal[i][j]=2;//"s";
        if(temp_ind_x==i && temp_ind_y==j-1)policy_to_get_goal[i][j]=3;//"w";
      }
    }
  }
 policy_to_get_goal[x_wsp_of_goal][y_wsp_of_goal]=4;

 std::cout<<"\nPolityka dotarcia do celu:\n";
 for(int i=1;i<il_wiermapy+1;i++){for(int j=1;j<il_kolmapy+1;j++){std::cout<<" "<<policy_to_get_goal[i][j];}std::cout<<"\n";} 
 std::cout<<"\n----------------------------";
 std::cout<<"\n0-north, 1-east, 2-south, 3-west";
 std::cout<<"\n4-goal, 5-obstacles";
 std::cout<<"\n----------------------------";
 //czyszczenie pamieci
 for(int i=0;i<il_wiermapy+1;i++){
   delete[] distance_to_goal[i];}//czyscimy wiersze
   delete[] distance_to_goal;//zwalniamy tablice wskaznikow do wierszy
 return 0;
}

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,
