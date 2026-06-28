#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

class TestTerminalSettings {
public:
    TestTerminalSettings();
    ~TestTerminalSettings();
private:
    void create_settings();
    struct termios old_settings;
    int stdin_fd{STDIN_FILENO};
};

class MainTerminalSettings {
public:
    MainTerminalSettings();
    ~MainTerminalSettings(); 
private:
    void create_settings();
    struct termios old_settings;
    int stdin_fd{STDIN_FILENO};
};
