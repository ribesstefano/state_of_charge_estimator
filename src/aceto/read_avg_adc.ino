/*
 * Analog Input Reads shunt current and battery tension
 */
const float kBatteryCapacity = 12.0; // Ampere-hour
const float kMeasuredCircuitCurrent = 1.81;
const float kMeasuredShuntVoltage = 0.027;
const float kShuntRes = kMeasuredShuntVoltage / kMeasuredCircuitCurrent; // Ohm
const float kScalerPartit = 51.45; // 3.07; 
const float kScalerTime = 1.0 / (60.0 * 60.0);
const int kShuntPin = A0;    // select the input pin for the ADC
const int kBatteryPin = A1;    // select the input pin for the ADC
const int kNumReads = 1000; // numero di medie
const int kDelay = 1;
const float kScalerADC = 1.0 / 1023.0;

float avgStartTime = 0.0;
float avgStopTime = 0.0;
float avgShuntVoltage = 0.0;
float avgBatteryVoltage = 0.0;
float usedCharge = 0.0;

int i = 0;
int shuntVoltage = 0;  // variable to store the value coming from the SHUNT sensor
int batteryVoltage = 0;  // variable to store the value coming from the BATTERY sensor
unsigned long timestamp = 0;
unsigned long timestampOffset = 0;
float Q = 0.0;

void setup_adc() {
  timestampOffset = millis();
  avgStartTime = timestampOffset;
  // kScalerADC = readVcc() / 1000.0 / 1023.0;
  analogReference(INTERNAL);
}

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

/**
 * @brief      Prints a value to serial.
 *
 * @param[in]  str        The string in front
 * @param[in]  value      The value to print
 * @param[in]  precision  The precision of the printing
 * @param[in]  unit       The unit of measurement (without \n)
 */
void print_val_serial(const char* str, const float value, const int precision,
    const char* unit) {
  Serial.print(str);
  Serial.print(value, precision);
  Serial.print(unit);
}

/**
 * @brief      Gets the residual time given a current in Ampere.
 *
 * @param[in]  measured_current  The measured current in Ampere
 *
 * @return     The residual time in seconds
 */
float get_residual_time(const float measured_current) {
  return kBatteryCapacity / measured_current;
}

// Max four ADC pins on the Arduino UNO
const uint8_t adc_pins[] = {A0, A1, A2, A3};
#define NUM_ADC_PINS sizeof(adc_pins) / sizeof(uint8_t)

/**
 * @brief      Gets the average value from ADC after a number of reads.
 *
 * @param[in]  adc_mask    The ADC mask, i.e. which ADC pins to use
 * @param[in]  num_reads   The number of reads
 * @param[in]  read_delay  The read delay between reads
 * @param      avg_value   The average value vector
 */
void inline get_avg_read(const int (&adc_mask)[NUM_ADC_PINS],
    const int num_reads, const int read_delay,
    float (&avg_value)[NUM_ADC_PINS]) {
  for (int i = 0; i < NUM_ADC_PINS; ++i) {
    avg_value[i] = 0.0;
  }
  // const float scaler = readVcc() / 1000.0 / 1023.0;
  const float scaler = kScalerADC;
  for (int i = 0; i < num_reads; ++i) {
    for (int j = 0; j < NUM_ADC_PINS; ++j) {
      if (adc_mask[j] > 0) {
        int read_val = analogRead(adc_pins[j]);
        // TODO: Check if second read has to be discarded.
        // read_val = analogRead(adc_pins[j]);
        avg_value[j] += float(read_val) * scaler;
      }
    }
    delay(read_delay);
  }
  for (int j = 0; j < NUM_ADC_PINS; ++j) {
    avg_value[j] /= float(num_reads);
  }
}


void loop_adc() {
  const int adc_mask[NUM_ADC_PINS] = {1, 1, 0, 0};
  float avg_values[NUM_ADC_PINS] = {0.0, 0.0, 0.0, 0.0};
  // Get average values and measure time interval
  avgStartTime = millis() - timestampOffset;
  get_avg_read(adc_mask, kNumReads, kDelay, avg_values);
  avgStopTime = millis() - timestampOffset;
  // Make conversions and calculations
  const float Vshunt = avg_values[0];
  const float Vbattery = avg_values[1] * kScalerPartit;
  const float Dt = float(avgStopTime - avgStartTime) / 1000.0; // seconds
  const float I = Vshunt / kShuntRes; // Ampere
  const float Q = (Dt * I); // Ampere * sec
  const float residual_time = get_residual_time(I); // hours
  usedCharge += Q;
  // Print to Serial output
  print_val_serial("Ah = ", usedCharge * kScalerTime, 6, " C\t\t"); // Print Ampere-hour
  print_val_serial("V = ", Vbattery, 4, " V\n"); // Print Volt Battery
  // Print to LCD
  print_val_lcd(0, 0, "Ah=", usedCharge * kScalerTime, 2, "");
  // print_val_lcd(0, 1, "V=", Vbattery, 3, "");
  print_val_lcd(0, 1, "Q=", interpolate(Vbattery), 0, "%");
  print_val_lcd(9, 1, "I=", I, 3, "");
  // print_val_lcd(9, 0, "t=", residual_time, 1, "h ");
  char block = 255;
  print_val_lcd(9, 0, "t=", residual_time, 1, String("h" + String(block)).c_str());
  // if (residual_time >= 24) {
  //   print_val_lcd(9, 0, "t=", residual_time / float(24), 1, "d "); // days
  // } else if (residual_time < 1) {
  //   print_val_lcd(9, 0, "t=", residual_time / float(60), 1, "m "); // minutes
  // } else {
  //   print_val_lcd(9, 0, "t=", residual_time, 1, String("h" + String(block)).c_str());
  // }
  // Store to SD
  print_val_sd(usedCharge * kScalerTime, "\t");
  print_val_sd(Vbattery, "\n");
  // print_bin_val_sd(usedCharge * kScalerTime);
}