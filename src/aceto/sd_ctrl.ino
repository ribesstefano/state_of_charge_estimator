/*
 * Pinout:
 * VCC  - 5V
 * MOSI - pin 11 (UNO) - pin 11 (Nano)
 * MISO - pin 12 (UNO) - pin 12 (Nano)
 * SCLK - pin 13 (UNO) - pin 13 (Nano)
 * CS   - pin 10 (UNO) - pin 10 (Nano)
 *
 * CS pin depends on your SD card shield or module.
 */
#include <SPI.h>
#include <SD.h>

#define LOG(info) {Serial.print("[INFO] "); Serial.println(info); }
#define ERROR(err) {Serial.print("[ERROR] "); Serial.println(err); }

const int CS_PIN = 10;
const char kLogFilename[] = "aceto.log";
const char kLogBinFilename[] = "aceto.bin";
unsigned long last_position = 0;
File dataFile;
File dataBinFile;

void setup_sd() {
  while (!SD.begin(CS_PIN)) {
    LOG("Unable to open SD card. Retrying in 5 seconds.");
    delay(5000);
  }
  LOG("SD card opened.");
  dataFile = SD.open(kLogFilename, FILE_WRITE);
  
  // ===========================================================================
  // NOTE: The default behavior is appending. By using seek(0), it starts
  // writing from the beginning of the file, thus overwriting it.
  // ===========================================================================
  dataFile.seek(0);

  if (!SD.exists(kLogFilename)) {
    LOG("Creating file " + String(kLogFilename));
    dataFile = SD.open(kLogFilename, FILE_WRITE);
    last_position = dataFile.size();
  }
  if (!SD.exists(kLogBinFilename)) {
    LOG("Creating binary file " + String(kLogBinFilename));
    dataBinFile = SD.open(kLogBinFilename, FILE_WRITE);
  }
  // // Read last value on SD
  // dataBinFile.seek(dataBinFile.size() - sizeof(float));
  // byte read_val[sizeof(float)];
  // dataBinFile.read(read_val, sizeof(float));
  // float read_val_f = *((float*)(&read_val));
  // Serial.print("[INFO] Last read value: ");
  // Serial.println(String(read_val_f));
}

void print_val_sd(const float value, const char* divider) {
  if (!SD.exists(kLogFilename)) {
    LOG("File ACETO.log doesn't exists. Creating it.");
    dataFile = SD.open(kLogFilename, FILE_WRITE);
  }
  static char str[15];
  const int minStringWidthIncDecimalPoint = 7;
  const int numVarsAfterDecimal = 6;
  dtostrf(value, minStringWidthIncDecimalPoint, numVarsAfterDecimal, str);
  const int written_bytes = dataFile.write(str);
  dataFile.write(divider);
  dataFile.flush();
  if (written_bytes == 0) {
    ERROR("Unable to write to file.");
  }
}

// void printDirectory(File dir, int numTabs) {
//   while (true) {
//     File entry =  dir.openNextFile();
//     if (! entry) {
//       // no more files
//       break;
//     }
//     for (uint8_t i = 0; i < numTabs; i++) {
//       Serial.print('\t');
//     }
//     Serial.print(entry.name());
//     if (entry.isDirectory()) {
//       Serial.println("/");
//       printDirectory(entry, numTabs + 1);
//     } else {
//       // files have sizes, directories do not
//       Serial.print("\t\t");
//       Serial.println(entry.size(), DEC);
//     }
//     entry.close();
//   }
// }


void print_bin_val_sd(const float value) {
  if (!SD.exists(kLogBinFilename)) {
    LOG("File ACETO.bin doesn't exists. Creating it.");
    dataBinFile = SD.open(kLogBinFilename, FILE_WRITE);
  }
  // Write to binary file
  const float fval = value;
  const int written_bytes = dataBinFile.write((byte*)(&fval), sizeof(float));
  dataBinFile.flush();
  if (written_bytes == 0) {
    ERROR("Unable to write to binary file.");
  }
  // File root = SD.open("/");
  // printDirectory(root, 0);
}

// void loop_sd() {
//   if (SD.exists("LOG.txt")) {
//     LOG("File LOG.txt exists.");
//   }
//   File dataFile = SD.open("LOG.txt", FILE_WRITE);
//   if (dataFile) {
//     LOG("File opened.");
//     dataFile.write("Hello world!\n");
//     dataFile.close();
//   } else {
//     ERROR("Unable to open LOG.txt");
//   }
//   delay(1000);
// }

template <typename T = float>
T read_binary(File &bin_file, const bool read_last = true) {
  union x {
      byte buffer[sizeof(T)];
      T val;
  } field;
  if (read_last) {
    bin_file.seek(bin_file.size() - sizeof(T));
  }
  bin_file.read(field.buffer, sizeof(T));
  return field.val;
}