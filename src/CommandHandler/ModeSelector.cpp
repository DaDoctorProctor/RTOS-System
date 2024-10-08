#include "ModeSelector.h"

// Initialize the static instance
ModeSelector& ModeSelector::getInstance() {
    static ModeSelector instance;
    return instance;
}

// Private constructor
ModeSelector::ModeSelector() {
    mode = 0; // Initialize mode to a default value
}

// Getter for the mode
int ModeSelector::getMode() const {
    return mode;
}

// Setter for the mode
void ModeSelector::setMode(int newMode) {
    mode = newMode;
}
