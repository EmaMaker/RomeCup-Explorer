// Able to achieve precise 4kHz with AtTiny clocking at 20MHz, LUT_SIZE = 32
// dreg needs to be calculated as shown in the paper and then fiddled with by hand, as I really do not know what Fs actually is
// Going lower than LUT_SIZE=32 makes the oscilloscope "lose" the trigger point more easily, but is still stable in frequency. A lower LUT_SIZE has the advantage of being easier to tune by hand, but admitingly once you establish a baseline for dreg, using simple proportions to tune the value exactly is not a problem
// Using this snippet I calculated Fsample (the frequency with which a generation cycle is executed) to be 1Mhz
// It outputs a triangle wave, Fsample is the inverse of an half period (double the frequency of the wave), since a cycle computes the rising edge and the following computes the falling edge
/*
// increment phase
phase_accumulator += dreg;
t = SINE_LUT[(phase_accumulator >> 8) & 0x7F];
DAC0.DATA = t1;
t1 = 255 -t1;  
*/

// AtTiny412 running at 20MHz

// https://www.nxp.com/docs/en/application-note/AN1771.pdf

// Not all AtTiny's reach exactly 4khz, because of the granularity (as explained in the article) of the decimal part.
// My solution: use a 4byte integer instead of a 2 byte one, use a single byte for the integer part and 3 bytes for the decimal part
// I limited the decimal part to 1 byte since the AtTiny only has 256B of RAM, the LUT is in RAM, and a 128B LUT is the biggest I can fit in memory while still using power of two

// try to keep LUT_SIZE to a power of two, so that the masking can be done efficiently
constexpr uint8_t LUT_SIZE = 32;
constexpr uint8_t LUT_MASK = LUT_SIZE - 1;
constexpr uint8_t LUT_SIZE_HALF = LUT_SIZE*0.5;
constexpr uint8_t LUT_SIZE_QUARTER = LUT_SIZE*0.25;
constexpr uint8_t MAX_VALUE = 255;

uint8_t SINE_LUT[LUT_SIZE];

// Delta = (LUT_SIZE * FREQ) / SAMPLE_FREQ
// Assume SAMPLE_FREQ 2.26khz
// Delta = 128 * 4000 / 1000000 = 0.512
// Integer part = 0 mask 0x0
// Decimal part = 0.512slightly
// 0.512 * 2^24 = 8589934.592, round to 8589935, mask 0x8589935 (calculated with gnome calc)

// by hand, slightly different than calculated (0x83)
uint32_t dreg = 0x42EACB;

uint32_t phase_accumulator = 0;

void setup() {  
  DACReference(INTERNAL4V3);
  noInterrupts();
  
  for (uint8_t i = 0; i < LUT_SIZE; i++) {
    // For the first quarter of the lut, generate a quarter of a sine wave
    if(i >= 0 && i <= LUT_SIZE_QUARTER) SINE_LUT[i] = (0.5f + 0.5f*sin((2 * PI * (float)i) / LUT_SIZE)) * MAX_VALUE;
    // For the second quarter, mirror the first quarter along the y axis
    else if(i > LUT_SIZE_QUARTER && i <= LUT_SIZE_HALF) SINE_LUT[i] = SINE_LUT[(LUT_SIZE_HALF-i)];
    // For the second half, mirror the first half along the x axis
    else SINE_LUT[i] = MAX_VALUE-SINE_LUT[(LUT_SIZE-i)];
  }

  // From megatinycore analogWrite(PIN_PA6)
  DAC0.CTRLA |= 0x41;
  while(1){
        // increment phase. We do not care about overflowing, it's actually beneficial because with can discard the carry bit and wrap back to zero, and start again from the beginning of the LUT without using if statements
        phase_accumulator += dreg;

        // Output to DAC
        // Shift to the right to extract the integer portion, mask the first 4 bits (size of LUT)        
        DAC0.DATA = SINE_LUT[(phase_accumulator >> 24) & LUT_MASK];
  }

}

void loop() {
}
