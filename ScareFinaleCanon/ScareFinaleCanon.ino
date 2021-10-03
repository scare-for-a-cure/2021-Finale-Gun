//Finale Code 2012 season, made by Xenia Knudsen (512) 586-4973
//James Manley (210)827-3050

//////////Writer notes//////////////////////////////////
///9.26/// James M - added cerial print ln notes to many different parts of the program
///and moved the sub functions up above the void loop and void setup  as the function has to be declared above where it will be used.
/////////

//special connection notes
//Master trigger is connected to +5v
//joystick trigger has to have a 10k resistor between it and ground.

//LIBRARIES
#include <RBD_Timer.h>  // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Button.h> // https://github.com/alextaujenis/RBD_Button


int DebugMode = 0;// 1= serial enabled, must be disabled for actual use otherwise outputs 1&2 will be stuck on. 


//variables
//inputs
RBD::Button keycard = A0;         //a keycard reader
//RBD::Button joystickTrigger(A1); //replacement joystick button
int joysticktrigger = A1;
int joystickLR = A2;    //joystick action
int joystickUD = A3;    //joystick action
RBD::Button reset(12); //replacement button
//RBD::Button masterTrigger(13);  //replacement button 
int masterTrigger = 13; // we aren't able to use the RBD button library on pin 13 because it doesnt have a pull up resister

//outputs
int fogRight = 0;       //fog machine
int fogLeft = 1;      //fog machine
int goreCanon = 2;     //canon
int blindLights = 3;   //lights

int Audio_powerup = 7;  //an audio clip
int Audio_moving = 6;   //an audio clip
int Audio_charging = 5;  //an audio clip
int Audio_firing = 4;   //an audio clip
int Audio_silence = A7; // silence audio clip

int lasers = 8;        
int pivotLR = 9;       //pneumatic action Left = High
int pivotUD = 10;      //pneumatic action Down = High
int barrelLights = 11; //lights


//other
int startup = 0; //0 if the wakeup sequence has not already been run, 1 if it has
int flickrstate = LOW; // whether the lights are on or off for flickering
int flickrstep = 13; // step in array for flickr sequence  if step is 12 or high it knows not to run
int flickrtimes[] = {300, 50, 100, 20, 10, 50, 30, 100, 20, 300, 20, 100};
int fogLR = 0; // left or right fog machine 
int brightness = 0; // variable for analog right for brightness of barrell lights 
int triggerVal =0;
int masterVal =0;



//Timers
RBD::Timer audioTimer; // 100ms delay for the audio tirgger to stop as well as gore canons
RBD::Timer rampUp; // 1 sec timer that increments light intensity charging sequence, when armed
RBD::Timer chase; // no description of what chase is supposed to accomplish yet
RBD::Timer flickr;  // variable time delay for flickering lights during bootup
RBD::Timer dramatic; // 2 second delay after bootup for aiming
RBD::Timer firedelay; // 0.5 second delay after pulling trigger
RBD::Timer firepause; // 2 second delay after canon has fired
RBD::Timer inputDelay; // 0.5 second delay to limit how many times a input is read



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////
///V- Sub Functions -V ///
//////////////////////////

//reset sequence ready - james 9/30
void resetSequence() { //reset the room, turn everything off
  Serial.println("Reset Start");
  digitalWrite(pivotUD, LOW);
  digitalWrite(pivotLR, LOW);
  digitalWrite(barrelLights, LOW);
  digitalWrite(lasers, LOW);
  digitalWrite(blindLights, LOW);
  startup = 0;
  Serial.println("Reset complete");
}//end resetSequence


void startUpSequence(){ //wakeup sequence for turret, run once keycard is inserted
  Serial.println("Start up Start");
  //VARIABLES
  flickrstep = 0;
  flickrstate = HIGH;
  digitalWrite(barrelLights, HIGH);
  digitalWrite(pivotUD, HIGH);
  digitalWrite(Audio_powerup,HIGH); //trigger the power up audio
  audioTimer.restart(); // reset the audio trigger timer
  
  flickr.setTimeout(200); // set the timer for the flickr for the first time step
  flickr.restart();
  startup = 1; //mark wakeup has been run
  Serial.println("Startup running");
}//end startUpSequence


void DirectionSelect(int x){ //set the values for pistons and sounds depending on direction LEFT = HIGH
       digitalWrite(pivotLR, x); // move gun left
       fogLR = x; //store decision in variable for use when firing canon
       digitalWrite(Audio_moving, HIGH); // start audio
       audioTimer.restart(); // reset the time to start 100ms timeout.
}//end directionSelect


//charging sequence ready - james 9/30
void chargingSequence(){ // arming sequence after joy stick trigger
  
  //start sequence
  Serial.println("Charging Sequence Starting");
  int brightness = 0;
  rampUp.restart();
  audioTimer.restart();
  digitalWrite(Audio_charging, HIGH);
  // audio and light intensifiying takes place in timer steps in main loop
}//end chargingSequence



void firingSequence(){
  Serial.println("Firing Sequence Start");
    firedelay.restart(); // start the 0.5 second delay
    chase.restart(); // start the 1.5 second barrel lights chase sequence
}//end firingSequence










/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////
///V-Main Functions-V///
////////////////////////

void setup() {
  // put your setup code here, to run once:
  //init serial coms
  if(DebugMode){
    Serial.begin(9600); // serial print has to be disabled while running the system as outputs 1 and 2 are stuck as always on if serial print is enabled.
  }
  Serial.println("Begin Initialization");
  // inint pin modes
  pinMode(fogLeft, OUTPUT);
  pinMode(fogRight, OUTPUT);
  pinMode(goreCanon, OUTPUT);
  pinMode(blindLights, OUTPUT);
  pinMode(Audio_firing, OUTPUT);
  pinMode(Audio_charging, OUTPUT);
  pinMode(Audio_moving, OUTPUT);
  pinMode(Audio_powerup, OUTPUT);
  pinMode(lasers, OUTPUT);
  pinMode(pivotLR, OUTPUT);
  pinMode(pivotUD, OUTPUT);
  pinMode(barrelLights, OUTPUT);
  digitalWrite(pivotUD, LOW);
  digitalWrite(pivotLR, LOW);
  digitalWrite(barrelLights, LOW);
  digitalWrite(lasers, LOW);
  digitalWrite(blindLights, LOW);
  
  // init timers
  audioTimer.setTimeout(100);
  audioTimer.onExpired();
  rampUp.setTimeout(3000);
  rampUp.onExpired();
  flickr.setTimeout(100); // this gets reset as the script runs
  flickr.onExpired(); 
  dramatic.setTimeout(2000);
  dramatic.onExpired();  
  firedelay.setTimeout(500);
  firedelay.onExpired();
  chase.setTimeout(1500);
  chase.onExpired();
  firepause.setTimeout(2000);
  firepause.onExpired();
  inputDelay.setTimeout(1000);
  inputDelay.onExpired();
  //stop all the timers once so they don't trigger things yet







  
  Serial.println("Initialization Complete");
} //end setup


void loop() {
  // put your main code here, to run repeatedly:
  if (reset.onPressed()) { //runs the reset sequence when reset button pressed doesnt matter if card is inserted
    resetSequence();
  }// end reset if statement 

 // Serial.println(analogRead(joystickLR));
  if((keycard.isPressed()) && (startup == 0)) { // runs start up first time keycard is inserted
    Serial.println("Key Card Inserted");
    startUpSequence();
    Serial.println(flickrstep);
  }//end if startup
    
  if((keycard.isPressed()) && (analogRead(joystickLR)<= 300 )&& (inputDelay.isExpired()) ){ //pivot left when joystick moved left
    Serial.println("Joystick Left Motion Detected");
    DirectionSelect(HIGH);
    flickr.restart();
    inputDelay.restart();
  }//end if
    
  if((keycard.isPressed()) && (analogRead(joystickLR)>= 750 ) && (inputDelay.isExpired())){ //pivot rightbwhen joystick moved right
    Serial.println("Joystick Right Motion Detected");
    DirectionSelect(LOW);
    inputDelay.restart();
  }//end if
    
//  if((keycard.isPressed()) && (joystickTrigger.onPressed())){ //runs the charging sequence when joystick trigger is pressed
  if((keycard.isPressed()) && digitalRead(joysticktrigger)&& ( inputDelay.isExpired())){
  //triggerVal = analogRead(joysticktrigger);
  //Serial.print("TriggerVal: ");
  //Serial.println(triggerVal);
  //if((keycard.isPressed()) && (triggerVal>= 1022) && ( inputDelay.isExpired())){
    Serial.println("Joystick Trigger Pressed");
    inputDelay.restart();
    chargingSequence();
    // it exits the sub function quickly and is handled more by the timer based ramp up section below
  }//end if
    
//  if((keycard.isPressed()) && (masterTrigger.onPressed())){ //runs the firing sequence when master triggger is pressed
//    masterVal = analogRead(masterTrigger);
//    Serial.print("masterVal:");
//    Serial.println(masterVal);
  if((keycard.isPressed()) && (digitalRead(masterTrigger)) && (inputDelay.isExpired())){
//    inputDelay.restart();
    Serial.println("Master Trigger Pressed");
    firingSequence();
    inputDelay.restart();
  }//end if

/// INPUT BASED EVENTS GO ABOVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIMING BASED EVENTS GO BELOW

////////// all audio //////////
// setting timer audio outside allows it to control all audio segments. and gore canon
  if(audioTimer.onExpired()){
    Serial.println("Audio trigger end");
    digitalWrite(Audio_firing, LOW);
    digitalWrite(Audio_charging, LOW);
    digitalWrite(Audio_moving, LOW);
    digitalWrite(Audio_powerup, LOW);
    digitalWrite(Audio_silence, LOW); 
    digitalWrite(goreCanon, LOW);
  }//if end



////////// STARTUP SEQUENCE //////////
//for handling flicr of barrel
  if(flickrstep < 12){ // if the flickr array hasnt been finished set the next state
    Serial.print("flickrrunning :");
    Serial.print(flickrstep);
    Serial.print(" , ");
    Serial.println(flickr.getValue());
    if(flickr.onExpired()){
      Serial.println("Flickering process step");
      flickrstate != flickrstate;
      digitalWrite(barrelLights, flickrstate);
      flickrstep++;
      flickr.setTimeout(flickrtimes[flickrstep]);
      flickr.restart();
    }//end if
    
    if (flickrstep >= 12){ //if the flickr steps have just now been finished run this script
      dramatic.restart(); // start 2 second timer before canon moves ot the left with audio trigger
      Serial.println("Flickering process complete");
      flickrstep=13;
    }//end if
    
  }//end flickring handling

  if(dramatic.onExpired()){ // 2 seconds after the flickr has stopped move the canon to the left and tirgger moving audio
    digitalWrite(pivotLR, HIGH);
    digitalWrite(Audio_moving, HIGH);
    audioTimer.restart();
    Serial.println("Dramatic pause complete, canon armed");
  }


////////// CHARGING SEQUENCE //////////
// running rampup outside of a sub function allows it to be shared amongst all functions
// this is handled outside of the charging sequence so it doesnt cause the program to get hungup in 1 segment.
  if(rampUp.isActive()){   //use active to trturn true while timer is running
    brightness = map(rampUp.getValue(),0, 3000, 0, 255);
    Serial.print("Brightnes: ");
    Serial.println(brightness);
    constrain(brightness, 0, 255);
    analogWrite(barrelLights, brightness);
  }// escape ramp up segment
  
  if(rampUp.onExpired()){
    digitalWrite(barrelLights, HIGH);
  }


 
  

////////// FIRING SEQUENCE //////////
  if(firedelay.onExpired()){ // wait the 0.5 seconds after triggerbeofre turning on fogs machines
    digitalWrite(Audio_firing, HIGH);
    audioTimer.restart();
    
    digitalWrite(lasers, HIGH);
    digitalWrite(fogLR, HIGH);
  }
  if(chase.isActive()){
    //what is the chase sequence is it like the ramp up or the flickr?
  }

  if(chase.onExpired()){
    Serial.println("Chase sequence complete");
    digitalWrite(blindLights, HIGH);
    digitalWrite(goreCanon, HIGH);
    audioTimer.restart(); //audio timer also handles ending signal to gore canons
    firepause.restart(); // start 2 second delay before turnign off lights
  }

  if(firepause.onExpired()){
    digitalWrite(lasers, LOW);
    digitalWrite(blindLights, LOW);
    digitalWrite(fogLeft, LOW);
    digitalWrite(fogRight, LOW);
    digitalWrite(Audio_silence, HIGH); 
    audioTimer.restart();
    Serial.println("Firing sequence complete");
  }

  
} //end loop
