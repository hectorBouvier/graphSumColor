#include "tree.h"

int main(int argc, char *argv[]){
	tree t=tree(std::string(argv[1]));
	t.colorMe();
	t.print();
	return 0;
	}

tree::tree(std::string filename){
	color=0;
	fp=1;
	fm=0;
	current=0;
	std::ifstream file(filename,std::ios::in);
	if (file){
		std::map<unsigned int, std::vector<unsigned int>> map;
		unsigned int root;
		std::istringstream flux;
		std::string lineStr;
		std::getline(file,lineStr);
		std::getline(file,lineStr);
		while(lineStr[1]!='}'){
			flux.clear();
			flux.str(lineStr);
			std::vector<unsigned int> rsons;
			std::istream_iterator<unsigned int> it(flux);
			std::istream_iterator<char> it2(flux);
			std::istream_iterator<unsigned int> end;
			root=*it;
			it2++;
			it2++;
			++it;
			while(it!=end){
				rsons.push_back(*it);
				it2++;
				it++;
				}
			map.insert(std::pair<unsigned int,std::vector<unsigned int>>(root,rsons));
			std::getline(file,lineStr);
			}
		std::set<unsigned int> keys;
		for (auto it=map.begin();it!=map.end();it++){
			keys.insert(it->first);
			}
		if(map.find(1)==map.end()){
			std::cerr<<"Root is missing\n";
			}
		std::stack<unsigned int> toDo;
		toDo.push(1);
		std::map<unsigned int, tree> temp;
		while(!toDo.empty()){
			unsigned int i=toDo.top();
			if (temp.find(i)==temp.end()){
				if (keys.find(i)!=keys.end()){
					for(auto it=map[i].begin();it!=map[i].end();it++){
						if(temp.find(*it)==temp.end()){
							toDo.push(*it);
							}
						}
					if (toDo.top()==i){
						std::vector<tree> toFill;
						for(auto it=map[i].begin();it!=map[i].end();it++){
							toFill.emplace_back(temp[*it]);
							}
						assert(!toFill.empty());
						temp.insert(std::pair<unsigned int, tree>(i,tree(i,toFill)));
						assert(!temp[i].sons.empty());
						toDo.pop();
						}
					}
				else{
					temp.insert(std::pair<unsigned int,tree>(i,tree(i)));
					toDo.pop();
					}
				}
			else{
				toDo.pop();
				}
			}
		assert(!temp[1].sons.empty());
		sons=temp[1].sons;
		assert(!sons.empty());
		id=1;
		isLeaf=sons.empty();
		}
	}

tree::tree(unsigned int i){
	id=i;
	color=0;
	fp=1;
	fm=0;
	current=0;
	isLeaf=true;
	}

tree::tree(unsigned int i, std::vector<tree> &v){
	id=i;
	color=0;
	isLeaf=false;
	fp=1;
	fm=0;
	current=0;
	assert(!v.empty());
	std::copy(v.begin(),v.end(),std::back_inserter(sons));
	assert(!sons.empty());
	}

tree::tree(){
	id=0;
	color=0;
	isLeaf=false;
	fp=1;
	fm=0;
	current=0;
	}


void tree::colorMe(unsigned int currentColor){
	tree *t=this;
	std::stack<tree *> toDo;
	toDo.push(t);
	//Maixmum set cardinality
	while(!toDo.empty()){
		t=toDo.top();
		if(!t->isLeaf){
			for(auto it=t->sons.begin();it!=t->sons.end();it++){
				if (it->color==0){
					if (it->current<currentColor){
						toDo.push(&(*it));
						}
					else{
						t->fp+=it->fm;
						t->fm+=it->fm>it->fp?it->fm:it->fp;
						}
					}
				}
			if(t->id==toDo.top()->id){
				t->current=currentColor;
				toDo.pop();
				}	
			}
		else{
			t->current=currentColor;
			toDo.pop();
			}
		}
	//coloring && cleaning
	this->coloringAndCleaning(this->fp>this->fm,currentColor);
	//fill the stack
	std::stack<tree *> toColor;
	this->fillStackToColor(toColor, true, currentColor);
	//step+1
	while(!toColor.empty()){
		toColor.top()->colorMe(currentColor+1);
		toColor.pop();
		}
	}

void tree::coloringAndCleaning(bool plus, unsigned int currentColor){
	if(plus && color==0){
		color=currentColor;
		}
	if (!isLeaf){
		fp=1;
		fm=0;
		for(auto it=sons.begin();it!=sons.end();it++){
			if(it->color==0||it->color==currentColor){
				it->coloringAndCleaning((it->fp>it->fm && !plus),currentColor);
				}
			}
		}
	}
		
void tree::fillStackToColor(std::stack<tree *> &toColor, bool parentColored, unsigned int currentColor){
	if(color==0 && parentColored){
		toColor.push(this);
		}
	if(!isLeaf){
		for(auto it=sons.begin();it!=sons.end();it++){
			if(it->color==0||it->color==currentColor){
				it->fillStackToColor(toColor,color!=0,currentColor);
				}
			}
		}
	}	

void tree::print()const{
	std::cout<<"ID : "<<id<<", color : "<<color<<"\n";
	if(!isLeaf){
		for (auto it=sons.begin();it!=sons.end();it++){
			it->print();
			}
		}
	}
