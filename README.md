# Actímetro para monitoramento do sono
Protótipo de um aparelho para o monitoramento do sono. 

## Apresentação
Neste repositório será disponibilizado o programa e outras informações pertinentes coletadas para a construção de um actímetro, aparelho utilizado para monitoramento do sono, utilizando um microcontrolador e sensores de baixo custo. Este projeto foi apresentado em forma de Iniciação Científica sob o título "Protótipo de sensor de movimento e luz com armazenamento de dados na nuvem para o monitoramento do sono", orientada pelos Professores Doutores Fábio Nakano e Mário Pedrazzoli Neto. 

## Contextualização
Hoje, são conhecidos mais de 80 distúrbios do sono que acometem a sociedade moderna, como apneia obstrutiva, insônia, bruxismo, entre outros. De acordo com a pesquisa "Episono São Paulo", da pesquisadora Monica Levy Andersen, a privação do sono pode alterar a percepção da fome, o humor, a memória, a atenção e ainda aumenta a predisposição a doenças como o diabetes. 
O Grupo de Desenvolvimento de Dispositivos Móveis (ainda não formal) e o Grupo Interdisciplinar de Pesquisa em Sono (GIPSO), ambos localizados na Escola de Artes, Ciências e Humanidades (EACH), iniciaram uma colaboração para construir um dispositivo computacional com sensores de movimento e luz, transmissão através de WiFi/Bluetooth e armazenamento de dados na nuvem.

## Microcontroladores
- Arduino Uno 
- WeMos D1 R1 Wifi ESP8266 

## Componentes
- **GY-91**: contém giroscópio, acelerômetro, compasso digital e sensor de pressão barométrica.
- **DS3231**: um Real Time Clock que mede o horário real.
- **Módulo Cartão Micro SD**: para salvar os dados gerados pelos componentes e, a princípio, salvá-los em um cartão micro SD.
- **APDS-9960**: sensor de luz RGB e de movimentos. 	

## Outras ferramentas
- Protoboard
- Resistor 1K
- Pen Drive Adaptador de Cartão SD

## Lista de ligações
### GY-91
VCC ~ 3.3V\
GND ~ GND\
SDA ~ A4\
SCL ~ A5\
SDO/SAO ~ Resistor 1K Conectado ao VCC

### DS3231
VCC ~ 3.3V\
GND ~ GND\
SDA ~ A4\
SCL ~ A5

### Módulo Cartão Micro SD
CS ~ SS/10\
SCK ~ 13\
MOSI ~ 11\
MISO ~ 12\
VCC ~ 5V\
GND ~ GND

### APDS-9960
INT ~ Digital 2\
VCC ~ 3.3V\
GND ~ GND\
SDA ~ A4\
SCL ~ A5\


## Bibliotecas utilizadas
- [SparkFun APDS9960 RGB and Gesture Sensor](https://www.arduino.cc/reference/en/libraries/sparkfun-apds9960-rgb-and-gesture-sensor/)
- [Rtc by Makuna](https://www.arduino.cc/reference/en/libraries/rtc-by-makuna/)
- [SdFat - Adafruit Fork](https://www.arduino.cc/reference/en/libraries/sdfat-adafruit-fork/)
- [Bolder Flight Systems MPU9250](https://www.arduino.cc/reference/en/libraries/bolder-flight-systems-mpu9250/)

