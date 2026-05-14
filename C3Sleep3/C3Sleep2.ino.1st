#include <WiFi.h>
#include <WiFiManager.h>
#include <time.h>
// Forza lo spegnimento di mDNS (il servizio che lo fa apparire nell'IDE)
#include <ESPmDNS.h>

#define PIN_SVEGLIA   GPIO_NUM_4 
#define PIN_LED       8          
#define PIN_OK_SIGNAL 5          // ALTO = Successo, BASSO = Problema/In corso

void setup() {
    // Inizializzazione rapida
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_OK_SIGNAL, OUTPUT);
    digitalWrite(PIN_OK_SIGNAL, LOW); // Partiamo cauti
    digitalWrite(PIN_LED, HIGH);

    MDNS.end();

    Serial.begin(115200);
    
    // WiFiManager con tempi stretti per non drenare il supercap in caso di errore
    WiFiManager wm;
    wm.setConnectTimeout(10);       // 10 secondi max per connettersi al router
    wm.setConfigPortalTimeout(60);  // 60 secondi max per il portale se non c'è rete

    bool res = wm.autoConnect("ESP32C3_Setup");

    if (res) {
        // WiFi OK -> Prendo l'ora
        configTime(0, 0, "pool.ntp.org");
        setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
        tzset();

        struct tm timeinfo;
        // Aspettiamo max 3 secondi per la sincronizzazione effettiva
        if (getLocalTime(&timeinfo, 3000)) {
            // --- SUCCESSO ---
            digitalWrite(PIN_OK_SIGNAL, HIGH); // Diciamo al Master che il dato è pronto
            
            // Invio ripetuto finché il Master tiene alto il pin 4
            while (digitalRead(PIN_SVEGLIA) == HIGH) {
                if (getLocalTime(&timeinfo)) {
                    Serial.printf("@%02d%02d%02d\n", 
                                  //timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                                  timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
                }
                delay(1000); // Un invio al secondo
            }
        } else {
            Serial.println("E_NTP_FAIL");
            // Il PIN 5 resta BASSO per segnalare errore
        }
    } else {
        Serial.println("E_WIFI_FAIL");
        // Il PIN 5 resta BASSO per segnalare errore
    }

    // --- PROCEDURA DI SPEGNIMENTO ---
    Serial.flush();
    digitalWrite(PIN_LED, LOW);
    digitalWrite(PIN_OK_SIGNAL, LOW);

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    esp_deep_sleep_enable_gpio_wakeup(1 << PIN_SVEGLIA, ESP_GPIO_WAKEUP_GPIO_HIGH);
    esp_deep_sleep_start();
}

void loop() {}