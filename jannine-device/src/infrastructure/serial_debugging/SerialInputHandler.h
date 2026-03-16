#ifndef SERIAL_INPUT_HANDLER_H
#define SERIAL_INPUT_HANDLER_H

#include <Arduino.h>

class SerialInputHandler {
public:
    SerialInputHandler();
    
    // Input processing
    bool checkInput();
    String getInput();
    void clearInput();
    
    // Callback
    typedef std::function<void(const String&)> InputCallback;
    void setInputCallback(InputCallback callback);

private:
    String inputBuffer;
    InputCallback onInput;
    
    void processChar(char c);
};

#endif // SERIAL_INPUT_HANDLER_H
