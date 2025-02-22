#include <dht.h>
#include <SPI.h>
#include <SdFat.h>
#define SD_CS_PIN 10
dht DHT;
int dhtpin[] = {2,3,4,5};
int currentDay = 1;
SdFat sd;
File dataFile;
float sensorValue, avgTemperature, humidity;
unsigned long lastMeasurementTime, startTime, startOfDay;
String dataString;
const int lm35Pins[] = {A0, A1, A2, A3, A4, A5};


void setup(){
  Serial.begin(9600);

  if(!sd.begin(SD_CS_PIN, SPI_HALF_SPEED)){
    Serial.println("Falha ao inicializar o cartão SD!");
    while (1);
  }
  Serial.println("Cartão SD inicializado com sucesso!");
  createLogFile();
  
}

void loop(){
  for(int i = 0; i<6; i++){
    Serial.println("arquivo carregado");
    delay(600000);
    collectData();
    saveToLogFile();
  }
  currentDay++;
  Serial.println("novo arquivo");
  createLogFile();
}

void collectData(){
  dataString = "";

  for(int i = 0; i < 6; i++){
    avgTemperature = 0;
    for(int j = 0; j < 100; j++){
      sensorValue = analogRead(lm35Pins[i]);
      avgTemperature += (sensorValue * (5.0 / 1023.0)) * 100.0;
      delay(10);
    }
    avgTemperature /= 100.0;
    dataString += "LM35-" + String(i + 1) + ", " + String(avgTemperature) + ", ";
    delay(10);
  }

  for(int i = 0; i < 4; i++){
    DHT.read11(dhtpin[i]);
    humidity = DHT.humidity;
    dataString += "DHT11-" + String(i + 1) + ", " + String(humidity) + ", ";
    delay(10);
  }
  dataString.remove(dataString.length() - 2);
}

void createLogFile(){
  String fileName = "hora" + String(currentDay) + ".txt";

  if(dataFile.open(fileName.c_str(), O_WRITE | O_CREAT | O_APPEND)){
    dataFile.close();
    Serial.println("Arquivo criado ou aberto: " + fileName);
  }else{
    Serial.println("Erro ao criar ou abrir o arquivo: " + fileName);
  }
}

void saveToLogFile(){
  String fileName = "hora" + String(currentDay) + ".txt";

  if(dataFile.open(fileName.c_str(), O_WRITE | O_APPEND)){
    Serial.println(dataString);
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("Dados salvos em " + fileName + ": " + dataString);
  }else{
    Serial.println("Erro ao abrir o arquivo: " + fileName);
  }
}
