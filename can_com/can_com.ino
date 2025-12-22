#include <SPI.h>
#include <mcp_can.h>
#include <LiquidCrystal.h>

// Pin Tanımları
const int SPI_CS_PIN = 10;
const int INT_PIN = 7;

// LCD Pin Tanımları (uygun pinleri kartınıza göre değiştirin)
// Not: Önceki sürümlerde d7 = 2 olarak kullanılıyordu — eğer ekranınız bağlıysa bunu deneyin.
const int rs = 8, en = 9, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// MCP_CAN nesnesini CS pini ile oluştur
MCP_CAN CAN0(SPI_CS_PIN);

void setup()
{
    // Seri haberleşmeyi başlat (Hız: 115200)
    Serial.begin(115200);
    Serial.println("CAN Bus Alici Baslatiliyor...");

    // LCD başlat
    lcd.begin(16, 2);
    lcd.setCursor(0,0);
    lcd.print("CAN Alici Basliyor");
    lcd.setCursor(0,1);
    lcd.print("Bekleyiniz...");
    delay(1000);

    // LCD startup self-test (blink display) to help debug wiring/contrast
    auto lcdStartupTest = [&]() {
        Serial.println("LCD startup test: blink display 4 times");
        lcd.setCursor(0,0);
        lcd.print("LCD TEST");
        lcd.setCursor(0,1);
        lcd.print("If blank, check pot/GND");
        delay(600);
        for(int i=0;i<4;i++){
            lcd.noDisplay(); delay(300);
            lcd.display(); delay(300);
        }
        lcd.clear();
    };
    lcdStartupTest();
    // Quick visible sanity check: show a simple HELLO message
    lcd.setCursor(0,0);
    lcd.print("HELLO");
    lcd.setCursor(0,1);
    lcd.print("LCD OK");
    delay(1500);
    lcd.clear();

    // CAN Modülünü Başlat
    // ÖNEMLİ: Modülünüzün üzerindeki kristal 8MHz ise MCP_8MHZ, 16MHz ise MCP_16MHZ yapın.
    // STM32 tarafı 500kbps ayarlandığı için burası da CAN_500KBPS olmalı.
    if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) 
    {
        Serial.println(">>> BASARILI: CAN Modulu Baslatildi! <<<");
    } 
    else 
    {
        Serial.println("!!! HATA: CAN Modulu bulunamadi veya baglanti hatasi !!!");
        Serial.println("Lutfen kablolari ve kristal hizini kontrol edin.");
        while(1); // Hata varsa burada duralım
    }

    // Modu Normal'e al (Veri alışverişi için gerekli)
    CAN0.setMode(MCP_NORMAL);
    
    // Kesme (Interrupt) pinini giriş yap
    pinMode(INT_PIN, INPUT);
    
    Serial.println("STM32'den veri bekleniyor...");
    Serial.println("-----------------------------------");
}

// Helper: format floats for LCD
void printFloatToLCD(float val, int width, int precision) {
    char buf[16];
    dtostrf(val, width, precision, buf);
    lcd.print(buf);
}

// Gösterge: x,y,z değerlerini LCD'ye yaz
void displayAccel(float x, float y, float z, long unsigned int rxId, uint8_t counter) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("X:"); printFloatToLCD(x, 6, 2);
    lcd.print(" Y:"); printFloatToLCD(y, 6, 2);
    lcd.setCursor(0,1);
    lcd.print("Z:"); printFloatToLCD(z, 6, 2);
    lcd.print(" C:"); lcd.print(counter);
}

void loop()
{
    // INT pini LOW olduğunda yeni bir mesaj gelmiş demektir
    if(!digitalRead(INT_PIN)) 
    {
        long unsigned int rxId;
        unsigned char len = 0;
        unsigned char rxBuf[8];

        // Veriyi tampondan (buffer) oku
        CAN0.readMsgBuf(&rxId, &len, rxBuf);

        // --- Tüm Gelen Mesajları Göster ---
        // Sadece STM32'den gelen (ID: 0x103) mesajları değil, hattaki her şeyi gösterir.
        // Eğer filtrelemek isterseniz: if(rxId == 0x103) { ... } içine alabilirsiniz.
        
        Serial.print("Mesaj Geldi -> ID: 0x");
        Serial.print(rxId, HEX); // ID'yi Hexade cimal (16'lık) formatta yaz
        
        Serial.print(" | Uzunluk: ");
        Serial.print(len);
        
        Serial.print(" | Veri: ");

        if(len == 0) {
            Serial.print("(no payload)");
            // Show a helpful message on the LCD so user knows why nothing appears
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("No payload");
            lcd.setCursor(0,1);
            lcd.print("ID:0x"); lcd.print(rxId, HEX);
            lcd.print(" L:"); lcd.print(len);
        }
        else {
            // Print payload as hex for debugging
            for(int i = 0; i < len; i++) {
                if(rxBuf[i] < 16) Serial.print('0');
                Serial.print(rxBuf[i], HEX);
                Serial.print(' ');
            }

            // STM32 kodumuzda 0. byte'a sayaç değeri koymuştuk; 7-byte paket bekliyoruz
            if(rxId == 0x103) {
                Serial.print(" (Sayac Degeri: ");
                Serial.print(rxBuf[0]);
                Serial.print(")");

                if(len == 7) {
                    uint8_t counter = rxBuf[0];
                    int16_t x = (int16_t)((rxBuf[2] << 8) | rxBuf[1]);
                    int16_t y = (int16_t)((rxBuf[4] << 8) | rxBuf[3]);
                    int16_t z = (int16_t)((rxBuf[6] << 8) | rxBuf[5]);

                    // ADXL345 full-res +/-2g: ~3.9 mg/LSB -> 0.0039 g/LSB
                    const float scale = 0.0039f;
                    float xf = x * scale;
                    float yf = y * scale;
                    float zf = z * scale;

                    Serial.print("  Parsed ADXL -> C:"); Serial.print(counter);
                    Serial.print(" X:"); Serial.print(xf); Serial.print("g");
                    Serial.print(" Y:"); Serial.print(yf); Serial.print("g");
                    Serial.print(" Z:"); Serial.print(zf); Serial.println("g");

                    // LCD'ye yaz
                    displayAccel(xf, yf, zf, rxId, counter);
                } else {
                    // Unexpected length — show length and hex on LCD second line
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("ID:0x"); lcd.print(rxId, HEX);
                    lcd.setCursor(0,1);
                    lcd.print("Len:"); lcd.print(len);
                }
            }
        }

        Serial.println(); // Alt satıra geç
        Serial.println("-----------------------------------");
    }
}