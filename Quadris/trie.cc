/*
* Trie.cc
* Trie.cc implements the methods in Trie.h
* Most of the implementation was already done previously in an assignment.
*/
#include <iostream>
#include <fstream>
#include <string>
#include "trie.h"

using namespace std;
const int ALPHABET = 26;

// Constructor
TrieNode::TrieNode() {
	isWord = false;
	for ( int i = 0; i < NumChars; i += 1 ) {
		ltrs[i] = NULL;
	}
}

// Destructor
TrieNode::~TrieNode() {
	for (int i = 0; i < NumChars; i++) {
		delete ltrs[i];
	}
}

// Find a word (command in this case)
string find(TrieNode* t, const string &w, string ans) {
	if (!w.length()) {
		string temp = "";
		if (t->isWord) {
			temp = ",";
			bool done = 1;
			for (int i = 0; i < ALPHABET; i++) {
				if (t->ltrs[i] != NULL && contains(*(t->ltrs[i]))) {
					temp += ans;
					done = 0;
					break;
				}
			}
			if (done) { return temp; }
		}
		for (int i = 0; i < ALPHABET; i++) {
			if (t->ltrs[i] != NULL && contains(*(t->ltrs[i]))) {
				if (temp[temp.length()-1] == ',') {
					temp += ans;
				}
				temp += (char)(i+(int)'a') + find(t->ltrs[i], w, ans + (char)(i+(int)'a'));
			}
		}
		return temp;
	} else if (t->ltrs[(int)w[0]-(int)'a'] == NULL) {
		return "~";
	} else {
		return w[0] + find(t->ltrs[(int)w[0]-(int)'a'], w.substr(1, w.length()-1), ans + w[0]);
	}
}

// boolean value to see if a command is valid
bool contains(TrieNode& t) {
	if (t.isWord) {
		return true;
	}
	for (int i = 0; i < ALPHABET; i++) {
		if (t.ltrs[i] != NULL && contains(*(t.ltrs[i]))){ return true; }
	}
	return false;
}

// insert a command
TrieNode* insert(TrieNode* t, const string &w) {
	if (!w.length()) {
		t->isWord = true;
		return t;
	}
	const string next = w.substr(1, w.length()-1);
	if (t->ltrs[(int)w[0]-(int)'a'] == NULL) {
		t->ltrs[(int)w[0]-(int)'a'] = new TrieNode;
	}
	t->ltrs[(int)w[0]-(int)'a'] =  insert(t->ltrs[(int)w[0]-(int)'a'], next);
	return t;
}

// remove a command
TrieNode* remove(TrieNode* t, const string &w) {
	if (!w.length())	{
		t->isWord = false;
		return t;
	} if (t->ltrs[(int)w[0]-(int)'a'] != NULL) {
		t->ltrs[(int)w[0]-(int)'a'] = remove(t->ltrs[(int)w[0]-(int)'a'], w.substr(1, w.length()-1));
		return t;
	} else {
		return t;
	}
}