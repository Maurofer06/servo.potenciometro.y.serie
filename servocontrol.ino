/* Esta practica consiste en el control de un servo a traves
de un pontenciometro y a su vez tambien comunicacion serie
*/

#define POTENCIOMETRO A0
#include <Servo.h>

Servo servoMotor;

int val;              // valor del potenciómetro
int angulo = 0;       // ángulo para el servo
bool modoSerial = false; // false = potenciómetro, true = monitor serie
unsigned long ultimoTiempoSerie = 0; // para volver al modo potenciómetro

void setup() {
  servoMotor.attach(9); // establecer pin del servo
  Serial.begin(9600);
  Serial.println("Modo inicial: Potenciómetro.");
  Serial.println("Escribe un ángulo (0-180) para control desde el Monitor Serie.");
}

void loop() {
  // Leer desde el monitor serie si hay datos
  if (Serial.available() > 0) {
    int entrada = Serial.parseInt();
    if (entrada >= 0 && entrada <= 180) {
      angulo = entrada;
      modoSerial = true;
      ultimoTiempoSerie = millis();
      Serial.print("Modo serie: ángulo = ");
      Serial.println(angulo);
    } else {
      Serial.println("Ángulo inválido. Debe estar entre 0 y 180.");
    }
  }

  // Si no estamos en modo serie, usar el potenciómetro
  if (!modoSerial) {
    val = analogRead(POTENCIOMETRO);
    angulo = map(val, 0, 1023, 0, 180);
  }

  // Enviar ángulo al servo
  servoMotor.write(angulo);
  delay(15);

  // Salir del modo serie si han pasado más de 5 segundos sin entrada
  if (modoSerial && (millis() - ultimoTiempoSerie > 5000)) {
    modoSerial = false;
    Serial.println("Sin entrada en serie. Volviendo a control por potenciómetro.");
  }
}
