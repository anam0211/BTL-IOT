Nối ESP32-S3 với cảm biến và còi
Rút USB trước khi nối dây. Tất cả GND nối chung.
1. Cảm biến nhiệt độ DS18B20
Chân cảm biến	Nối đến ESP32
+	3V3
RA NGOÀI	GPIO4
−	GND


Giữ điện trở kéo lên OUT–3V3 như mạch đã chạy được. Nếu chưa có, mắc 4,7kΩ giữa OUT và 3V3.
2. Cảm biến lửa hồng ngoại
Chân cảm biến	Nối đến ESP32
VCC / +	3V3
DO / D0	GPIO6
GND / −	GND


3. Transistor BC337 là gì
BC337: nhìn mặt phẳng có chữ hướng về bạn, chân chĩa xuống, thứ tự trái C — giữa B — phải E.
Điểm nối	Nối đến
Còi +	3V3 ESP32
Còi −	C của BC337
E của BC337	GND ESP32
GPIO8	Một đầu điện trở 1kΩ
Đầu còn lại của 1kΩ	B của BC337
Một đầu điện trở 10kΩ	B của BC337
Đầu còn lại của 10kΩ	GND
Diode: đầu có vạch	Còi + / 3V3
Diode: đầu không vạch	Còi − / C


Ba chân C, B, E cắm vào ba nhóm lỗ riêng trên breadboard. Diode mắc song song với còi.
Điều kiện báo động
- Nhiệt độ > 30°C hoặc nhận tín hiệu hồng ngoại → còi bật.
- Nhiệt độ ≤ 30°C và hết tín hiệu hồng ngoại được 1 giây → còi tắt.
