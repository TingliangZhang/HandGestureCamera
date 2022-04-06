// 定义MUX的引脚
int MUX[]={16,17,18,25,26,27};

int* Decode83(int I){
  static int R[]={0,0,0};
  int R0=0;
  int R1=0;
  int R2=0;
  switch (I){
    case 0:{R0=0;R1=0;R2=0;}
    case 1:{R0=0;R1=0;R2=1;}
    case 2:{R0=0;R1=1;R2=0;}
    case 3:{R0=0;R1=1;R2=1;}
    case 4:{R0=1;R1=0;R2=0;}
    case 5:{R0=1;R1=0;R2=1;}
    case 6:{R0=1;R1=1;R2=0;}
    case 7:{R0=1;R1=1;R2=1;}
//    default:printf("error\n");
  }
  R[0]=R0;
  R[1]=R1;
  R[2]=R2;
  return R;
  }

void SetPort1(int P){
  int *R;
  R=Decode83(P);
  for (int i = 0; i < 3; i++ ){
    digitalWrite(MUX[i], R[i]);
    Serial.println("\n digitalWrite:");
    Serial.println(i);
    Serial.println(R[i]);
  }
  }

void SetPort2(int P){
  int* R;
  R=Decode83(P);
  for (int i = 0; i < 3; i++ ){
    digitalWrite(MUX[i+3], R[i]);
  }
  }

void ClearPins(){
  for (int i = 0; i < 6; i++ ){
    digitalWrite(MUX[i], LOW);
  }
  }

void Link(int A, int B){
  SetPort1(A);
  SetPort2(B);
  Serial.print(A);
  Serial.print(":");
  Serial.println(B);
//  Serial.print("\n");
}

void SweepPort(){
  ClearPins();
  for (int i = 0; i < 8; i++ ){
    for (int j = i+1; j < 8; j++ ){
      Link(i,j);
      delay(1000);
//      frequencySweepRaw();
    }
  }
  }

void setup() {
  // Set MUX Output 
  for (int i = 0; i < 6; i++ ){
    pinMode(MUX[i], OUTPUT);
  }

  // Begin serial at 115200 baud for output
  Serial.begin(115200);
  Serial.println("AD5933 Test Started!");
}

void loop() {
  // put your main code here, to run repeatedly:
//  SweepPort();
}
