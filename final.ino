#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

const char *ssid = "Wassup dude..";
const char *password = "tapemarga";

#define pinSensor D1
#define pinLED 4
#define pinBuzzer D3

// Siapkan variabel untuk menyimpan data sensor
String url;

// Siapkan variabel wificlient
WiFiClient client;

void setup()
{
    Serial.begin(9600);
    pinMode(pinSensor, INPUT);
    pinMode(pinLED, OUTPUT);
    pinMode(pinBuzzer, OUTPUT);

    WiFi.hostname("NodeMCU");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        // Menunggu koneksi WiFi terhubung
        digitalWrite(pinLED, LOW); // LED off
        delay(500);
    }

    // Apabila terkoneksi
    digitalWrite(pinLED, HIGH); // LED on
}

void loop()
{
    int api = digitalRead(pinSensor);
    if (api == LOW)
    {
        // Tidak ada api, wa tdk jalan
        Serial.println("Tidak ada api");
    }
    else
    {
        // Ada api, wa jalan
        Serial.println("Api Terdeteksi");
        // Aktifkan buzzer
        digitalWrite(pinBuzzer, HIGH);
        delay(500); // Berdurasi 500ms
        digitalWrite(pinBuzzer, LOW);
        // Kirim notifikasi WA
        kirim_wa("Bahaya!!! Telah Ditemukan Titik Api di Tempat Anda. Segera Evakuasi Keluarga Anda. Waspadalah");
    }
    delay(1000);
}

void kirim_wa(String pesan)
{
    url = "http://api.callmebot.com/whatsapp.php?phone=6281241617892&text=" + urlencode(pesan) + "&apikey=4013707";
    // Kirim pesan
    postData();
}

void postData()
{
    // Siapkan variabel untuk menampung status konek atau tidak
    int httpCode;

    // Siapka variabel untuk http
    HTTPClient http;

    // Eksekusi link url
    http.begin(client, url);
    httpCode = http.POST(url);

    // Uji nilai variabel
    if (httpCode == 200)
    {
        Serial.println("Pesan terkirim");
    }
    else
    {
        Serial.println("Pesan gagal terkirim");
    }
    http.end();
}

String urlencode(String str)
{
    String encodedString = "";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i=0; i<str.length(); i++)
    {
        c = str.charAt(i);
        if (c == ' ')
        {
            encodedString += '+';
        }
        else if (isalnum(c))
        {
            encodedString += c;
        }
        else
        {
            code1 = (c & 0xf) + '0';
            if ((c & 0xf) > 9)
            {
                code1 = (c & 0xf) - 10 + 'A';
            }
            c = (c >> 4) & 0xf;
            code0 = c + '0';
            if (c > 9)
            {
                code0 = c - 10 + 'A';
            }
            code2 = '\0';
            encodedString += '%';
            encodedString += code0;
            encodedString += code1;
        }
        yield();
    }
    Serial.println(encodedString);
    return encodedString;
}
