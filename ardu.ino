// --- Mapeamento de Hardware ---
#include <LiquidCrystal.h>

#define but1  5          //botão 1 no pino digital 5
#define but2  6          //botão 2 no pino digital 6
#define but3  7          //botão 3 no pino digital 7
#define led1  8          //LED1 no pino digital 8
#define led2  9          //LED2 no pino digital 9


// --- Variáveis Globais ---
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 3, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int ledState1 = LOW;        //armazena o estado atual do LED1
int ledState2 = LOW;        //armazena o estado atual do LED2
int buttonState1;           //leitura atual do botão 1
int buttonState2;           //leitura atual do botão 2
int buttonState3;           //leitura atual do botão 3
int lastButtonState1 = LOW; //estado anterior do botão 1
int lastButtonState2 = LOW; //estado anterior do botão 2
int lastButtonState3 = LOW; //estado anterior do botão 3

 

unsigned long lastDebounceTime = 0;   //
long debounceDelay = 20000;           //

unsigned long randNumber;             //armazena o número aleatório
unsigned long minRandomNumber = 2000;     
unsigned long maxRandomNumber = 5000;    

unsigned long time1,time2;
int button3Pressed = LOW;
int printcount = 0;

int takeReading = LOW;

//auxiliares para leitura dos botões
int reading1;
int reading2;
int reading3;

//determinam quais botões foram pressionados
int oddNumber ;
int evenNumber ;


// --- Configurações Iniciais ---
void setup()
{
      pinMode(but1,  INPUT); //entrada botão 1
      pinMode(but2,  INPUT); //entrada botão 2
      pinMode(but3,  INPUT); //entrada botão 3
      pinMode(led1, OUTPUT); //saída LED1
      pinMode(led2, OUTPUT); //saída LED2

      lcd.begin(16, 2);
      lcd.print("hello, world!");
      Serial.begin(9600);    //inicializa Serial com 9600 baud rate
      
      //Inicializa leds
      digitalWrite(led1, ledState1);
      digitalWrite(led2, ledState2);
    
     //Gera números aleatórios pelo ruído da entrada analógica
      randomSeed(analogRead(0));
      
} //end setup
    
    
// --- Loop Infinito ---
void loop() 
{
      float x = millis()/1000;
      
      lcd.setCursor(0, 1);
      // print the number of seconds since reset:
      //lcd.print(millis() / 1000);
      lcd.print(String(x));
      
      lcd.setCursor(0, 1);
      //lcd.print(millis() / 1000);
      x = millis()/1000;
      lcd.print(String(x));

      
      if( printcount == 0)
      {
          Serial.print("Pressione But3");
          lcd.print("But3");
          printcount = 1;
          takeReading = LOW;    //impede leitura dos botões 1 e 2
          oddNumber = LOW;
          evenNumber = LOW;
      }
      
      if( button3Pressed == HIGH)
      {
          // apaga LEDs
          digitalWrite(led1, LOW);
          digitalWrite(led2, LOW);
          
          // Gera um tempo de espera aleatório
          randNumber = random(minRandomNumber,maxRandomNumber); 
          delay(randNumber);
          
          //se o tempo de espera for impar:
          if ( randNumber & 1 == 1)
          {
            //liga LED1
              digitalWrite(led1, HIGH);
              digitalWrite(led2, LOW);
              time1 = micros(); //Captura o tempo em us
              
              oddNumber = HIGH;
          }
          
          //se o tempo de espera for par
          else
          {
            //liga LED2
              digitalWrite(led1, LOW);
              digitalWrite(led2, HIGH);
              time2 = micros();
              
              evenNumber = HIGH;
          }
          button3Pressed = LOW;
          
      }

      reading3 = digitalRead(but3);   //lê botão3
      
      //testa botões
      if(takeReading == HIGH)
      {
        //lê botão 1 se tempo de espera for ímpar
          if(oddNumber == HIGH)
              reading1 = digitalRead(but1);
              
        //lê botão 2 se tempo de espera for par
          if(evenNumber == HIGH)    
              reading2 = digitalRead(but2);
      } 
    
      // Se os botões forem pressionados ou ocarionarem ruído
      if (reading1 != lastButtonState1 || reading2 != lastButtonState2 || reading3 != lastButtonState3) 
      {
        // reinicia tempo de bounce
        lastDebounceTime = micros();
      }
      
      if ((micros() - lastDebounceTime) > debounceDelay) 
      {
          
         // testa a mudança de estado dos botões
          if (reading1 != buttonState1)
          {
                buttonState1 = reading1;
                
                //imprime tempo de reação
                if (buttonState1 == HIGH) 
                {
                  Serial.print("TR: ");
                  Serial.print(lastDebounceTime - time1);
                  Serial.print("us");
                  lcd.begin(16, 2);
                  lcd.print("TR: ");
                  lcd.print(String(lastDebounceTime - time1));
                  lcd.print("us");
                  //lcd.print();
                  printcount = 0;
                }
          }

          if (reading2 != buttonState2)
          {
                buttonState2 = reading2;
                
                //imrpime tempo de reação
                if (buttonState2 == HIGH) 
                {
                  Serial.print(16, 2);
                  Serial.print("TR: ");
                  Serial.print(lastDebounceTime - time2);
                  Serial.print("us");
                  lcd.begin(16, 2);
                  lcd.print("TR: ");
                  lcd.print(String(lastDebounceTime - time2));
                  lcd.print("us");
                  //lcd.print();
                  printcount = 0;
                }
          }

          //testa um novo pressionar do botão 3, para reiniciar jogo
          if (reading3 != buttonState3)
          {
                buttonState3 = reading3;
                
                if (buttonState3 == HIGH) 
                {
                  Serial.print(16, 2);
                  Serial.print("iniciando");
                  lcd.begin(16, 2);
                  lcd.print("iniciando");
                  button3Pressed = HIGH;
                  takeReading = HIGH;
                }
          }
      }
    
      // salva leituras dos botões
      lastButtonState1 = reading1;
      lastButtonState2 = reading2;
      lastButtonState3 = reading3;

} //end loop
