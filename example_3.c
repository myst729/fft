#include "fft.h"

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03
#define SAMPLES 64 // MUST be a power of 2
#define SAMPLING_FREQUENCY 40 // Hz, must be less than 10000 due to ADC
#define AMPLITUDE 4
#define START_FREQUENCY 2
#define STOP_FREQUENCY 16.4
#define STEP_SIZE 0.1

double vReal[SAMPLES];
double vImag[SAMPLES];
uint8_t power;
unsigned long time;

void setup() {
  power = fft_exponent(SAMPLES);
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
}

void loop() {
  Serial.println("Frequency\tDetected\ttakes (ms)");
  Serial.println("=======================================\n");
  for (double frequency = START_FREQUENCY; frequency <= STOP_FREQUENCY; frequency += STEP_SIZE) {
    /* Build raw data */
    double cycles = ((SAMPLES - 1) * frequency) / SAMPLING_FREQUENCY;
    for (uint16_t i = 0; i < SAMPLES; i++) {
      vReal[i] = (AMPLITUDE * (sin((i * (TWO_PI * cycles)) / SAMPLES))) / 2.0;
      vImag[i] = 0; //Reset the imaginary values vector for each new frequency
    }
    /*
    Serial.println("Data:");
    PrintVector(vReal, SAMPLES, SCL_TIME);
    */
    time = millis();
    fft_windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD); /* Weigh data */
    /*
    Serial.println("Weighed data:");
    PrintVector(vReal, SAMPLES, SCL_TIME);
    */
    fft_compute(vReal, vImag, SAMPLES, power, FFT_FORWARD); /* Compute FFT */
    /*
    Serial.println("Computed Real values:");
    PrintVector(vReal, SAMPLES, SCL_INDEX);
    Serial.println("Computed Imaginary values:");
    PrintVector(vImag, SAMPLES, SCL_INDEX);
    */
    fft_complex_to_magnitude(vReal, vImag, SAMPLES); /* Compute magnitudes */
    /*
    Serial.println("Computed magnitudes:");
    PrintVector(vReal, SAMPLES >> 1, SCL_FREQUENCY);
    */
    double x;
    double v;
    fft_major_peak(vReal, SAMPLES, SAMPLING_FREQUENCY, &x, &v);
    Serial.print(frequency);
    Serial.print(": \t\t");
    Serial.print(x, 4);
    Serial.print("\t\t");
    Serial.print(millis() - time);
    Serial.println(" ms");
  }
  while(1); /* Run Once */
  // delay(2000); /* Repeat after delay */
}

void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType) {
  for (uint16_t i = 0; i < bufferSize; i++) {
    double abscissa = 0.0;
    /* Print abscissa value */
    switch (scaleType) {
      case SCL_INDEX:
        abscissa = (i * 1.0);
        break;
      case SCL_TIME:
        abscissa = (i * 1.0) / SAMPLING_FREQUENCY;
        break;
      case SCL_FREQUENCY:
        abscissa = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
        break;
    }
    Serial.print(abscissa, 6);
    if (scaleType == SCL_FREQUENCY) {
      Serial.print("Hz");
    }
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}
