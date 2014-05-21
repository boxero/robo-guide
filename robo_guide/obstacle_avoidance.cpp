inline void odjedz_od_scian(Comm::NXTComm comm,int current_direction, int new_north_direction){
 //odjezdzam od sciany na odleglosc pozwalajaca na obrot, odczyt przedni
 do
 {
  //if(NXT::Sensor::GetSonarValue(&comm, IN_4)<30)
  if(NXT::Sensor::GetSMUXvalue(&comm,IN_2,1,1)<30){
    std::cout<<"\nPrzeszkoda z przodu jest zbyt blisko, odjezdzam od sciany";
	do
	 {
	  NXT::Motor::SetReverse(&comm, OUT_C, 10);//lewe
	  NXT::Motor::SetReverse(&comm, OUT_B, 10);//prawe
	  Wait(50);
	 }
	//while(NXT::Sensor::GetSonarValue(&comm, IN_4)<32);
	while(NXT::Sensor::GetSMUXvalue(&comm,IN_2,1,1)<32);
	NXT::Motor::SetForward(&comm, OUT_C, 0);//lewe
	NXT::Motor::SetForward(&comm, OUT_B, 0);//prawe
   }
  //odjezdzam od sciany na odleglosc pozwalajaca na obrot, odczyt z lewej strony
  //if(NXT::Sensor::GetSonarValue(&comm, IN_3)<30)
  if(NXT::Sensor::GetSMUXvalue(&comm,IN_2,4,1)<30){
    std::cout<<"\nPrzeszkoda z lewej strony jest zbyt blisko, odjezdzam od sciany";
	turn_to_specified_map_direction(comm,IN_4,(current_direction+3)%4,new_north_direction);
	do
	{
	 NXT::Motor::SetReverse(&comm, OUT_C, 10);//lewe
	 NXT::Motor::SetReverse(&comm, OUT_B, 10);//prawe
	 Wait(50);
	}
   //while(NXT::Sensor::GetSonarValue(&comm, IN_4)<32);
   while(NXT::Sensor::GetSMUXvalue(&comm,IN_2,1,1)<32);
   NXT::Motor::SetForward(&comm, OUT_C, 0);//lewe
   NXT::Motor::SetForward(&comm, OUT_B, 0);//prawe
   turn_to_specified_map_direction(comm,IN_4,current_direction,new_north_direction);
  }
 //odjezdzam od sciany na odleglosc pozwalajaca na obrot, odczyt z prawej strony
 //if(NXT::Sensor::GetSonarValue(&comm, IN_2)<30)
 if(NXT::Sensor::GetSMUXvalue(&comm,IN_2,2,1)<30){
   std::cout<<"\nPrzeszkoda z prawej strony jest zbyt blisko, odjezdzam od sciany";
   turn_to_specified_map_direction(comm,IN_4,(current_direction+1)%4,new_north_direction);
   do
   {
    NXT::Motor::SetReverse(&comm, OUT_C, 10);//lewe
    NXT::Motor::SetReverse(&comm, OUT_B, 10);//prawe
    Wait(50);
   }
   //while(NXT::Sensor::GetSonarValue(&comm, IN_4)<32);
   while(NXT::Sensor::GetSMUXvalue(&comm,IN_2,1,1)<32);
   NXT::Motor::SetForward(&comm, OUT_C, 0);//lewe
   NXT::Motor::SetForward(&comm, OUT_B, 0);//prawe
   turn_to_specified_map_direction(comm,IN_4,current_direction,new_north_direction);
  }
 }
 //while(NXT::Sensor::GetSonarValue(&comm, IN_4)<30 && NXT::Sensor::GetSonarValue(&comm, IN_3)<30 && NXT::Sensor::GetSonarValue(&comm, IN_2)<30);
 while(NXT::Sensor::GetSMUXvalue(&comm,IN_2,1,1)<30 && NXT::Sensor::GetSMUXvalue(&comm,IN_2,4,1)<30 && NXT::Sensor::GetSMUXvalue(&comm,IN_2,2,1)<30);	
}

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,