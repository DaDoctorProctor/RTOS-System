#include "WebServerHandler.h"

const char WebServerHandler::htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Command Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    .command-button {
      width: 75px; /* Set the desired width */
      height: 25px; /* Set the desired height */
      margin: 2px; /* Optional: add some margin for spacing */
    }
    .button-row {
      display: flex; /* Use flexbox to layout buttons in a row */
      margin-bottom: 10px; /* Space between rows */
    }
  </style>
</head>
<body>
  <h1>Homero's development terminal</h1>
  <form id="commandForm" action="/send_command" method="POST">
    <input type="text" id="commandInput" name="command" placeholder="Enter command">
    <input type="submit" value="Send">
  </form>

  <div class="button-row">
    <button class="command-button" onclick="sendCommand('VFUP')">VFUP</button>
    <button class="command-button" onclick="sendCommand('VFDOWN')">VFDOWN</button>
    <button class="command-button" onclick="sendCommand('VSTEP')">VSTEP</button>
    <button class="command-button" onclick="sendCommand('VSTOP')">VSTOP</button>
  </div>

  <div class="button-row">
    <button class="command-button" onclick="sendCommand('CSTART')">CSTART</button>
    <button class="command-button" onclick="sendCommand('CSTOP')">CSTOP</button>
    <button class="command-button" onclick="sendCommand('CCW')">CCW</button>
    <button class="command-button" onclick="sendCommand('CW')">CW</button>
    <button class="command-button" onclick="sendCommand('CPOS')">POSITION</button>
    <button class="command-button" onclick="sendCommand('SPD255')">MAX</button>
    <button class="command-button" onclick="sendCommand('SPD180')">MID</button>
  </div>

  <div class="button-row">
    <button class="command-button" onclick="sendCommand('CMODE1')">CMODE1</button>
    <button class="command-button" onclick="sendCommand('CMODE2')">CMODE2</button>
    <button class="command-button" onclick="sendCommand('HOME')">HOME</button>
    <button class="command-button" onclick="sendCommand('REF0')">REF0</button>
    <button class="command-button" onclick="sendCommand('RF+500')">REF+500</button>
    <button class="command-button" onclick="sendCommand('RF-500')">REF-500</button>
    <button class="command-button" onclick="sendCommand('REF2900')">REF2900</button>

  </div>
    <div class="button-row">
    <button class="command-button" onclick="sendCommand('GETDATA')">GDATA</button>
    <button class="command-button" onclick="sendCommand('YAUTO')">Y_AUTO</button>
    <button class="command-button" onclick="sendCommand('XAUTO')">X_AUTO</button>
    <button class="command-button" onclick="sendCommand('GAUTO')">G_AUTO</button>
  </div>

  <p>%RESPONSE%</p>
  <p>%RESPONSE1%</p>

  <script>
    function sendCommand(command) {
      document.getElementById('commandInput').value = command;
      document.getElementById('commandForm').submit();
    }
  </script>
</body>
</html>
)rawliteral";

WebServerHandler& WebServerHandler::getInstance() {
  static WebServerHandler instance;
  return instance;
}

WebServerHandler::WebServerHandler() : server(80), responseMessage(""), command(""), ssid(nullptr), password(nullptr) {}

void WebServerHandler::begin(const char* ssid, const char* password) {
  if (ssid && password) {
    this->ssid = ssid;
    this->password = password;
  } else {
    Serial.println("Error: SSID or password not provided.");
    return;
  }
  
  connectToWiFi();
  setupWebServerRoutes();
  server.begin();
  Serial.println("HTTP server started");
}

void WebServerHandler::connectToWiFi() {

  // IPAddress local_IP(192, 168, 0, 111);
  // IPAddress gateway(192, 168, 0, 1);
  // IPAddress subnet(255, 255, 255, 0);

  IPAddress local_IP(172,20,10,11);
  IPAddress gateway(172,20,10,1);
  IPAddress subnet(255,255,255,240);

  WiFi.config(local_IP, gateway, subnet);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

String WebServerHandler::processor(const String& var) {
  if (var == "RESPONSE") {
    return responseMessage;
  }

  if (var == "RESPONSE1") {
    return confirmationMessage;
  }

  return String();
}

void WebServerHandler::taskWrapper(void* pvParameters) {
  WebServerHandler* instance = static_cast<WebServerHandler*>(pvParameters);
  instance->handleClientTask();
}

void WebServerHandler::handleClientTask() {
  while(true) {
    server.handleClient();
    vTaskDelay(10 / portTICK_PERIOD_MS); // 10ms delay
  }
}

void WebServerHandler::handleRoot() {
  responseMessage = "";
  //confirmationMessage = "";
  String htmlContent = String(htmlPage);
  htmlContent.replace("%RESPONSE%", responseMessage);
  htmlContent.replace("%RESPONSE1%", confirmationMessage);
  server.send(200, "text/html", htmlContent);
}

// void WebServerHandler::handleSendCommand() {
//   if (server.hasArg("command")) {
//     command = server.arg("command");
//     Serial.println("Received command: " + command);
//     responseMessage = "Command received: " + command;
//   } else {
//     responseMessage = "Command not received";
//   }
//   String htmlContent = String(htmlPage);
//   htmlContent.replace("%RESPONSE%", responseMessage);
//   vTaskDelay(50 / portTICK_PERIOD_MS);
//   htmlContent.replace("%RESPONSE1%", confirmationMessage);
//   server.send(200, "text/html", htmlContent);
// }


void WebServerHandler::handleSendCommand() {
  if (server.hasArg("command")) {
    command = server.arg("command");
    Serial.println("Received command: " + command);

    // Example: sending GET request to another ESP32
    evaluateData();

  } else {
    responseMessage = "Command not received";
  }

  String htmlContent = String(htmlPage);
  htmlContent.replace("%RESPONSE%", responseMessage);
  htmlContent.replace("%RESPONSE1%", confirmationMessage);
  server.send(200, "text/html", htmlContent);
}

void WebServerHandler::setupWebServerRoutes() {
  server.on("/", [this]() { this->handleRoot(); });
  server.on("/send_command", HTTP_POST, [this]() { this->handleSendCommand(); });
}


void WebServerHandler::evaluateData(){
  FuzzyHandler& fuzzyHandler = FuzzyHandler::getInstance();
  VerticalSystemHandler& verticalSystemHandler = VerticalSystemHandler::getInstance();
  CommandHandler& commandHandler = CommandHandler::getInstance();
  EncoderHandler& encoderHandler = EncoderHandler::getInstance();
  if (command == "GETDATA" || command == "GAUTO") {
      //String url = "http://192.168.0.114/getData"; // Replace with your endpoint
      String url = "http://172.20.10.12/getData";
      HTTPClient http;

      Serial.print("Sending GET request to: ");
      Serial.println(url);

      // Your logic to send GET request
      if (http.begin(url)) {
        int httpCode = http.GET();
        if (httpCode > 0) {
          if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            responseMessage = "Received payload from ESP32: " + payload;

              String inputString = payload;
              int firstInt, secondInt;

              // Find the position of the comma
              int commaPosition = inputString.indexOf(',');

              // Extract the first integer
              String firstPart = inputString.substring(0, commaPosition);
              firstInt = firstPart.toInt();
              fuzzyHandler.setPixelsX(firstInt);
              
              // Extract the second integer
              String secondPart = inputString.substring(commaPosition + 1);
              secondInt = secondPart.toInt();
              verticalSystemHandler.setPixelsY(secondInt);

          } else {
            responseMessage = "Error receiving payload. HTTP code: " + String(httpCode);
          }
        } else {
          responseMessage = "Connection failed";
        }
        http.end();
        
      } else {
        responseMessage = "Unable to connect to server";
      }
    } else {
      responseMessage = "Invalid command: " + command;
  }
  
}







String WebServerHandler::getCommand() {
  return command;
}

void WebServerHandler::setCommand(const String& newCommand) {
  command = newCommand;
}

void WebServerHandler::setConfirmation(const String& newConfirmation) {
  confirmationMessage = newConfirmation;
}
