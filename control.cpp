#include <iostream>
#include "control.hpp"

Control::Control() :
	state_{State::Welcome},
	settings_lvl_{0},
	mode_choice_{0},
	num_choice_{0},
	new_test_{true},
	test_{},
        modes_{"TIME", "WORDS"},
        num_opts_words_{10, 25, 50, 100},
        num_opts_time_{15, 30, 45, 60} {}
 
bool Control::loop(char c) {
    switch(state_) {
        using enum State;
        case Welcome:
	    return Control::welcome(c);
 	case Menu:
	    return Control::menu(c);
	case PreTest:
	    return Control::pre_test(c);
	case Test:
	    return Control::test(c);
	case PostTest:
	    return Control::post_test(c);
    }
}

bool Control::welcome(char c) {
    CLEAR
    std::cout << 
R"(  ______                    _             __   ______                
 /_  __/__  _________ ___  (_)___  ____ _/ /  /_  __/_  ______  ___ 
  / / / _ \/ ___/ __ `__ \/ / __ \/ __ `/ /    / / / / / / __ \/ _ \
 / / /  __/ /  / / / / / / / / / / /_/ / /    / / / /_/ / /_/ /  __/
/_/  \___/_/  /_/ /_/ /_/_/_/ /_/\__,_/_/    /_/  \__, / .___/\___/  
                                                 /____/_/)" << "\n";
    std::cout << "\n[ENTER] to continue or [q] to quit\n"; 
    if (int(c) == 13) {
        state_ = State::Menu;
	return Control::menu(); 
    }
    HIDE_CURSOR
    return c != 'q';
}

bool Control::menu(char c) {
    CLEAR
    std::cout << "SELECT TEST OPTIONS\n";
    std::cout << "[tab] to cycle choice\n";
    std::cout << "[ENTER] to make choice\n";
    std::cout << "[u] to undo previous choice\n";
    std::cout << "[b] to go back to title\n";
    std::cout << "[q] to quit\n\n";

    if (c == 'b') {
        state_ = State::Welcome;
	return Control::welcome(); 
    }

    if (int(c) == 13) {
        settings_lvl_++;
	if (settings_lvl_ == 3) {
	    settings_lvl_ = 0;
	    state_ = State::PreTest; 
	    new_test_ = true;
	    return Control::pre_test();
	}
	HIDE_CURSOR
	return Control::menu(); 
    }

    if (c == 'u') {
        if (settings_lvl_ > 0) {
            settings_lvl_--;
	}
	HIDE_CURSOR
	return Control::menu();
    }
    std::cout << ((settings_lvl_ == 0) ? ">" : ""); 
    std::cout << "MODE:\t";
    if (settings_lvl_ == 0 && int(c) == 9) {
        ++mode_choice_ %= modes_.size();
    }
    for (int i{0}; i < modes_.size(); i++) {
        if (i == mode_choice_) {
            std::cout << "\x1b[30;100m" + modes_[i] + "\x1b[0m\t";
	} else {
            std::cout << modes_[i] + "\t";
	}
    }
    std::cout <<  "\n";

    std::cout << ((settings_lvl_ == 1) ? ">" : ""); 
    std::cout << modes_[mode_choice_] + ":\t";
    const std::vector<int>& opts_ = mode_choice_ ? num_opts_words_: num_opts_time_;
    if (settings_lvl_ == 1 && int(c) == 9) {
        ++num_choice_ %= opts_.size();
    }
    if (num_choice_ > opts_.size()) {
        num_choice_ = opts_.size() - 1;
    }
    for (int i{0}; i < opts_.size(); i++) {
        if (i == num_choice_) {
            std::cout << "\x1b[30;100m" << opts_[i] << "\x1b[0m\t";
	} else {
            std::cout << opts_[i] << "\t";
	}
    }
    std::cout << "\n";
    
    std::cout << ((settings_lvl_ == 2) ? ">" : "") << "START" << "\n"; 
    HIDE_CURSOR
    return c != 'q';
}

bool Control::pre_test(char c) {
    CLEAR
    if (!new_test_ && c) {
        state_ = State::Test;
        return Control::test(c);
    }
    const std::vector<int>& opts_ = mode_choice_ ? num_opts_words_: num_opts_time_;
    int num = opts_[num_choice_];
    std::cout << modes_[mode_choice_] << ": " << num << "\n(press [ENTER] to exit early)\n";
    if (new_test_) {
	int word_count = mode_choice_ ? num : (int)(((num/60.0) * 300.0));
        test_.new_test(word_count);
	new_test_ = false;
    }
    std::cout << "\n\n";
    test_.print();
    HIDE_CURSOR
    return c != 'q';
}

bool Control::test(char c) { 
    CLEAR
    Control::TestTerminalSettings tts;
    const std::vector<int>& opts_ = mode_choice_ ? num_opts_words_: num_opts_time_;
    countdown_= opts_[num_choice_];
    start_time_ = std::chrono::steady_clock::now();
    bool update = true;
    
    while (true) {
	if (int(c) == 13) {
	    state_ = State::Menu;
	    return Control::menu();
	}
        if (update) test_.update(c);
	update = read(STDIN_FILENO, &c, 1) > 0;
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start_time_);

        countdown_ = mode_choice_ ? 
	       opts_[num_choice_] - test_.num_words_completed()
	     : opts_[num_choice_] - duration.count();

        if (countdown_ <= 0) {
            test_.duration = duration.count();
	    break;
	} 

	CLEAR
	std::cout << modes_[mode_choice_] << ": " << countdown_ << "\n(press [ENTER] to exit early)"; 
        std::cout << "\n\n";
        test_.print();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    state_ = State::PostTest;
    return Control::post_test(); 
}

bool Control::post_test(char c) {
    CLEAR
    std::cout << "WPM: " << test_.wpm() << "\n";
    std::cout << "Accuracy: " << test_.accuracy() << "\n";
    std::cout << "Press [ENTER] to return to the menu or [q] to quit\n";
    if (int(c) == 13) {
        state_ = State::Menu;
	return Control::menu(); 
    }
    return c != 'q';
}
