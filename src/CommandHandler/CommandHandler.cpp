// CommandHandler.cpp
#include "CommandHandler.h"

// Singleton instance getter
CommandHandler& CommandHandler::getInstance() {
    static CommandHandler instance; // Static instance ensures it's created once
    return instance;
}

// Private constructor for singleton
CommandHandler::CommandHandler() {}

// FreeRTOS task function
void CommandHandler::taskWrapper(void* pvParameters) {
    CommandHandler* instance = static_cast<CommandHandler*>(pvParameters);
    instance->taskCommand();
}

// Task process function
void CommandHandler::taskCommand() {
    while(true) {
        handleCommand();
        vTaskDelay(10 / portTICK_PERIOD_MS); // 10ms delay
    }
}


// Function to process serial commands
void CommandHandler::handleCommand() {
    WebServerHandler& webServerHandler = WebServerHandler::getInstance();

    // If you want to get the command received, you can use getCommand method
    String commandReceived = webServerHandler.getCommand();

    if(commandReceived == ""){
        return;
    }

    commandInterpreter(commandReceived);

    webServerHandler.setCommand(""); // Clear the commandReceived variable
    //webServerHandler.setConfirmation("");
}

void CommandHandler::commandInterpreter(String command){
    WheelHandler& wheelHandler = WheelHandler::getInstance();
    VerticalSystemHandler& verticalSystemHandler = VerticalSystemHandler::getInstance();
    HomeHandler& homeHandler = HomeHandler::getInstance();
    EncoderHandler& encoderHandler = EncoderHandler::getInstance();
    FuzzyHandler& fuzzyHandler = FuzzyHandler::getInstance();
    ModeSelector& modeSelector = ModeSelector::getInstance();

    char cmdArray[8];
    command.trim(); // Remove any leading/trailing whitespace
    command.toCharArray(cmdArray, sizeof(cmdArray)); // Convert command to char array
    const char* cmd = command.c_str(); // Convert command to const char* for comparison
    String msg = "";

    // Start stop commands
    if (strcmp(cmd, "CSTART") == 0) {
        wheelHandler.setMotorEnabled(true); // Start command to enable the motors
        msg = "[D]: STR";
    } else if (strcmp(cmd, "CSTOP") == 0) {
        wheelHandler.setMotorEnabled(false); // Stop command to disable the motors
        msg = "[D]: STP";
    
    // Wheel commands
    } else if (cmdArray[0] == 'S' && cmdArray[1] == 'P' && cmdArray[2] == 'D') {
        char CMD0X[4] = {cmdArray[3], cmdArray[4], cmdArray[5], '\0'}; // Changes the current reference value of the wheel motor
        wheelHandler.setMovement(atoi(CMD0X));
        msg = "[D]: ROTATORY SPEED CHANGED -> " + String(CMD0X);
    } else if (strcmp(cmd, "CW") == 0) {
        wheelHandler.setWheelDirection(false); // Changes the direction of the motor: CW
        msg = "[D]: CW";
    } else if (strcmp(cmd, "CCW") == 0) {
        wheelHandler.setWheelDirection(true); // Changes the direction of the motor: CCW
        msg = "[D]: CCW";
    } else if (strcmp(cmd, "CRPOS") == 0) {
        encoderHandler.resetPosition(); // Sets the encoder ticks to 0
        msg = "[D]: Position: " + String(encoderHandler.getPosition());
    } else if (strcmp(cmd, "CPOS") == 0) {
        msg = "[D]: Encoder position (current): " + String(encoderHandler.getPosition() + " (previous): " + String(encoderHandler.getPPosition())); // Gets the encoder position (wheel)
    
    
    } else if (cmdArray[0] == 'R' && cmdArray[1] == 'E' && cmdArray[2] == 'F') {
        char CMD0X[5] = {cmdArray[3], cmdArray[4], cmdArray[5], cmdArray[6], '\0'}; // Changes the current reference value of the wheel motor
        fuzzyHandler.setReference(atoi(CMD0X));
        msg = "[D]: Reference -> " + String(CMD0X);
    } else if (strcmp(cmd, "HOME") == 0) {
        homeHandler.setRun(true);
        msg = "[D]: Home";
    } else if (strcmp(cmd, "RF+500") == 0) {
        fuzzyHandler.setReference(fuzzyHandler.getReference()+500);
        msg = "[D]: RF+500";
    } else if (strcmp(cmd, "RF-500") == 0) {
        fuzzyHandler.setReference(fuzzyHandler.getReference()-500);
        msg = "[D]: RF-500";
    }

    
    // Mode commands
    else if (strcmp(cmd, "CMODE1") == 0) {
        msg = "[D]: System set to mode: M1"; 
        modeSelector.setMode(1);
    } else if (strcmp(cmd, "CMODE2") == 0) {
        msg = "[D]: System set to mode: M2";
        modeSelector.setMode(2);
    } else if (strcmp(cmd, "CMODE3") == 0) {
        modeSelector.setMode(3);
        msg = "[D]: System set to mode: M3";
    } 

    // Vertical

    else if (cmdArray[0] == 'V' && cmdArray[1] == 'P') {
        if (cmdArray[2] == '-' && isdigit(cmdArray[3]) && isdigit(cmdArray[4]) && isdigit(cmdArray[5])){
            char VMD0X[5] = {'-',cmdArray[3], cmdArray[4], cmdArray[5], '\0'};
            verticalSystemHandler.setSteps(atoi(VMD0X)*100);
            verticalSystemHandler.setVerticalSystemEnabled(true);
            msg = "[D]: VERTICAL POSITION DOWN: " + String(VMD0X);
        } else if (isdigit(cmdArray[2]) && isdigit(cmdArray[3]) && isdigit(cmdArray[4])) {
            char VMD0X[4] = {cmdArray[2], cmdArray[3], cmdArray[4], '\0'};
            verticalSystemHandler.setSteps(atoi(VMD0X)*100);
            verticalSystemHandler.setVerticalSystemEnabled(true);
            msg = "[D]: VERTICAL POSITION UP: " + String(VMD0X);
        } 

    } else if (strcmp(cmd, "VFUP") == 0){
        verticalSystemHandler.setSteps(10000000);
        verticalSystemHandler.setVerticalSystemEnabled(true);
        msg = "[D]: FULL UP";
    } else if (strcmp(cmd, "VFDOWN") == 0){
        verticalSystemHandler.setSteps(-10000000);
        verticalSystemHandler.setVerticalSystemEnabled(true);
        msg = "[D]: FULL DOWN";
    } else if (strcmp(cmd, "VF1REV") == 0){
        verticalSystemHandler.setSteps(800*10);
        verticalSystemHandler.setVerticalSystemEnabled(true);
        msg = "[D]: Moving 1 REV";
    } else if (strcmp(cmd, "VCAL") == 0){
        msg = "[D]: Calibrating";
    } else if (strcmp(cmd, "VSTOP") == 0){
        verticalSystemHandler.activateStop();
        msg = "[D]: Forced stop activated";
    } else if (strcmp(cmd, "VSTEP") == 0){
        msg = "[D]: Current VSystem position:" + String(verticalSystemHandler.getStepsMoved());
    

    } else if (strcmp(cmd, "YAUTO") == 0){
        int pixels = verticalSystemHandler.getPixelsY();
        // stall process to wait for gdata to finish
        
        int steps = verticalSystemHandler.VerticalInterpreter(pixels);
        verticalSystemHandler.setSteps(steps);
        verticalSystemHandler.setVerticalSystemEnabled(true);
        
        msg = "[D]: Y AUTO OPERATION";
    } else if (strcmp(cmd, "XAUTO") == 0){
        int pixels = fuzzyHandler.getPixelsX();
        // stall process to wait for gdata to finish
        
        int ref = fuzzyHandler.HorizontalInterpreter(pixels);
        fuzzyHandler.setReference(encoderHandler.getPosition()+ref);
        
        msg = "[D]: X AUTO OPERATION: " + String(pixels) + " : " + String(ref);

    } else if (strcmp(cmd, "GAUTO") == 0){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        int pixels_x = fuzzyHandler.getPixelsX();
        int pixels_y = verticalSystemHandler.getPixelsY();
        // stall process to wait for gdata to finish
        int ref = fuzzyHandler.HorizontalInterpreter(pixels_x);
        int steps = verticalSystemHandler.VerticalInterpreter(pixels_y);
        fuzzyHandler.setReference(encoderHandler.getPosition()+ref);
        verticalSystemHandler.setSteps(steps);
        verticalSystemHandler.setVerticalSystemEnabled(true);

        msg = "[D]: G AUTO OPERATION: " + String(pixels_x) + " : " + String(ref);

    }

    
    sendResponse(msg); // send the resulting command
}



void CommandHandler::sendResponse(String msg){
    WebServerHandler& webServerHandler = WebServerHandler::getInstance();
    webServerHandler.setConfirmation(msg);
    Serial.println(msg);   
}


