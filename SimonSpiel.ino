#include <Wire.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f,16,2);  // 0x3f  oder 0x27

int ledr = 3;
int ledy = 4;
int ledb = 5;
int ledg = 6;
int tasterr = 33;
int tastery = 34;
int tasterb = 35;
int tasterg = 36;
int tasterStart = 9;
int leds[4] = {ledr, ledy, ledb, ledg};
int level = 1;
int kette[25];
int interval=800;
int check;
unsigned long lastPush;
int i=0;
int raus=0; //später benutzt um aus geschachtelte while-loops raus zu kommen
unsigned long timer; // für wenn man LEDs mit millis() ansteuern will 
int rgbr=10; //RGB-LED rot farbe bzw blau, grün; HINWEIS: rgb Werte 0-255
int rgbb=12;
int rgbg=11;
int wahl=A0; //Drehpotentiometer; analogRead ergibt ein Wert von 0-1023
int beliebig = 0; //Flag für Beliebig Modus
int rgbFlag = 0; //Flag für rgb Modus
int ledsFake[6] = {ledr, ledy, ledb, ledg, 125, 150};
int rgbFake = 0; //Flag für FakeOut Modus
int scoreKlassik=0; int scoreBeliebig=0; int scoreRGB=0; int scoreFakeOut=0; int scoreRev=0;
int kette2[25];
int rev =0;
int lautspr=2;
int toneFlag=0;
int scoreTone=0;
 
void ketteMake(){
  for (int n=0; n < 25; n++){
  kette[n]=leds[random(0,4)]; // random(min,max) returns numbers up to max-1
  }
}

void ketteMakeRGBFake(){
    for (int n=0; n < 25; n++){
      if(n == 0){
        kette[n]=leds[random(0,4)];
      }
  else{
    kette[n]=ledsFake[random(0,6)]; // random(min,max) returns numbers up to max-1
  }
  }
}

void ketteReverse(){ //
int x=1;
int y=0;
while(x<=level){
  kette2[y] = kette[level-x];
  x++;
  y++;
}
}

void ketteShow(){ //irgendein problem hier macht es so dass keine kette gezeigt wird
  for (int n=0; n<level; n++){
    digitalWrite(kette[n], HIGH);
    delay(interval);
    digitalWrite(kette[n], LOW);
    delay(interval/2);
    lastPush=millis();
  }
}

void ketteShowTone(){ //irgendein problem hier macht es so dass keine kette gezeigt wird
  for (int n=0; n<level; n++){
    digitalWrite(kette[n], HIGH);
    if (kette[n]==ledr){
      tone(lautspr,500);
    }
    else if (kette[n]==ledy){
      tone(lautspr,1000);
    }
    else if(kette[n]==ledb){
      tone(lautspr,1500);
      }
      else if (kette[n]==ledg){
      tone (lautspr,2000);
      }
    delay(interval);
    digitalWrite(kette[n], LOW);
    noTone(lautspr);
    delay(interval/2);
    lastPush=millis();
  }
}
void ketteShowRGB(){
  for (int n=0; n<level; n++){
    if(kette[n] == ledr){
    analogWrite(rgbr, 250);
    analogWrite(rgbg, 0);
    analogWrite(rgbb, 0);
    delay(interval);
    analogWrite(rgbr, 0);
    analogWrite(rgbg, 0);
    analogWrite(rgbb, 0);
    delay(interval/2);
    lastPush=millis();
    }
    else if(kette[n] == ledy){
    analogWrite(rgbr, 250);
    analogWrite(rgbg, 250);
    analogWrite(rgbb, 0);
    delay(interval);
    analogWrite(rgbr, 0);
    analogWrite(rgbg, 0);
    analogWrite(rgbb, 0);
    delay(interval/2);
    lastPush=millis();
    }
    else if(kette[n] == ledb){
    analogWrite(rgbr, 0);
    analogWrite(rgbg, 0);
    analogWrite(rgbb, 250);
    delay(interval);
    analogWrite(rgbr, 0);
    analogWrite(rgbg, 0);
    analogWrite(rgbb, 0);
    delay(interval/2);
    lastPush=millis();
    }
    else if(kette[n] == ledg){
    analogWrite(rgbr, 0);
    analogWrite(rgbg, 250);
    analogWrite(rgbb, 0);
    delay(interval);
    analogWrite(rgbr, 0);
    analogWrite(rgbg, 0);
    analogWrite(rgbb, 0);
    delay(interval/2);
    lastPush=millis();
    }
    else if(kette[n] == 125){//lila
    analogWrite(rgbr, 250);
    analogWrite(rgbg, 0);
    analogWrite(rgbb, 250);
    delay(interval);
    analogWrite(rgbr, 0);
    analogWrite(rgbg, 0);
    analogWrite(rgbb, 0);
    delay(interval/2);
    lastPush=millis();
    }
    else if(kette[n] == 150){//weiss
    analogWrite(rgbr, 250);
    analogWrite(rgbg, 250);
    analogWrite(rgbb, 250);
    delay(interval);
    analogWrite(rgbr, 0);
    analogWrite(rgbg, 0);
    analogWrite(rgbb, 0);
    delay(interval/2);
    lastPush=millis();
    }
  }
    }  

void gewonnen(){
  timer=millis();
  while((millis()-timer)<1000){
    analogWrite(rgbg, 250);
  }
  if(beliebig == 1){ //erzeugt eine neue Kette während Beliebig Modus
    ketteMake();
  }
  analogWrite(rgbg,0);
  level++;  //Erhöht den Level d.h. Länge den gezeigten Kette
  i=0;  //Bei nächsten Iteration wird die Kette weider von Anfang anfangen
  if(interval >300){ //interval wird kurzer; hat ein untere Grenze
  interval = interval*0.85;
  }
  delay(250); // for debugging in rgbFlag
  }
  
  void gewonnenTone(){
  timer=millis();
  tone(lautspr,4000,250);
  while((millis()-timer)<1000){
    analogWrite(rgbg, 250);
  }
  if(beliebig == 1){ //erzeugt eine neue Kette während Beliebig Modus
    ketteMake();
  }
  analogWrite(rgbg,0);
  level++;  //Erhöht den Level d.h. Länge den gezeigten Kette
  i=0;  //Bei nächsten Iteration wird die Kette weider von Anfang anfangen
  if(interval >300){ //interval wird kurzer; hat ein untere Grenze
  interval = interval*0.85;
  }
  delay(250); // for debugging in rgbFlag
  }

void verloren(){
  Serial.println(millis()); //zeigt wann verloren() durchgefuhrt war
  timer=millis();
  if((rgbFlag == 1)&&(scoreRGB < level-1)){    //Platzhalter fuer Hochpunktstand wenn LCD angeshlossen ist
    scoreRGB=level-1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New High Score:");
    lcd.setCursor(0, 1);
    lcd.print(scoreRGB);
  }
  else if((beliebig==1)&&(scoreBeliebig < level-1)){
    scoreBeliebig=level-1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New High Score: ");
    lcd.setCursor(0, 1);
    lcd.print(scoreBeliebig);
  }
  else if((rgbFake==1)&&(scoreFakeOut < level-1)){
    scoreFakeOut=level-1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New High Score: ");
    lcd.setCursor(0, 1);
    lcd.print(scoreFakeOut);
  }
  else if(scoreKlassik < level-1){
    scoreKlassik=level-1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New High Score:");
    lcd.setCursor(0, 1);
    lcd.print(scoreKlassik);
  }
    else if((rev==1)&&(scoreRev < level-1)){
    scoreRev=level-1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New High Score: ");
    lcd.setCursor(0, 1);
    lcd.print(scoreRev);
  }
  
  while((millis() - timer) <2500){
    if((millis()-timer) <=500){
      digitalWrite(ledr,HIGH);
      digitalWrite(ledy,HIGH);
      digitalWrite(ledb,HIGH);
      digitalWrite(ledg,HIGH);
    }
    if (((millis()-timer)>500) && ((millis()-timer)<=1000)){
      digitalWrite(ledr,LOW);
      digitalWrite(ledy,HIGH);
      digitalWrite(ledb,HIGH);
      digitalWrite(ledg,HIGH);
    }
    if (((millis()-timer)>1000) && ((millis()-timer)<=1500)){
      digitalWrite(ledr,LOW);
      digitalWrite(ledy,LOW);
      digitalWrite(ledb,HIGH);
      digitalWrite(ledg,HIGH);
    }
     if (((millis()-timer)>1500) && ((millis()-timer)<=2000)){
      digitalWrite(ledr,LOW);
      digitalWrite(ledy,LOW);
      digitalWrite(ledb,LOW);
      digitalWrite(ledg,HIGH);
    }
    if (((millis()-timer)>2000) && ((millis()-timer)<=2500)){
      digitalWrite(ledr,LOW);
      digitalWrite(ledy,LOW);
      digitalWrite(ledb,LOW);
      digitalWrite(ledg,LOW);
      analogWrite(rgbr,250);
    }

 }
   raus=1;
   i=0;
   level = 1;
   interval = 800;
   lcd.clear();
   }

   void verlorenTone(){
  Serial.println(millis()); //zeigt wann verloren() durchgefuhrt war
  timer=millis();
  tone(lautspr,100,2000);
  if((toneFlag == 1)&&(scoreTone < level-1)){    //Platzhalter fuer Hochpunktstand wenn LCD angeshlossen ist
    scoreTone=level-1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New High Score:");
    lcd.setCursor(0, 1);
    lcd.print(scoreTone);
  }
  else if((beliebig==1)&&(scoreBeliebig < level-1)){
    scoreBeliebig=level-1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New High Score: ");
    lcd.setCursor(0, 1);
    lcd.print(scoreBeliebig);
  }
  else if((rgbFake==1)&&(scoreFakeOut < level-1)){
    scoreFakeOut=level-1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New High Score: ");
    lcd.setCursor(0, 1);
    lcd.print(scoreFakeOut);
  }
  else if(scoreKlassik < level-1){
    scoreKlassik=level-1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New High Score:");
    lcd.setCursor(0, 1);
    lcd.print(scoreKlassik);
  }
    else if((rev==1)&&(scoreRev < level-1)){
    scoreRev=level-1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New High Score: ");
    lcd.setCursor(0, 1);
    lcd.print(scoreRev);
  }
  
  while((millis() - timer) <2500){
    if((millis()-timer) <=500){
      digitalWrite(ledr,HIGH);
      digitalWrite(ledy,HIGH);
      digitalWrite(ledb,HIGH);
      digitalWrite(ledg,HIGH);
    }
    if (((millis()-timer)>500) && ((millis()-timer)<=1000)){
      digitalWrite(ledr,LOW);
      digitalWrite(ledy,HIGH);
      digitalWrite(ledb,HIGH);
      digitalWrite(ledg,HIGH);
    }
    if (((millis()-timer)>1000) && ((millis()-timer)<=1500)){
      digitalWrite(ledr,LOW);
      digitalWrite(ledy,LOW);
      digitalWrite(ledb,HIGH);
      digitalWrite(ledg,HIGH);
    }
     if (((millis()-timer)>1500) && ((millis()-timer)<=2000)){
      digitalWrite(ledr,LOW);
      digitalWrite(ledy,LOW);
      digitalWrite(ledb,LOW);
      digitalWrite(ledg,HIGH);
    }
    if (((millis()-timer)>2000) && ((millis()-timer)<=2500)){
      digitalWrite(ledr,LOW);
      digitalWrite(ledy,LOW);
      digitalWrite(ledb,LOW);
      digitalWrite(ledg,LOW);
      analogWrite(rgbr,250);
    }

 }
   raus=1;
   i=0;
   level = 1;
   interval = 800;
   lcd.clear();
   }

void klassik(){
Serial.println("Klassik Start");
if(i>level){
  i=0;  
}
  if((rgbFlag == 1)||(rgbFake==1)){
    ketteShowRGB();
  }

   else{
    ketteShow();
   }
    while (i<=level){
    if(raus == 1){ //neue20
    Serial.println("returning"); //showMe
    return;
      }
    Serial.println(level); //zeigt ob innerhalb while-loop
  if(i == level){
    lastPush=millis();
    gewonnen();
    klassik();  
  }
  else if(kette[i] > 100){
    i++;
  }
  else if(digitalRead(tasterr) == LOW){
        lastPush=millis();
        digitalWrite(ledr,HIGH);
    delay(250); //billig entprellen
    digitalWrite(ledr,LOW);
    check = ledr;
    if(check == kette[i]){
         i++;
          }
    else{
      verloren();
      break;
    }
  }
  else if(digitalRead(tastery) == LOW){
            lastPush=millis();
            digitalWrite(ledy,HIGH);
    delay(250);  //billig entprellen
    digitalWrite(ledy,LOW);
    check = ledy;
    if(check == kette[i]){
        i++;
         }
    else{
      verloren();
      break;
    }
  }
  else if(digitalRead(tasterb) == LOW){
            lastPush=millis();
            digitalWrite(ledb,HIGH);
    delay(250);  //billig entprellen
    digitalWrite(ledb,LOW);
    check = ledb;
    if(check == kette[i]){
        i++;
         }
    else{
      verloren();
      break;
    }
  }
  else if(digitalRead(tasterg) == LOW){
            lastPush=millis();
            digitalWrite(ledg,HIGH);
    delay(250);  //billig entprellen
    digitalWrite(ledg,LOW);
    check = ledg;
    if(check == kette[i]){
         i++;
          }
    else{
      verloren();
      break;
    }
  } 
  else if((millis() - lastPush) >= 3000){
    Serial.println("timeout"); //showME: verloren durch timeout
    verloren();
    break;  
  }
}
}

void klassikTone(){
Serial.println("KlassikTone Start");
if(i>level){
  i=0;  
}
  if((rgbFlag == 1)||(rgbFake==1)){
    ketteShowRGB();
  }

   else{
    ketteShowTone();
   }
    while (i<=level){
    if(raus == 1){ //neue20
    Serial.println("returning"); //showMe
    return;
      }
    Serial.println(level); //zeigt ob innerhalb while-loop
  if(i == level){
    lastPush=millis();
    gewonnenTone();
    klassikTone();  
  }
  else if(kette[i] > 100){
    i++;
  }
  else if(digitalRead(tasterr) == LOW){
        lastPush=millis();
        digitalWrite(ledr,HIGH);
        tone(lautspr,500);
    delay(250); //billig entprellen
    digitalWrite(ledr,LOW);
    noTone(lautspr);
    check = ledr;
    if(check == kette[i]){
         i++;
          }
    else{
      verlorenTone();
      break;
    }
  }
  else if(digitalRead(tastery) == LOW){
            lastPush=millis();
            digitalWrite(ledy,HIGH);
            tone(lautspr,1000);
    delay(250);  //billig entprellen
    digitalWrite(ledy,LOW);
    noTone(lautspr);
    check = ledy;
    if(check == kette[i]){
        i++;
         }
    else{
      verlorenTone();
      break;
    }
  }
  else if(digitalRead(tasterb) == LOW){
            lastPush=millis();
            digitalWrite(ledb,HIGH);
            tone(lautspr,1500);
    delay(250);  //billig entprellen
    digitalWrite(ledb,LOW);
    noTone(lautspr);
    check = ledb;
    if(check == kette[i]){
        i++;
         }
    else{
      verlorenTone();
      break;
    }
  }
  else if(digitalRead(tasterg) == LOW){
            lastPush=millis();
            digitalWrite(ledg,HIGH);
            tone(lautspr,2000);
    delay(250);  //billig entprellen
    digitalWrite(ledg,LOW);
    noTone(lautspr);
    check = ledg;
    if(check == kette[i]){
         i++;
          }
    else{
      verlorenTone();
      break;
    }
  } 
  else if((millis() - lastPush) >= 3000){
    Serial.println("timeout"); //showME: verloren durch timeout
    verlorenTone();
    break;  
  }
}
}

void reverse(){
  ketteReverse();
  Serial.println("reverse Start");
if(i>level){
  i=0;  
}
  if((rgbFlag == 1)||(rgbFake==1)){
    ketteShowRGB();
  }

   else{
    ketteShow();
   }
    while (i<=level){
    if(raus == 1){ //neue20
    Serial.println("returning"); //showMe
    return;
      }
    Serial.println(level); //zeigt ob innerhalb while-loop
  if(i == level){
    lastPush=millis();
    gewonnen();
    reverse();  
  }
  else if(kette[i] > 100){
    i++;
  }
  else if(digitalRead(tasterr) == LOW){
        lastPush=millis();
        digitalWrite(ledr,HIGH);
    delay(250); //billig entprellen
    digitalWrite(ledr,LOW);
    check = ledr;
    if(check == kette2[i]){
         i++;
          }
    else{
      verloren();
      break;
    }
  }
  else if(digitalRead(tastery) == LOW){
            lastPush=millis();
            digitalWrite(ledy,HIGH);
    delay(250);  //billig entprellen
    digitalWrite(ledy,LOW);
    check = ledy;
    if(check == kette2[i]){
        i++;
         }
    else{
      verloren();
      break;
    }
  }
  else if(digitalRead(tasterb) == LOW){
            lastPush=millis();
            digitalWrite(ledb,HIGH);
    delay(250);  //billig entprellen
    digitalWrite(ledb,LOW);
    check = ledb;
    if(check == kette2[i]){
        i++;
         }
    else{
      verloren();
      break;
    }
  }
  else if(digitalRead(tasterg) == LOW){
            lastPush=millis();
            digitalWrite(ledg,HIGH);
    delay(250);  //billig entprellen
    digitalWrite(ledg,LOW);
    check = ledg;
    if(check == kette2[i]){
         i++;
          }
    else{
      verloren();
      break;
    }
  } 
  else if((millis() - lastPush) >= 3000){
    Serial.println("timeout"); //showME: verloren durch timeout
    verloren();
    break;  
  }
}
}

void(* resetFunc) (void) = 0; //ermöglicht ein Software-basierte Reset




void setup() {
pinMode(ledr, OUTPUT);
pinMode(ledy, OUTPUT);
pinMode(ledb, OUTPUT);
pinMode(ledg, OUTPUT);
pinMode (lautspr,OUTPUT);
pinMode(tasterr, INPUT_PULLUP);
pinMode(tastery, INPUT_PULLUP);
pinMode(tasterb, INPUT_PULLUP);
pinMode(tasterg, INPUT_PULLUP);
pinMode(tasterStart, INPUT_PULLUP);
pinMode(rgbr, OUTPUT);
pinMode(rgbg, OUTPUT);
pinMode(rgbb, OUTPUT);
randomSeed(analogRead(5)); // Gebraucht sodass random() wirklich random wirt
Serial.begin(9600);

lcd.begin();   // init display: 6 columns / 2 rows
  lcd.clear();
  lcd.setCursor(0,0); // column: 0, row: 0
}

void loop() {
//Serial.println(digitalRead(tasterr));

if(raus == 1){ // die vershiedene Flags wird zuruckgesetzt
  raus = 0; beliebig = 0; rgbFlag = 0; rgbFake=0; rev=0;toneFlag=0;
}
if(analogRead(wahl) <=200){  //klassik Modus
lcd.setCursor(0, 0);
lcd.print("Modus: Klassik   ");
lcd.setCursor(0, 1);
lcd.print("High Score:  ");
lcd.setCursor(13,1);
lcd.print(scoreKlassik);
   if(scoreKlassik <10){
    lcd.setCursor(14,1);
    lcd.print(" ");
   }
    if(digitalRead(tasterStart) == LOW){

       lastPush = millis();
       Serial.println("ketteMake start"); //showMe
       ketteMake(); // ketteMake hier sodass die Reihenfolge bei klassik() erhalten bleibt innerhalb die Runde
       Serial.println("klassik start"); //showMe
       klassik(); 
     }
}

if((analogRead(wahl) >200)&&(analogRead(wahl)<=400)){ //beliebig Modus
lcd.setCursor(0, 0);
lcd.print("Modus: Beliebig");
lcd.setCursor(0, 1);
lcd.print("High Score:  ");
lcd.setCursor(13,1);
lcd.print(scoreBeliebig);
   if(scoreBeliebig <10){
    lcd.setCursor(14,1);
    lcd.print(" ");
   }
    if(digitalRead(tasterStart) == LOW){
       beliebig = 1;
       lastPush = millis();
       Serial.println("ketteMake start"); //showMe
       ketteMake(); // ketteMake hier sodass die Reihenfolge bei klassik() erhalten bleibt innerhalb die Runde
       Serial.println("beleibig start"); //showMe
       klassik(); 
     }
}

if((analogRead(wahl) >400)&&(analogRead(wahl)<=600)){ //rgbModus
lcd.setCursor(0, 0);
lcd.print("Modus: RGB      ");
lcd.setCursor(0, 1);
lcd.print("High Score:  ");
lcd.setCursor(13,1);
lcd.print(scoreRGB);
   if(scoreRGB <10){
    lcd.setCursor(14,1);
    lcd.print(" ");
   }
    if(digitalRead(tasterStart) == LOW){
       rgbFlag = 1;
       lastPush = millis();
      Serial.println("ketteMake start"); //showMe
       ketteMake(); // ketteMake hier sodass die Reihenfolge bei klassik() erhalten bleibt innerhalb die Runde
       Serial.println("rgb start"); //showMe
       klassik(); 
     }
}

if((analogRead(wahl) >600)&&(analogRead(wahl)<=800)){ //FakeOut Modus
lcd.setCursor(0, 0);
lcd.print("Modus: FakeOut    ");
lcd.setCursor(0, 1);
lcd.print("High Score:  ");
lcd.setCursor(13,1);
lcd.print(scoreFakeOut);
   if(scoreFakeOut <10){
    lcd.setCursor(14,1);
    lcd.print(" ");
   }
    if(digitalRead(tasterStart) == LOW){
       rgbFake = 1;
       lastPush = millis();
       Serial.println("ketteMakeRGBFake start"); //showMe
       ketteMakeRGBFake(); // FakeOut hat seine eigene kette
       Serial.println("FakeOut start"); //showMe
       klassik(); 
     }
}

if((analogRead(wahl) >800)&&(analogRead(wahl)<=1000)){ //rückwärts modus
lcd.setCursor(0, 0);
lcd.print("Modus: Reverse   "); // heisst "reverse" da ü,ä auf LCD zu machen braucht extra arbeit
lcd.setCursor(0, 1);
lcd.print("High Score:  ");
lcd.setCursor(13,1);
lcd.print(scoreRev);
   if(scoreRev <10){
    lcd.setCursor(14,1);
    lcd.print(" ");
   }
    if(digitalRead(tasterStart) == LOW){
       rev = 1;
       lastPush = millis();
       Serial.println("ketteMake start"); //showMe
       ketteMake(); // 
       Serial.println("Reverse start"); //showMe
       reverse(); 
     }
}


if(analogRead(wahl)>1000){//Leere Platz
lcd.setCursor(0, 0);
lcd.print("Modus: Tone    ");
lcd.setCursor(0, 1);
lcd.print("High Score:            ");
       lcd.setCursor(13,1);
lcd.print(scoreTone);
   if(scoreTone <10){
    lcd.setCursor(14,1);
    lcd.print(" ");
   }
    if(digitalRead(tasterStart) == LOW){
       toneFlag = 1;
       lastPush = millis();
       Serial.println("ketteMake start"); //showMe
       ketteMake(); // 
       Serial.println("tone start"); //showMe
       klassikTone(); 
     
}
     }


if(digitalRead(tasterr) == LOW){
  digitalWrite(ledr, HIGH);}
if(digitalRead(tastery) == LOW){
  digitalWrite(ledy,HIGH);}
if(digitalRead(tasterb) == LOW){
  digitalWrite(ledb,HIGH);}
if(digitalRead(tasterg) == LOW){
  digitalWrite(ledg,HIGH);} 

if((digitalRead(tasterr) == LOW)&&(digitalRead(tasterg)== LOW)){
  resetFunc();  //reset Funktion falls den Spiel nicht mehr startet
}

  else{
    digitalWrite(ledr,LOW);
    digitalWrite(ledy,LOW);
    digitalWrite(ledb,LOW);
    digitalWrite(ledg,LOW);
    analogWrite(rgbr, 0);
    analogWrite(rgbg, 0);
    analogWrite(rgbb, 0);
  }

}
