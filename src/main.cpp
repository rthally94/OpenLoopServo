#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

SemaphoreHandle_t xSerialSemaphore;

void TaskSerialMenu( void *pvParameters );

void setup() {
  Serial.begin(9600);

  while (!Serial) {;}

  if( xSerialSemaphore == NULL ) {
    xSerialSemaphore = xSemaphoreCreateMutex();
    if( ( xSerialSemaphore) != NULL ) {
      xSemaphoreGive (( xSerialSemaphore ));
    }
  }

  xTaskCreate(
    TaskSerialMenu,
    (const portCHAR *) "Serial Menu",
    128,
    NULL,
    2,
    NULL
  );
}

// Moves the motor for the input number of seconds.
void move(float time) {
  bool dir = false;
  if(time < 0) {
    dir = true;
    time *= -1;
  }

  Serial.print("Dir: ");
  Serial.print(dir);
  Serial.print(" | Time: ");
  Serial.println(time);
  delay(time);
}

void TaskSerialMenu( void *pvParameters __attribute__((unused)) ) {
  String userInput = "face";
  String Menu = 
    "Main Menu:\n"
    "1. Enter a value\n"
    "2. Exit"
    "\n\n"
    "Enter a value: "
    ;

  Serial.print("!");

  while(1) {
    if( xSemaphoreTake( xSerialSemaphore, (TickType_t) 5 ) == pdTRUE ) {
      
      if(Serial.available() > 0) {
        userInput = Serial.readStringUntil('\n');

        Serial.print("Your choice");
        Serial.println(userInput);
      }

      xSemaphoreGive( xSerialSemaphore );
    }
    vTaskDelay( pdMS_TO_TICKS(20) );
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
}