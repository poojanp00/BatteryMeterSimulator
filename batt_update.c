#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "batt.h"


int set_batt_from_ports(batt_t *batt){
  int isNeg = BATT_VOLTAGE_PORT & 0x80000000;
  //Checks if port is negative by checking flag bit
  if(isNeg){
    return 1;//fail
  }

  batt->volts = BATT_VOLTAGE_PORT;
  //percent given calculation using volts
  int percent = (batt->volts - 3000) / 8;
  if(percent > 100)
    percent = 100;
  else if(percent < 0)
    percent = 0;
  //set fields
  batt->percent = percent;
  batt->mode = (1 & BATT_STATUS_PORT);
  return 0;//success
}//set_batt_from_ports

int set_display_from_batt(batt_t batt, int *display){
  //array for bit masks for digits 0-9 to "light up"
  char bit[10]= {0b0111111, 0b0000011, 0b1101101, 0b1100111, 0b1010011, 0b1110110, 0b1111110, 0b0100011, 0b1111111, 0b1110111};
  int right, left, middle, leftperc, rightperc;

  int voltage = batt.volts;
  int percent1 = batt.percent;
  //mod used to check for voltage rounding
  int mod = voltage%10;

  //using modulus operator to find right, middle, and left values.
  voltage/=10;
  right = voltage %10;
  voltage/=10;
  middle = voltage %10;
  voltage/=10;
  left = voltage %10;
  //test cases for rounding
  if(mod>=5 && right != 9)
    right++;
  else if(mod >= 5 && right == 9 && middle != 9){
    right = 0;
    middle++;}
  else if(mod >= 5 && right == 9 && middle == 9 && left !=9){
    right = 0;
    middle = 0;
    left++;}
  //percent digits for right and left values
  rightperc = percent1 %10;
  percent1/=10;
  leftperc = percent1 %10;
  int percent = batt.percent;
  int allzero = 0;

  // The follwing if/else conditionals checks if battery percent falls within range, and masks appropriate bits to main bit sequence
  if(percent >= 5 && percent <= 29)
    allzero |= (0b10000 << 24);
  else if(percent >= 30 && percent <= 49)
    allzero |= (0b11000 << 24);
  else if(percent >= 50 && percent <= 69)
    allzero |= (0b11100 << 24);
  else if(percent >= 70 && percent <= 89)
    allzero |= (0b11110 << 24);
  else if(percent >= 90 && percent <= 100)
    allzero |= (0b11111 << 24);

  // The following if/else conditionals check if the mode is set to volts or percent. If volts/percent then appropriate mask is put over allzeros
  if(!batt.mode){
    allzero |= (0b011 <<21);      //set mode volts
    allzero |= (bit[left] <<14);  //left digit shifter 14
    allzero |= (bit[middle]<<7);  //middle digit shifted 7
    allzero |= bit[right];        //right digit unshifted
  }
  //
  else if(batt.mode && percent == 100){
    allzero |= (0b100 <<21);        //set mode percent
    allzero |= bit[1]<<14;          //right digit (1) shifted 14
    allzero |= bit[leftperc] << 7;  //left digit (0) shifted 7
    allzero |= bit[rightperc];      //middle digit (0) unshifted
  }
  else if(batt.mode && leftperc == 0){
    allzero |= (0b100 <<21);        //set mode percent
    allzero |= bit[rightperc];      //right digit (0) unshifted
  }
  else{
    allzero |= (0b100 <<21);         //set mode percent
    allzero |= (bit[leftperc] <<7);  //left digit shifted 7
    allzero |= bit[rightperc];       //right digit unshifted
  }

  *display = allzero;               //set display to updated bit sequence
  return 0;//success
}//set_display_from_batt


int batt_update(){
  //update function which simulates battery functions.
  batt_t batt = {.volts=-100, .percent=-1, .mode=-1};
  int r1 = set_batt_from_ports(&batt);  //instance of batt_t to run function
  int display = 0;
  int r2 = set_display_from_batt(batt, &display);
  if(r1 == 0 && r2 == 0){
    BATT_DISPLAY_PORT = display; //if both functions return 0 (success)
    return 0;} //success
  return 1;//fail
}//update
