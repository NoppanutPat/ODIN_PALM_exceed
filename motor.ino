//PWM

//#define MOTORA1 12

//#define MOTORA2 13



//Digital

#define MOTORB1 12

#define MOTORB2 13

/*

 * VCC -> 5V

 * GNG -> GND

 */

//if you want to control the RPM of motor use PWM(~) PIN.

void setup() {

  Serial.begin(9600);

//  pinMode(MOTORA1, OUTPUT);

//  pinMode(MOTORA2, OUTPUT);

  

  pinMode(MOTORB1, OUTPUT);

  pinMode(MOTORB2, OUTPUT);

}

void loop() {

  Serial.println("255");

  /*digitalWrite(MOTORB1 , HIGH);

  digitalWrite(MOTORB2 , LOW);

  analogWrite(MOTORA1 , 255);

  analogWrite(MOTORA2 , LOW);*/

  analogWrite(MOTORB1 , 150);
  analogWrite(MOTORB1 , 0);



  //delay (5000);

  Serial.println("100");

  analogWrite(MOTORB1 , 0);
  analogWrite(MOTORB2 , 150);

  
  //digitalWrite(MOTORB1 , LOW);

  //digitalWrite(MOTORB2 , HIGH);

//  analogWrite(MOTORA1 , LOW);

  //analogWrite(MOTORA2 , 100);

  delay (5000);

}
