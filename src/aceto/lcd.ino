#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>

#define USE_LCD_I2C
// #define USE_LCD_OLED

#ifdef USE_LCD_I2C
// Pinout:
// SDA -> A4
// SCL -> A5
const unsigned int I2C_ADDR = 0x27; // Define I2C Address where the PCF8574A is
LiquidCrystal_I2C lcd(I2C_ADDR, 20, 4);
#elif defined USE_LCD_OLED

#else
const int rs = 9, en = 8, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#endif

/**
 * @brief      The LCD setup.
 */
void setup_lcd() {
  lcd.begin(16, 2);
#ifdef USE_LCD_I2C
  lcd.backlight();
#endif
}

/**
 * @brief      Prints a value to LCD.
 *
 * @param[in]  row        The row of the display
 * @param[in]  column     The column of the display
 * @param[in]  str        The string to print in front of the value
 * @param[in]  value      The value
 * @param[in]  precision  The precision of the print
 * @param[in]  unit       The unit to be printed afterwards
 */
void print_val_lcd(const int row, const int column, const char* str,
    const float value, const int precision, const char* unit) {
  lcd.setCursor(row, column);
  lcd.print(str);
  lcd.print(value, precision);
  lcd.print(unit);
}