//LIBRARIES
#include <RBD_Timer.h>  // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Button.h> // https://github.com/alextaujenis/RBD_Button
#include <Adafruit_NeoPizel.h>



#define Num_Power 30
#define Num_Charge  30
#define Num_Barrel  30 

Adafruit_NeoPixel pixels(90,11, NEO_RGB + NEOKHZ800);

// inputs
RBD:Button flickr(A1); //currently joystick
RBD:Button charge(12); // currentlyr reset
RBD:Button barrel(13); // currently master

/// variables
int flickstate = 0;
int chargestate = 0;
int barrelstate =0;

////////////////////
/// Sub Functions///
////////////////////
void flickr_run(int on){
  if(on == 1){
    light = 255;    
  }
  else{
    light = 0;
  }
  for(int i=0; i< Num_Power; i++){
    pixles.setPixelColor(i, pixels.Color(light,light,light);
  }
  pixels.show();
}



void charge_bright(int val){
  for(int i= Num_Power ; i < (Num_Power + Num_Charge) ; i++){
    pixles.setPixelColor(i, pixels.Color(val,val,val);
  }
  pixels.show();
}

void chase_sequence(int off){
  for(int i= Num_Power ; i < (Num_Power + Num_Charge + Num_Barrel) ; i++){
    if( (( i + off ) /3 ) == 0){
      pixles.setPixelColor(i, pixels.Color(255,255,255);
    }
    else{
       pixles.setPixelColor(i, pixels.Color(0,0,0);
    }
  pixels.show();
}


//////////////////////
/// Main Functions ///
//////////////////////


void setup() {
  // put your setup code here, to run once:
  pixels.begin();
  pixels.clear();
}

void loop() {
  if(flickr.onPressed()){
    flickstate != flickstate;
    flickr_run(flickrstate);
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
