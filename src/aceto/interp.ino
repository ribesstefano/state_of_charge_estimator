
/*
 * Is using malloc() and free() a bad idea in Arduino?
 *  Reference: https://arduino.stackexchange.com/questions/682/is-using-malloc-and-free-a-really-bad-idea-on-arduino
 */

float snelheid;

union u_tag {
   byte b[4];
   float fval;
} u;

const float coeff[] = {0.0, 0.0, 0.0, 0.0};
const int kNumCoeffs = 4;

float interpolate(const float x_in) {
  // u.b[0] = snelheidArray[0];
  // u.b[1] = snelheidArray[1];
  // u.b[2] = snelheidArray[2];
  // u.b[3] = snelheidArray[3];
  // snelheid = u.fval;
  
  float x = x_in;
  float y = coeff[0];
  // NOTE: We start from the second coefficient.
  for (int i = 1; i < kNumCoeffs; ++i) {
     y += x * coeff[i];
     x *= x;
  }
  return y;
}