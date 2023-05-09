#pragma once

enum windowNumber {MENU, CHOICE, PLAY, EXIT}windowNumber;
enum isFullscreen {FULLSCREEN, WINDOW}screenStatusNeeded, currentScreenStatus;

void playButton() {
    windowNumber = CHOICE;
}

void exitButton() {
    windowNumber = EXIT;
}

void switchScreen() {
    currentScreenStatus == WINDOW ? screenStatusNeeded = FULLSCREEN : screenStatusNeeded = WINDOW;
}