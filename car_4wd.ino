// #include <IRremoteInt.h>
#include <IRremote.h>


#include <Servo.h>

const int pin_mcu_int = 2;
const int pin_mcc_enb = 3;
const int pin_mcc_in4 = 4;
const int pin_mcc_in3 = 5;
const int pin_mcc_in2 = 6;
const int pin_mcc_in1 = 7;
const int pin_mcc_ena = 8;

const int pin_ping_echo = 9;
const int pin_ping_trig = 10;

const int pin_servo = 11;

const int pin_ir_rx = 12;

const int pin_left_forward = pin_mcc_in4;
const int pin_left_reverse = pin_mcc_in3;
const int pin_right_forward = pin_mcc_in2;
const int pin_right_reverse = pin_mcc_in1;

// globals
Servo servo;
IRrecv ir_rx(pin_ir_rx);
decode_results ir_results;


void setup() {
  Serial.begin(9600);
  Serial.println("setup");
  // put your setup code here, to run once:
  pinMode(pin_mcc_ena, OUTPUT);
  pinMode(pin_mcc_enb, OUTPUT);
  pinMode(pin_mcc_in1, OUTPUT);
  pinMode(pin_mcc_in2, OUTPUT);
  pinMode(pin_mcc_in3, OUTPUT);
  pinMode(pin_mcc_in4, OUTPUT);
  
  digitalWrite(pin_mcc_ena, HIGH);
  digitalWrite(pin_mcc_enb, HIGH);

  servo.attach(pin_servo);

  ir_rx.enableIRIn(); // Start the receiver

}


void trace(String s) {
  return;
  Serial.println(s);
}

void servo_forward() {
  servo.writeMicroseconds(1350);
}

void servo_left() {
  servo.writeMicroseconds(1700);
}

void servo_right() {
   servo.writeMicroseconds(900);

}



void turn_left() {
  trace("left");
  servo_left();
  digitalWrite(pin_left_forward, HIGH);
  digitalWrite(pin_left_reverse, LOW);
  digitalWrite(pin_right_forward, LOW);
  digitalWrite(pin_right_reverse, HIGH);
}

void turn_right() {
  trace("right");
  servo_right();
  digitalWrite(pin_left_forward, LOW);
  digitalWrite(pin_left_reverse, HIGH);
  digitalWrite(pin_right_forward, HIGH);
  digitalWrite(pin_right_reverse, LOW);
}


void forward() {
  trace("forward");
  servo_forward();
  digitalWrite(pin_left_forward, HIGH);
  digitalWrite(pin_left_reverse, LOW);
  digitalWrite(pin_right_forward, HIGH);
  digitalWrite(pin_right_reverse, LOW);
}

void reverse() {
  trace("reverse");
  digitalWrite(pin_left_forward, LOW);
  digitalWrite(pin_left_reverse, HIGH);
  digitalWrite(pin_right_forward, LOW);
  digitalWrite(pin_right_reverse, HIGH);
}


void stop() {
  trace("stop");
  digitalWrite(pin_mcc_in1, HIGH);
  digitalWrite(pin_mcc_in2, HIGH);
  digitalWrite(pin_mcc_in3, HIGH);
  digitalWrite(pin_mcc_in4, HIGH);
}


void remote_control() {
  
  while(true) {
    if (Serial.available() > 0) {
      int key = Serial.read();
      switch(key) {
        case 'F':       // forward
          forward();
          break;
        case 'B':       // back
          reverse();
          break;
        case 'L':       // left
        case 'G':       // forward left
        case 'H':       // back left
          turn_left();
          break;
        case 'R':       // right
        case 'I':       // forward right
        case 'J':       // back right
          turn_right();
          break;
          turn_left();
          break;
        case 'S':       // stop
        case 'D':       // stop all
          stop();
          break;
        default:
          break;
      }
    }

  }
}


void loop() {
  remote_control();
  if (ir_rx.decode(&ir_results)) {
   Serial.print("received IR: ");
   Serial.println(ir_results.value, HEX);
   ir_rx.resume(); // Receive the next value
  }
  turn_left();
  delay(1000);
  
  stop();
  delay(1000);
  
  turn_right();
  delay(1000);

  stop();
  delay(1000);

  forward();
  delay(1000);

  stop();
  delay(1000);
  
  reverse();
  delay(1000);

  stop();
  delay(1000);
}