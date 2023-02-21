#include "fft.h"

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03
#define SAMPLES 64 // MUST be a power of 2
#define SAMPLING_FREQUENCY 100 // Hz, must be less than 10000 due to ADC
#define AMPLITUDE 100
#define SIGNAL_FREQUENCY 1000
#define ANALOG_PIN A0

double vReal[SAMPLES];
double vImag[SAMPLES];
uint8_t power;
unsigned long time;
unsigned int sampling_period_us;

void setup() {
  power = fft_exponent(SAMPLES);
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
}

void loop() {
  /* SAMPLING */
  time = micros();
  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = analogRead(ANALOG_PIN);
    vImag[i] = 0;
    while(micros() - time < sampling_period_us) {
      //empty loop
    }
    time += sampling_period_us;
  }
  /* Print the results of the sampling according to time */
  Serial.println("Data:");
  PrintVector(vReal, SAMPLES, SCL_TIME);
  fft_windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD); /* Weigh data */
  Serial.println("Weighed data:");
  PrintVector(vReal, SAMPLES, SCL_TIME);
  fft_compute(vReal, vImag, SAMPLES, power, FFT_FORWARD); /* Compute FFT */
  Serial.println("Computed Real values:");
  PrintVector(vReal, SAMPLES, SCL_INDEX);
  Serial.println("Computed Imaginary values:");
  PrintVector(vImag, SAMPLES, SCL_INDEX);
  fft_complex_to_magnitude(vReal, vImag, SAMPLES); /* Compute magnitudes */
  Serial.println("Computed magnitudes:");
  PrintVector(vReal, SAMPLES >> 1, SCL_FREQUENCY);
  double x;
  double v;
  fft_major_peak(vReal, SAMPLES, SAMPLING_FREQUENCY, &x, &v);
  Serial.print(x, 6);
  Serial.print(", ");
  Serial.println(v, 6);
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
