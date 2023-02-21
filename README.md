# FFT

Fast Fourier Transform in C. This is a port of [arduinoFFT](https://github.com/kosme/arduinoFFT).

### API

```c
uint8_t fft_revision(void);
uint8_t fft_exponent(uint16_t value);
void fft_parabola(double x1, double y1, double x2, double y2, double x3, double y3, double *a, double *b, double *c);
void fft_swap(double *x, double *y);
void fft_compute(double *vReal, double *vImag, uint16_t samples, uint8_t power, uint8_t dir);
void fft_complex_to_magnitude(double *vReal, double *vImag, uint16_t samples);
void fft_dc_removal(double *vData, uint16_t samples);
void fft_windowing(double *vData, uint16_t samples, uint8_t windowType, uint8_t dir);
void fft_major_peak(double *vData, uint16_t samples, double samplingFrequency, double *f, double *v);
void fft_major_peak_parabola(double *vData, uint16_t samples, double samplingFrequency, double *f);
```
