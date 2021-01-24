
void setup() {
  Serial.begin(19200);
  // Serial.begin(9600);
  setup_lcd();
  setup_sd();
  setup_adc();
}

void loop() {
  // static int i = 0;
  // print_bin_val_sd(3.14 * i);
  // print_val_sd(3.14 * i, "\n");
  // ++i;

  loop_adc();
}