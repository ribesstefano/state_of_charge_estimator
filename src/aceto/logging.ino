
#if 0
#define VERBOSE_LEVEL 1
#define LOG(info) {Serial.print("[INFO] "); Serial.println(info); }
#define LOGINFO(info, x) { \
  if (x > VERBOSE_LEVEL) { \
    Serial.print("[INFO] "); Serial.println(info); }

#define ERROR(err) {Serial.print("[ERROR] "); Serial.println(err); }
#define LOGERROR(err, x) { \
  if (x > VERBOSE_LEVEL) { \
    Serial.print("[ERROR] "); Serial.println(err); }
#endif