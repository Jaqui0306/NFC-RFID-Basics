#include <SPI.h>              // Librería para comunicación SPI
#include <Adafruit_PN532.h>  // Librería para controlar el módulo PN532

// ---------- DEFINICIÓN DE PINES (SPI) ----------
#define SS   4    // Pin de selección del dispositivo (Chip Select)
#define SCK  14   // Reloj SPI
#define MISO 12   // Datos del PN532 hacia la ESP32
#define MOSI 13   // Datos de la ESP32 hacia el PN532

// ---------- CREACIÓN DEL OBJETO PN532 ----------
Adafruit_PN532 nfc(SS);  // Se crea el objeto 'nfc' usando comunicación SPI

void setup() {
  Serial.begin(115200);  // Inicializa comunicación serial a 115200 baudios
  Serial.println("Iniciando lector NFC...");

  // ---------- INICIALIZACIÓN DEL BUS SPI ----------
  SPI.begin(SCK, MISO, MOSI, SS);  
  // Se configuran los pines SPI personalizados para la ESP32

  nfc.begin();  // Inicializa el módulo PN532

  // ---------- VERIFICAR SI EL MÓDULO ESTÁ CONECTADO ----------
  uint32_t versiondata = nfc.getFirmwareVersion();  // Obtiene versión del firmware

  if (!versiondata) {  // Si no se detecta el módulo
    Serial.println("No se encontró PN532");
    while (1);  // Se queda en un bucle infinito (detiene el programa)
  }

  Serial.println("PN532 detectado correctamente");

  // ---------- CONFIGURACIÓN DEL MÓDULO ----------
  nfc.SAMConfig();  
  // Configura el PN532 en modo lector de tarjetas NFC/RFID
}

void loop() {
  uint8_t success;         // Variable para verificar si se detectó tarjeta
  uint8_t uid[7];          // Arreglo para almacenar el UID (máximo 7 bytes)
  uint8_t uidLength;       // Variable para guardar la longitud del UID

  // ---------- ESPERA DE TARJETA ----------
  success = nfc.readPassiveTargetID(
    PN532_MIFARE_ISO14443A, // Tipo de tarjeta (NFC tipo A)
    uid,                    // Arreglo donde se guardará el UID
    &uidLength              // Dirección de la variable longitud
  );

  // ---------- SI SE DETECTA UNA TARJETA ----------
  if (success) {
    Serial.println("\nTarjeta detectada");

    // ---------- MOSTRAR UID EN HEXADECIMAL (BASE 16) ----------
    Serial.print("HEX: ");
    for (int i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX);  // Convierte cada byte a hexadecimal
      Serial.print(" ");
    }
    Serial.println();

    // ---------- MOSTRAR UID EN DECIMAL (BASE 10) ----------
    Serial.print("DEC: ");
    for (int i = 0; i < uidLength; i++) {
      Serial.print(uid[i], DEC);  // Convierte cada byte a decimal
      Serial.print(" ");
    }
    Serial.println();

    // ---------- MOSTRAR UID EN BINARIO (BASE 2) ----------
    Serial.print("BIN: ");
    for (int i = 0; i < uidLength; i++) {
      Serial.print(uid[i], BIN);  // Convierte cada byte a binario
      Serial.print(" ");
    }
    Serial.println();

    // ---------- RETARDO ----------
    delay(2000);  
    // Evita que la misma tarjeta se lea múltiples veces rápidamente
  }
}

