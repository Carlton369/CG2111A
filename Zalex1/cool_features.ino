void setupLED() {
  // put your setup code here, to run once:
  pinMode(45, OUTPUT);  //buzzers
  pinMode(43, OUTPUT);  //red LED
  pinMode(41, OUTPUT);  //green LED
}

void buzzer() {
  for (int i = 0; i < 3; i ++) {
  for(int i = 0; i < 150; i ++) {
      digitalWrite(45,HIGH);
      delay(1.5);
      digitalWrite(45,LOW);
      delay(1.5); 
    }
    for(int i = 0; i < 75; i ++) {
      digitalWrite(45,HIGH);
      delay(2);
      digitalWrite(45,LOW);
      delay(2); 
    }
  }
}

void green() {
  buzzer();
  digitalWrite(41, HIGH);
  
}

void red(){
  buzzer();
  digitalWrite(43, HIGH);
  
}

void blank(){
  digitalWrite(41,LOW);
  digitalWrite(43,LOW);
}

/*void loop() {
  // put your main code here, to run repeatedly:
  green();
  red();


  delay(100000000);

  // for(int i = 0; i < 30; i ++) {  //R
  //    digitalWrite(45,HIGH);
  //    delay(2);
  //    digitalWrite(45,LOW);
  //    delay(2); 
  // }
  // delay(500);
  //  for(int i = 0; i < 100; i ++) {
  //    digitalWrite(45,HIGH);
  //    delay(2);
  //    digitalWrite(45,LOW);
  //    delay(2); 
  // }
  // delay(500);
  //  for(int i = 0; i < 30; i ++) {
  //    digitalWrite(45,HIGH);
  //    delay(2);
  //    digitalWrite(45,LOW);
  //    delay(2); 
  // }
  // delay(1000);
  // for(int i = 0; i < 100; i ++) {  //G
  //    digitalWrite(45,HIGH);
  //    delay(2);
  //    digitalWrite(45,LOW);
  //    delay(2); 
  // }
  // delay(500);
  //  for(int i = 0; i < 100; i ++) {
  //    digitalWrite(45,HIGH);
  //    delay(2);
  //    digitalWrite(45,LOW);
  //    delay(2); 
  // }
  // delay(500);
  //  for(int i = 0; i < 30; i ++) {
  //    digitalWrite(45,HIGH);
  //    delay(2);
  //    digitalWrite(45,LOW);
  //    delay(2); 
  // }
  // delay(1000);
  // for(int i = 0; i < 30; i ++) {
  //    digitalWrite(45,HIGH);
  //    delay(2);
  //    digitalWrite(45,LOW);
  //    delay(2); 
  // }
  // delay(500);
  //  for(int i = 0; i < 100; i ++) {  //W
  //    digitalWrite(45,HIGH);
  //    delay(2);
  //    digitalWrite(45,LOW);
  //    delay(2); 
  // }
  // delay(500);
  //  for(int i = 0; i < 100; i ++) {
  //    digitalWrite(45,HIGH);
  //    delay(2);
  //    digitalWrite(45,LOW);
  //    delay(2); 
  // }
  // delay(1000);
}*/
