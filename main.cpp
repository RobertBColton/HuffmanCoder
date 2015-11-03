/**
 * @file main.cpp
 * @brief Entry point for Huffman encoder and decoder.
 *
 * @section License
 *
 * This file is a part of my Huffman coder.
 * Copyright (C) 2015 Robert B. Colton
 *
 * This program is free software: you can redistribute it and/or modify it under 
 * the terms of the GNU General Public License as published by the Free Software 
 * Foundation, either version 3 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
 * FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <string>

struct Code {
public:
  char const symbol_;
  int frequency_;
  Code const* left_;
  Code const* right_;

  Code(): symbol_(0), frequency_(0), left_(NULL), 
    right_(NULL) { }
  Code(const char symbol): symbol_(symbol), frequency_(0), left_(NULL), 
    right_(NULL) { }
  ~Code() {
    delete left_;
    delete right_;
  }
};

struct CodeComparator
{
  bool operator()(Code const* a, Code const* b) const {
    return a->frequency_ > b->frequency_;
  }
};

void printTree(Code const* node, int spacing) {
  std::stack<Code const*> children;
  std::stack<int> widths;
  int width = spacing;
  while (!children.empty() || node) {
    if (node) {
      children.push(node);
      widths.push(width);
      node = node->left_;
      width += spacing;
    } else {
      node = children.top();
      children.pop();
      char symbol = node->symbol_;
      if (symbol == '\n') { symbol = '*'; }
      else if (symbol == 0) { symbol = '<'; }
      width = widths.top();
      widths.pop();
      std::cout << std::setw(width) << symbol << '\n';
      node = node->right_;
      width += spacing;
    }
  }
}

int main() {
  // display copyright and program details
  std::cout << "Huffman Coder by Robert B. Colton, Copyright (C) 2015\n" <<
    "Enter the text to be encoded followed by the end of file signal:"
    << std::endl;
  // enter the text to be encoded and subsequently decoded
  std::string plainText;
  char buffer[4096];
  while (true) {
    size_t elements = fread(buffer, 1, sizeof(buffer), stdin);
    plainText += std::string(buffer, elements);
    if (elements < sizeof(buffer)) {
       break;
    }
  }

  std::vector<Code*> symbols;
  // find all of the unique symbols in the string
  for (size_t i = 0; i < plainText.length(); ++i) {
    auto it = symbols.begin();
    // increment this symbol's frequency if we've encountered it before
    for (; it != symbols.end(); ++it) {
      Code *const node = *it;
      if (node->symbol_ == plainText[i]) {
        node->frequency_++;
        break;
      }
    }
    // if we haven't encountered the symbol yet, make a new entry for it
    if (it == symbols.end()) {
      symbols.push_back(new Code(plainText[i]));
    }
  }
  // sort the symbols by frequency and show the most and least frequency symbol
  std::sort(symbols.begin(), symbols.end(), CodeComparator());
  std::cout << "Symbols: " << symbols.front()->symbol_ << " " << 
    symbols.back()->symbol_ << '\n';

  // build the tree until we reach the root node whose left child is the most
  // frequent symbol
  while (symbols.size() > 1) {
    Code *const node = new Code();
    node->left_ = symbols.back();
    symbols.pop_back();
    node->right_ = symbols.back();
    symbols.pop_back();
    node->frequency_ = node->left_->frequency_ + node->right_->frequency_;
    auto it = symbols.rbegin();
    while (it != symbols.rend()) {
      if ((*it)->frequency_ > node->frequency_) break;
      it++;
    }
    symbols.insert(it.base(), node);
  }
  Code const* root = symbols.back();
  symbols.pop_back();

  // traverse the tree and cache all of the prefix codes
  std::unordered_map<char, std::string> prefix;
  std::string code = "";
  
  printTree(root, 2);

  std::stack<Code const*> children;
  std::stack<std::string> path;
  Code const* node = root;
  while (!children.empty() || node) {
    if (node) {
      children.push(node);
      path.push(code);
      node = node->left_;
      code += '0';
    } else {
      node = children.top();
      children.pop();
      if (node->left_ == NULL && node->right_ == NULL) {
        prefix[node->symbol_] = code.substr(0, code.length() - 1);
        std::cout << node->symbol_ << ' ' << code.substr(0, code.length() - 1) 
          << '\n';
      }
      code = path.top();
      path.pop();
      node = node->right_;
      code += '1';
    }
  }

  // use the prefix table to encode the original string
  std::string cipherText = "";
  for (size_t i = 0; i < plainText.length(); ++i) {
    cipherText += prefix[plainText[i]];
  }

  std::cout << "Encoded:\n" << cipherText << '\n';

  // walk the tree while iterating the huffman encoded string
  plainText = "";
  node = root;
  for (size_t i = 0; i < cipherText.length(); ++i) {
    if (node->left_ == NULL && node->right_ == NULL) {
      plainText += node->symbol_;
      node = root;
    }
    if (cipherText[i] == '0') {
      node = node->left_;
    } else {
      node = node->right_;
    }
  }
  // show the decoded string
  std::cout << "Decoded:\n" << plainText << '\n';
  std::cout << "Compression: %" << (cipherText.length() / 
    (double)((plainText.length() + 1) * 8)) * 100 << '\n';
  // give the user a chance to read it and then return control to the OS
  std::cout << "Press ENTER to continue." << std::endl;
  std::cin.sync();
  std::cin.ignore();
}
