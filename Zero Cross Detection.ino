//This program was written as a draft to give information about how the future codes of the project will work.

int ac_load = 5; //Triac Pin
int dimming = 256; // Dimming level (0-128) That can be change real project. This number is draft for program.

void setup() {

pinMode(ac_load, OUTPUT); // setting AC load
attachInterrupt(0,zerocross,RISING); //physical interrupt for zero cross detetion

}

void loop() {
  
    for (int i=5; i <= 256; i++){
    dimming=i;
    delay(10);

}

void zerocross()  // function to be fired at the zero crossing
{
  //Assuming the network frequency is 50 Hz, we arrive at 1000ms/50 = 20 milliseconds. Since it is ac current, we have 2 peaks. 
  //This means that one pulse is equivalent to 10 milliseconds. 
  //Also, after the 0 point, it will have to wait a certain amount of time to trigger the triac. In the prototype code I assumed this as 256ns.

  int dimtime = (39*dimming);    // for 50hz  
  delayMicroseconds(dimtime);    // off cycle
  digitalWrite(AC_LOAD, HIGH);   // triac firing
  delayMicroseconds(10);         // triac On propagation delay for 50 hz.
  digitalWrite(AC_LOAD, LOW);    // triac Off
}



