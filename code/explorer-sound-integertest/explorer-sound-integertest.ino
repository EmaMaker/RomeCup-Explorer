// AtTiny412
// 16Mhz tuned following https://github.com/SpenceKonde/megaTinyCore/blob/master/megaavr/extras/Ref_Tuning.md
// millis/micros disabled

#include <Arduino.h>

constexpr uint8_t LUT_SIZE = 32;
constexpr uint8_t LUT_SIZE_HALF = LUT_SIZE*0.5;
constexpr uint8_t LUT_SIZE_QUARTER = LUT_SIZE*0.25;
constexpr uint8_t MAX_VALUE = 64;

constexpr float SAMPLE_FREQ = 57500.0;

uint8_t SINE_LUT[LUT_SIZE];

//constexpr float delta_phi = 4000.0f / SAMPLE_FREQ * (float)LUT_SIZE;
constexpr float delta_phi = 0.068987212 * (float)LUT_SIZE;
float phase = 0;

// constexpr uint8_t ddelta_phi = 0.05465 * LUT_SIZE;
// uint8_t phase = 0;

void setup() {
  DACReference(INTERNAL4V3);

  for (uint8_t i = 0; i < LUT_SIZE; i++) {
    // SINE_LUT[i] = (0.5f + 0.5*sin((2 * PI * (float)i) / LUT_SIZE)) * MAX_VALUE;

    //For the first quarter of the lut, generate a quarter of a sine wave
    if(i >= 0 && i <= LUT_SIZE_QUARTER) SINE_LUT[i] = (0.5f + 0.5*sin((2 * PI * (float)i) / LUT_SIZE)) * MAX_VALUE; // (uint8_t) (MAX_VALUE_HALF + (MAX_VALUE_HALF-1) * sin((2 * PI * (float)i) / LUT_SIZE));
    else if(i > LUT_SIZE_QUARTER && i <= LUT_SIZE_HALF) SINE_LUT[i] = SINE_LUT[(LUT_SIZE_HALF-i)];
    else SINE_LUT[i] = MAX_VALUE-SINE_LUT[(LUT_SIZE-i)];
  }

  DAC0.CTRLA |= 0x41;
  uint8_t t = 0;
  uint16_t t1 = 0, t2 = 1010;
  while(1){
    t1 += t2;
    if(t1 >= 10000){
      //t = (t+1) % LUT_SIZE;

      // a bit of bit-shifting magic
      t *= ! ((t >> 5) & 1);
      t1 -= 10000;
      DAC0.DATA = SINE_LUT[t++];
    }

      
    // less noise but less control in frequency
    //if (t == LUT_SIZE) t = 0 ;
    // increment phase
    
    /*phase += delta_phi;
    // handle wraparound           
    if (phase >= LUT_SIZE) phase -= LUT_SIZE;
    DAC0.DATA = SINE_LUT[(uint8_t)phase];
    */
  }

}

void loop() {
}
