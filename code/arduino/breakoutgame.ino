#include <Bounce2.h>

#include <Wire.h>

const unsigned int Button_Pin=7;
const int MPU=0x68; 
int16_t AcX,AcY,AcZ;
const int NUM_AXES=3;
  
const int BUFFER_SIZE=16;
int buffer[NUM_AXES][BUFFER_SIZE];
int buffer_pos[NUM_AXES]={0};
Bounce button(Button_Pin, 20);

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(19200);
  pinMode(Button_Pin, INPUT);
}

int get_axis(const int axis){
  delay(1);
  buffer[axis][buffer_pos[axis]]=Wire.read()<<8|Wire.read();
  buffer_pos[axis]=(buffer_pos[axis]+1)%BUFFER_SIZE;
  long sum=0;
  for(int i=0; i<BUFFER_SIZE; i++)
  sum+= buffer[axis][i];
  return round(sum/BUFFER_SIZE);
  }
  
  int get_x(){return get_axis(0);}
  int get_y(){return get_axis(1);}
  int get_z(){return get_axis(2);}
    
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true);
  Serial.print(get_x());
  Serial.print(" ");
  Serial.print(get_y());
  Serial.print(" ");
  Serial.print(get_z());
  Serial.print(" ");
  if(button.update())
    Serial.println(button.read()==HIGH ? "1" : "0");
  else
    Serial.println("0");
    
  delay(50);
}
