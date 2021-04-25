#include "iostream"
#include "string"
#include "lua.hpp"
#include "map"

#ifdef linux 
const int maxspace = 3;
#else
const int maxspace = 2;
#endif

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
		root = new Node(" ", Color::Black);
	}

	void insert(string str)
	{
		Node* curroot = root;
		for (unsigned int i = 0; i < str.length(); i++)
		{
			string s;
			bool b = false;
			if (~(str.at(i) >> (maxspace * 8)) == 0)
			{
				s = str.substr(i, maxspace);
				b = true;
			}
			else {
				s = str.substr(i, 1);
			}
			Node* findnode = _find(curroot, s);
			if (findnode == NULL)
			{
				for (unsigned int j = i; j < str.length(); j++)
				{
					if (~(str.at(j) >> (maxspace * 8)) == 0)
					{
						s = str.substr(j, maxspace);
						j += maxspace - 1;
					}
					else {
						s = str.substr(j, 1);
					}
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
				i+= maxspace - 1;
			}
		}
	}
	string match(std::string str, std::string args)
	{
		std::string s, newstr = "";
		unsigned int i = 0, j = 0;
		while (i < str.length())
		{
			j = i;
			Node* curroot = root;
			if (~(str.at(i) >> (maxspace * 8)) == 0)
			{
				s = str.substr(i, maxspace);
				j += maxspace;
			}
			else {
				s = str.substr(i, 1);
				j += 1;
			}
			Node* findnode = _find(curroot, s);
			if (findnode)
			{
				curroot = findnode;
				unsigned int maxend = j;
				for (unsigned int k = j; k < str.length(); )
				{
					std::string curs;
					if (~(str.at(k) >> (maxspace * 8)) == 0)
					{
						curs = str.substr(k, maxspace);
						k += maxspace;
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
				if (maxend > j)
				{
					newstr += args;
				}
				else {
					newstr += s;
				}
				i = maxend;
			}
			else {
				i = j;
				newstr += s;
			}
		}
		return newstr;
	}
	bool matchbool(std::string str)
	{
		std::string s;
		unsigned int i = 0, j = 0;
		while (i < str.length())
		{
			j = i;
			Node* curroot = root;
			if (~(str.at(i) >> (maxspace * 8)) == 0)
			{
				s = str.substr(i, maxspace);
				j += maxspace;
			}
			else {
				s = str.substr(i, 1);
				j += 1;
			}
			Node* findnode = _find(curroot, s);
			if (findnode)
			{
				curroot = findnode;
				for (unsigned int k = j; k < str.length(); )
				{
					std::string curs;
					if (~(str.at(k) >> (maxspace * 8)) == 0)
					{
						curs = str.substr(k, maxspace);
						k += maxspace;
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
							return true;
						}
						else {
							curroot = findnode;
						}
					}
					else {
						return false;
					}
				}
				return false;
			}
			else {
				return false;
			}
		}
		return false;
	}
	void preTree(Node* node)
	{
		if (node != NULL)
		{
			map<string, Node*>::iterator it;
			map<string, Node*>::iterator end;
			if (node->next.size() != 0)
			{
				std::cout << endl;
			}
			it = node->next.begin();
			end = node->next.end();
			while (it != end)
			{
				std::cout << " " << it->first;
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
	int max(int a, int b) {
		return a > b ? a : b;
	}
};

int trie_newTrieNode(lua_State* L) {
	TrieNode **node = (TrieNode **)lua_newuserdata(L, sizeof(TrieNode *));
	*node = new TrieNode();
	luaL_getmetatable(L, "triearr");
	lua_setmetatable(L, -2);
	return 1;
}

int trie_insert(lua_State* L){
	TrieNode **node = (TrieNode **)lua_touserdata(L, 1);
	luaL_argcheck(L, node != NULL, 1, "invalid user data");

	luaL_checktype(L, -1, LUA_TSTRING);
	string param = lua_tostring(L, -1);
	(*node)->insert(param);
	return 0;
}

int trie_match(lua_State* L) {
	TrieNode **node = (TrieNode **)lua_touserdata(L, 1);
	luaL_argcheck(L, node != NULL, 1, "invalid user data");

	luaL_checktype(L, -1, LUA_TSTRING);
	string args = lua_tostring(L, -1);
	luaL_checktype(L, -2, LUA_TSTRING);
	string str = lua_tostring(L, -2);

	string s = (*node)->match(str, args);
	lua_pushstring(L, s.c_str());
	return 1;
}

int trie_matchbool(lua_State* L) {
	TrieNode** node = (TrieNode**)lua_touserdata(L, 1);
	luaL_argcheck(L, node != NULL, 1, "invalid user data");

	luaL_checktype(L, -1, LUA_TSTRING);
	string param = lua_tostring(L, -1);
	bool s = (*node)->matchbool(param);
	lua_pushboolean(L, s);
	return 1;
}

int trie_print(lua_State* L) {
	TrieNode** node = (TrieNode**)lua_touserdata(L, 1);
	luaL_argcheck(L, node != NULL, 1, "invalid user data");

	string s = (*node)->print((*node)->root);
	lua_pushstring(L, s.c_str());
	return 1;
}

const luaL_Reg lib_m[] = {
	{"create", trie_newTrieNode},
	{NULL, NULL},
};

const luaL_Reg lib_f[] = {
	{"insert", trie_insert},
	{"match", trie_match},
	{"matchbool", trie_matchbool},
	{"print", trie_print},
	{NULL, NULL},
};

extern "C" {
	int luaopen_Trie(lua_State* L)
	{
		luaL_newmetatable(L, "triearr");
		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);
		lua_settable(L, -3);
		luaL_setfuncs(L, lib_f, 0);
		luaL_newlib(L, lib_m);
		return 1;
	}}

int main()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_requiref(L, "Trie", luaopen_Trie, 1);
	luaL_dofile(L, "main05.lua");
	lua_close(L);
	return 0;
}
