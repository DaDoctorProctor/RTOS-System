#include <Arduino.h>
#include "Wheel/WheelHandler.h"
#include "Wheel/EncoderHandler.h"
#include "FuzzyHandler/FuzzyHandler.h"
#include "VerticalSystemHandler/VerticalSystemHandler.h"
#include "WebServerHandler/WebServerHandler.h"
#include "ModeSelector.h"


// CommandLineProcess.h
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H


class CommandHandler {
public:
    static CommandHandler& getInstance(); // Singleton instance getter
    void handleCommand(); // Function to process serial commands
    static void taskWrapper(void* pvParameters);  // Task wrapper function

    void setFlag(bool flag);
    bool getFlag() const;

private:
    CommandHandler(); // Private constructor for singleton
    CommandHandler(const CommandHandler&) = delete; // Delete copy constructor
    CommandHandler& operator=(const CommandHandler&) = delete; // Delete assignment operator

    void taskCommand();
    void commandInterpreter(String command);
    void sendResponse(String msg);


    bool flag = false;

};

#endif // COMMAND_HANDLER_H
