/*
* Trie.h
* Trie.h implements the TrieNode class which is used to sort and find TrieNodes.
* We use TrieNodes to easily store commands, and have user access them without typing the full command.
* Most code taken from the Trie node already wrote back in the assignment question involving Tries and word auto-completion.
*/

#ifndef __TRIE_H__
#define __TRIE_H__

#include <string>

struct TrieNode {
	enum {NumChars = 26};
	bool isWord;
	TrieNode *ltrs[NumChars];
	
	// Constructor
	TrieNode();
	
	// Destructor
	~TrieNode();
}; 

std::string find(TrieNode*, const std::string&, std::string);
bool contains(TrieNode&);
TrieNode* insert(TrieNode*, const std::string&);
TrieNode* remove(TrieNode*, const std::string&);

#endif