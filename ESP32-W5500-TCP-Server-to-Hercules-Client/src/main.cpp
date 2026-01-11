#include <SPI.h>
#include <Ethernet.h>

#define W5500_CS 4
#define W5500_MOSI 18
#define W5500_MISO 17
#define W5500_SCK 8

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 50); 
IPAddress gateway(192, 168, 1, 1);   // Ağ geçidi (genellikle router IP)
IPAddress subnet(255, 255, 255, 0);  // Alt ağ maskesi
IPAddress dns(8, 8, 8, 8);           // DNS sunucusu (Google DNS)
EthernetServer server(5000); 

void setup() {
  Serial.begin(115200);

  SPI.begin(W5500_SCK, W5500_MISO, W5500_MOSI);
  Ethernet.init(W5500_CS); 
  Serial.println("Ethernet baslatiliyor...");
  
  Ethernet.begin(mac, ip, dns, gateway, subnet);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("W5500 bulunamadi! Kablolari kontrol et.");
    while (true);
  }
  
  server.begin();
  Serial.print("Server baslatildi. IP Adresi: ");
  Serial.println(Ethernet.localIP());
}

void loop() {

  EthernetClient client = server.available();

  if (client) {
    Serial.println("Yeni bir istemci baglandi!");
    
    while (client.connected())
    {
      if (client.available())  // Hercules'ten gelen veriyi oku
      {  
        String request = client.readStringUntil('\r');
        Serial.print("Gelen Mesaj: ");
        Serial.println(request);
        
        client.println("");
        client.println("Mesaj Alindi!"); // PC'ye (Hercules) cevap gönder
        
        // Örnek bir sensör verisi simülasyonu gönderelim
        int sensorDegeri = random(0, 100); 
        client.print("Sensor Verisi: ");
        client.println(sensorDegeri);
      }
    }
    client.stop();
    Serial.println("Istemci ayrildi.");
  }
}