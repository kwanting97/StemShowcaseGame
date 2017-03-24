//
//  main.cpp
//

#include <iostream>
#include <sstream>
#include <fstream>
#include "prefixTree.h"

using namespace std;

/* In this game we want to know if the letters in the word entered by
 * the player were indeed drawn from the letters provided by the game.
 * The most straightforward (and linear) way to do this is along the lines
 * suggested by Sam in class because we also need to maintain counts (e.g.
 * the game might give the user 2 As and 3Ds, and you want to keep track of
 * that) This method returns true if playerStr can be made exclusively from
 * the letters in gameLetters.
 */
bool isContainedIn(string playerStr, string gameLetters) {
   int availableLetterCounts[256]; // Num pats in a byte

   for (int i = 0; i < 256; i++) {
      availableLetterCounts[i] = 0;
   }

   for (int i = 0; i < gameLetters.length(); i++) {
      int j = toupper(gameLetters[i]);
      availableLetterCounts[j]++;
   }

   for (int i = 0; i < playerStr.length(); i++) {
      int j = toupper(playerStr[i]);
      if (availableLetterCounts[j]-- <= 0)
         return false;
   }

   return true;
}

bool isValidWord(const string& s) {
   for(int i = 0; i < s.length(); i++) {
      char c = s[i];
      bool isUpperCase = c >= 65 && c <= 90;
      bool isLowerCase = c >= 97 && c <= 122;
      if(isUpperCase && isLowerCase)
         return false;
   }
   return true;
}

void importDict(istream& is, Trie& t) {
   string line, tmp;
   while (getline(is, line)) {
      istringstream iss(line);
      while(iss >> tmp) {

         if(isValidWord(tmp))
            t.insert(tmp);
      }
   }
}

string charToString(char c) {
   stringstream ss;
   string tmp;
   ss << c;
   ss >> tmp;
   return tmp;
}

int comb(const string& gameLetters, const Trie& t, int level, string str,
         int len, Trie& t2) {
   int cnt = 0;
   if(level > 0) {
      for(int i = 0; i < len; i++) {
         cnt += comb(gameLetters, t, level - 1, str + gameLetters[i], len, t2);
      }
   } else {
      //cout << str << endl;
      if(t.find(str) > 0 && t2.find(str) == 0 &&
         isContainedIn(str, gameLetters)) {
         t2.insert(str);
         cout << "  " << str << endl;
         cnt += t.find(str) * str.length();
      }
   }
   return cnt;
}

int allComb(const string& gameLetters, const Trie& t) {
   int cnt = 0;
   Trie t2;
   int len = (int)gameLetters.length();
   for(int i = 1; i <= gameLetters.length(); i++) {
      cnt += comb(gameLetters, t, i, "", len, t2);
   }
   return cnt;
}

int main(int argc, const char * argv[]) {
   srand((unsigned int)time(0L));

   Trie dict, inputed;
   const string FILE_NAME = "cleaned-words.txt";
   string dir = "/Users/rix/Desktop/College/FootHill/Classes/";
   dir += "Winter 2017/CS2B/InClass/FHTree/FHTree/";
   //string FILE_NAME = "words";
   //string dir = "/usr/share/dict/";

   ifstream ifs(dir + FILE_NAME);
   cout << "Loading dictionary...\n";
   importDict(ifs, dict);
   cout << "Dictionary loaded!\n\n";

   const int GAME_TIME = 60;
   const int GAME_LETTER_SIZE = 7;

   int score = 0;
   long startTime = time(0L);
   long endTime = startTime + GAME_TIME;

   const string vows("aeiou");
   const string cons("bcdfghjklmnpxqwrtysvz");
   string gameLetters = "";
   for(int i = 0; i < GAME_LETTER_SIZE; i++) {
      if(i % 2 == 0) {
         gameLetters += vows[rand() % 5];
      } else {
         gameLetters += cons[rand() % 21];
      }
   }

   long timeLeft = endTime - time(0L);
   while(timeLeft > 0) {
      timeLeft = endTime - time(0L);
      cout << "Time left: " << timeLeft << "s.\n";
      cout << "Score: " << score << endl;
      cout << "Letters: " << gameLetters << endl;
      cout << "Word: ";
      string input;
      getline(cin, input);

      timeLeft = endTime - time(0L);
      if(timeLeft < 0) {
         cout << "Time's up!\n\n";
         break;
      }

      if(inputed.find(input)) {
         cout << "It's already exist!\n";
         cout << "Penalty -1\n";
         cout << endl;
         score -= 1;
         continue;
      } else if(dict.find(input) > 0) {
         inputed.insert(input);
      }

      if(isContainedIn(input, gameLetters) && dict.find(input) > 0) {
         cout << "Valid!\n";
         score += input.length();
      } else {
         cout << "Invalid!\n";
         int penalty = (input == "" ? 2 : 1);
         cout << "Penalty -" << penalty << endl;
         score -= penalty;
      }

      cout << endl;
   }

   cout << "Final score: " << score << endl;
   cout << "All possible words of " << gameLetters << ":\n";
   int cnt = allComb(gameLetters, dict);
   cout << "Total possible points: " << cnt << endl;
   if(cnt != 0)
      cout << "Score(%): " << (double)score / cnt * 100 << "%" << endl;

   cout << endl;

   return 0;
}
