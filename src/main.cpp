#include <iostream>
#include <string>
#include <chrono>
#include "control.hpp"

int main() {
    termios og_termios_;
    tcgetattr(STDIN_FILENO, &og_termios_);
    termios termios_ = og_termios_; 

    termios_.c_cc[VMIN] = 0;
    termios_.c_cc[VTIME] = 0;
    cfmakeraw(&termios_);
    termios_.c_lflag &= ~(ICANON | ECHO);
    termios_.c_oflag |= OPOST;
    tcsetattr(STDIN_FILENO, TCSANOW, &termios_);

    char ch{'\0'};
    bool run = true;
    Control ctrl;
    run = ctrl.loop(ch);

    while (std::cin.get(ch) && run) {
	CLEAR
        run = ctrl.loop(ch);
        if (!run) break;
    }

    CLEAR
    RESTORE_CURSOR
    tcsetattr(STDIN_FILENO, TCSANOW, &og_termios_);
    return 0;
}
