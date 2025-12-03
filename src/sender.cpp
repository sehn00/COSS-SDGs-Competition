#include <WiFi.h>
#include <esp_now.h>

// 브로드캐스트 주소 (모든 ESP-NOW 디바이스에게 전송)
uint8_t broadcastAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct {
  uint32_t counter;
} PingMsg;

PingMsg msg;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println("=== A 보드 (Sender) 시작 ===");

  // ESP-NOW는 STA 모드에서 사용
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init 실패");
    return;
  }
  Serial.println("ESP-NOW init 성공");

  // 브로드캐스트 peer 등록
  esp_now_peer_info_t peer{};
  memcpy(peer.peer_addr, broadcastAddr, 6);
  peer.channel = 0;      // 0 = 현재 채널 사용
  peer.encrypt = false;

  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("peer 등록 실패");
    return;
  }
  Serial.println("peer 등록 성공, 브로드캐스트 전송 준비 완료");
}

unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL_MS = 500;

void loop() {
  unsigned long now = millis();
  if (now - lastSend >= SEND_INTERVAL_MS) {
    lastSend = now;
    msg.counter++;

    esp_err_t result = esp_now_send(broadcastAddr, (uint8_t*)&msg, sizeof(msg));
    Serial.print("PING 보냄, cnt = ");
    Serial.print(msg.counter);
    Serial.print(" / 결과 = ");
    Serial.println(result == ESP_OK ? "OK" : "ERR");
  }
}
