#include <Arduino.h>

constexpr uint8_t LUT_SIZE = 50;
constexpr uint8_t LUT_SIZE_HALF = LUT_SIZE*0.5;
constexpr uint8_t LUT_SIZE_QUARTER = LUT_SIZE*0.25;
constexpr uint8_t MAX_VALUE = 255;

constexpr float SAMPLE_FREQ = 73550.7437f;

uint8_t SINE_LUT[LUT_SIZE];

//constexpr float delta_phi = 4000.0f / SAMPLE_FREQ * (float)LUT_SIZE;
constexpr float delta_phi = 0.054384 * (float)LUT_SIZE;
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
  while(1){
        // increment phase
        phase += delta_phi;
        // handle wraparound           
        if (phase >= LUT_SIZE) phase -= LUT_SIZE;
        DAC0.DATA = SINE_LUT[(uint8_t)phase];
  }

}

void loop() {
}
