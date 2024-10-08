#ifndef WEBSERVERHANDLER_H
#define WEBSERVERHANDLER_H

#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "VerticalSystemHandler/VerticalSystemHandler.h"
#include "FuzzyHandler/FuzzyHandler.h"
#include "CommandHandler/CommandHandler.h"

class WebServerHandler {
public:
  static WebServerHandler& getInstance();  // Singleton instance getter
  void begin(const char* ssid, const char* password);  // Initialize with SSID and password
  void handleClientTask();  // Task to handle clients
  static void taskWrapper(void* pvParameters);  // Task wrapper function
  String getCommand();  // Getter for command

  void setCommand(const String& newCommand);
  void setConfirmation(const String& newConfirmation);

private:
  WebServerHandler();  // Private constructor

  WebServer server;

  String responseMessage;
  String command;

  String confirmationMessage;
  String commandConfirmation;
  
  
  
  const char* ssid;
  const char* password;

  void connectToWiFi();
  void setupWebServerRoutes();
  void handleRoot();
  void handleSendCommand();
  String processor(const String& var);

  static const char htmlPage[];

  void evaluateData();
};

#endif // WEBSERVERHANDLER_H
