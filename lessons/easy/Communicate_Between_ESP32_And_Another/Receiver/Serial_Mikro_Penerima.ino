//GPIO17 itu tx2
//GPIO16 itu rx2

String data;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial2.begin(9600);
}

void loop() {
// berikut data yang dibaca dari serial monitor sendiri, kemudian dikirim ke perangkat lain
  while(Serial.available() > 0){
  delay(10);
  char d = Serial.read();
  data += d;
}
if (data.length() > 0){
  Serial2.print(data);
  data = "";
}

// berikut code untuk membaca data dari perangkat lain
while(Serial2.available() > 0){
  delay(10);
  char d = Serial2.read();
  data += d;
}
if (data.length() > 0){
  Serial.println(data);
  data = "";
}
}
