#include "iostream"
#include "string"
#include "lua.hpp"
#include "map"
#include "windows.h"

using namespace std;

//red表示包含
enum Color { Red, Black };

struct Node
{
	string str;
	Color color;
	map<string, Node*> next;

	Node(string str, Color color) {
		this->str = str;
		this->color = color;
	}
};

class TrieNode
{
public:
	Node* root;

	TrieNode() {
		root = new Node("", Color::Black);
	}

	void insert(string str)
	{
		Node* curroot = root;
		for (int i = 0; i < str.length(); i++)
		{
			string s;
			bool b = false;
			if (~(str.at(i) >> 8) == 0)
			{
				s = str.substr(i, 2);
				b = true;
			}
			else {
				s = str.substr(i, 1);
			}
			Node* findnode = _find(curroot, s);
			if (findnode == NULL)
			{
				for (int j = i; j < str.length(); j++)
				{
					if (~(str.at(j) >> 8) == 0)
					{
						s = str.substr(j, 2);
						j++;
					}
					else {
						s = str.substr(j, 1);
					}
					curroot->str = s;
					Node* node = new Node(s, Color::Black);
					curroot->next[s] = node;
					curroot = node;
				}
				curroot->color = Color::Red;
				break;
			}
			else
			{
				if (findnode->next.size() == 0)
				{
					findnode->color = Color::Red;
				}
				curroot = findnode;
			}
			if (b)
			{
				i++;
			}
		}
	}
	Node* remove(Node* node);
	string match(Node* root, string str) 
	{
		string s, newstr = "";
		int i = 0, j = 0;
		while (i < str.length())
		{
			j = i;
			Node* curroot = root;
			if (~(str.at(i) >> 8) == 0)
			{
				s = str.substr(i, 2);
				j += 2;
			}
			else {
				s = str.substr(i, 1);
				j += 1;
			}
			Node* findnode = _find(curroot, s);
			if (findnode)
			{
				curroot = findnode;
				int maxend = j;
				for (int k = j; k < str.length(); )
				{
					string curs;
					if (~(str.at(k) >> 8) == 0)
					{
						curs = str.substr(k, 2);
						k += 2;
					}
					else {
						curs = str.substr(k, 1);
						k += 1;
					}
					Node* findnode = _find(curroot, curs);
					if (findnode)
					{
						if (findnode->color == Color::Red)
						{
							maxend = max(maxend, k);
							curroot = findnode;
						}
						else {
							curroot = findnode;
						}
					}
					else {
						break;
					}
				}
				newstr += "**";
				i = maxend;
			}
			else {
				i = j;
				newstr += s;
			}
		}
		return newstr;
	}
	void preTree(Node* node)
	{
		if (node != NULL)
		{
			map<string, Node*>::iterator it;
			map<string, Node*>::iterator end;
			if (node->next.size() != 0)
			{
				cout << endl;
			}
			it = node->next.begin();
			end = node->next.end();
			while (it != end)
			{
				cout << " " << it->first;
				it++;
			}	
			it = node->next.begin();
			end = node->next.end();
			while (it != end)
			{
				preTree(it->second);
				it++;
			}
		}
	}
	string print(Node* node)
	{
		string str = "";
		if (node != NULL)
		{
			map<string, Node*>::iterator it;
			map<string, Node*>::iterator end;
			it = node->next.begin();
			end = node->next.end();
			while (it != end)
			{
				str += it->first;
				it++;
			}
			it = node->next.begin();
			end = node->next.end();
			while (it != end)
			{
				str += print(it->second);
				it++;
			}
		}
		return str;
	}
private:
	Node* _find(Node* node, string c) {
		return (node->next.find(c) == node->next.end()) ? NULL : node->next[c];
	}
};


int main() {
	TrieNode* tree = new TrieNode();
	tree->insert("习近平");
	tree->insert("平近习");
	tree->insert("xjp");
	tree->insert("习太子");
	tree->insert("习明泽");
	tree->insert("老习");
	tree->insert("温家宝");
	tree->insert("温x");
	tree->insert("温jia宝");
	tree->insert("温宝宝");
	tree->preTree(tree->root);
	cout << endl;
	string s = "ss习近平按暴奸时暴奸发放";
	s = tree->match(tree->root, s);
	cout << s << endl;
	s = tree->print(tree->root);
	cout<<s<<endl;
	return 0;
}