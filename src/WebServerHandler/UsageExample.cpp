#include "UsageExample.h"
#include <Arduino.h>

UsageExample::UsageExample(WebServerHandler& handler) : webServerHandler(handler) {}

String UsageExample::printCommand() {
  String command = webServerHandler.getCommand();
  Serial.println("Accessed command: " + command);
  return command;
}
