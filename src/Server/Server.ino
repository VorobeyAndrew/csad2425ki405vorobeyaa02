void setup() {
  // Begin serial communication at 9600 baud rate
  Serial.begin(9600);
}

void loop() {
  // Check if data is available from the client
  if (Serial.available()) {
    String message = Serial.readString();  // Read the message from the client

    // Modify the message (e.g., convert to uppercase)
    message.toUpperCase();

    // Send the modified message back to the client
    Serial.println(message);
  }
}