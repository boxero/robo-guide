//sense distance to obstacles on the left, right and forward direction - rotating radar
inline int sense(Comm::NXTComm comm, int port1,int direct_temp, double *ktemp){
  NXT::Motor::Stop(&comm, OUT_B, 0);
  NXT::Motor::Stop(&comm, OUT_C, 0);
  int speeddd=2;
  NXT::Motor::GoTo2(&comm, OUT_A, speeddd, -90, 1);
  int temp=0;
  for(int i=1;i<11;i++){
	temp+=NXT::Sensor::GetSonarValue(&comm, port1);
	Wait(10);
	std::cout<<"\ntemp="<<temp;
   }
  temp/=10;
  std::cout<<"\nSonarValueRightAverage = "<<temp;
  ktemp[(direct_temp+1)%4]=temp;
  Wait(10);
  NXT::Motor::GoTo2(&comm, OUT_A, speeddd, 0, 1);
  temp=0;
  for(int i=1;i<11;i++){
	temp+=NXT::Sensor::GetSonarValue(&comm, port1);
	Wait(10);
	std::cout<<"\ntemp="<<temp;
   }
  temp/=10;
  std::cout<<"\nSonarValueFrontAverage = "<<temp;
  ktemp[direct_temp]=temp+7;
  Wait(10);
  NXT::Motor::GoTo2(&comm, OUT_A, speeddd, 90, 1);
  temp=0;
  for(int i=1;i<11;i++){
	temp+=NXT::Sensor::GetSonarValue(&comm, port1);
	Wait(10);
	std::cout<<"\ntemp="<<temp;
   }
  temp/=10;
  std::cout<<"\nSonarValueleftAverage = "<<temp;
  ktemp[(direct_temp+3)%4]=temp;
  Wait(10);
  NXT::Motor::GoTo2(&comm, OUT_A, speeddd, 0, 1);
  Wait(10);		  
  return 0;
 }

//sense distance to obstacles on the left, right and forward direction - static radar, port1-left,port2-right,port3-forward
inline int sense2(Comm::NXTComm comm,int port1,int port2,int port3,int direct_temp, double *ktemp){
  NXT::Motor::Stop(&comm, OUT_B, 0);
  NXT::Motor::Stop(&comm, OUT_C, 0);
  int temp1,temp2,temp3;		
  //temp1=NXT::Sensor::GetSonarValue(&comm, port1);//left
  //temp2=NXT::Sensor::GetSonarValue(&comm, port2);//right
  //temp3=NXT::Sensor::GetSonarValue(&comm, port3);//forward
  temp1=NXT::Sensor::GetSMUXvalue(&comm, IN_2, 4, 1);//left
  temp2=NXT::Sensor::GetSMUXvalue(&comm, IN_2, 2, 1);//right
  temp3=NXT::Sensor::GetSMUXvalue(&comm, IN_2, 1, 1);//forward
  std::cout<<"\nSonarValueleft = "<<temp1;
  std::cout<<"\nSonarValueright = "<<temp2;
  std::cout<<"\nSonarValueforward = "<<temp3;
  ktemp[(direct_temp+3)%4]=temp1+8;
  ktemp[(direct_temp+1)%4]=temp2+8;
  ktemp[direct_temp]=temp3+7;
  return 0;
 } 

//Basic version (18-05-2014) developed by artem@matman.uwm.edu.pl,
