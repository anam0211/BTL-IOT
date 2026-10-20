# Hệ thống cảnh báo nhiệt độ và lửa với ESP32-S3

Sử dụng DS18B20, cảm biến lửa hồng ngoại và còi qua transistor BC337.

- Nhiệt độ **trên 30°C** hoặc hồng ngoại nhận tín hiệu → **còi bật**.
- Nhiệt độ **không quá 30°C** và hết tín hiệu hồng ngoại được **1 giây** → **còi tắt**.

> Ngưỡng 30°C dùng để thử nghiệm. Phiên bản này chưa tích hợp MQ-2.

## 1. Linh kiện

| Linh kiện | Số lượng |
|---|---:|
| ESP32-S3 | 1 |
| Module DS18B20, chân + OUT − | 1 |
| Cảm biến lửa hồng ngoại có chân DO | 1 |
| Còi chủ động hoạt động được ở 3,3V | 1 |
| Transistor BC337 | 1 |
| Điện trở 1kΩ | 1 |
| Điện trở 10kΩ cho BC337 | 1 |
| Điện trở kéo lên 4,7kΩ cho DS18B20 | 1 nếu module chưa có |
| Diode 1N4148 | 1 |
| Breadboard, dây nối, cáp USB truyền dữ liệu | Theo nhu cầu |

Nếu không có 4,7kΩ, có thể dùng **hai điện trở 10kΩ song song** thành 5kΩ để thử DS18B20 với dây ngắn.

## 2. Nối nguồn chung

**Rút USB trước khi cắm hoặc thay dây.**

| Chân ESP32 | Nối đến |
|---|---|
| 3V3 | Thanh nguồn + trên breadboard |
| GND | Thanh nguồn − trên breadboard |

- Các chân ghi **3V3** bên dưới đều nối về thanh +.
- Các chân ghi **GND** đều nối về thanh −.
- Tất cả GND phải nối chung với ESP32.
- Nếu thanh nguồn bị ngắt ở giữa, nối dây cầu đúng cực giữa các đoạn đang sử dụng.
- Các số GPIO là số in trên bo, không phải vị trí chân đếm từ đầu hàng.

## 3. Nối cảm biến nhiệt độ DS18B20

| Chân DS18B20 | Nối đến |
|---|---|
| + | 3V3 |
| OUT | GPIO4 |
| − | GND |

### Điện trở kéo lên

Nếu module đã có điện trở kéo lên thì giữ nguyên.

Nếu chưa có, nối:

| Điện trở 4,7kΩ | Nối đến |
|---|---|
| Một đầu | OUT của DS18B20 |
| Đầu còn lại | 3V3 |

Nếu dùng hai điện trở 10kΩ song song: **mỗi con đều nối một đầu vào OUT, đầu còn lại vào 3V3**.

Không mắc điện trở này nối tiếp giữa OUT và GPIO4.

## 4. Nối cảm biến lửa hồng ngoại

| Chân cảm biến | Nối đến |
|---|---|
| VCC / + | 3V3 |
| DO / D0 | GPIO6 |
| GND / − | GND |
| AO, nếu có | Để trống |

Nối theo chữ in trên module. Code sử dụng loại cảm biến có **DO xuống LOW khi nhận tín hiệu**.

## 5. Nối còi qua BC337

### Xác định chân BC337

Với BC337 TO-92 đã dùng trong mạch, nhìn **mặt phẳng có chữ hướng về bạn, ba chân chĩa xuống**:

| Trái | Giữa | Phải |
|---|---|---|
| C | B | E |

Đối chiếu datasheet nếu dùng linh kiện khác.

### Nối từng dây

| Điểm nối | Nối đến |
|---|---|
| Còi + | 3V3 |
| Còi − | C của BC337 |
| E của BC337 | GND |
| GPIO8 | Một đầu điện trở 1kΩ |
| Đầu còn lại của 1kΩ | B của BC337 |
| Một đầu điện trở 10kΩ | B của BC337 |
| Đầu còn lại của 10kΩ | GND |
| Diode: đầu có vạch | Còi + / 3V3 |
| Diode: đầu không có vạch | Còi − / C |

- Diode mắc **song song còi**, không mắc nối tiếp.
- Đảo chiều diode có thể gây chập nguồn khi BC337 bật.
- Điện trở 1kΩ và 10kΩ không phân cực.
- GPIO8 điều khiển B qua 1kΩ; không cấp nguồn trực tiếp cho còi.

### Lưu ý breadboard

**Ba chân C, B, E phải cắm vào ba nhóm lỗ riêng.**

Ví dụ: A40, A41, A42 thuộc ba hàng riêng. Không cắm C, B, E vào A40, B40, C40 vì ba lỗ đó thông điện với nhau.

## 6. Cài đặt Arduino IDE

1. Cài gói bo **esp32 by Espressif Systems** trong Boards Manager.
2. Chọn bo **ESP32S3 Dev Module**.
3. Trong Library Manager, cài:
   - **OneWire** của Paul Stoffregen.
   - **DallasTemperature** của Miles Burton.
4. Chọn đúng cổng trong **Tools → Port**.

Với bo có hai cổng USB:

| Cổng đang sử dụng | USB CDC On Boot |
|---|---|
| USB native, nối trực tiếp ESP32-S3 | Enabled |
| COM/UART, qua chip chuyển USB–UART | Disabled |

Sau khi đổi thiết lập USB CDC, cần nạp lại code.

## 7. Code tổng hợp

```cpp
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

  // Giu trang thai hong ngoai them 1 giay.
  if (digitalRead(FLAME_PIN) == LOW) {
    coHongNgoai = true;
    lanNhanHongNgoai = now;
  }

  if (coHongNgoai && now - lanNhanHongNgoai >= 1000) {
    coHongNgoai = false;
  }

  // Cho DS18B20 chuyen doi, van theo doi hong ngoai.
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
```

## 8. Nạp và kiểm tra

1. Nhấn **Verify**, sau đó **Upload**.
2. Chờ thông báo **Hash of data verified**.
3. Mở **Serial Monitor → 115200 baud**.
4. Nếu màn hình trống, chọn lại đúng COM rồi nhấn RST một lần.

Ví dụ:

```text
Nhiet do: 29.69 C | Hong ngoai: KHONG | Coi: TAT
Nhiet do: 31.06 C | Hong ngoai: KHONG | Coi: BAT
Nhiet do: 29.69 C | Hong ngoai: CO | Coi: BAT
```

### Thử nhiệt độ

Ủ thân DS18B20 bằng hai ngón tay khoảng 20–30 giây. Khi nhiệt độ vượt 30°C, còi bật. Thả tay và chờ nhiệt độ giảm về không quá 30°C để còi tắt nếu không có hồng ngoại.

Nếu nhiệt độ phòng đã trên 30°C, còi bật ngay là đúng theo code.

### Thử hồng ngoại

Để nhiệt độ không vượt ngưỡng. Hướng remote TV vào đầu đen cảm biến, cách khoảng 5–10 cm, rồi bấm giữ một nút.

Khi nhận tín hiệu, còi bật. Thả nút khoảng 1 giây, còi tắt nếu nhiệt độ cũng không vượt ngưỡng.

Remote chỉ dùng để thử phản ứng hồng ngoại và đường DO, chưa xác nhận độ nhạy phát hiện cháy.

## 9. Xử lý lỗi nhanh

| Lỗi | Cách kiểm tra |
|---|---|
| Không tìm thấy OneWire.h | Cài thư viện OneWire trong Library Manager |
| Không tìm thấy DallasTemperature.h | Cài thư viện DallasTemperature |
| Failed to connect / No serial data received | Đóng Serial Monitor; giữ BOOT, nhấn thả RST, thả BOOT; chọn lại COM và Upload |
| Nạp được nhưng Serial trống | Kiểm tra COM, USB CDC, baud 115200; nhấn RST |
| CHUA DOC DUOC liên tục | Kiểm tra DS18B20: + → 3V3, OUT → GPIO4, − → GND và điện trở kéo lên |
| Vừa bật còi thì mất kết nối | Rút USB; kiểm tra chiều diode, C–B–E và các hàng breadboard; thử riêng DS18B20 khi đã ngắt nhánh còi |
| Serial báo còi bật nhưng không kêu | Kiểm tra còi +/−, GPIO8 qua 1kΩ đến B, E xuống GND, C nối còi − |
| Serial báo còi tắt nhưng vẫn kêu | Kiểm tra nhánh C–E bị nối tắt hoặc B bị nối vào nguồn |
| Đèn DO nháy nhưng không nhận hồng ngoại | Kiểm tra DO → GPIO6, GND chung và mức kích hoạt LOW |

Khi chưa đọc được DS18B20, code không kích báo động theo nhiệt độ; nhánh hồng ngoại vẫn hoạt động.

## 10. Thay đổi ngưỡng

Sửa dòng sau rồi nạp lại:

```cpp
const float NGUONG_NHIET = 30.0;
```

Ví dụ đặt `35.0` để thử với ngưỡng 35°C.
