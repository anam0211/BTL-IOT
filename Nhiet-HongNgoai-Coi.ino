#include <OneWire.h>
#include <DallasTemperature.h>

const int TEMP_PIN = 4;
const int FLAME_PIN = 6;
const int BUZZER_PIN = 8;

const float NGUONG_NHIET = 30.0;

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

float nhietDo = 0;
bool nhietDoHopLe = false;
bool coHongNgoai = false;

unsigned long lanNhanHongNgoai = 0;
unsigned long batDauDoNhiet = 0;
unsigned long lanInTruoc = 0;

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(FLAME_PIN, INPUT_PULLUP);

  sensors.begin();
  sensors.setResolution(12);
  sensors.setWaitForConversion(false);

  sensors.requestTemperatures();
  batDauDoNhiet = millis();
}

void loop() {
  unsigned long now = millis();

  // Hồng ngoại: giữ trạng thái thêm 1 giây.
  if (digitalRead(FLAME_PIN) == LOW) {
    coHongNgoai = true;
    lanNhanHongNgoai = now;
  }

  if (coHongNgoai && now - lanNhanHongNgoai >= 1000) {
    coHongNgoai = false;
  }

  // Đọc sau khi DS18B20 có đủ thời gian đo.
  if (now - batDauDoNhiet >= 800) {
    float t = sensors.getTempCByIndex(0);

    nhietDoHopLe =
      (t != DEVICE_DISCONNECTED_C && t >= -55 && t <= 125);

    if (nhietDoHopLe) {
      nhietDo = t;
    }

    sensors.requestTemperatures();
    batDauDoNhiet = millis();
  }

  bool quaNhiet = nhietDoHopLe && nhietDo > NGUONG_NHIET;
  bool baoDong = quaNhiet || coHongNgoai;

  digitalWrite(BUZZER_PIN, baoDong ? HIGH : LOW);

  if (now - lanInTruoc >= 500) {
    lanInTruoc = now;

    Serial.print("Nhiet do: ");
    if (nhietDoHopLe) {
      Serial.print(nhietDo, 2);
      Serial.print(" C");
    } else {
      Serial.print("CHUA DOC DUOC");
    }

    Serial.print(" | Hong ngoai: ");
    Serial.print(coHongNgoai ? "CO" : "KHONG");

    Serial.print(" | Coi: ");
    Serial.println(baoDong ? "BAT" : "TAT");
  }
}