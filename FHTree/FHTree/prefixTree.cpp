//
//  prefixTree.cpp
//  FHTree
//
//  Created by Rix Lai on 3/14/17.
//  Copyright © 2017 None. All rights reserved.
//

#include "prefixTree.h"

bool Trie::isValidChar(int c) const {
   return (c >= 0 && c <= 25);
}

int Trie::convertChar(char c) const {
   if (c >= 65 && c <= 90)
      c += 32; // Convert to lowercase
   c -= 97; // Shift ascii mapping

   if(!isValidChar(c))
      throw new IllegalCharacterException();

   return int(c);
}

char Trie::convertFromInt(int i) {
   if(!isValidChar(i))
      throw new IllegalCharacterException();

   return (i + 97);
}

void Trie::insert(std::string word) {
   if(find(word) > 0)
      return;
   Node* current = head;
   for(int i = 0; i < word.length(); i++) {
      int c = convertChar(word[i]);
      Node* n = current->children[c];
      if(n != NULL) {
         current = n;
      } else {
         Node* newNode = new Node();
         current->children[c] = newNode;
         current = newNode;
      }
   }

   current->count++; // increment count for last char
   size++;
}

int Trie::find(std::string word) const{
   Node* current = head;
   for(int i = 0; i < word.length(); i++) {
      char c = convertChar(word[i]);
      Node* n = current->children[c];
      if(n != NULL) {
         current = n;
      } else {
         return 0;
      }
   }
   // Zero count means there is no such word
   return current->count;
}


void Trie::traverse(Processor& proc) {
   subTraverse(proc, head);
}

void Trie::subTraverse(Processor& proc, Node* n) {
   if(n == NULL)
      return;

   std::vector<Node*>::iterator iter = n->children.begin();
   int i = 0;
   for(; iter != n->children.end(); iter++) {
      if(*iter != NULL) {
         proc(convertFromInt(i));
         subTraverse(proc, *iter);
      }
      i++;
   }
}

Trie::~Trie() {
   subDestruct(head);
}

void Trie::subDestruct(Node* n) {
   if(n == NULL)
      return;

   std::vector<Node*>::iterator iter = n->children.begin();
   for(; iter != n->children.end(); iter++) {
      if(*iter != NULL) {
         subDestruct(*iter);
         delete *iter;
         *iter = NULL;
      }
   }
}


