//Muhammad_Zidan_Alhilali
//24_545364_SV_25663

#include <ESP32Servo.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
Servo servo1, servo2, servo3, servo4, servo5;

#define pinServo1 19
#define pinServo2 5
#define pinServo3 18
#define pinServo4 17
#define pinServo5 16
#define pinPIR 13

float roll, pitch, yaw;
int servoAwal = 90; //kondisi servo awal 90 derajat 

void setup() {
  Serial.begin(115200);

  if (!mpu.begin()) { //mengecek apakah mpu nya berhasil diinisialisasi atau tidak
    Serial.println("MPU6050 gagal diinisialisasi!"); //pesan jika gagal
    while (1);
  }
  Serial.println("MPU6050 siap!"); //pesan jika berhasil

  pinMode(pinPIR, INPUT);

  servo1.attach(pinServo1);
  servo2.attach(pinServo2);
  servo3.attach(pinServo3);
  servo4.attach(pinServo4);
  servo5.attach(pinServo5);

  servo1.write(servoAwal);
  servo2.write(servoAwal);
  servo3.write(servoAwal);
  servo4.write(servoAwal);
  servo5.write(servoAwal);
}


void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  roll = atan2(-a.acceleration.x, a.acceleration.z) * 180 / PI; //menghitung sudut kemiringan roll(x) dengan cara mengalikan percepatan sumbu x dan z lalu dikali 180/PI biar hasilnya derajat
  pitch = atan2(a.acceleration.y, sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.z * a.acceleration.z)) * 180 / PI; //menghitung sudut dari pitch
  yaw = g.gyro.z;  //ini rotasi yaw yang diambil dari putaran di sumbu z
  int pirState = digitalRead(pinPIR); //ini untuk baca sensor PIR

  if (roll > 10) {  //kondisi jika roll positif, servo 1 dan 2 akan berputar melawan arah
    servo1.write(servoAwal - 90);
    servo2.write(servoAwal - 90);
  } else if (roll < -10) {  //kondisi jika roll negatif, servo 1 dan 2 akan berputar searah bidang
    servo1.write(servoAwal + 90);
    servo2.write(servoAwal + 90);
  } else {
    servo1.write(servoAwal);
    servo2.write(servoAwal);
  }

  if (pitch > 10) {  //kalo pitch positfi, servo 3 dan 4 akan berputar searah bidang 
    servo3.write(servoAwal + 90);
    servo4.write(servoAwal + 90);
  } else if (pitch < -10) {  //kalo pitch negatif, servo 3 dan 4 akan berputar berlawanan bidang (di soal kayanya typo atau engga ya? tapi ini saya buat berlawanan aja biar ada bedanya)
    servo3.write(servoAwal - 90);
    servo4.write(servoAwal - 90);
  } else {
    servo3.write(servoAwal);
    servo4.write(servoAwal);
  }

  if (abs(yaw) > 1) {  //mengecek apakah ada gerakah dari yaw, abs(yaw) tu untuk mutlak yaw jadi apapun gerakannya negatif atau positif tetep dihitung bergerak
    int arahYaw = (yaw > 0) ? 1 : -1; //ini buat ngecek yaw nya tu ke kanan atau ke kiri (posditif/negatif)
    servo5.write(servoAwal + (90 * arahYaw));  //servonya akan muter sesuai arah yawnya
    delay(1000);  //servo akan diam 1 detik
    
    servo5.write(servoAwal);  //dan kembali ke posisi awal
  }

  if (pirState == HIGH) { //kondisi jika ada gerakan di PIR
    servo1.write(servoAwal + 45); //seevonya akan gerak acak
    servo2.write(servoAwal - 45);
    servo3.write(servoAwal + 90);
    servo4.write(servoAwal - 90);
    servo5.write(servoAwal + 20);

    delay(1000);
    
    servo1.write(servoAwal); //setelah satu detik, servo akan kembali ke posisi awal (90 derajat)
    servo2.write(servoAwal);
    servo3.write(servoAwal);
    servo4.write(servoAwal);
    servo5.write(servoAwal);
  }
}

