#include "test_string.hpp"

TestString::TestString() : 
	curr{0},
        chars_{},
        duration{1} {}

void TestString::print() {
    bool last = false;
    for (int i{0}; i < chars_.size(); i++) {
	char c = chars_[i].c;
	if (curr == i) {
            std::cout << "\x1b[0m\x1b[100m" << c << "\x1b[0m";
	} else if (chars_[i].typed && chars_[i].correct) {
            std::cout << (last ? "" : "\x1b[32m") << c;
	    last = true;
	} else if (chars_[i].typed && !chars_[i].correct) {
            std::cout << (last || i == 0 ? "\x1b[31m" : "") << c;
	    last = false;
	} else {
	    std::cout << c;
	}
    }
    std::cout << "\r\n" << std::flush;
}

void TestString::update(char c) {
    if (int(c) == 127) {
        chars_[curr].typed = false;
	curr -= curr == 0 ? 0 : 1;
    } else {
	chars_[curr].typed = true;
	chars_[curr].correct = (c == chars_[curr].c);
        ++curr;
    }
}

void TestString::new_test(int num_words) {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_int_distribution<int> dist(0, words_.size() - 1);

    chars_.clear();
    curr = 0;
    duration = 1;
    for (int i{0}; i < num_words; i++) {
	std::string word = words_[dist(mt)];
	for (char c : word) {
            chars_.push_back(TestString::TestCharacter{c});
	}
	if (i != num_words - 1) chars_.push_back(TestString::TestCharacter{' '});
    }
}

int TestString::num_words_completed() {
    int count{0};
    for (int i{0}; i < curr; i++) {
        if (chars_[i].c == ' ') count++;
    }
    if (curr == chars_.size()) count++;
    return count;
}

int TestString::wpm() {
    int words = std::ranges::count_if(chars_, [](const auto& c) {return c.correct && c.typed;}) / 5;
    double minutes = duration / 60.0;
    return static_cast<int>(words / minutes);
}

double TestString::accuracy() {
    return static_cast<double>(std::ranges::count_if(chars_, [](const auto& c) {return c.correct && c.typed;})) /
	    std::ranges::count_if(chars_,[](const auto& c) {return c.typed;});
}
