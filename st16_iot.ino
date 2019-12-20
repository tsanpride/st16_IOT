/*******************************************************************
 *                  IDS pada Pintu berbasis IOT                    *
 *                                                                 *
 *  Perangkat yang akan mendeteksi pintu ketika terbuka dan        *
 *  mengirim notifikasi ke handphone melalui Telegram              *
 *                                                                 *
 *  kelompok IOT STSN16                                            *
 *******************************************************************/
 
#include <UniversalTelegramBot.h> //lib untuk Telegram
#include <ESP8266WiFi.h> //lib modul esp8266
#include <WiFiClientSecure.h> //lib untuk tesambung wifi

//------- Pengaturan WiFi -------
char ssid[] = "ST16-IOT";       // SSID (nama) wifi
char password[] = "xxxxxxxx";  // password wifi

#define TELEGRAM_BUTTON_PIN D5

// ------- konfigurasi Telegram --------
#define BOT_TOKEN "1001562174:AAHDgOZsMKVJCi-evRDd2mF58znpq1uFkWE"  // Bot Token TelegramBot
#define CHAT_ID "671161718" // Chat ID akun pengguna

// SSL klien untuk library
WiFiClientSecure client;

UniversalTelegramBot bot(BOT_TOKEN, client);

String ipAddress = "";

volatile bool pintuTerbuka = false;

void setup() {

  Serial.begin(115200);

  pinMode(TELEGRAM_PIN, INPUT);

  // NOTE:
  // attachInterupt berfungsi untuk menahan kondisi pintuTerbuka ketika 
  // sedang menyambungkan internet 
  
  attachInterrupt(TELEGRAM_PIN, pintu_dibuka, RISING);

  // Mengatur WiFi kedalam mode 'station' dan memutus sambungan ke AP
  // jika sebelumnya sempat tersambung
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Menyambungkan ke WiFi
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  ipAddress = ip.toString();

}

void pintu_dibuka() {
  Serial.println("Ada yang membuka pintu!");
  int pintu = digitalRead(TELEGRAM_PIN);
  if(pintu == HIGH)
  {
    pintuTerbuka = true;
  }
  return;
}

void kirim_pesan_telegram() {
  String message = "SSID:  ";
  message.concat(ssid);
  message.concat("\n");
  message.concat("IP: ");
  message.concat(ipAddress);
  message.concat("\n");
  if(bot.sendMessage(CHAT_ID, message, "Markdown")){
    Serial.println("Notifikasi terkirim");
  }
  pintuTerbuka = false;
}

void loop() {

  if ( pintuTerbuka ) {
    kirim_pesan_telegram();
  }
}
