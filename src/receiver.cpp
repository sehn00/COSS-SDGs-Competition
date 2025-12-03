#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ==== 핀 설정 ====
const int SDA_PIN = 3;   // I2C LCD - SDA
const int SCL_PIN = 4;   // I2C LCD - SCL

const int LED_PIN = 6;
const int VIB_PIN = 5;   // NPN tr base

LiquidCrystal_I2C lcd(0x27, 16, 2);

uint8_t senderMac[] = {0xB8, 0xF8, 0x62, 0xB4, 0x76, 0xC8};

// ==== RSSI 관련 변수 ====
volatile int   lastRssi = -127;
volatile bool  hasRssi  = false;
float          rssiAvg  = -100.0f;
const float    alpha    = 0.2f;

// ==== 근접/차단 임계값 ====
const int RSSI_NEAR_THRESHOLD   = -68;
const int RSSI_BLOCK_ENTER      = -53;
const int RSSI_BLOCK_EXIT       = -54;

bool tooCloseState = false;

// ==== ESP-NOW 수신 콜백 ====
void onRecv(const esp_now_recv_info* info, const uint8_t* data, int len) {
  if (!info || !info->rx_ctrl) return;

  const uint8_t* mac = info->src_addr;
  for (int i = 0; i < 6; i++) {
    if (mac[i] != senderMac[i]) return;
  }

  int rssi = info->rx_ctrl->rssi;
  lastRssi = rssi;
  hasRssi  = true;

  if (rssiAvg < -99.0f) {
    rssiAvg = (float)rssi;
  } else {
    rssiAvg = alpha * (float)rssi + (1.0f - alpha) * rssiAvg;
  }
}

// ==== 출력 제어 ====
void updateOutputs(bool isNear, bool tooClose, int rssi, float avg, bool has) {
  if (!has) {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(VIB_PIN, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting signal");
    lcd.setCursor(0, 1);
    lcd.print("No RSSI yet");

    Serial.println("NO_SIGNAL");
    return;
  }

  if (tooClose) {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(VIB_PIN, LOW);

    lcd.clear();

    Serial.print("last=");
    Serial.print(rssi);
    Serial.print(" avg=");
    Serial.print(avg, 1);
    Serial.println(" -> TOO_CLOSE");
  }
  else if (isNear) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(VIB_PIN, HIGH);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PRIORITY SEAT");

    lcd.setCursor(0, 1);
    lcd.print("PLEASE GIVE SEAT");

    Serial.print("last=");
    Serial.print(rssi);
    Serial.print(" avg=");
    Serial.print(avg, 1);
    Serial.println(" -> NEAR");
  }
  else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(VIB_PIN, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scanning...");
    lcd.setCursor(0, 1);
    lcd.print("RSSI:");
    lcd.print(avg, 1);
    lcd.print("dBm");

    Serial.print("last=");
    Serial.print(rssi);
    Serial.print(" avg=");
    Serial.print(avg, 1);
    Serial.println(" -> FAR");
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(VIB_PIN, OUTPUT);
  digitalWrite(VIB_PIN, LOW);

  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();

  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(onRecv);
}

void loop() {
  static unsigned long lastUpdate = 0;
  unsigned long now = millis();

  if (now - lastUpdate >= 500) {
    lastUpdate = now;

    int   rssiCopy;
    float avgCopy;
    bool  hasCopy;

    noInterrupts();
    rssiCopy = lastRssi;
    avgCopy  = rssiAvg;
    hasCopy  = hasRssi;
    interrupts();

    if (hasCopy) {
      if (!tooCloseState && avgCopy >= RSSI_BLOCK_ENTER) {
        tooCloseState = true;
      }
      else if (tooCloseState && avgCopy <= RSSI_BLOCK_EXIT) {
        tooCloseState = false;
      }
    }

    bool isNear = (hasCopy && !tooCloseState && avgCopy > RSSI_NEAR_THRESHOLD);
    updateOutputs(isNear, tooCloseState, rssiCopy, avgCopy, hasCopy);
  }
}
