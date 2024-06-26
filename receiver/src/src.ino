#define LDR_PIN A0
#define LIGHT_THRESHOLD 300 // Depends on the light intensity of the room
#define LAMBDA 5 // Must be same with the transmitter

bool previous_state;
bool current_state;

int index = 0;
char buffer[7];

void setup() {
  Serial.begin(9600);
}

void loop() {
  current_state = read_comm();

  if (!current_state && previous_state) {
    char byte_received = get_byte();
    // Serial.print("Received byte: ");
    // Serial.println(byte_received);
    fill_buffer(byte_received);
  }

  previous_state = current_state;
}

bool read_comm() {
  int voltage = analogRead(LDR_PIN);
  return voltage > LIGHT_THRESHOLD ? true : false;
}

char get_byte() {
  char character = 0;
  delay(LAMBDA * 1.5);
  for (int i = 0; i < 8; i++) {
    character = character | read_comm() << i;
    delay(LAMBDA);
  }
  return character;
}

void fill_buffer(const char& ch) {
  if(ch == '*') {
    index = 0;
    // Serial.print("Received buffer: ");
    Serial.println(buffer);
  } else if(index < 7) {
    buffer[index] = ch;
    index++;
  } else {
    Serial.println("ERR: Buffer is full");
  }
}
