#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <thread>
#include "test_string.hpp"

#define CLEAR std::cout << "\033[2J\033[H" << std::flush;
#define HIDE_CURSOR std::cout << "\033[?25l" << std::flush;
#define RESTORE_CURSOR std::cout << "\033[?25h" << std::flush;

enum class State {
    Welcome,
    Menu,
    PreTest,
    Test,
    PostTest
};

class Control {
public:
        Control();
        bool loop(char c = 0);
	bool welcome(char c = 0);
	bool menu(char c = 0);
	bool pre_test(char c = 0);
	bool test(char c = 0);
	bool post_test(char c = 0);
private:
	State state_;
	int settings_lvl_;
	int countdown_;
	std::chrono::time_point<std::chrono::steady_clock> start_time_;
	int mode_choice_;
	int num_choice_;
	bool new_test_;
	TestString test_;
	std::vector<std::string> modes_;
	const std::vector<int> num_opts_words_;
	const std::vector<int> num_opts_time_;

        class TestTerminalSettings {
        private:
	    struct termios old_settings;
	    int stdin_fd;
	public: 
            TestTerminalSettings() : stdin_fd(STDIN_FILENO) {
                struct termios new_settings;
		tcgetattr(stdin_fd, &old_settings);
                new_settings = old_settings;
                new_settings.c_lflag &= ~(ICANON | ECHO);
                new_settings.c_cc[VMIN] = 0;
                new_settings.c_cc[VTIME] = 0;
                tcsetattr(stdin_fd, TCSANOW, &new_settings);
	    }
	     ~TestTerminalSettings() {
                tcflush(stdin_fd, TCIFLUSH);
                tcsetattr(stdin_fd, TCSANOW, &old_settings);
	     }
	};
};
