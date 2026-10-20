Mô hình cảnh báo nhiệt độ và lửa với ESP32-S3
Mô hình đọc nhiệt độ bằng DS18B20, nhận tín hiệu từ cảm biến lửa hồng ngoại và điều khiển còi qua transistor BC337.

Còi bật khi nhiệt độ trên 30°C hoặc cảm biến hồng ngoại nhận tín hiệu. Còi tắt khi nhiệt độ không quá 30°C và không còn tín hiệu hồng ngoại trong 1 giây.
Ngưỡng 30°C dùng để thử nghiệm trên bàn, không phải ngưỡng xác định cháy. Đây là mô hình học tập. Phiên bản này chưa tích hợp MQ-2.

1. Chức năng
- Đọc nhiệt độ DS18B20 và hiển thị trên Serial Monitor.
- Theo dõi chân DO của cảm biến lửa hồng ngoại.
- Bật còi khi có ít nhất một điều kiện báo động.
- Giữ báo động hồng ngoại thêm 1 giây sau tín hiệu cuối để dễ quan sát khi thử bằng remote TV.
- Đọc nhiệt độ theo chu kỳ, vẫn theo dõi hồng ngoại trong lúc chờ DS18B20 chuyển đổi.
- Hiển thị trạng thái nhiệt độ, hồng ngoại và còi.
Nhiệt độ hợp lệ	Hồng ngoại đang kích hoạt hoặc trong thời gian giữ 1 giây	Còi
≤ 30°C	Không	Tắt
> 30°C	Không	Bật
Bất kỳ	Có	Bật
Chưa đọc được nhiệt độ	Không	Tắt, Serial báo chưa đọc được
Chưa đọc được nhiệt độ	Có	Bật theo hồng ngoại


2. Linh kiện
Linh kiện	Số lượng	Ghi chú
ESP32-S3	1	Bo dùng trong mô hình là loại N16R8, hai cổng USB-C
Module DS18B20	1	Có các chân +, OUT, −
Module cảm biến lửa hồng ngoại	1	Loại có chân DO, cấp được 3V3
Còi chủ động	1	Chọn loại hoạt động được ở 3,3V; cấp DC là kêu
Transistor NPN BC337	1	Điều khiển còi
Điện trở 1kΩ	1	Giữa GPIO8 và chân B
Điện trở 10kΩ	1	Giữa B và GND, giữ transistor tắt khi đầu điều khiển chưa ổn định
Điện trở 4,7kΩ	1 nếu cần	Kéo OUT của DS18B20 lên 3V3, nếu module chưa có sẵn
Diode 1N4148	1	Mắc song song còi; dập xung với còi điện từ
Breadboard và dây nối	Theo nhu cầu	Dây đực–đực cho breadboard; cái–đực khi nối vào hàng chân đực của module
Cáp USB có truyền dữ liệu	1	Kết nối ESP32-S3 với máy tính


Nếu chỉ có điện trở 10kΩ, có thể dùng hai điện trở 10kΩ mắc song song để tạo khoảng 5kΩ thay cho 4,7kΩ khi thử DS18B20 với dây ngắn. Hai điện trở này không thay thế điện trở 10kΩ ở chân B của BC337.

Diode không tạo ra âm thanh. Vai trò dập xung áp dụng cho tải có cuộn dây; việc còi kêu khi cấp DC chỉ xác nhận đó là còi chủ động, chưa xác định nó là loại điện từ hay piezo.
3. Quy ước chân
Các số dưới đây là số GPIO in trên bo, không phải thứ tự chân đếm từ đầu hàng.
Tín hiệu	GPIO
OUT/DQ của DS18B20	GPIO4
DO/D0 của cảm biến lửa hồng ngoại	GPIO6
Điều khiển BC337 qua điện trở 1kΩ	GPIO8


Nếu đổi chân nối, phải đổi hằng số tương ứng trong code.
4. Đấu dây
Rút USB và các nguồn khác trước khi cắm hoặc thay dây.
4.1. Nguồn chung
- Nối 3V3 của ESP32 đến một đoạn thanh nguồn + trên breadboard.
- Nối GND của ESP32 đến đoạn thanh nguồn − tương ứng.
- Lấy nguồn cho các module từ hai thanh này.
- GND của DS18B20, cảm biến hồng ngoại và BC337 phải nối chung với GND ESP32.
- Một số breadboard ngắt thanh nguồn ở giữa. Nếu dùng hai đoạn khác nhau, nối dây cầu đúng cực giữa các đoạn.
- Thanh nguồn breadboard chỉ dẫn điện sau khi được nối với nguồn ESP32.
Mạch này sử dụng nguồn 3V3 cho cảm biến và còi tương thích 3,3V. Không cần lấy nguồn từ chân 5Vin để chạy cấu hình này.
4.2. DS18B20
Chân module DS18B20	Nối đến
+ / VCC / VDD	3V3
XUẤT / DỮ LIỆU / DQ	GPIO4
− / GND	GND


Giữ điện trở kéo lên giữa OUT và 3V3:
- Nếu module đã có điện trở kéo lên phù hợp, dùng điện trở sẵn trên module.
- Nếu chưa có, gắn điện trở 4,7kΩ: một đầu vào OUT, đầu còn lại vào 3V3.
- Với hai điện trở 10kΩ mắc song song: cả hai đều có một đầu vào OUT và đầu còn lại vào 3V3.
- Điện trở kéo lên không mắc nối tiếp giữa OUT và GPIO4.
Nếu dùng DS18B20 rời dạng TO-92 thay cho module: nhìn mặt phẳng có chữ hướng về người đọc, chân hướng xuống, thứ tự trái sang phải là GND – DQ – VDD. Với đầu dò có dây, xác định dây theo tài liệu của đầu dò, không suy đoán chỉ theo màu.
4.3. Cảm biến lửa hồng ngoại
Chân cảm biến	Nối đến
VCC / +	3V3
GND / −	GND
DO / D0	GPIO6
AO, nếu có	Để trống


Nối theo chữ in trên module. Code sử dụng loại module đã thử có DO xuống LOW khi nhận tín hiệu.
4.4. Còi và BC337
Với BC337 dạng TO-92 đã dùng trong mô hình, nhìn mặt phẳng có chữ hướng về người đọc, ba chân hướng xuống, thứ tự trái sang phải là:
Trái	Giữa	Phải
C — Collector	B — Base	E — Emitter


Đối chiếu datasheet đúng linh kiện trước khi cắm. DS18B20 rời có hình dáng giống transistor nhưng có sơ đồ chân khác.
Điểm nối	Nối đến
Còi +	3V3
Còi −	C của BC337
E của BC337	GND
GPIO8	Một đầu điện trở 1kΩ
Đầu còn lại của điện trở 1kΩ	B của BC337
Một đầu điện trở 10kΩ	B của BC337
Đầu còn lại của điện trở 10kΩ	GND, cùng điểm với E
Diode: đầu có vạch	Còi + / 3V3
Diode: đầu không có vạch	Còi − / C


Diode mắc song song hai chân còi. Đảo chiều diode có thể gây chập nguồn khi BC337 bật.

Không nối còi trực tiếp vào GPIO8: GPIO8 điều khiển chân B qua điện trở 1kΩ; dòng cấp còi đi qua nhánh C–E.
4.5. Các nhóm lỗ trên breadboard
Trong vùng cắm linh kiện của breadboard thông dụng:
- A1, B1, C1, D1, E1 thông điện với nhau.
- F1, G1, H1, I1, J1 thông điện với nhau.
- Hai nhóm A–E và F–J cùng hàng không thông qua khe giữa.
- A1 và A2 thuộc hai hàng khác nhau.
Do đó, C, B, E của BC337 phải nằm ở ba nhóm điện riêng. Có thể dùng ba hàng số khác nhau. Không cắm cả ba chân vào A40, B40, C40 vì sẽ nối chập chúng.
5. Chuẩn bị Arduino IDE
1. Cài hỗ trợ bo esp32 by Espressif Systems trong Boards Manager nếu chưa có.
2. Chọn Tools → Board → ESP32 Arduino → ESP32S3 Dev Module. Tên nhóm menu có thể khác giữa các phiên bản.
3. Mở Library Manager, cài:
   - OneWire của Paul Stoffregen.
   - DallasTemperature của Miles Burton.
4. Dùng cáp USB có truyền dữ liệu nối bo với máy tính.
5. Chọn đúng cổng trong Tools → Port.
Với bo hai cổng USB, cần khớp thiết lập Serial với cổng đang dùng:
Cổng kết nối	USB CDC On Boot
USB native, nối trực tiếp ESP32-S3	Enabled
COM/UART, qua chip chuyển USB–UART như CH343	Bị vô hiệu hóa


Cách phân biệt chắc chắn là xem nhãn cổng và tài liệu của bo. Không mặc định vị trí trái/phải giống nhau trên mọi bo.

Nếu dùng USB native và có các mục tương ứng, chọn USB Mode → Hardware CDC and JTAG, Upload Mode → UART0 / Hardware CDC. Các mục Flash/PSRAM chọn theo đúng bo; không cần thay các thiết lập đã nạp và chạy Serial ổn định.
6. Code tổng hợp
Trong Arduino IDE, tạo một sketch, dán toàn bộ code sau và lưu. Ví dụ đặt tên sketch là CanhBaoNhietDoHongNgoai.
- TEMP_PIN = 4: DS18B20.
- FLAME_PIN = 6: cảm biến lửa hồng ngoại.
- BUZZER_PIN = 8: còi qua BC337.
- NGUONG_NHIET = 30.0: ngưỡng thử nghiệm, đơn vị °C.
- Còi kêu liên tục khi còn điều kiện báo động.
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

  // Giu bao dong hong ngoai them 1 giay sau tin hieu cuoi.
  if (digitalRead(FLAME_PIN) == LOW) {
    coHongNgoai = true;
    lanNhanHongNgoai = now;
  }

  if (coHongNgoai && now - lanNhanHongNgoai >= 1000) {
    coHongNgoai = false;
  }

  // DS18B20 12-bit can toi da 750 ms de chuyen doi.
  // Cho 800 ms, dong thoi van theo doi hong ngoai.
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
DS18B20 cần tối đa 750 ms cho một lần chuyển đổi ở độ phân giải 12-bit. Code chờ 800 ms bằng mốc thời gian thay vì dùng một lệnh delay dài.

Khi chưa đọc được nhiệt độ hợp lệ, nhánh nhiệt độ không kích còi; nhánh hồng ngoại vẫn hoạt động. Bản demo này chưa có còi báo lỗi cảm biến riêng.
7. Nạp và mở Serial Monitor
1. Nhấn Verify để biên dịch.
2. Nhấn Upload.
3. Chờ thông báo ghi flash thành công và Hash of data verified.
4. Nếu bo chưa chạy chương trình sau khi nạp, nhấn rồi thả RST một lần.
5. Chọn lại cổng COM nếu số cổng thay đổi.
6. Mở Serial Monitor, chọn 115200 baud.
Ví dụ đầu ra, các số nhiệt độ chỉ để minh hoạ:
Nhiet do: 29.69 C | Hong ngoai: KHONG | Coi: TAT
Nhiet do: 31.06 C | Hong ngoai: KHONG | Coi: BAT
Nhiet do: 29.69 C | Hong ngoai: CO | Coi: BAT
Trong lúc chờ lần đo đầu tiên, có thể thấy CHUA DOC DUOC trong thời gian ngắn.
8. Cách kiểm tra
Nên kiểm tra từng nhánh trước khi thử đồng thời.
8.1. Nhiệt độ
1. Để cảm biến ngoài không khí và quan sát nhiệt độ ban đầu.
2. Ủ phần thân cảm biến bằng hai ngón tay khoảng 20–30 giây.
3. Khi nhiệt độ vượt 30°C, Serial hiện Coi: BAT và còi kêu.
4. Thả tay, chờ nhiệt độ giảm về không quá 30°C.  Nếu không có tín hiệu hồng ngoại, còi tắt.
Nếu nhiệt độ phòng đã trên 30°C, còi bật ngay là đúng theo ngưỡng đang đặt. Có thể đổi NGUONG_NHIET sang giá trị phù hợp để thử.
8.2. Hồng ngoại
Để nhiệt độ không quá ngưỡng để kiểm tra riêng nhánh này.
1. Hướng đầu phát remote TV vào đầu thu màu đen của cảm biến, thử ở khoảng cách 5–10 cm.
2. Bấm giữ một nút.
3. Khi module nhận được tín hiệu: Serial hiện Hong ngoai: CO và còi bật.
4. Thả nút. Sau khoảng 1 giây không có tín hiệu, còi tắt nếu nhiệt độ cũng không vượt ngưỡng.
Remote dùng để thử phản ứng hồng ngoại và đường tín hiệu DO; bài thử này không xác nhận độ nhạy phát hiện cháy. Khả năng phản ứng phụ thuộc remote, khoảng cách và độ nhạy đang chỉnh trên module.

Nếu module có biến trở, chỉnh từng chút rồi kiểm tra lại đèn DO và Serial. Không đặt mặc định rằng mọi module đều có cùng mức kích hoạt.
8.3. Kiểm tra điều kiện kết hợp
Tình huống	Kết quả mong đợi
Nhiệt độ không quá ngưỡng, không có hồng ngoại	Còi tắt
Nhiệt độ vượt ngưỡng, không có hồng ngoại	Còi bật
Nhiệt độ không quá ngưỡng, có hồng ngoại	Còi bật
Cả nhiệt độ và hồng ngoại đều kích hoạt	Còi bật
Ngừng hồng ngoại nhưng nhiệt độ vẫn vượt ngưỡng	Còi tiếp tục kêu
Cả hai điều kiện đều hết, đã qua thời gian giữ hồng ngoại	Còi tắt


9. Lỗi thường gặp
Thiếu OneWire.h hoặc DallasTemperature.h
Mở Library Manager, cài đúng hai thư viện ở mục 5 rồi biên dịch lại. Không xoá các dòng include để bỏ qua lỗi.
Kết nối không được với ESP32-S3: Không nhận được dữ liệu nối tiếp
1. Đóng Serial Monitor và các phần mềm đang dùng cổng COM.
2. Giữ BOOT, nhấn rồi thả RST, sau đó thả BOOT để vào chế độ nạp.
3. Chọn lại cổng trong Tools → Port, không mặc định số COM giữ nguyên.
4. Upload lại.
5. Nếu vẫn lỗi: rút USB, tạm tháo dây các thiết bị khỏi ESP32, cắm lại USB và thử nạp khi chỉ còn bo.
6. Nạp thành công rồi rút USB, nối lại từng nhánh đã kiểm tra.
Nếu bo chỉ nạp được sau khi tháo dây, kiểm tra việc đấu dây và nguồn trước khi ghép lại.
Nạp thành công nhưng Serial Monitor trống
- Kiểm tra cổng COM đã chọn.
- Kiểm tra USB CDC On Boot khớp với cổng USB native hoặc COM/UART.
- Nếu đổi thiết lập USB CDC, phải nạp lại sketch.
- Đặt Serial Monitor 115200 baud.
- Nhấn rồi thả RST một lần.
- Code in trạng thái mỗi 500 ms, kể cả khi chưa kích cảm biến.
Serial hiện CHUA DOC DUOC liên tục
- Kiểm tra + → 3V3, − → GND, OUT → GPIO4.
- Xác nhận đúng module DS18B20; nhãn + OUT − riêng lẻ chưa đủ xác định mã cảm biến.
- Kiểm tra điện trở kéo lên OUT–3V3 và tiếp xúc dây.
- Kiểm tra thanh nguồn breadboard có bị ngắt ở giữa không.
- Rút USB trước khi sửa dây, cắm lại để chương trình khởi động và nhận cảm biến.
Vượt ngưỡng nhiệt độ thì mất kết nối hoặc bo khởi động lại
Đó là lúc code bật còi. Nhánh còi đấu sai hoặc nguồn không ổn định có thể gây lỗi đúng thời điểm này.
1. Rút USB.
2. Ngắt toàn bộ nhánh còi, BC337 và diode khỏi ESP32; giữ DS18B20 cùng điện trở kéo lên.
3. Giữ nguyên code và thử cho nhiệt độ vượt ngưỡng.
4. Nếu lúc này vẫn đọc ổn định, kiểm tra lại nhánh còi: chiều diode, chân C–B–E, nhóm lỗ breadboard và nguồn khi có tải.
Không tăng điện áp cấp cho còi để xử lý lỗi này. Dùng đúng điện áp hoạt động của còi.
Serial báo Coi: BAT nhưng còi không kêu
- Kiểm tra dấu +/− trên còi.
- Kiểm tra GPIO8 qua 1kΩ đến B, E xuống GND, C nối còi −.
- Kiểm tra điện trở 1kΩ không bị cắm cả hai đầu vào cùng nhóm lỗ.
- Kiểm tra diode mắc song song đúng chiều, không mắc nối tiếp đường cấp còi.
- Kiểm tra loại còi: bản demo dùng còi chủ động, không dùng tone().
Có thể thử còi riêng bằng đúng nguồn định mức để xác nhận nó còn hoạt động.
Còi kêu liên tục
Trước tiên xem Serial:
- Nếu Coi: BAT: xem nhiệt độ có vượt ngưỡng hoặc Hong ngoai có đang là CO không.
- Nếu Coi: TAT nhưng vẫn kêu: kiểm tra nhánh transistor có bị nối tắt, nhầm chân hoặc chân B bị nối với nguồn.
- Điện trở 10kΩ B–GND giúp giữ trạng thái tắt khi đầu điều khiển chưa ổn định; nó không sửa được lỗi nối tắt C–E.
Đèn DO nháy nhưng Serial không báo hồng ngoại
- Kiểm tra đang nối DO/D0, không phải AO.
- Kiểm tra dây DO đến đúng GPIO6 và có GND chung.
- Kiểm tra module đã được cấp điện áp phù hợp.
- Code này sử dụng mức kích hoạt LOW. Nếu module khác có logic ngược, cần đối chiếu tài liệu và sửa điều kiện digitalRead.
10. Thay đổi ngưỡng và chân nối
Trong code, sửa các hằng số sau rồi nạp lại:
const int TEMP_PIN = 4;
const int FLAME_PIN = 6;
const int BUZZER_PIN = 8;
const float NGUONG_NHIET = 30.0;
Ví dụ đổi ngưỡng thử sang 35°C bằng cách đặt NGUONG_NHIET = 35.0.

Phiên bản hiện tại không dùng hysteresis: nhiệt độ dao động sát ngưỡng có thể làm còi bật/tắt theo từng lần đo. Khi phát triển tiếp, có thể bổ sung ngưỡng tắt thấp hơn ngưỡng bật.
11. Tài liệu tham khảo
- DS18B20 — datasheet của Analog Devices / Maxim
- BC337 — datasheet của onsemi
- DallasTemperature — thư viện và hướng dẫn
- OneWire — thư viện của Paul Stoffregen
- Arduino ESP32 — GPIO API
- Arduino ESP32 — USB CDC và nạp chương trình
- ESP32-S3 — chế độ boot và nạp thủ công
- ESP32-S3 — brownout và lỗi nguồn
- TDK — tài liệu còi điện từ và diode bảo vệ
