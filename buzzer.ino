const int buzzer = 40; //buzzer to arduino pin 40 (PG1)

void buzzer_setup(){

  DDRG |= 0b00000010;  //set pin 1 as output

}

void alarm(){
 
  tone(buzzer, 523); // C5
  delay(1000);        // ...for 1 sec
  tone(buzzer, 415);     // Ab4
  delay(1000);        // ...for 1sec
  
}