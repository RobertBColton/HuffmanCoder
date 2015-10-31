/**
 * @file main.cpp
 * @brief Entry point for huffman encoder and decoder.
 *
 * @section License
 *
 * This file is a part of my huffman coder.
 * Copyright (C) 2015 Robert B. Colton
 *
 * This program is free software: you can redistribute it and/or modify it under 
 * the terms of the GNU General Public License as published by the Free Software 
 * Foundation, either version 3 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include <iostream>
#include <queue>
#include <string>

struct Code {
public:
  const char symbol_;
  int frequency_;
  const Code* left_;
  const Code* right_;
  
  Code(const char symbol): symbol_(symbol), frequency_(0), left_(NULL), 
    right_(NULL) { }
  ~Code() {
    delete left_;
    delete right_;
  }
};

struct CodeComparator
{
  bool operator()(const Code* a, const Code* b) const {
    return a->frequency_ > b->frequency_;
  }
};

void encode() {

}

void decode() {

}

int main() {
  std::priority_queue<Code*, std::vector<Code*>, CodeComparator > codeTree;
  std::cout << "Huffman Coder by Robert B. Colton, Copyright (C) 2015\n" <<
    "Please enter a string to be encoded followed by the enter key:" << 
    std::endl;
  std::string plainText;
  std::cin >> plainText;

  std::string str = "";
  for (int i = 0; i < 26; ++i) {
    std::cout << str + "0\n";
    str += "1";
  }

  for (size_t i = 0; i < plainText.length(); i++) {
    for (auto it = codeTree.begin(); it != codeTree.end(); it++) {
      if (it.symbol_ = plainText[i]) {
        it.frequency_++;
        break;
      }
    }
    codeTree.push(new Code(plainText[i]));
  }
}
