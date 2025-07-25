// ==== ピン設定 ====
const int pulse_pin = 3;
const int analog_pin_a0 = A0;
const int analog_pin_a1 = A1;

// ==== 初期値設定 ====
unsigned long default_on_time = 300;        // μs
unsigned long default_cycle_time = 30000;   // μs
bool use_analog_input = true;

unsigned long on_time = default_on_time;
unsigned long cycle_time = default_cycle_time;

unsigned long last_loop_start = 0;

void setup() {
  pinMode(pulse_pin, OUTPUT);
  Serial.begin(115200);
  last_loop_start = micros();  // 初回基準
}

void loop() {
  // ==== 時間調整 ====
  if (use_analog_input) {
    int val_a0 = analogRead(analog_pin_a0);
    int val_a1 = analogRead(analog_pin_a1);
    cycle_time = map(val_a0, 0, 1023, 10000, 100000);
    on_time    = map(val_a1, 0, 1023, 0, 500);
  }

  unsigned long now = micros();
  unsigned long elapsed_time = now - last_loop_start;

  // ==== 高精度タイミング合わせ（ビジーウェイト） ====
  while (elapsed_time < cycle_time) {
    now = micros();
    elapsed_time = now - last_loop_start;
  }

  // ==== 次ループの基準時間セット ====
  last_loop_start = micros();

  // ==== パルス出力 ====
  digitalWrite(pulse_pin, HIGH);
  delayMicroseconds(on_time);
  digitalWrite(pulse_pin, LOW);

  // ==== シリアル表示 ====
  Serial.print("on_time: ");
  Serial.print(on_time);
  Serial.print("us, elapsed_time: ");
  Serial.print(elapsed_time);
  Serial.println("us");
}

