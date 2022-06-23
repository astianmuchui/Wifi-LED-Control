 #include <WiFi.h>
  const char* ssid = "hotspot_name"; 
  const char* password = "hotspot_password"; 
  WiFiServer server(80);
  
  String header;
  String output26State = "off";
  String output27State = "off";
  
  const int output26 = 2;
  const int output27 = 27;
  
  void setup() {
  Serial.begin(115200);
  pinMode(output26, OUTPUT);
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  }
  
  void loop(){
  WiFiClient client = server.available(); 
  if (client) {      
  Serial.println("New Client.");
  String currentLine = "";       
  while (client.connected()) {    
  if (client.available()) {   
  char c = client.read();  
  Serial.write(c);          
  header += c;
  if (c == '\n') {     
  if (currentLine.length() == 0) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  
  // turns the GPIOs on and off
  if (header.indexOf("GET /26/on") >= 0) {
  Serial.println("GPIO 26 on");
  output26State = "on";
  digitalWrite(output26, HIGH);
  } else if (header.indexOf("GET /26/off") >= 0) {
  Serial.println("GPIO 26 off");
  output26State = "off";
  digitalWrite(output26, LOW);
  } else if (header.indexOf("GET /27/on") >= 0) {
  Serial.println("GPIO 27 on");
  output27State = "on";
  digitalWrite(output27, HIGH);
  } else if (header.indexOf("GET /27/off") >= 0) {
  Serial.println("GPIO 27 off");
  output27State = "off";
  digitalWrite(output27, LOW);
  }
  // Create a web page
   client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #555555;}</style></head>");
  
  client.println("<body><h1>ESP32 Web Server</h1>");
  client.println("<p>GPIO 26 - State " + output26State + "</p>");
client.println();
  break;
  } else {   currentLine = "";  }
  } else if (c != '\r') {   currentLine += c;   }
  }
  
  }
  header = "";
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
  }
  }
