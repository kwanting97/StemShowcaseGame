//
//  prefixTree.h
//  FHTree
//
//  Created by Rix Lai on 3/14/17.
//  Copyright © 2017 None. All rights reserved.
//

#ifndef prefixTree_h
#define prefixTree_h

#include <vector>
#include <iostream>

class Node {
   friend class Trie;
private:
   int count;
   std::vector<Node*> children;
public:
   Node() {
      count = 0;
      children = std::vector<Node*>(26);
   }
};

class Processor {
public:
   void operator() (char c) {
      std::cout << c;
   }
};

class Trie {
private:
   Node* head;
   int size;
   void subTraverse(Processor& proc, Node* n);
   void subDestruct(Node* n);
   int convertChar(char c) const;
   char convertFromInt(int i);
   bool isValidChar(int c) const;
public:
   class IllegalCharacterException {};
   Trie() {
      head = new Node();
      size = 0;
   };
   ~Trie();
   void insert(std::string word);
   int find(std::string word) const;
   void traverse(Processor& proc);
};

#endif /* prefixTree_h */
