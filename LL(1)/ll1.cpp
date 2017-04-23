#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <iomanip>
using namespace std;

multimap<string, vector<string> > sentence;
map<string, string> rsentence;
map<string, vector<string> > rightMap;
set<string> nterm;
set<string> term;
map<string, bool> toEmpty;
map<string, set<string> > First;
map<string, set<string> > Follow;
vector<vector<string> > rightSide;
map<string, map<string, set<string> > > Select;
string start;

bool isNterm(const string &s)
{
	return s[0] == '<' && s.back() == '>';
}

bool isAllNterm(const vector<string>& right)
{
	for(auto& item:right)
	{
		if(!isNterm(item))
		{
			return false;
		}
	}
	return true;
}

bool isToEmpty(const string& s)
{
	auto it = sentence.find(s);
	int cnt = sentence.count(s);

	while(cnt--)
	{
		if(it->second.size() == 1 && it->second[0] == "^")
		{
			return true;
		}
		if(isAllNterm(it->second))
		{
			bool flg = true;
			for(auto& item:it->second)
			{
				if(item == s || !isToEmpty(item))
				{
					flg = false;
					break;
				}
			}
			if(flg)
			{
				return true;
			}
		}
		++it;
	}
	return false;
}

void getFirst(const string& s)
{
	if(!First[s].empty()) return;
	auto it = sentence.find(s);
	int cnt = sentence.count(s);

	while(cnt--)
	{
		for(auto& item:it->second)
		{
			if(!isNterm(item))
			{
				First[s].insert(item);
				break;
			}
			else
			{
				if(item == s)
				{
					break;
				}
				getFirst(item);
				for(auto& f:First[item])
				{
					First[s].insert(f);
				}
				if(!toEmpty[item])
				{
					break;
				}
			}
		}
		++it;
	}
}

string getLeftSide(const vector<string>& right)
{
	string sen;
	for(auto& item:right)
	{
		sen += item + " ";
	}
	return rsentence[sen];
}

bool isLast(const vector<string>& right, const string& s)
{
	for(int i = right.size()-1; i >=0; --i)
	{
		if(s == right[i])
		{
			return true;
		}
		if(!isNterm(right[i]) || !toEmpty[right[i]])
		{
			return false;
		}
	}
	return false;
}

void getFollow(const string& s)
{
	if(!Follow[s].empty()) return;
	if(s == start) Follow[s].insert("#");

	for(auto& it:rightSide)
	{
		for(int i = 0; i < it.size()-1; ++i)
		{
			if(s == it[i])
			{
				if(!isNterm(it[i+1]))
				{
					Follow[s].insert(it[i+1]);
				}
				else
				{
					for(auto& f:First[it[i+1]])
					{
						Follow[s].insert(f);
					}
				}
			}
		}

		if(isLast(it, s))
		{
			string left = getLeftSide(it);
			getFollow(left);
			for(auto& item:Follow[left])
			{
				Follow[s].insert(item);
			}
		}
	}

	Follow[s].erase("^");
}

bool isAllToEmpty(const vector<string>& right)
{
	for(auto& item:right)
	{
		if(!toEmpty[item]) return false;
	}
	return true;
}

string getRightSide(const vector<string>& right)
{
	string sen;
	for(auto& item:right)
	{
		sen += item + " ";
	}
	return sen;
}

void getSelect(const string& s)
{
	auto it = sentence.find(s);
	int cnt = sentence.count(s);

	while(cnt--)
	{
		string sen = getRightSide(it->second);
		for(auto& item:it->second)
		{
			if(!isNterm(item))
			{
				Select[s][sen].insert(item);
				break;
			}
			else
			{
				for(auto& f:First[item])
				{
					Select[s][sen].insert(f);
				}
				if(!toEmpty[item])
				{
					break;
				}
			}
		}
		if(isAllToEmpty(it->second))
		{
			for(auto& f:Follow[s])
			{
				Select[s][sen].insert(f);
			}
		}
		Select[s][sen].erase("^");
		++it;
	}
}

void notLL1(const string& t, string err)
{
	cout << "not ll(1): " << t << ": " << err << endl;
	exit(1);
}

int main(int argc, char* argv[])
{
	if(argc != 2) return 1;

	bool hasStart = false;
	ifstream fin(argv[1]);
	string line;
	while(getline(fin, line))
	{
		istringstream is; 
		is.str(line + " |"); 

		string t, item, sen;
		is >> t;
		is >> item;

		if(!hasStart)
		{
			start = t;
			hasStart = true;
		}

		vector<string> right;
		while(is >> item)
		{
			if(item == "|")
			{
				sentence.insert(make_pair(t, right));
				rsentence.insert(make_pair(sen, t));
				rightMap.insert(make_pair(sen, right));
				nterm.insert(t);
				rightSide.push_back(right);

				sen = "";
				right.clear();
				continue;
			}
			right.push_back(item);
			sen += item + " ";
			if(!isNterm(item)) term.insert(item);
		}
	}

	for(auto& nt:nterm)
	{
		set<string> hasCommon;
		auto it = sentence.find(nt);
		int cnt = sentence.count(nt);

		while(cnt--)
		{
			if(it->second[0] == nt)
			{
				notLL1(nt, "left recursive");
			}
			if(hasCommon.find(it->second[0]) != hasCommon.end())
			{
				notLL1(nt, "has left common element");
			}
			hasCommon.insert(it->second[0]);
			++it;
		}
	}

	for(auto& nt:nterm)
	{
		if(isToEmpty(nt))
		{
			toEmpty[nt] = true;
		}
		else
		{
			toEmpty[nt] = false;
		}
	}
	toEmpty["^"] = true;

	for(auto& nt:nterm)
	{
		getFirst(nt);
	}

	for(auto& nt:nterm)
	{
		getFollow(nt);
	}

	for(auto& nt:nterm)
	{
		getSelect(nt);

		set<string> hasIntersection;
		for(auto& Set:Select[nt])
		{
			for(auto& item:Set.second)
			{
				cout << nt << "(" << item << "): " << Set.first << endl;
				if(hasIntersection.find(item) != hasIntersection.end())
				{
					notLL1(nt, "the selection set has an intersection");
				}
				hasIntersection.insert(item);
			}
		}
	}
#if 1
	cout << "-------------------------- First: --------------------------" << endl;
	for(auto& nt:nterm)
	{
		cout << nt << ":";
		for(auto& item:First[nt])
		{
			cout << "  " << item;
		}
		cout << endl;
	}

	cout << "-------------------------- Follow: --------------------------" << endl;
	for(auto& nt:nterm)
	{
		cout << nt << ":";
		for(auto& item:Follow[nt])
		{
			cout << "  " << item;
		}
		cout << endl;
	}

	cout << "-------------------------- Select: --------------------------" << endl;
	for(auto& nt:nterm)
	{
		for(auto& Set:Select[nt])
		{
			cout << nt << "->" << " " << Set.first << ":";
			for(auto& item:Set.second)
			{
				cout << "  " << item;
			}
			cout << endl;
		}
	}
#endif
}