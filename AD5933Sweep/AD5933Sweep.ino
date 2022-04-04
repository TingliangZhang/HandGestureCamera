/*
ad5933-test
    Reads impedance values from the AD5933 over I2C and prints them serially.
*/

#include <Wire.h>
#include "AD5933.h"

#define START_FREQ  (80000)
#define FREQ_INCR   (10000)
#define NUM_INCR    (3)
#define REF_RESIST  (10000)

double gain[NUM_INCR+1];
int phase[NUM_INCR+1];

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
      frequencySweepRaw();
    }
  }
  }



void setup(void)
{
  // Set MUX Output 
  for (int i = 0; i < 6; i++ ){
    pinMode(MUX[i], OUTPUT);
  }
  
  // Begin I2C
  Wire.begin();

  // Begin serial at 115200 baud for output
  Serial.begin(115200);
  Serial.println("AD5933 Test Started!");

  // Perform initial configuration. Fail if any one of these fail.
  if (!(AD5933::reset() &&
        AD5933::setInternalClock(true) &&
        AD5933::setStartFrequency(START_FREQ) &&
        AD5933::setIncrementFrequency(FREQ_INCR) &&
        AD5933::setNumberIncrements(NUM_INCR) &&
        AD5933::setPGAGain(PGA_GAIN_X1)))
        {
            Serial.println("FAILED in initialization!");
            while (true) ;
        }

  // Perform calibration sweep
  if (AD5933::calibrate(gain, phase, REF_RESIST, NUM_INCR+1))
    Serial.println("Calibrated!");
  else
    Serial.println("Calibration failed...");
}

void loop(void)
{
//  frequencySweepEasy();
  SweepPort();

  // Delay
  delay(10000);
  
//  // Easy to use method for frequency sweep
//  frequencySweepEasy();
//
//  // Delay
//  delay(5000);
//
//  // Complex but more robust method for frequency sweep
//  frequencySweepRaw();
//
//  // Delay
//  delay(5000);
}

// Easy way to do a frequency sweep. Does an entire frequency sweep at once and
// stores the data into arrays for processing afterwards. This is easy-to-use,
// but doesn't allow you to process data in real time.
void frequencySweepEasy() {
    // Create arrays to hold the data
    int real[NUM_INCR+1], imag[NUM_INCR+1];

    // Perform the frequency sweep
    if (AD5933::frequencySweep(real, imag, NUM_INCR+1)) {
      // Print the frequency data
      int cfreq = START_FREQ/1000;
      for (int i = 0; i < NUM_INCR+1; i++, cfreq += FREQ_INCR/1000) {
        // Print raw frequency data
        Serial.print(cfreq);
        Serial.print(": R=");
        Serial.print(real[i]);
        Serial.print("/I=");
        Serial.print(imag[i]);

        // Compute impedance
        double magnitude = sqrt(pow(real[i], 2) + pow(imag[i], 2));
        double impedance = 1/(magnitude*gain[i]);
        Serial.print("  |Z|=");
        Serial.println(impedance);
      }
      Serial.println("Frequency sweep complete!");
    } else {
      Serial.println("Frequency sweep failed...");
    }
}

// Removes the frequencySweep abstraction from above. This saves memory and
// allows for data to be processed in real time. However, it's more complex.
void frequencySweepRaw() {
    // Create variables to hold the impedance data and track frequency
    int real, imag, i = 0, cfreq = START_FREQ/1000;

    // Initialize the frequency sweep
    if (!(AD5933::setPowerMode(POWER_STANDBY) &&          // place in standby
          AD5933::setControlMode(CTRL_INIT_START_FREQ) && // init start freq
          AD5933::setControlMode(CTRL_START_FREQ_SWEEP))) // begin frequency sweep
         {
             Serial.println("Could not initialize frequency sweep...");
         }

    // Perform the actual sweep
    while ((AD5933::readStatusRegister() & STATUS_SWEEP_DONE) != STATUS_SWEEP_DONE) {
        // Get the frequency data for this frequency point
        if (!AD5933::getComplexData(&real, &imag)) {
            Serial.println("Could not get raw frequency data...");
        }

        // Print out the frequency data
        Serial.print(cfreq);
        Serial.print(": R=");
        Serial.print(real);
        Serial.print("/I=");
        Serial.print(imag);

        // Compute impedance
        double magnitude = sqrt(pow(real, 2) + pow(imag, 2));
        double impedance = 1/(magnitude*gain[i]);
        Serial.print("  |Z|=");
        Serial.println(impedance);

        // Increment the frequency
        i++;
        cfreq += FREQ_INCR/1000;
        AD5933::setControlMode(CTRL_INCREMENT_FREQ);
    }

    Serial.println("Frequency sweep complete!");

    // Set AD5933 power mode to standby when finished
    if (!AD5933::setPowerMode(POWER_STANDBY))
        Serial.println("Could not set to standby...");
}
