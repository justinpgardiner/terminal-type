#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>

class TestString {
private:
        inline static std::vector<std::string> load_all_words() {
            std::vector<std::string> words{};
            std::ifstream read("../resources/words.txt");
	    if (!read.is_open()) {
                std::cerr << "CANNOT OPEN WORD FILE\nRUN THE PROGRAM FROM INSIDE THE DIRECTORY THAT CONTAINS IT\n";
		std::abort();
	    }
	    std::string word{};
            while (std::getline(read, word)) {
                words.push_back(word);
            }
	    return words;
        }
        struct TestCharacter {
            char c;
            bool typed = false;
            bool correct = false;
	} typedef TestCharacter;

        const int print_width = 100;
        std::vector<TestCharacter> chars_;
	int curr;

public:
	TestString(); 
        void new_test(int num_words);
	void print();
        int num_words_completed();
	int wpm();
	double accuracy();
	void update(char c);
	int duration;
	inline static const std::vector<std::string> words_ = load_all_words();
};
