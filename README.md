
# STM32 & Arduino CAN Bus Communication Project

Bu proje, **STM32F407** mikrodenetleyicisi ile **Arduino Uno** arasında **CAN Bus (Controller Area Network)** protokolü kullanılarak güvenilir bir veri iletişimi sağlamayı amaçlamaktadır. Sistem, STM32 tarafındaki bir sensörden (ADXL345) alınan verileri okur, CAN hattı üzerinden paketler ve Arduino tarafındaki LCD ekranda görüntüler.


## 📋 Proje Özeti

Proje iki ana düğümden (node) oluşmaktadır:
1.  **Verici (Transmitter) - STM32F4:** 3 eksenli ivmeölçerden (ADXL345) veri okur ve bu veriyi CAN Bus üzerinden yayınlar.
2.  **Alıcı (Receiver) - Arduino Uno:** CAN Bus hattını dinler, gelen veri paketini ayrıştırır ve sonuçları 16x2 LCD ekranda kullanıcıya sunar.

## 🛠 Donanım Gereksinimleri

Bu projeyi gerçekleştirmek için aşağıdaki donanımlara ihtiyacınız vardır:

* **STM32 Tarafı:**
    * STM32F407G-DISC1 (Discovery Board)
    * SN65HVD230 CAN Transceiver Modülü
    * ADXL345 İvmeölçer Sensörü (I2C/SPI)
* **Arduino Tarafı:**
    * Arduino Uno R3
    * MCP2515 CAN Bus Modülü (SPI Arayüzlü)
    * 16x2 Karakter LCD Ekran
    * 10k Potansiyometre (LCD kontrast ayarı için)
* **Genel:**
    * Jumper kablolar
    * Breadboard

## 🔌 Bağlantı Şeması (Pinout)

### 1. STM32F407 Bağlantıları
| STM32 Pin | Modül | Açıklama |
| :--- | :--- | :--- |
| **PB8** | CAN Tx Modülü | CAN_RX (Alternatif Pin) |
| **PB9** | CAN Tx Modülü | CAN_TX (Alternatif Pin) |
| **PB6** | ADXL345 | I2C1_SCL |
| **PB7** | ADXL345 | I2C1_SDA |
| **5V / GND** | Modüller | Güç Beslemesi |

### 2. Arduino Uno Bağlantıları (MCP2515 & LCD)
| Arduino Pin | Modül | Fonksiyon |
| :--- | :--- | :--- |
| **D10** | MCP2515 | CS (Chip Select) |
| **D11** | MCP2515 | MOSI |
| **D12** | MCP2515 | MISO |
| **D13** | MCP2515 | SCK |
| **D2** | LCD | RS |
| **D3** | LCD | E (Enable) |
| **D4-D7** | LCD | Data Yolları (4-bit mod) |

*Not: İki CAN modülü arasındaki **CAN_H** ve **CAN_L** uçları birbirine, **GND** hatları ise ortak toprağa bağlanmalıdır.*
<img width="3603" height="1572" alt="project wiring diagram" src="https://github.com/user-attachments/assets/a41d413f-3a18-4369-99ce-dd5e2dc45d30" />

## 💻 Yazılım ve Kütüphaneler

### STM32 (Verici)
* **IDE:** STM32CubeIDE 
* **Framework:** HAL Library (Hardware Abstraction Layer)
* **Ayarlar:**
    * CAN1 aktif (Prescaler ve Time Quanta ayarları 500kbps veya 250kbps hıza göre yapılmalı).
    * I2C1 aktif (ADXL345 okuması için).

### Arduino (Alıcı)
* **IDE:** Arduino IDE
* **Kütüphaneler:**
    * `mcp_can` (Cory J. Fowler) - MCP2515 kontrolü için.
    * `LiquidCrystal` - LCD ekran kontrolü için.

## 🚀 Kurulum ve Kullanım

1.  **Bağlantıları Yapın:** Yukarıdaki şemaya ve tabloya göre devre bağlantılarını breadboard üzerinde kurun.
2.  **STM32 Kodunu Yükleyin:** `CAN_COM` klasöründeki projeyi derleyin ve STM32F4 kartına flashlayın.
3.  **Arduino Kodunu Yükleyin:** `can_com` klasöründeki `.ino` dosyasını Arduino IDE ile açın, gerekli kütüphaneleri indirin ve karta yükleyin.
4.  **Sistemi Başlatın:** Her iki karta da güç verin.
5.  **Test:** ADXL345 sensörünü hareket ettirdiğinizde, Arduino'ya bağlı LCD ekranda değişen değerleri görmelisiniz.


