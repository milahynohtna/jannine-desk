#include "SerialInputHandler.h"

SerialInputHandler::SerialInputHandler() 
    : inputBuffer(""), onInput(nullptr) {
}

bool SerialInputHandler::checkInput() {
    bool hasInput = false;
    
    while (Serial.available()) {
        char c = Serial.read();
        processChar(c);
        
        if (c == '\n' && inputBuffer.length() > 0) {
            hasInput = true;
        }
    }
    
    return hasInput;
}

void SerialInputHandler::processChar(char c) {
    if (c == '\n') {
        inputBuffer.trim();
        
        if (inputBuffer.length() > 0) {
            if (onInput) {
                onInput(inputBuffer);
            }
            clearInput();
        }
    } else {
        inputBuffer += c;
    }
}

String SerialInputHandler::getInput() {
    return inputBuffer;
}

void SerialInputHandler::clearInput() {
    inputBuffer = "";
}

void SerialInputHandler::setInputCallback(InputCallback callback) {
    onInput = callback;
}
