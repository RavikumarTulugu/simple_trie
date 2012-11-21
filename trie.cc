
#include<boost/filesystem.hpp>
#include<string.h>
#include<stdio.h>

using namespace boost;

bool
isDisjoint(char *p1, char *p2)
{
	unsigned int slashCount2 = 0, slashCount1 = 0;
	for(char *ptr1 = p1, *ptr2 = p2;(*ptr1 && *ptr2); ptr1++, ptr2++) {
		if ((*ptr1 != *ptr2) && (slashCount1 == slashCount2)) 
			return true;
		if (*ptr1 == '/') 
			slashCount1++;
		if (*ptr2 == '/') 
			slashCount2++;
	}
	return false;
}

class Trie
{
	Trie *next[256];
	unsigned int charCount[256];
	unsigned int useCount;
	static void _print(Trie *t)
	{
		for(unsigned int i = 0 ; i < 256 ; i++) 
			if (t->next[i]){
				fprintf(stderr," %c:%d ", i, t->charCount[i]);
				_print(t->next[i]);
			}
		return;
	}

	static void _delete(Trie *root, Trie *t, const char *key)
	{
		if (t && *key){
			Trie **oldslot = &t->next[*key];
			--t->charCount[*key];
			--t->useCount;
			key++;
			Trie::_delete(root, *oldslot, key);
			if(!t->charCount[*--key]) *oldslot = NULL;
			if (!t->useCount){
				if (t != root) delete t;
			}
		}
		return;
	}

	public:
	Trie()
	{
		for(unsigned int i = 0 ; i < 256; i++){ 
			next[i] = NULL;
			charCount[i] = 0;
		}
		useCount = 0;
		return;
	}

	~Trie() 
	{ 
		//FIXME: pending to be implemented. 
		return; 
	}

	void operator+(std::string item)
	{
		Trie *pointer = this;
		for(const char *key = item.c_str(); *key; key++){
			Trie *npointer = pointer->next[*key];
			if (!npointer) pointer->next[*key] = new Trie;
			++pointer->useCount;
			++pointer->charCount[*key];
			pointer = pointer->next[*key];
		}
		return;
	}

	void operator-(std::string item)
	{
		Trie::_delete(this, this, item.c_str());
		return;
	}

	bool operator[](std::string item)
	{
		Trie *pointer = this;
		const char *key = item.c_str();
		unsigned int length = 0;
		for(const char *key = item.c_str(); *key; key++){
			pointer = pointer->next[*key];
			if (!pointer) break;
			length++;
		}
		return length == item.size();
	}

	void print()
	{
		Trie::_print(this);
		return;
	}
};

int 
main(int ac, char **av)
{
#if 0
	boost::filesystem::path prev = "/homes/akorp/group";
	boost::filesystem::path cur = "/homes/akorp/group/docs";
	int rc = prev.compare(cur);
	std::cerr<<"\ncompare returns :"<<rc;
	char *p1 = "/homes/akorp/group";
	char *p2 = "/homes/akorp/group/docs";
	char *p3 = "/homes/akorp/group/junk";
	char *p4 = "/home/akorp/group/junk";
	char *p5 = "/homes/akorp/group/";
	char *p6 = "/homes/akorp/group/";
	char *p7 = "/homes/akorp/group";
	std::cerr<<"\n"<<p1<<" "<<p2<<" "<<" "<<(isDisjoint(p1, p2) ? "True" : "False");
	std::cerr<<"\n"<<p2<<" "<<p3<<" "<<" "<<(isDisjoint(p2, p3) ? "True" : "False");
	std::cerr<<"\n"<<p3<<" "<<p4<<" "<<" "<<(isDisjoint(p3, p4) ? "True" : "False");
	std::cerr<<"\n"<<p5<<" "<<p6<<" "<<" "<<(isDisjoint(p5, p6) ? "True" : "False");
	std::cerr<<"\n"<<p6<<" "<<p7<<" "<<" "<<(isDisjoint(p6, p7) ? "True" : "False");

	Trie t; 
	t + std::string("to");
	std::cerr<<"\nAdd to ------------------";
	t.print();
	t + std::string("tea");
	std::cerr<<"\nAdd tea ------------------";
	t.print();
	t + std::string("ted");
	std::cerr<<"\nAdd ted ------------------";
	t.print();
	t + std::string("ten");
	std::cerr<<"\nAdd ten ------------------";
	t.print();
	t + std::string("in");
	std::cerr<<"\nAdd in ------------------";
	t.print();
	std::cerr<<"\nAdd inn ------------------";
	t + std::string("inn");
	t.print();
	std::cerr<<"\nfind to ------------------"<<(t[std::string("to")] ? "True" : "False");
	std::cerr<<"\nfind tea ------------------"<<(t[std::string("tea")] ? "True" : "False");
	std::cerr<<"\nfind ted ------------------"<<(t[std::string("ted")] ? "True" : "False");
	std::cerr<<"\nfind ten ------------------"<<(t[std::string("ten")] ? "True" : "False");
	std::cerr<<"\nfind in ------------------"<<(t[std::string("in")] ? "True" : "False");
	std::cerr<<"\nfind inn ------------------"<<(t[std::string("inn")] ? "True" : "False");
	std::cerr<<"\nfind jack ------------------"<<(t[std::string("jack")] ? "True" : "False");
	std::cerr<<"\n Deletion test";
	t - std::string("to");
	std::cerr<<"\nRem to ------------------";
	t.print();
	t - std::string("tea");
	std::cerr<<"\nRem tea ------------------";
	t.print();
	t - std::string("ted");
	std::cerr<<"\nRem ted ------------------";
	t.print();
	t - std::string("ten");
	std::cerr<<"\nRem ten ------------------";
	t.print();
	t - std::string("in");
	std::cerr<<"\nRem in ------------------";
	t.print();
	std::cerr<<"\nRem inn ------------------";
	t - std::string("inn");
	t.print();
	std::cerr<<"\nfind to ------------------"<<(t[std::string("to")] ? "True" : "False");
	std::cerr<<"\nfind tea ------------------"<<(t[std::string("tea")] ? "True" : "False");
	std::cerr<<"\nfind ted ------------------"<<(t[std::string("ted")] ? "True" : "False");
	std::cerr<<"\nfind ten ------------------"<<(t[std::string("ten")] ? "True" : "False");
	std::cerr<<"\nfind in ------------------"<<(t[std::string("in")] ? "True" : "False");
	std::cerr<<"\nfind inn ------------------"<<(t[std::string("inn")] ? "True" : "False");
	std::cerr<<"\nfind jack ------------------"<<(t[std::string("jack")] ? "True" : "False");
	return 0;
#endif 
}
