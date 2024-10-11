#define trigPin 16
#define echoPin 17
#define Dist 50
#define Min 30

const int FORWARD_SPEED = 200;
const int SLOW_SPEED = 100;

int pins[] = {33, 25, 14, 26, 27, 32};

// Array: {IN1, IN2, IN3, IN4, VelocidadA, VelocidadB}
int moves[][6] = {
  {1, 0, 0, 1, FORWARD_SPEED, FORWARD_SPEED},  // Forward
  {0, 1, 1, 0, FORWARD_SPEED, FORWARD_SPEED},  // Backward
  {1, 0, 1, 0, SLOW_SPEED, FORWARD_SPEED},     // Left curve
  {1, 0, 0, 1, FORWARD_SPEED, SLOW_SPEED},     // Right curve
  {1, 0, 0, 1, SLOW_SPEED, 255},               // Left spin
  {0, 1, 1, 0, 255, SLOW_SPEED},               // Right spin
  {0, 0, 0, 0, 0, 0}                           // Stop
};

void setup() {
  for (int i = 0; i < 6; i++) pinMode(pins[i], OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void move(int mov[]) {  
  for (int i = 0; i < 4; i++) {
    digitalWrite(pins[i], mov[i]);
  }
  analogWrite(pins[4], mov[4]);
  analogWrite(pins[5], mov[5]);
}

long Distancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH, 30000);
  if (duracion == 0) return -1;
  long distancia = duracion * 0.034 / 2;
  return distancia;
}

void loop() {
  long distancia = Distancia();
  if (distancia < 0) {    
    move(moves[6]);
    return;
  }

  Serial.print("Distancia: ");
  Serial.println(distancia);

  if (distancia < Min) {
    int avoidObstacle[] = {1, 0, 1, 0, SLOW_SPEED / 2, SLOW_SPEED};
    move(avoidObstacle);
    delay(500);
  } else if (distancia < Dist) {    
    int slow[] = {1, 0, 0, 1, SLOW_SPEED, SLOW_SPEED};
    move(slow);
  } else {    
    move(moves[0]);
  }
  delay(50);
}
