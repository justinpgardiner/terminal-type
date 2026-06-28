#include <iostream>
#include <string>
#include <chrono>
#include "control.hpp"
#include "terminal_settings.hpp"

int main() {
    MainTerminalSettings settings{};
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
    return 0;
}
