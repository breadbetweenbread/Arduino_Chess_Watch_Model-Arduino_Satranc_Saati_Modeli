#include <LCD_I2C.h>       // LCD için I2C kütüphanesini içe aktar
LCD_I2C lcd(0x27, 16, 2);  // LCD ye isim ver ve parametreleri gir (4 satırlı için 16,4 yazılmalı)

#define but1 2  // but1 adlı değişken oluştur ve 2 numaralı pine ata (ilk buton)
#define but2 3  // but2 adlı değişken oluştur ve 3 numaralı pine ata (ikinci buton)
#define but3 6  // but3 adlı değişken oluştur ve 6 numaralı pine ata (sol buton)
#define but4 7  // but4 adlı değişken oluştur ve 7 numaralı pine ata (orta (seçme butonu) buton)
#define but5 8  // but5 adlı değişken oluştur ve 8 numaralı pine ata (sağ buton)

#define swtch 4  // swtch adlı değişken oluştur ve 4 numaralı pine ata (başlatma anahtarı)
#define buzz 5   // buzz adlı bir değişken oluştur ve 5 numaralı pine ata (buzzer)

bool but1_state;  // Rakipler arası geçiş için bool tipinde but1_state adında bir değişken ata


int min;   // İlk rakibin dakikası
int sec;   // İlk rakibin saniyesi
int min2;  // İkinci rakibin dakikası
int sec2;  // İkinci rakibin saniyesi

int stun = 11;  // seçim ekranı için cursorun ilk konumu
int msg;        // seçimlerden sonra verilecek mesaj için değişken

char *message[] = { "  1dk 5dk 10dk", "Sure 1 dakikadir", "Sure 5 dakikadir", "Sure 10 dkdir", "Anahtari aciniz" };  // seçimler için mesajlar

unsigned long new_time;  // millis fonksiyonu ile çalışabilmek için büyük veri tutabilen unsigned long tipinde iki değişken atandı
unsigned long old_time;

void timer1() {                      // timer1 adında özel bir fonksiyon tanımlandı
  stun = 11;                         // seçim cursorunun ilk konumunu ayarla
  msg = 0;                           // yazdırmak için ilk mesajı seç
  while (digitalRead(swtch) == 1) {  // anahtara bağlı olan bir while döngüsü süreyi başlatır ve durdurur

    if (min < 0 || min2 < 0) {  // eğer ilk veya ikinci rakibin süresi bitmişse, uyar
      while (min < 0 || min2 < 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sure Bitti!");
        digitalWrite(buzz, HIGH);
        delay(100);
        digitalWrite(buzz, LOW);
        delay(500);
        if (digitalRead(swtch) == 0)
          break;
      }
    }

    if (digitalRead(but1) == 1) {       // but1 den gelen veriyi oku eğer 1 e eşitse (butona basılmışsa)
      but1_state = !(but1_state);       // but1_state değerini tersine çevir (0 ise 1, 1 ise 0 yap)
      while (digitalRead(but1) == 1) {  // eğer butona basılmaya devam ediliyorsa döngüye al
        delay(100);                     // 100 ms bekle
        if (digitalRead(but1) == 0)     // eğer butona artık basılmıyorsa
          break;                        // döngüden çık
      }
    }

    if (digitalRead(but2) == 1) {       // but2 den gelen veriyi oku eğer 1 e eşitse (butona basılmışta)
      but1_state = !(but1_state);       // but1_state değerini tersine çevir
      while (digitalRead(but2) == 1) {  // eğer halen düğmeye basılıyorsa döngüye al
        delay(100);                     // 100 ms bekle
        if (digitalRead(but2) == 0)     // eğer artık butona basılmıyorsa
          break;                        // döngüden çık
      }
    }
    switch (but1_state) {  // but1_state değişkeni ile bir switch-case zinciri oluştur

      case 0:  // but1_state değeri 0 ise


        lcd.setCursor(9, 0);  // lcd nin 0. yani il satırının 9. sütunundan başlayarak "min" yaz
        lcd.print("min");
        lcd.setCursor(13, 0);  // lcd nin 0. yani il satırının 13. sütunundan başlayarak "sec" yaz
        lcd.print("sec");

        lcd.setCursor(0, 0);  // lcd nin 0. yani il satırının 0. sütunundan başlayarak "min" yaz
        lcd.print("min");
        lcd.setCursor(4, 0);  // lcd nin 0. yani il satırının 4. sütunundan başlayarak "sec" yaz
        lcd.print("sec");

        new_time = (millis() / 1000);  // millis fonksiyonundan gelen ms değerini saniyeye çevirip new_time değişkenine ata
        if (old_time < new_time) {     // eğer old_time değişkeni new_time değişkeninden küçükse
          sec2--;                      // sec2 değişkenini 1  azalat
          old_time = new_time;         // ve old_time değişkenini new_time değişkenine eşitle (bu sayede old_time hep new_time dan küçük olacak)
          lcd.clear();                 // lcd ekrandaki yazıları temizle (olası görüntü hatalrını gidermek için)
        }

        if (sec2 < 1) {  // eğer sec2 değeri 1 den küçükse
          min2--;        // min değerini 1 azalt
          sec2 = 60;     // ve sec2 değerini 60 yap (dakika-saniye döngüsü)
        }




        lcd.setCursor(9, 1);  // lcd nin 1. yani ikinci satırının 9. sütunundan başlayarak "min2 değişkenini" yaz
        lcd.print(min2);
        lcd.setCursor(13, 1);  // lcd nin 1. yani ikinci satırının 13. sütunundan başlayarak "sec2 değişkenini" yaz
        lcd.print(sec2);

        lcd.setCursor(0, 1);  // lcd nin 1. yani ikinci satırının 0. sütunundan başlayarak "min değişkenini" yaz
        lcd.print(min);
        lcd.setCursor(4, 1);  // lcd nin 1. yani ikici satırının 4. sütunundan başlayarak "sec değişkenini" yaz
        lcd.print(sec);

        break;  // switch-case yapısında case0 işleminin bir alt işlem olan case1 e geçmesini önleyen break komutu (eğer konulmazsa case0 içinde case1 de çalışır)

      case 1:  // but1_state değeri 1 ise


        lcd.setCursor(0, 0);  // lcd nin 0. yani il satırının 0. sütunundan başlayarak "min" yaz
        lcd.print("min");
        lcd.setCursor(4, 0);  // lcd nin 0. yani il satırının 4. sütunundan başlayarak "sec" yaz
        lcd.print("sec");

        lcd.setCursor(9, 0);  // lcd nin 0. yani il satırının 9. sütunundan başlayarak "min" yaz
        lcd.print("min");
        lcd.setCursor(13, 0);  // lcd nin 0. yani il satırının 13. sütunundan başlayarak "sec" yaz
        lcd.print("sec");

        new_time = (millis() / 1000);  // new_time değişkenine millisden gelen ms değerini saniye yapıp ata
        if (old_time < new_time) {     // eğer old_time küçükse new_time dan
          sec--;                       // sec değerini 1 azalt
          old_time = new_time;         // ve old_time değirini new_time değerine ata (bu sayde old newden hep küçük olur)
          lcd.clear();                 // alası görünüm sorunlarının önüne geçmek için lcd ekranı temizle
        }

        if (sec < 1) {  // eğer sec değeri 1 den küçükse
          min--;        // min değerini 1 azalt
          sec = 60;     // ve sec değerini 60 yaz (dakika-saniye döngsüsü)
        }



        lcd.setCursor(0, 1);  // lcd nin 1. yani ikinci satırının 0. sütunundan başlayarak "min değişkenini" yaz
        lcd.print(min);
        lcd.setCursor(4, 1);  // lcd nin 1. yani ikinci satırının 4. sütunundan başlayarak "sec değişkenini" yaz
        lcd.print(sec);

        lcd.setCursor(9, 1);  // lcd nin 1. yani ikinci satırının 9. sütunundan başlayarak "min2 değişkenini" yaz
        lcd.print(min2);
        lcd.setCursor(13, 1);  // lcd nin 1. yani ikinci satırının 13. sütunundan başlayarak "sec2 değişkenini" yaz
        lcd.print(sec2);


        break;
    }
  }
}

void select() {  // süre seçimleri için select adında bir fonksiyon tanımla

  lcd.clear();                       // ekranı temizle
  while (digitalRead(swtch) == 0) {  // anahtar kapalı ise döngüye gir

    lcd.setCursor(0, 0);  // ekranın ilk satır ve sütundan başlayarak msg değikeninin karşılık geldiği message dizisini yazdır
    lcd.print(message[msg]);


    lcd.setCursor(stun, 1);  // stun değişkenine göre seçim  cursorunu hareket ettir
    lcd.print("^");

    switch (stun) {                    // stun değeri ile bir switch-case yapısı oluştur
      case 3:                          // seçim cursoru  3. sütünda ise (1dk)
        if (digitalRead(but4) == 1) {  // eğer but4 (seçim tuşu) değeri 1 ise
          lcd.clear();                 // lcd ekranı temizle
          but1_state = 0;              // but1_state değerini 0 yap
          min = 1;                     // dakikayı 1 e ayarla
          min2 = 1;                    // diğerin rakibin dakikasını 1 e ayarla
          sec = 0;                     // saniyeleri 0 yap
          sec2 = 0;
          msg = 1;                // 1. mesajı seç
          stun = 15;              // seçim cursorunu 1. sütuna taşı (görsel hataları engellemek için)
          lcd.setCursor(0, 1);    // cursoru 0. sütun 1. satıra ayarla
          lcd.print(message[4]);  // message dizisindeki 4. metni yazdır

          while (digitalRead(but4) == 1) {  // eğer seçim butonuna halen basılı tutuluyorsa döngüye gir
            delay(100);                     // 100 ms bekle
            if (digitalRead(but4) == 0) {   // basılmıyorsa döngüden çık
              break;
            }
          }
        }
        break;  // switch-case yapısında case0 işleminin bir alt işlem olan case7 e geçmesini önleyen break komutu (eğer konulmazsa case3 içinde case7 de çalışır)


      case 7:                          // seçim cursoru 7. sütünda ise (5dk)
        if (digitalRead(but4) == 1) {  // but4 değeri 1 ise (seçim butonuna basıldıysa)
          lcd.clear();                 // lcd ekranı temizle
          but1_state = 0;              //but1_state değerini 0 yap
          min = 5;                     // rakiplerin sürelerini 5dk yap
          min2 = 5;
          sec = 0;
          sec2 = 0;
          msg = 2;                // 2. mesajı seç
          stun = 15;              // seçim cursorunu 15. sütuna al
          lcd.setCursor(0, 1);    // cursoru 0. satır 1. sütuna al
          lcd.print(message[4]);  // message dizindeki 4 metni yaz

          while (digitalRead(but4) == 1) {  // eğer seçim butonuna halen basılıyorsa döngüye gir
            delay(100);                     // 100 ms bekle
            if (digitalRead(but4) == 0) {   // düğmeye basılmıyorsa döngüden çık
              break;
            }
          }
        }
        break;  // switch-case yapısında case7 işleminin bir alt işlem olan case11 e geçmesini önleyen break komutu (eğer konulmazsa case7 içinde case11 de çalışır)

      case 11:                         // seçim cursoru 11. sütunda ise (10dk)
        if (digitalRead(but4) == 1) {  // seçim butonuna basıldıysa
          lcd.clear();                 // lcd ekranı temizle
          but1_state = 0;              // but1 state değişkeni 0 olsun (sıranın beyaza geçmesi)
          min = 10;                    // süreleri 10dk yap
          min2 = 10;
          sec = 0;
          sec2 = 0;
          msg = 3;                // 3. mesajı seç
          stun = 15;              // seçim cursorunu 15. sütuna al
          lcd.setCursor(0, 1);    // cursoru 0. sütun 1. satıra al
          lcd.print(message[4]);  // message dizisindeki 4. metni yazdır

          while (digitalRead(but4) == 1) {  // eğer seçim butonuna basılı tutuluyorsa döngüye gir
            delay(100);                     // 100 ms bekle
            if (digitalRead(but4) == 0) {   // basılmıyorsa döngüden çık
              break;
            }
          }
        }
        break;
    }


    if (digitalRead(but5) == 1) {  // seçim cursorunu sağa hareket ettirir
      stun = stun + 4;
      lcd.clear();
      while (digitalRead(but5) == 1) {
        delay(100);
        if (digitalRead(but5) == 0) {
          break;
        }
      }
    }

    else if (digitalRead(but3) == 1) {  // seçim cursorunu sola hareket ettirir
      stun = stun - 4;
      lcd.clear();
      while (digitalRead(but3) == 1) {
        delay(100);
        if (digitalRead(but3) == 0) {
          break;
        }
      }
    }
  }
}

void setup() {
  lcd.begin();           // lcd ekranı başlat
  lcd.backlight();       // lcd ekranın arka ışığını yak
  pinMode(but1, INPUT);  // but1 değişkenini giriş olarak ata
  pinMode(but2, INPUT);  // but2 değişkenini giriş olarak ata
  pinMode(but3, INPUT);  // but3 değişkenini giriş olarak ata
  pinMode(but4, INPUT);  // but4 değişkenini giriş olarak ata
  pinMode(but5, INPUT);  // but5 değişkenini giriş olarak ata

  pinMode(swtch, INPUT);  // swtch değişkenini giriş olarak ata
  pinMode(buzz, OUTPUT);  // buzz adlı değişkeni çıkış olarak ata
}

void loop() {
  timer1();  // timer1 fonksiyonunu başlat
  select();  // select fonksiyonunu başlat



  lcd.setCursor(9, 0);  // bunların ne işe yaradığını daha önce yazdım :D
  lcd.print("min");
  lcd.setCursor(13, 0);
  lcd.print("sec");

  lcd.setCursor(0, 0);
  lcd.print("min");
  lcd.setCursor(4, 0);
  lcd.print("sec");

  lcd.setCursor(0, 1);
  lcd.print(min);
  lcd.setCursor(4, 1);
  lcd.print(sec);

  lcd.setCursor(9, 1);
  lcd.print(min2);
  lcd.setCursor(13, 1);
  lcd.print(sec2);
}