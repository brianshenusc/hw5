#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"

using namespace std;


// Add prototypes of helper functions here

// recursive helpper function to solve wordle
std::set<std::string> wordle_helper(const std::string& in, const std::string& floating, const std::set<std::string>& dict, size_t index, string word);

// checks if the current word can become a result
bool check_valid(const std::string& in, std::string floating, std::string& word);

// gets the intersection of two sets
std::set<std::string> setIntersection(std::set<std::string>& s1, const std::set<std::string>& s2);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here

    std::set<std::string> output;

    string word = "";
    // sets up word
    bool isComplete = true;
    for(size_t i = 0; i < in.size(); i++) {
        word = word + '-';
        if(in[i] == '-') {
            isComplete = false;
        }
    }
    // checks for case where an empty string is inputted
    if(isComplete && in.size() != 0) {
        output.insert(in);
        return output;
    }
    // recursive call
    output = wordle_helper(in, floating, dict, 0, word);
    // finds the intersection of the sets and returns it
    std::set<std::string> intersectionSet = setIntersection(output, dict);

    return intersectionSet;

}

// Define any helper functions here

// returns a set of possible combinations for a single letter place
std::set<std::string> wordle_helper(const std::string& in, const std::string& floating, const std::set<std::string>& dict, size_t index, string word) {
        std::set<std::string> combinations;
        // base case - inserts the completed word into combinations
        if(index == word.size()) {
            combinations.insert(word);
            return combinations;
        }
        // checks for letters that are already given in the input
        if(in[index] != '-') {
            word[index] = in[index];
            std::set<std::string> temp = wordle_helper(in, floating, dict, index+1, word);
            combinations.insert(temp.begin(), temp.end());
        }
        else {
            // traverses through the alphabet
            for(int i = 0; i < 26; i++) {
                // if the letter is blank, set to a
                if(word[index] == '-') {
                    word[index] = 'a';
                }
                // if the letter is between a-y, increment to the next letter
                else if(word[index] != '-' && word[index] != 'z') {
                    word[index] = char(word[index] + 1);
                }								
                // checks if the word is still valid, continues to next letter if false
                if(!check_valid(in, floating, word)) {
                    continue;
                }
                // if the word is finished, then it is inserted into combinations
                if(word.size() - 1 == index) {
                    combinations.insert(word);
                }
                // recursive call for the next index in the word
                std::set<std::string> temp = wordle_helper(in, floating, dict, index+1, word);
                combinations.insert(temp.begin(), temp.end());
            }
        }
        return combinations;

      
    }


// // checks if ALL floating letters are in a given string
// bool check_floating(const std::string& in, const std::string& floating, std::string word) {
//     // std::string letters = "";
// 		// for(size_t i = 0; i < in.size(); i++) {
// 		// 	if(in[i] == '-') {
// 		// 		letters = letters + word[i];
// 		// 	}
// 		// }
// 		// bool isValid = true;
//     // for(size_t i = 0; i < floating.size(); i++) {
//     //     if(letters.find(floating[i]) == std::string::npos) {
//     //         return false;
//     //     }
//     //     else {
//     //         int x = letters.find(floating[i]);
//     //         if(letters.size() != 0) {
//     //             letters.erase(letters.begin() + x);
//     //         }
            
//     //     }
//     // }
//     // return isValid;


// 	bool isValid = true;
//     for(size_t i = 0; i < floating.size(); i++) {
//         if(word.find(floating[i]) == std::string::npos) {
//             return false;
//         }
//         else {
//             int x = word.find(floating[i]);
//             if(word.size() != 0) {
//                 word.erase(word.begin() + x);
//             }
            
//         }
//     }
//     return isValid;

// }


// checks if the current word should be continued 
// bool check_valid(const std::string& in, const std::string floating, std::string& word) {	
//     // cout << word << endl;
//     if(floating.size() == 0) {
//         return true;
//     }
//     int num_blank = 0;
// 	std::string letters = "";
// 	for(size_t i = 0; i < in.size(); i++) {
//         if(in[i] == '-') {
//             num_blank++;
//         }
// 		if(in[i] == '-' && word[i] != '-') {
// 			letters = letters + word[i];
// 		}
// 	}

//     // cout << "WORD: " << word << "  BEFORE: " << letters;
//     for(size_t i = 0; i < floating.size(); i++) {
//         if(letters.find(floating[i]) != std::string::npos) {
//             int x = letters.find(floating[i]);
//             if(letters.size() != 0) {
//                 letters.erase(letters.begin() + x);
//             }
//         }

//     }
//     // cout << "  AFTER: " << letters << "   WORD: " << word;

//     // if the possible number of blanks is less than filled blanks, returns false
//     if(num_blank - floating.size() < letters.size()) {
//         return false;
//     }
//     return true;

// }

// checks if the current word should be continued 
bool check_valid(const std::string& in, std::string floating, std::string& word) {	
    // loops through "in" and counts the number of blanks, number of filled letters, and if the letter is blank 
    // for "in" but not for "word", then checks if that letter is in floating and removes from floating if yes
    int num_blanks = 0, num_non_blanks = 0, floating_in_word = 0;
    int floating_size = floating.size();
    for(size_t i = 0; i < in.size(); i++) {
        if(in[i] == '-') {
            num_blanks++;
        }
        if(in[i] == '-' && word[i] != '-') {
            num_non_blanks++;
            // if the current letter exists in floating, delete that letter from floating and increment floating_in_word
            if(floating.find(word[i]) != std::string::npos) {
                int x = floating.find(word[i]);
                if(floating.size() != 0) {
                    floating_in_word++;
                    floating.erase(floating.begin() + x);
                }
            }

        }
    }
    // if the possible number of blanks is less than filled blanks, returns false
    if(num_blanks - floating_size < num_non_blanks - floating_in_word) {
        return false;
    }
    return true;

}



// finds the intersection of a set (from HW 2)
std::set<std::string> setIntersection(std::set<std::string>& s1, const std::set<std::string>& s2)
{
	std::set<std::string> intersection;
	// if something is in both sets, then it is inserted into intersection
	std::set<std::string>::iterator it;
	for(it = s1.begin(); it != s1.end(); ++it) {
        if(s2.find(*it) != s2.end()) {
            intersection.insert(*it);
        }
	}
	return intersection;
}