#include <SoftwareSerial.h>

#include <Servo.h>

#include "DHT.h"

#define DHTPIN 4

#define DHTTYPE DHT11

#define SW 7

//#define LED 5

#define trigger_pin 2

#define echo_pin 3

#define servo 9

#define rainPin A0

#define MOTORA1 9

#define MOTORA2 10

#define LDR 17



Servo myservo;

SoftwareSerial se_read(12, 13);  // write only

SoftwareSerial se_write(10, 11); // read only



struct ProjectData

{

  int32_t rain_status;

  int32_t light_status;

  float temp;

  float humid;

  int32_t finish;

  int32_t start;

} project_data;



struct ServerData

{

  int32_t drop_auto_status;

  int32_t drop_status;

  int32_t round_status;

} server_data;



const char GET_SERVER_DATA = 1;

const char GET_SERVER_DATA_RESULT = 2;

const char UPDATE_PROJECT_DATA = 3;



void send_to_nodemcu(char code, void *data, char data_size)

{

  char *b = (char *)data;

  char sent_size = 0;

  while (se_write.write(code) == 0)

  {

    delay(1);

  }

  while (sent_size < data_size)

  {

    sent_size += se_write.write(b, data_size);

    delay(1);

  }

}



void setup()

{
  // put your setup code here, to run once:

  Serial.begin(115200);

  se_read.begin(38400);

  se_write.begin(38400);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(trigger_pin, OUTPUT);

  pinMode(echo_pin, INPUT);

  myservo.attach(servo);

  myservo.write(170);

  while (!se_read.isListening())

  {

    se_read.listen();

  }

  pinMode(SW, INPUT);

  //  pinMode(LED, OUTPUT);

  pinMode(rainPin, INPUT);

  pinMode(LDR, INPUT);

  pinMode(MOTORA1, INPUT);

  pinMode(MOTORA2, INPUT);

  Serial.println((int)sizeof(ServerData));

  Serial.println("ARDUINO READY!");

}



uint32_t last_sent_time = 0;

boolean is_data_header = false;

char expected_data_size = 0;

char cur_data_header = 0;

char buffer[256];

int8_t cur_buffer_length = -1;

int32_t b = -1;



int count_start, count_finish, rain;

long duration, cm, floor_length;

long microsecondsToCentimeters(long microseconds)

{

  return microseconds / 29 / 2;

}



int door = 0;

int thresholdValue = 800;

int s;

DHT dht(DHTPIN, DHTTYPE);



int rain_count = 0;

int ldr = -1;



void loop()

{

  uint32_t cur_time = millis();

  analogWrite(MOTORA1 , 125);

  //send to nodemcu

  /*
    int a = digitalRead(SW);
    if(a == 0){
     Serial.println("Press");
     b*=-1;
     delay(500);
    }
    project_data.plus = b;
    if(server_data.plus == 1){
     digitalWrite(LED,HIGH);
    }
    else{
     digitalWrite(LED,LOW);
    }
  */



  //write code here//

  //////////// Switch read //////////

  /*
    s = digitalRead(SW);
    Serial.print("Switch : ");
    Serial.println(s);
  */



  /////////// COUNTING BY ULTRASONIC /////////

  digitalWrite(trigger_pin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigger_pin, HIGH);

  delayMicroseconds(5);

  digitalWrite(trigger_pin, LOW);

  duration = pulseIn(echo_pin, HIGH);

  cm = microsecondsToCentimeters(duration);

  if (cm < 10)

  {

    count_start++;

    project_data.start = count_start;

    delay(500);

  }

  Serial.print("Distance: ");

  Serial.println(cm);

  Serial.print("count_start: ");

  Serial.println(count_start);

  delay(500);



  /////////// HUMIDITY /////////

  float h = dht.readHumidity();

  project_data.humid = h;

  Serial.print("humid: ");

  Serial.println(project_data.humid);

  float t = dht.readTemperature();

  project_data.temp = t;

  Serial.print("Temp: ");

  Serial.println(project_data.temp);



  /////////// RAIN SENSOR /////////

  int sensorValue = analogRead(rainPin);

  if (sensorValue < thresholdValue)

  {

    rain = 0;

    rain_count = 0;

  }

  else

  {

    rain = 1;

  }

  project_data.rain_status = rain;

  Serial.print("Rain Status: ");

  Serial.println(rain);



  ///////////// LDR CENTER ////////////

  ldr = analogRead(LDR);

  Serial.print("LDR : ");

  Serial.println(ldr);

  project_data.light_status = ldr;

  //write code here//



  if (cur_time - last_sent_time > 345)

  { //always update

    //Serial.println(project_data.plus);

    send_to_nodemcu(GET_SERVER_DATA, &server_data, sizeof(ServerData));



    last_sent_time = cur_time;

  }



  //read from sensor....

  //send to nodemcu



  //read data from server pass by nodemcu

  while (se_read.available())

  {

    char ch = se_read.read();

    Serial.print("RECV: ");

    Serial.println((byte)ch);

    if (cur_buffer_length == -1)

    {

      cur_data_header = ch;

      switch (cur_data_header)

      {

        case GET_SERVER_DATA_RESULT:

          //unknown header

          expected_data_size = sizeof(ServerData);

          cur_buffer_length = 0;

          break;

      }

    }

    else if (cur_buffer_length < expected_data_size)

    {

      buffer[cur_buffer_length++] = ch;

      if (cur_buffer_length == expected_data_size)

      {

        switch (cur_data_header)

        {

          case GET_SERVER_DATA_RESULT:

            {

              ServerData *data = (ServerData *)buffer;

              //use data to control sensor

              /*
                    Serial.print("temp status: ");
                    Serial.println(data->temp);
                    Serial.print("light: ");
                    Serial.println(data->light_lux);
                    Serial.print("sound status: ");
                    Serial.println(data->sound);
                    Serial.print("door: ");
                    Serial.println(data->door);
                    Serial.print("PLUS: ");
                    Serial.println(data->plus);
                    if(data->door == 1) {
                    digitalWrite(LED_BUILTIN, HIGH);
                    } else {
                    digitalWrite(LED_BUILTIN, LOW);
                    }
                    server_data.plus = data->plus;
              */

              Serial.print("drop auto status: ");

              Serial.println(data->drop_auto_status);



              Serial.print("drop status: ");

              Serial.println(data->drop_status);



              Serial.print("round status: ");

              Serial.println(data->round_status);



              //write code here//

              if (data->drop_status == 1)

              {

                //                digitalWrite(LED, HIGH);

                Serial.println("DROP");

                myservo.write(55);

                delay(1500);

                myservo.write(0);

                delay(1000);

                myservo.write(170);

                delay(1000);

                count_finish++;

                project_data.finish = count_finish;

                Serial.print("count_finish: ");

                Serial.println(count_finish);

                delay(1000);


              }

              else

              {

                //                digitalWrite(LED, LOW);

              }



              /////////// DROP THE CLOTHES /////////

              if (data->drop_auto_status == 0 && rain == 1)

              {

                if (h < 80)

                {

                  Serial.println("DROP");

                  myservo.write(55);

                  delay(1500);

                  myservo.write(0);

                  delay(1000);

                  myservo.write(170);

                  delay(1000);

                  count_finish++;

                  project_data.finish = count_finish;

                  Serial.print("count_finish: ");

                  Serial.println(count_finish);

                  delay(1000);

                }

                else if (data->drop_auto_status == 1 && rain == 1)

                {

                  Serial.println("MOVE AROUND");

                  analogWrite(MOTORA1, 120);

                  delay(400);
                  
                  analogWrite(MOTORA1, LOW);

                }

              }



              ///////// DROP THE CLOTH IF IT RAINING //////////



              if (rain == 0 && rain_count < count_finish - count_start)

              {

                Serial.println("Drop!!");

                //Serial.println("255");

                analogWrite(MOTORA1 , 125);

                //Serial.println("100");

                myservo.write(55);

                delay(1500);

                myservo.write(0);

                delay(1000);

                myservo.write(170);

                delay(1000);

                rain_count++;

              }

              else {



                analogWrite(MOTORA1, LOW);



              }





              /////////// MANUAL MOVE /////////



              Serial.println("Manual move Activate!!!");

              //Serial.println(data->drop_auto_status);



              if (s == 0)

              {

                Serial.println("255");



                /*digitalWrite(MOTORB1 , HIGH);
                  digitalWrite(MOTORB2 , LOW);
                  analogWrite(MOTORA1 , 255);
                  analogWrite(MOTORA2 , LOW);*/



                analogWrite(MOTORA1 , 125);



                //delay (5000);



                Serial.println("100");



                //digitalWrite(MOTORB1 , LOW);



                //digitalWrite(MOTORB2 , HIGH);



                //  analogWrite(MOTORA1 , LOW);



                //analogWrite(MOTORA2 , 100);



                //delay (5000);

              }

              else //if (s == 1 && data->drop_auto_status == 1)

              {

                analogWrite(MOTORA1, LOW);

              }



              /////////////////// SERVER MANUAL ROUND ////////////////////



              if (data->round_status == 1) {



                Serial.println("255");

                analogWrite(MOTORA1 , 125);

                Serial.println("100");



              }



              else {



                analogWrite(MOTORA1, LOW);



              }



              //write code here//



              send_to_nodemcu(UPDATE_PROJECT_DATA, &project_data, sizeof(ProjectData));

              Serial.println("Send data");

            }

            break;

        }

        cur_buffer_length = -1;

      }

    }

  }

}
