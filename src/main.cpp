#include <Arduino.h>

#define uS_TO_H_FACTOR 3600000000
#define TIME_TO_SLEEP  12           //Hours of deep sleep mode

#define SIM800L_TX 26               
#define SIM800L_RX 27
#define SIM800L_POWER 23

#define PIN_SENSOR 35               //GPIO to receive the sensor value
#define PIN_BATTERY 34              //GPIO to check the battery voltage

//Variables to connect to the network
const String APN = "";
const String USER = "";
const String PASSWORD = "";

//Variables to send request to thingspeak
const String request = "";
const String con_type = "TCP";
const String url = "api.thingspeak.com";
const int port = 80;

//Messages to send via SMS
const String battery_message = "LOW BATTERY";
const String db_link = "";                                          //Data base link to send via sms
const String pressure_message = "Pressure: ";
const String link_message = " bar. Lastest data: " + db_link;
const String phone_number[] = {                                     //Phone numbers you want to send SMS
  
};

//Variables to get the sensor value
float bar_value;
float bar_pin;
float bar_voltage;

//Variables to check the battery voltage
float battery_pin;
float battery_voltage;
boolean low_battery;


/*************************************************************************************
* READ RESPONSE FROM SIM800L
**************************************************************************************/
void serial_data(){
  while(Serial2.available() != 0){
    Serial.write(Serial2.read());
  }
}


/************************************************************************************
 * GET SENSOR VALUE
*************************************************************************************/
void read_sensor(){
  bar_pin = analogRead(PIN_SENSOR);   //Get ADC value
  bar_voltage = (bar_pin/1240)*5/3;   //Conversion to volts

  //Rounding to 1 decimal
  bar_voltage = round(bar_voltage*10);
  bar_voltage = bar_voltage/10;

  bar_value = 3*(bar_voltage-0.5);    //Conversion to bar
}

/************************************************************************************
 * CHECK BATTERY VOLTAGE
*************************************************************************************/
void battery_check(){
  battery_pin = analogRead(PIN_BATTERY);      //Get ADC value
  battery_voltage = (battery_pin/1240)*1.27;  //Conversion to volts

  if (battery_voltage <= 3.3){                //The battery shield should works properly until 3.2 V
    low_battery = true;
  } else{
    low_battery = false;
  }
}


/***********************************************************************************
 * UPLOAD DATA
 ***********************************************************************************/
void data_upload(){
  
  Serial2.println("AT+CIPSHUT");                                        //Close any connection
  delay(2000);
  //serial_data();
  
  Serial2.println("AT+CSTT=" + APN + "," + USER + "," + PASSWORD);      //APN configuration
  delay(3000);
  //serial_data();
  
  Serial2.println("AT+CIICR");                                          //GPRS connection
  delay(3000);
  //serial_data();

  Serial2.println("AT+CIFSR");                                          //Get local IP
  delay(2000);
  //serial_data();

  Serial2.println("AT+CIPSTART=" + con_type + "," + url + "," + port);  //Connection type, URL and port
  delay(6000);
  //serial_data();

  Serial2.println("AT+CIPSEND");                                        //Start the upload
  delay(4000);
  //serial_data();

  Serial2.println(request + bar_value);                                 //GET or POST request
  delay(5000);

  Serial2.println((char)26);                                            //Send data - 0x1A
  delay(5000);
  //serial_data();

  Serial2.println("AT+CIPSHUT");                                        //Close connection
  delay(2000);
  //serial_data();
}


/******************************************************************************************
* SEND SMS
******************************************************************************************/
void send_sms(String mensaje){

  Serial2.println("AT+CIPSHUT");                                            //Close any connection
  delay(1000);
  //serial_data();

  Serial2.println("AT+CMGF=1");                                            //SMS mode
  delay(200);
  //serial_data();

  for (int i=0; i<(sizeof(phone_number)/sizeof(phone_number[0])); i++){   //Send SMS to specified phone numbers
    Serial2.print("AT+CMGS=\"+34" + phone_number[i] + "\"\r");  
    delay(200);
    //serial_data();

    Serial2.println(mensaje);
    delay(500);
    //serial_data();

    Serial2.println((char)26);
    delay(5000);
    //serial_data();
  } 
}


void setup(){
	Serial.begin(115200);                                           //Serial communication with ESP32
  delay(500);

  pinMode(SIM800L_POWER, OUTPUT);
  digitalWrite(SIM800L_POWER, HIGH);                              //Power on SIM800L
  delay(10000);

  Serial2.begin(9600, SERIAL_8N1, SIM800L_TX, SIM800L_RX);        //Serial communication with SIM800L
  delay(500);

  battery_check();

  read_sensor();

  data_upload();

  if (low_battery){
    send_sms(pressure_message + bar_value + link_message + "\r" + battery_message);
  } else{
    send_sms(pressure_message + bar_value + link_message);
  }

  digitalWrite(SIM800L_POWER, LOW);                               //Power off SIM800L

	esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_H_FACTOR);  //Set sleep timer

  delay(500);

	
	esp_deep_sleep_start();                                         //Go to sleep now
}

void loop(){}

