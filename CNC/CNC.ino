#include <SD.h>     // library za SD čitat
#include <SPI.h>    // library za serijsku komunikaciju između arduina i SD čitača 
#include <Servo.h>  // library za Servo motore

int flag_Read_SD,flag_main;
int X[5],Y[5];
const int ChipSelect = 4, DontUsePin = 10;         // library za SD koristi pin 10 pa ga mi ne koristimo
const int ServoPin_x = 5, ServoPin_y = 6;          // Definiranje signal pinova za servo motore

File file;                                         // objekt za file
Servo Pointer_x, Pointer_y;                        // objekt za servo motore

void setup() {  
  Serial.begin(9600);
  // SD ČITAČ
  pinMode(DontUsePin,OUTPUT);                      // mora biti postavljen kao izlazni
  digitalWrite(DontUsePin,HIGH);
  if (!SD.begin(ChipSelect)){                      // provjerava da li se je inicijalizirala komunikacija između čitača i arduina
    Serial.println("SD not initialized!");  
  }else{
    Serial.println("SD initialized!");  
  }
  
  // SERVO MOTORI
  Pointer_x.attach(ServoPin_x);                    // povezuje signal pin za x servo sa objektom pointer_x
  Pointer_y.attach(ServoPin_y);                    // povezuje signal pin za y servo sa objektom pointer_y
  
  flag_Read_SD = 1;                                // korisim da mi se samo jednom izvrši čitanje SD kartice
  flag_main = 1;                                   // koristim kako bi se glavni program odvrtio samo jedanput, barem onaj dio programa koji služi za pohranjivanje koordinata u polja x i y
}

void loop() {                                      // glavni program 
  if (flag_main == 1){
    Read_SD(&X[0],"text_x.txt");                   // pozivam funkciju za pohranu x koordinata
    Read_SD(&Y[0],"text_y.txt");                   // pozivam funkciju za pohranu y koordinata

    Move_servo(&X[0],&Y[0]);
    
    flag_Read_SD = 0;
    flag_main = 0;
  }
}

void Read_SD(int Axis[],String FileName){         // funkcija za čitanje SD kartice
  int Index = 0;                                  
  String Text="";                                 // string za citanje tekta sa SD kartice  
  char C;                                         // karakter za čitanje sa SD kartice, pošto čita slovo po slovo 
  
  if(flag_Read_SD == 1){
    file = SD.open(FileName,FILE_READ);           // čita text.txt file
    if (file){                                    // provjerava ako se .txt file otvorio
      Serial.print("Učitava se "); 
      Serial.println(FileName);    
      while((C = file.read()) != -1){             // while čita tekstualnu datoteku i sprema u novi string 
        if (C =='\n'){
          Axis[Index] = Text.toInt();
          Index++;
          Text = "";
        }else{
          Text = Text + C;         
        }
      }     
      file.close();                                              
      Serial.println("...Gotovo....");
    }else{
      Serial.println("Nije moguće otvoriti file!");  
    }      
  }
  delay(3000); 
}

void Move_servo(int x_axis[],int y_axis[]){
  int ServoDelay = 1000;
  int i,Num = 4;
  for (i=0;i<Num;i++){
    Pointer_x.write(x_axis[i]);
    Pointer_y.write(y_axis[i]);
    delay(ServoDelay);    
  }  
}
