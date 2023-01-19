#include "autocomplete.h"

TrieNode* getNewNode(void)
{
	struct TrieNode* pNode = new TrieNode;
	pNode->isEndOfWord = false;
	for (int i = 0; i < ALPHABET_SIZE; i++)
		pNode->children[i] = nullptr;

	return pNode;
}

void insert(TrieNode* root, std::string key)
{
	TrieNode* node = root;

	for (size_t i = 0; i < key.length(); i++)
	{
		int index = key[i] - 'a';
		if (!node->children[index])
			node->children[index] = getNewNode();

		node = node->children[index];
	}
	node->isEndOfWord = true;
}

bool search(struct TrieNode* root, std::string key)
{
	struct TrieNode* node = root;

	for (size_t i = 0; i < key.length(); i++)
	{
		int index = key[i] - 'a';
		if (!node->children[index])
			return false;

		node = node->children[index];
	}
	return (node != nullptr && node->isEndOfWord);
}

bool isEmpty(TrieNode* root)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (root->children[i])
			return false;
	return true;
}

TrieNode* remove(TrieNode* root, std::string key, int depth)
{
	if (!root)
		return nullptr;
	if (depth == key.size()) {
		if (root->isEndOfWord)
			root->isEndOfWord = false;

		if (isEmpty(root)) {
			delete (root);
			root = nullptr;
		}
		return root;
	}
	int index = key[depth] - 'a';
	root->children[index] = remove(root->children[index], key, depth + 1);                                                                                          
	if (isEmpty(root) && root->isEndOfWord == false) {
		delete (root);
		root = nullptr;
	}
	return root;
}

void hasPrefix(TrieNode* nodeCurrent, int& countEnds)
{
	if (nodeCurrent->isEndOfWord)
		countEnds++;
	for (size_t i = 0; i < ALPHABET_SIZE; i++)
	{
		if (nodeCurrent->children[i])
		{
			hasPrefix(nodeCurrent->children[i], countEnds);
		}
	}
}

void findMinPrefixes(TrieNode* root, char buf[], int ind, std::string& res)
{
	if (!root)
		return;
	int count = 0;
	hasPrefix(root, count);
	if (count == 1)
	{
		buf[ind] = '\0';
		res += std::string(buf) + " ";
		return;
	}
	for (int i = 0; i < ALPHABET_SIZE; ++i)
	{
		if (root->children[i])
		{
			char currentChar = i + 'a';
			buf[ind] = currentChar;
			findMinPrefixes(root->children[i], buf, ind + 1, res);
		}
	}
}

void wordSubstitution(TrieNode* root, std::string& result, std::string currentWord)
{
	if (!root)
		return;
	if (currentWord.empty())
		return;
	char buf[50];
	size_t indexChar = 0;
	int counterWords = 0;
	bool charPresence = true;
	while (indexChar < currentWord.length())
	{
		if (!charPresence)
			return;
		charPresence = false;
		for (int i = 0; i < ALPHABET_SIZE; ++i)
		{
			if (root->children[i])
			{
				if (currentWord[indexChar] == i + 'a')
				{
					buf[indexChar] = i + 'a';
					indexChar++;
					root = root->children[i];
					charPresence = true;
					i = ALPHABET_SIZE;
					if (root->isEndOfWord && indexChar == currentWord.length())
					{
						buf[indexChar] = '\0';
						result += std::string(buf) + " ";

						counterWords++;
					}
					else if (indexChar == currentWord.length())
					{
						buf[indexChar] = '\0';
						result += std::string(buf);
					}
				}
			}
		}
	}
	if (counterWords == 0)
		result.clear();
	charPresence = false;
	wordSubstitutionInner(root, buf, indexChar, result, counterWords, charPresence);
}

void wordSubstitutionInner(TrieNode* root, char buf[], int indexChar, std::string& result, int& counterWords, bool endWord)
{
	if (endWord)
	{
		buf[indexChar] = '\0';
		result += std::string(buf) + " ";
		counterWords++;
	}
	for (int i = 0; i < ALPHABET_SIZE; ++i)
	{
		if (counterWords >= 3)
			return;
		if (root->children[i])
		{
			buf[indexChar] = i + 'a';
			endWord = root->children[i]->isEndOfWord;
			wordSubstitutionInner(root->children[i], buf, indexChar + 1, result, counterWords, endWord);
		}
	}

}