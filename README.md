# COSS-SDGs-Competition
2025 COSS SDGs 기반 사회문제 해결 경진대회 대상(1위) 수상작

<br>

## 비접촉형 교통약자 배려 시스템 (Non-Contact Priority System)
*COSS SDGs 사회문제 해결 경진대회 — 대상(1위) 수상 프로젝트*

고령자·임산부·장애인 등 교통약자가 **양보 요청을 직접 하기 어려운 문제**를 해결하기 위해 개발한 비접촉식 인지·알림 시스템.  
ESP32 RSSI 기반 거리 판단 + 장치 내장 LED/진동/LCD 표시를 통해 **소음 없이 자연스럽게 양보를 유도**함.

---

## 🚀 프로젝트 개요

- **근접 감지 자동 인식**  
  ESP32 Wi-Fi RSSI 기반으로 사용자의 위치를 인지하고 거리 구간(FAR, NEAR, TOO CLOSE)으로 분류

- **소음 없는 알림 방식**  
  진동모터, LED, LCD 메시지를 활용하여 자연스럽게 양보 요청

- **3D 프린팅 기반 일체형 디자인**  
  직접 설계한 팔걸이 구조물에 ESP32 + 진동모터 + LCD + 배터리 일체형 내장

- **구조 안전성 검증**  
  Autodesk Fusion에서 500N 하중 가정 변형/응력 시뮬레이션 수행

---

## 🛠 기술 스택

- **Hardware**: ESP32-C3, 진동모터, LCD, 3D 프린팅 하우징  
- **Software**: C/C++ (Arduino, ESP32), ESP-NOW 기반 브로드캐스트 통신, RSSI 평균/임계값 기반 거리 판단 로직
- **Design/Simulation**: Fusion360 모델링 및 구조해석  

---

## 📡 동작 알고리즘 (요약)

1. ESP32가 주변 Wi-Fi 패킷 수신  
2. RSSI 측정 → 노이즈 제거 및 평균화  
3. 거리 상태 판별  
   - RSSI < -68 dBm → **FAR**  
   - -68 dBm ≤ RSSI < -53 dBm → **NEAR**  
   - RSSI ≥ -53 dBm → **TOO CLOSE**  
4. 상태에 따라 LED/진동/LCD 메시지를 출력하여 양보 요청

---

## 📈 기대 효과

- 교통약자가 직접 요청하지 않아도 양보 상황을 자연스럽게 유도  
- 버스·지하철 등 공공 교통수단의 배려 좌석 접근성을 개선  
- 상업적/공공기관 연동 시 확장 가능성 높음  

---

## 🏆 수상 내역

- **COSS 주관 SDGs 사회문제 해결 경진대회 – 대상(1위)**  
  (프로젝트명: 비접촉형 교통약자 배려 시스템)

---

## 📜 License

This repository is public for portfolio and review purposes only.

**Copyright (c) 2025. All rights reserved.**  
Copying, modification, redistribution, or reuse of the code or design in this project is **not permitted** without explicit permission from the author.
