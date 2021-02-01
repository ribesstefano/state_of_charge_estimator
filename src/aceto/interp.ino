
/*
 * Is using malloc() and free() a bad idea in Arduino?
 *  Reference: https://arduino.stackexchange.com/questions/682/is-using-malloc-and-free-a-really-bad-idea-on-arduino
 */
// TODO: To read a float from a binary value.
float snelheid;
union u_tag {
   byte b[4];
   float fval;
} u;

// NOTE: The zero coefficient starts at index zero.
// const float coeff[] = {10.465, 0.0262};
const float coeff[] = {-398.0, 38.042};
const int kNumCoeffs = sizeof(coeff) / sizeof(float);

float interpolate(const float x_in) {
  // TODO: To read a float from a binary value.
  // u.b[0] = snelheidArray[0];
  // u.b[1] = snelheidArray[1];
  // u.b[2] = snelheidArray[2];
  // u.b[3] = snelheidArray[3];
  // snelheid = u.fval;
  float x = x_in;
  float y = coeff[0];
  for (int i = 1; i < kNumCoeffs; ++i) { // We start from the second coefficient
     y += x * coeff[i];
     x *= x;
  }
  return y;
}