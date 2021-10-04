//LIBRARIES
#include <RBD_Timer.h>  // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Button.h> // https://github.com/alextaujenis/RBD_Button
#include <Adafruit_NeoPixel.h>



#define Num_Power 30 // # of leds in the power/flicker section
#define Num_Charge  30 //numer of leds in the chargeing section
#define Num_Barrel  30 // number of leds in the chasing leds section

Adafruit_NeoPixel pixels(90,11, NEO_RGB); //total leds / pin / led type

// inputs
RBD::Button flickr(A1); //currently joystick
RBD::Button charge(12); // currentlyr reset
RBD::Button barrel(13); // currently master

/// variables
int flickstate = 0;
int chargestate = 0;
int barrelstate =0;


////////////////////
/// Sub Functions///
////////////////////
void flickr_run(int on){
  int light = 0;
  if(on == 1){
    light = 255;    
  }
  else{
    light = 0;
  }
  for(int i=0; i< Num_Power; i++){
    pixels.setPixelColor(i, pixels.Color(light,light,light));
  }
  pixels.show();
}



void charge_bright(int val){
  for(int i= Num_Power ; i < (Num_Power + Num_Charge) ; i++){
    pixels.setPixelColor(i, pixels.Color(val,val,val));
  }
  pixels.show();
}

void chase_sequence(int off){
  for(int i= Num_Power ; i < (Num_Power + Num_Charge + Num_Barrel) ; i++){
    if( (( i + off ) /3 ) == 0){
      pixels.setPixelColor(i, pixels.Color(255,255,255));
    }
    else{
       pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
    pixels.show();
  }
}


//////////////////////
/// Main Functions ///
//////////////////////


void setup(){
  // put your setup code here, to run once:
  pixels.begin();
  pixels.clear();
}

void loop(){
  if(flickr.onPressed()){
    flickstate != flickstate;
    flickr_run(flickstate);
  }
  
  if(charge.onPressed()){ // each time the button is pressed the lghts will get slightly brigther
    chargestate = chargestate + 3;
    if (chargestate == 256){
      chargestate =0;
    }
    charge_bright(chargestate);
  }

  if(barrel.onPressed()){
    ++barrelstate;
    if(barrelstate ==3){
      barrelstate = 0;
    }
    chase_sequence(barrelstate);
  }
  

}
