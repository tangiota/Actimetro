// bibliotecas e variáveis globais

#include "MPU9250.h"
#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <RtcDS3231.h>
#include <SPI.h>
#include "SdFat.h"
RtcDS3231<TwoWire> Rtc(Wire);

MPU9250 IMU(Wire,0x69);
int status;

const uint8_t chipSelect = SS;
const uint32_t SAMPLE_INTERVAL_MS = 1000;
#define FILE_BASE_NAME "Data"
SdFat sd;
SdFile file;
uint32_t logTime;
const uint8_t ANALOG_COUNT = 4;

SparkFun_APDS9960 apds = SparkFun_APDS9960();
    
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;

// mensagens de erro salvas em flash 
#define error(msg) sd.errorHalt(F(msg))


//------------------------------------------------------------------------------

void writeHeader() {
// diagramação do cabeçalho da tabela de dados
  file.print(F("Data"));
  file.print("\t");
  file.print(F("Hora"));
  file.print("\t");
  file.print(F("Luz Ambiente"));
  file.print("\t");
  file.print(F("Luz Vermelha"));
  file.print("\t");
  file.print(F("Luz Verde"));
  file.print("\t");
  file.print(F("Luz Azul"));
  file.print("\t");
  file.print(F("AceleracaoX"));
  file.print("\t");
  file.print(F("AceleracaoY"));
  file.print("\t");
  file.print(F("AceleracaoZ"));
  file.print("\t");
  file.print(F("GyroX"));
  file.print("\t");
  file.print(F("GyroY"));
  file.print("\t");
  file.print(F("GyroZ"));
  file.print("\t");
  file.print(F("MagX"));
  file.print("\t");
  file.print(F("MagY"));
  file.print("\t");
  file.print(F("MagY"));
  file.print("\t");
  file.print(F("Temperatura"));
  
  file.println();
}

void logData() {
// leitura do sensor de cor
  if (  !apds.readAmbientLight(ambient_light) ||
        !apds.readRedLight(red_light) ||
        !apds.readGreenLight(green_light) ||
        !apds.readBlueLight(blue_light) ) {
    Serial.println("Error reading light values");
  } else {
    file.print(ambient_light);
    file.print("\t");
    file.print(red_light);
    file.print("\t");
    file.print(green_light);
    file.print("\t");
    file.print(blue_light);
    file.print("\t");
  }
// leitura do acelerômetro
    file.print(IMU.getAccelX_mss());
    file.print("\t");
    file.print(IMU.getAccelY_mss());
    file.print("\t");
    file.print(IMU.getAccelZ_mss());
    file.print("\t");
    file.print(IMU.getGyroX_rads());
    file.print("\t");
    file.print(IMU.getGyroY_rads());
    file.print("\t");
    file.print(IMU.getGyroZ_rads());
    file.print("\t");
    file.print(IMU.getMagX_uT());
    file.print("\t");
    file.print(IMU.getMagY_uT());
    file.print("\t");
    file.print(IMU.getMagZ_uT());
    file.print("\t");
    file.print(IMU.getTemperature_C());
  
  file.println();
}


void setup() {
// criação do arquivo CSV para armazenamento dos dados
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME "00.csv";
  
  Serial.begin(9600);

    Rtc.Begin();
    
// chama a função responsável por data e hora
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) {
        if (Rtc.LastError() != 0) {
//            Serial.print("RTC communications error = ");
//            Serial.println(Rtc.LastError());
        } else {
//            Serial.println("RTC lost confidence in the DateTime!");
            Rtc.SetDateTime(compiled);
        }
    }

    if (!Rtc.GetIsRunning()) {
//        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }
    
    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled)  {
//        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    } else if (now > compiled) {
//        Serial.println("RTC is newer than compile time. (this is expected)");
    } else if (now == compiled) {
//        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
    
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
  
  while(!Serial) {}
  
  status = IMU.begin();
  if (status < 0) {
    while(1) {}
  }

  if ( apds.init() ) {
  } else {
  }

  if ( apds.enableLightSensor(false) ) {
  } else {
  }
  
  while (!Serial) {
    SysCall::yield();
  }
//  Serial.println(F("Type any character to start"));
//  while (!Serial.available()) {
//    SysCall::yield();
//  }
  
// Inicializa na velocidade mais alta suportada pela placa até 50 MHz. 
// Se um erro SPI acontecer, teste uma velocidade menor 
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }
  
  delay(500);
// encontra o nome de um arquivo ainda não usado 
  if (BASE_NAME_SIZE > 50) {
    error("FILE_BASE_NAME too long");
  }
  while (sd.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      error("Can't create file name");
    }
  }
  
  if (!file.open(fileName, O_WRONLY | O_CREAT | O_EXCL)) {
    error("file.open");
  }
// leitura de qualquer dado da Serial 
  do {
    delay(10);
  } while (Serial.available() && Serial.read() >= 0);

// escreve o cabeçalho
  writeHeader();

// comece em um múltiplo do intervalo de amostra
  logTime = micros()/(1000UL*SAMPLE_INTERVAL_MS) + 1;
  logTime *= 1000UL*SAMPLE_INTERVAL_MS;
}

void loop() {

    if (!Rtc.IsDateTimeValid()) {
        if (Rtc.LastError() != 0) {
//            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        } else {
//            Serial.println("RTC lost confidence in the DateTime!");
        }
    }

    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);
    Serial.println();  

// leitura do sensor
    IMU.readSensor();
// display do dado
// tempo para o próximo registro.
  logTime += 1000UL*SAMPLE_INTERVAL_MS;

// espera por um tempo em log
  int32_t diff;
  do {
    diff = micros() - logTime;
  } while (diff < 0);

// checa se a taxa de dados é muito alta.
  if (diff > 10) {
    error("Missed data record");
  }

  logData();

 // força a gravação de dados no SD e atualiza a entrada do diretório para evitar perda de dados 
  if (!file.sync() || file.getWriteError()) {
    error("write error");
  }

  if (Serial.available()) {
// fecha o arquivo e para
    file.close();
//    Serial.println(F("Done"));
    SysCall::halt();
  }
  
  delay(100);
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt) {
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
    
    file.print (dt.Day());
    file.print ("/");
    file.print (dt.Month());
    file.print ("/");
    file.print (dt.Year());
    file.print("\t");
    file.print (dt.Hour());
    file.print (":");
    file.print (dt.Minute());
    file.print (":");
    file.print (dt.Second());
    file.print("\t");

}
