// emergency servo protocol auto land
unsigned long losscon=0;
int losscount1=0;

 
 
    if(lossping<-100){
      if(millis()-losscon>=100){
        losscount+=1;
        if(losscount>=10000){
            losscount=0;
            digitalWrite(BUZZER,HIGH);
            delay(10);
            digitalWrite(BUZZER,HIGH);
        }
        // stay on position
        Trottle=1500;
        Yaw=1500;
        Pitch=1500;
        Roll=1500;
        Mode=1540; // Loiter mode
      }
      if(lmillis()-losscon>=10000){
        // Return to Land
        Mode=1690; // RTL mode
      }
      if(lmillis()-losscon>=60000){
        for (int freq=1500;freq<=1800;freq++) {
            tone(BUZZER,freq,10);
        }
      }
    }

