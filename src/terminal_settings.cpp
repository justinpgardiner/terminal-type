#include "terminal_settings.hpp"

void TestTerminalSettings::create_settings() {
    tcgetattr(stdin_fd, &old_settings);
    struct termios new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    new_settings.c_cc[VMIN] = 0;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(stdin_fd, TCSANOW, &new_settings);
}

void MainTerminalSettings::create_settings() {
    tcgetattr(STDIN_FILENO, &old_settings);
    struct termios new_settings = old_settings;
    new_settings.c_cc[VMIN] = 0;
    new_settings.c_cc[VTIME] = 0;
    cfmakeraw(&new_settings);
    new_settings.c_lflag &= ~(ICANON | ECHO);
    new_settings.c_oflag |= OPOST;
    tcsetattr(stdin_fd, TCSANOW, &new_settings);
}

TestTerminalSettings::TestTerminalSettings() {
    create_settings();
}

MainTerminalSettings::MainTerminalSettings() {
    create_settings();
}

TestTerminalSettings::~TestTerminalSettings() {
    tcflush(stdin_fd, TCIFLUSH);
    tcsetattr(stdin_fd, TCSANOW, &old_settings);
}

MainTerminalSettings::~MainTerminalSettings() {
    tcflush(stdin_fd, TCIFLUSH);
    tcsetattr(stdin_fd, TCSANOW, &old_settings);
}


