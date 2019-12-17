#include<iostream>
#include<vector>
#include<string>
#include<set>
#include<map>
#include<stack>
#include<fstream>
#include<sstream>
#include<iterator>
#include<utility>
#include<cassert>
#include<algorithm>

class tree{
	protected :
		unsigned int id;
		std::vector<tree> sons;
		unsigned int fp;
		unsigned int fm;
		unsigned int current;
		unsigned int color;
		bool isLeaf;

		tree(unsigned int);
		tree(unsigned int, std::vector<tree> &);
		void coloringAndCleaning(bool, unsigned int);
		void fillStackToColor(std::stack<tree *> &, bool, unsigned int);
	public : 
		tree(std::string filename);
		tree();
		void colorMe(unsigned int currentColor=1);
		void print()const;
	};
