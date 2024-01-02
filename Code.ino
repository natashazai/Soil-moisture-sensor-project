int soilPin = 13;//declaration of the pin that will read the input

unsigned long myTime;
float moisture;
const int dry = 0;//measuring the lowest value when the soil is dry
const int wet = 2700;//measuring the highest value when the sensor is in water

long total2 = 0;
long total3 = 0;


int period1 = 600000;
unsigned long time_now1 = 0;

unsigned long period2 = 8.64e+7;
unsigned long time_now2 = 0;

#define ARRAY_SIZE 600
uint8_t minutesarray[ARRAY_SIZE];
uint8_t minutesarray_i = 0; //Index of next element


#define AVERAGE_SIZE 150
uint8_t average[AVERAGE_SIZE];
uint8_t average_i = 0; //Index of next element

long smooth2() {
  long average2;
  total2 = total2 - minutesarray[minutesarray_i];//substract the last reading
  minutesarray[minutesarray_i] = analogRead(soilPin);
  total2 = total2 + minutesarray[minutesarray_i];//add value to total
  minutesarray_i = minutesarray_i + 1;
  if (minutesarray_i >= ARRAY_SIZE){
    minutesarray_i = 0;
  }
  average2 = total2 / ARRAY_SIZE;
  return average2;
  
}

long smooth3() {
  long average3;
  total3 = total3 - average[average_i];//substract the last reading
  average[average_i] = analogRead(soilPin);
  total3 = total3 + average[average_i];//add value to total
  average_i = average_i + 1;
  if (average_i >= AVERAGE_SIZE){
    average_i = 0;
  }
  average3 = total3 / AVERAGE_SIZE;
  return average3;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//set up the baud rate as 9600
  pinMode(soilPin,INPUT);//declare the pin 13 as an input pin
}

void loop() {
  // put your main code here, to run repeatedly

  int value = analogRead(soilPin); 
  int percentageMoisture = map(value, wet, dry, 100, 0);
  Serial.print("Moisture percentage: ");
  Serial.print(percentageMoisture);
  Serial.println("%");
  
  myTime = millis();//using function millis to check the time for every measurement

  if (millis() > time_now1 + period1){
    Serial.println("It has been 10 minutes.");
    myTime = millis();
    if(minutesarray_i < 600){ 
      //Runs if averages array is not full
  
      minutesarray[minutesarray_i] = smooth2();//do another one over different pieriod of time
      minutesarray_i++;
   }else{
    unsigned long time_now;//if it is full, start filling it again
   }
  }

  if (millis() > time_now2 + period2){
    Serial.println("It has been 24 hours.");
    myTime = millis();
    if(average_i < 145){ 
      //Runs if averages array is not full
  
      average[average_i] = smooth3();
      average_i++;
   }
  }
  
  if(millis() >= time_now1 + period1){
        time_now1 += period1;
  Serial.print("Average value per 10 minutes: "); 
  Serial.println(smooth2());
  Serial.println();
  }

  if(millis() >= time_now2 + period2){
        time_now2 += period2;
  Serial.print("Average value per 24 hour period: "); 
  Serial.println(smooth3());

  for(int minutesarray_i = 0; minutesarray_i < 600; minutesarray_i++){
    Serial.print(minutesarray[minutesarray_i]);
    Serial.println();
  }
  for(int average_i = 0; average_i < 144; average_i++){
    Serial.print(average[average_i]);
    Serial.println();
  }
}
}
