#include<cstdio> 
#include<cstring> 

#include "AVL.hpp"

//using namespace std;

AVL::Node::Node(const string& str, Node* parent, Node* left, Node* right){
	element = str;
	parent = parent;
	left = left;
	right = right;
}

AVL::Node* AVL::Node::getParent() const{ return parent; }
AVL::Node* AVL::Node::getLeft() const{ return left; }
AVL::Node* AVL::Node::getRight() const{ return right; }
string AVL::Node::getElement() const{ return element; }
int AVL::Node::getHeight() const{ return height; }

void AVL::Node::setLeft(Node* newNode){ left = newNode; }
void AVL::Node::setRight(Node* newNode){ right = newNode; }
void AVL::Node::setParent(Node* newNode){ parent = newNode; }
void AVL::Node::setElement(string e){ element.assign(e); }

bool AVL::Node::isLeft() const{
	if(getParent() != NULL){
		if((getParent()->getElement()).compare(getElement()) > 0){
			return true;
		}
	}
	return false;
}
bool AVL::Node::isRight() const{
	if(getParent()!= NULL){
		if((getParent()->getElement()).compare(getElement()) < 0){
			return true;
		}
	}
	return false;
}
int AVL::Node::rightChildHeight() const{ 
	if(getRight() == NULL){
		return 0;
	}
	return getRight()->getHeight(); 
}

int AVL::Node::leftChildHeight() const{ 
	if(getLeft() == NULL){
		return 0;
	}
	return getLeft()->getHeight(); 
}

int AVL::Node::updateHeight(){
	int maxHeight;
	if(rightChildHeight() >= leftChildHeight()){
		maxHeight = rightChildHeight();
	}
	else{
		maxHeight = leftChildHeight();
	}
	maxHeight += 1;
	height = maxHeight;
	return maxHeight; 
}

bool AVL::Node::isBalanced(){
	int balance = leftChildHeight() - rightChildHeight();
	if(balance > 1 || balance < -1){
		return false;
	}
	return true;
}

AVL::AVL() : size(0), root(NULL), count(0){}

AVL::AVL(const AVL& avl){
	this->size = 0; 
	this->root = NULL;
	string str, data;
	int pos;
	avl.getPreorder(avl.root, str);
	while(!str.empty()){
		pos = str.find(" ");
		data = str.substr(0, pos);
		this->add(data);
		str = str.substr(pos + 1);
	}
}

bool AVL::contains(string str){
	if(root == NULL){
		return false;
	}
	Node* head = root;
	
	while(head != NULL){
		if(head->getElement().compare(str) < 0){//right
			head = head->getRight();
		}
		else if(head->getElement().compare(str) > 0){//left
			head = head->getLeft();
		}
		else{
			return true;
		}
	}
	return false;
}

AVL::~AVL(){
	while(this->root != NULL){
		this->rmv(this->root->getElement());
	}
}

bool AVL::add(string str){
	if(contains(str)){
		return false;
	}
	
	size++;
	Node* head = root;
	Node* newNode;
	
	if(head == NULL){
		head = new Node(str, NULL, NULL, NULL); 
		head->setParent(NULL);
		head->setRight(NULL);
		head->setLeft(NULL);
		head->updateHeight();
		root = head;
	}
	else{
		while(true){
			if((head->getElement()).compare(str) > 0){
				if(head->getLeft() == NULL){
					newNode = new Node(str, head, NULL, NULL);
					newNode->setParent(head);
					newNode->setRight(NULL);
					newNode->setLeft(NULL);
					newNode->updateHeight();
					head->setLeft(newNode);
					head = newNode;
					break;
				}
				else{
					head = head->getLeft();
				}
			}
			else{
				if(head->getRight() == NULL){
					newNode = new Node(str, head, NULL, NULL);
					newNode->setParent(head);
					newNode->setRight(NULL);
					newNode->setLeft(NULL);
					newNode->updateHeight();
					head->setRight(newNode);
					head = newNode;
					break; 
				}
				else{
					head = head->getRight();
				}
			}
		}
		
		while(head !=NULL){
			head->updateHeight();
			if(!head->isBalanced()){
				int balance = head->leftChildHeight() - head->rightChildHeight();
				
				if(balance > MAX_HEIGHT_DIFF && str.compare(head->getLeft()->getElement()) < 0){
					//rightRotate
					Node *oldHeadParent = head->getParent();
					Node *newHead = head->getLeft();
					Node *T2 = newHead->getRight();
					int pos = 0;
					if(head->isRight()){
						pos = 1;
					}
					newHead->setRight(head);  
					newHead->setParent(oldHeadParent);
					head->setParent(newHead);
					head->setLeft(T2);  
					if(T2!=NULL){
						T2->setParent(head);
					}
					
					if(oldHeadParent == NULL){
						root = newHead;
					}
					else{
						if(pos==1){
							oldHeadParent->setRight(newHead);
						}
						else{
							oldHeadParent->setLeft(newHead);
						}
						
					}
					head->updateHeight();  
					newHead->updateHeight();
				}
				else if(balance < -MAX_HEIGHT_DIFF && str.compare(head->getRight()->getElement()) > 0){
					//leftRotate
					Node *oldHeadParent = head->getParent();
					Node *newHead = head->getRight();
					Node *T2 = newHead->getLeft();
					int pos = 0;
					if(head->isRight()){
						pos = 1;
					}
					
					newHead->setLeft(head);
					newHead->setParent(oldHeadParent);
					head->setParent(newHead);
					head->setRight(T2);
					if(T2!=NULL){
						T2->setParent(head);
					}
					
					if(oldHeadParent == NULL){
						root = newHead;
					}
					else{
						if(pos==1){
							oldHeadParent->setRight(newHead);
						}
						else{
							oldHeadParent->setLeft(newHead);
						}
					}
					
					head->updateHeight();  
					newHead->updateHeight();
				}
				else if(balance > MAX_HEIGHT_DIFF && str.compare(head->getLeft()->getElement()) > 0 ){
					////leftRolate head->left
					Node *oldHeadParent = head->getParent();
					Node *newHead = head->getLeft()->getRight();
					Node *T2 = newHead->getLeft();
					int pos = 0;
					if(head->isRight()){
						pos = 1;
					}
					newHead->setLeft(head->getLeft());
					head->getLeft()->setRight(T2);
					
					if(T2!=NULL){
						T2->setParent(head->getLeft());
					}
					
					head->getLeft()->setParent(newHead);
					newHead->setParent(head);
					
					head->getLeft()->updateHeight();  
					newHead->updateHeight();
					
					head->setLeft(newHead);
					
					//rightRotate head
					newHead = head->getLeft();
					T2 = newHead->getRight();
					
					newHead->setRight(head);  
					head->setLeft(T2);  
					if(T2!=NULL){
						T2->setParent(head);
					}
					
					head->setParent(newHead);
					newHead->setParent(oldHeadParent);
					
					if(oldHeadParent == NULL){
						root = newHead;
					}
					else{
						if(pos==1){
							oldHeadParent->setRight(newHead);
						}
						else{
							oldHeadParent->setLeft(newHead);
						}
					}
					
					head->updateHeight();
					newHead->updateHeight();  
				}
				else if(balance < -MAX_HEIGHT_DIFF && str.compare(head->getRight()->getElement()) < 0){
					//RightRolate head->right
					Node *oldHeadParent = head->getParent();
					Node *newHead = head->getRight()->getLeft();
					Node *T2 = newHead->getRight();
					int pos = 0;
					if(head->isRight()){
						pos = 1;
					}
					
					newHead->setRight(head->getRight()); 
					head->getRight()->setLeft(T2);
					if(T2!=NULL){
						T2->setParent(head->getRight());
					}
					
					head->getRight()->setParent(newHead);
					newHead->setParent(head);
					
					head->getRight()->updateHeight();  
					newHead->updateHeight();
					
					head->setRight(newHead);
					//leftRotate
					newHead = head->getRight();
					T2 = newHead->getLeft();
					
					newHead->setLeft(head);
					head->setRight(T2);
					
					if(T2!=NULL){
						T2->setParent(head);
					}
					
					head->setParent(newHead);
					newHead->setParent(oldHeadParent);
					
					if(oldHeadParent == NULL){
						root = newHead;
					}
					else{
						if(pos==1){
							oldHeadParent->setRight(newHead);
						}
						else{
							oldHeadParent->setLeft(newHead);
						}
					}
					
					head->updateHeight();  
					newHead->updateHeight();
				}
			}
			else{
				head = head->getParent();
			}
		}
	}
	
	return true;
}

bool AVL::rmv(string str){
	if(!contains(str)){
		return false;
	}
	Node* head = root;
	
	while(head->getElement() != str){
		if(head->getElement()> str){
			head = head->getLeft();
		}
		else{
			head = head->getRight();
		}
	}
	
	Node* left = head->getLeft();
	Node* right = head->getRight();
	Node* parent = head->getParent();
	Node* temp;
	//Antikatastash diagrafomenou komvou kai diagrafh
	if(left==NULL && right==NULL){
		if(head->isLeft()){
			parent->setLeft(NULL);
		}
		else if(head->isRight()){
			parent->setRight(NULL);
		}
		else{
			root = NULL;
		}
		
		if(parent!=NULL){
			parent->updateHeight();
			temp = parent;
		}
		temp = parent;
	}
	else if(left == NULL){
		right->setParent(parent);
		if(head->isLeft()){
			parent->setLeft(right);
		}
		else if(head->isRight()){
			parent->setRight(right);
		}
		else{
			root = right;
		}
		if(parent!=NULL){
			parent->updateHeight();
			temp = parent;
		}
		else{
			temp = right;
		}
	}
	else if(right == NULL){
		left->setParent(parent);
		if(head->isLeft()){
			parent->setLeft(left);
		}
		else if(head->isRight()){
			parent->setRight(left);
		}
		else{
			root = left;
		}
		
		if(parent!=NULL){
			parent->updateHeight();
			temp = parent;
		}
		else{
			temp = left;
		}
	}
	else{
		Node* succ = right;
		if(right->getLeft() != NULL){
			Node* succParent;
			
			while(succ->getLeft() != NULL){
				succ = succ->getLeft();
			}
			
			succParent = succ->getParent();
			succParent->setLeft(succ->getRight());
			if(succ->getRight() != NULL){
				succ->getRight()->setParent(succParent);
			}
			succ->setParent(parent);
			
			succ->setRight(right);
			succ->setLeft(left);
			left->setParent(succ);
			right->setParent(succ);
			right->updateHeight();
			
			while(succParent != succ){
				succParent->updateHeight();
				succParent = succParent->getParent();
			}
			
		}
		else{
			succ->setParent(parent);
			if(left != NULL){
				succ->setLeft(left);
				left->setParent(succ);
			}
		}
		
		if(head->isLeft()){
			parent->setLeft(succ);
		}
		else if(head->isRight()){
			parent->setRight(succ);
		}
		else{
			root = succ;
		}
		succ->updateHeight();
		temp = succ;
	}
	
	delete head;
	head = temp;
	
	//Elegxos isorropias
	while(head !=NULL){
		head->updateHeight();
		if(!head->isBalanced()){
			int balance = head->leftChildHeight() - head->rightChildHeight();
			
			if(balance > MAX_HEIGHT_DIFF && head->getLeft()->leftChildHeight() >= head->getLeft()->rightChildHeight()){
				//rightRotate
				Node *oldHeadParent = head->getParent();
				Node *newHead = head->getLeft();
				Node *T2 = newHead->getRight();
				int pos = 0;
				if(head->isRight()){
					pos = 1;
				}
				newHead->setRight(head);  
				newHead->setParent(oldHeadParent);
				head->setParent(newHead);
				head->setLeft(T2);  
				if(T2!=NULL){
					T2->setParent(head);
				}
				
				if(oldHeadParent == NULL){
					root = newHead;
				}
				else{
					if(pos==1){
						oldHeadParent->setRight(newHead);
					}
					else{
						oldHeadParent->setLeft(newHead);
					}
					
				}
				head->updateHeight();
				newHead->updateHeight();
			}
			else if(balance < -MAX_HEIGHT_DIFF && head->getRight()->leftChildHeight() <= head->getRight()->rightChildHeight()){
				//leftRotate
				Node *oldHeadParent = head->getParent();
				Node *newHead = head->getRight();
				Node *T2 = newHead->getLeft();
				int pos = 0;
				if(head->isRight()){
					pos = 1;
				}
				
				newHead->setLeft(head);
				newHead->setParent(oldHeadParent);
				head->setParent(newHead);
				head->setRight(T2);
				if(T2!=NULL){
					T2->setParent(head);
				}
				
				if(oldHeadParent == NULL){
					root = newHead;
				}
				else{
					if(pos==1){
						oldHeadParent->setRight(newHead);
					}
					else{
						oldHeadParent->setLeft(newHead);
					}
				}
				
				head->updateHeight();  
				newHead->updateHeight();
			}
			else if(balance > MAX_HEIGHT_DIFF && head->getLeft()->leftChildHeight() < head->getLeft()->rightChildHeight()){
				////leftRolate head->left
				Node *oldHeadParent = head->getParent();
				Node *newHead = head->getLeft()->getRight();
				Node *T2 = newHead->getLeft();
				int pos = 0;
				if(head->isRight()){
					pos = 1;
				}
				newHead->setLeft(head->getLeft());
				head->getLeft()->setRight(T2);
				
				if(T2!=NULL){
					T2->setParent(head->getLeft());
				}
				
				head->getLeft()->setParent(newHead);
				newHead->setParent(head);
				
				head->getLeft()->updateHeight();  
				newHead->updateHeight();
				
				head->setLeft(newHead);
				
				//rightRotate head
				newHead = head->getLeft();
				T2 = newHead->getRight();
				
				newHead->setRight(head);  
				head->setLeft(T2);  
				if(T2!=NULL){
					T2->setParent(head);
				}
				
				head->setParent(newHead);
				newHead->setParent(oldHeadParent);
				
				if(oldHeadParent == NULL){
					root = newHead;
				}
				else{
					if(pos==1){
						oldHeadParent->setRight(newHead);
					}
					else{
						oldHeadParent->setLeft(newHead);
					}
				}
				
				head->updateHeight();
				newHead->updateHeight();  
			}
			else if(balance < -MAX_HEIGHT_DIFF && head->getRight()->leftChildHeight() > head->getRight()->rightChildHeight()){
				//RightRolate head->right
				Node *oldHeadParent = head->getParent();
				Node *newHead = head->getRight()->getLeft();
					Node *T2 = newHead->getRight();
				int pos = 0;
				if(head->isRight()){
					pos = 1;
				}
				
				newHead->setRight(head->getRight()); 
				head->getRight()->setLeft(T2);
				if(T2!=NULL){
					T2->setParent(head->getRight());
				}
				
				head->getRight()->setParent(newHead);
				newHead->setParent(head);
				
				head->getRight()->updateHeight();  
				newHead->updateHeight();
				
				head->setRight(newHead);
				//leftRotate
				newHead = head->getRight();
				T2 = newHead->getLeft();
				
				newHead->setLeft(head);
				head->setRight(T2);
				
				if(T2!=NULL){
					T2->setParent(head);
				}
				
				head->setParent(newHead);
				newHead->setParent(oldHeadParent);
				
				if(oldHeadParent == NULL){
					root = newHead;
				}
				else{
					if(pos==1){
						oldHeadParent->setRight(newHead);
					}
					else{
						oldHeadParent->setLeft(newHead);
					}
				}
				
				head->updateHeight();  
				newHead->updateHeight();
			}
		}
		else{
			head = head->getParent();
		}
	}
	return true;
}

void AVL::print2DotFile(char *filename)const{
	
	string dotStr= "graph AVL{\n";
	
	string name = &filename[0];
	strcat(filename, ".dot");
	dotStr +=  "{ shape=ellipse color=black }\n";
	getDotFile(root,dotStr);
	dotStr += "}";
	ofstream file;
	file.open (filename);
	file << dotStr;
	file.close();
	string str1 =  "dot " + name + ".dot -Tpng -o " + name + ".png";
	const char* cmd = &str1[0];
	system(cmd);
}

void AVL::getDotFile(Node* node, string& str)const{
	
	if (node == NULL)
		return; 
	
	if(node->getParent()!= NULL){
		str += node->getParent()->getElement() + " -- " + "{" + node->getElement() + "}\n";
	}
	else{
		str += node->getElement() + "\n";
	}
	getDotFile(node->getLeft(), str);
	getDotFile(node->getRight(), str);
}

void AVL::pre_order(std::ostream& out)const{
	string str;
	getPreorder(root, str);
	out << str;
}

void AVL::getPreorder(Node* node, string& str) const { 
	if (node == NULL)
		return; 
	str += node->getElement() + " ";
	
	getPreorder(node->getLeft(), str);
	getPreorder(node->getRight(), str); 
}

std::ostream& operator<<(std::ostream& out, const AVL& tree){ 
	tree.pre_order(out);
	string res="avl" + to_string(tree.count);
	tree.print2DotFile(&res[0]);
	int &c = const_cast <int &> (tree.count); 
	c++;
	return out;
}

AVL& AVL::operator =(const AVL& avl){
	string str, data;
	int pos;
	avl.getPreorder(avl.root, str);
	
	while(this->root != NULL){
		this->rmv(this->root->getElement());
	}
	while(!str.empty()){
		pos = str.find(" ");
		data = str.substr(0, pos);
		this->add(data);
		str = str.substr(pos + 1);
	}
	return *this;
}
AVL AVL::operator +(const AVL& avl){
	AVL avlRtrn;
	
	string str, data;
	int pos;
	this->getPreorder(this->root, str);
	avl.getPreorder(avl.root, str);
	
	while(!str.empty()){
		pos = str.find(" ");
		data = str.substr(0, pos);
		avlRtrn.add(data);
		str = str.substr(pos + 1);
	}
	
	return avlRtrn;
}
AVL& AVL::operator +=(const AVL& avl){
	string str, data;
	int pos;
	avl.getPreorder(avl.root, str);
	
	while(!str.empty()){
		pos = str.find(" ");
		data = str.substr(0, pos);
		this->add(data);
		str = str.substr(pos + 1);
	}
	
	return *this;
}
AVL& AVL::operator +=(const string& e){
	this->add(e);
	return *this;
}
AVL& AVL::operator -=(const string& e){
	this->rmv(e);
	return *this;
}
AVL AVL::operator +(const string& e){
	AVL avlRtrn;
	
	string str, data;
	int pos;
	this->getPreorder(this->root, str);
	
	while(!str.empty()){
		pos = str.find(" ");
		data = str.substr(0, pos);
		avlRtrn.add(data);
		str = str.substr(pos + 1);
	}
	
	avlRtrn.add(e);
	return avlRtrn;
}
AVL AVL::operator -(const string& e){
	AVL avlRtrn;
	
	string str, data;
	int pos;
	this->getPreorder(this->root, str);
	
	while(!str.empty()){
		pos = str.find(" ");
		data = str.substr(0, pos);
		avlRtrn.add(data);
		str = str.substr(pos + 1);
	}
	
	avlRtrn.rmv(e);
	return avlRtrn;
}

AVL::Iterator::Iterator(Node* root){
	if (root != NULL){
		stack<Node *> s; 
		stack<Node *> temp; 
		s.push(root); 
		
		while (!s.empty()) {
			Node *node = s.top(); 
			temp.push(node);
			s.pop(); 
			if (node->getRight()) 
				s.push(node->getRight()); 
			if (node->getLeft()) 
				s.push(node->getLeft()); 
		}
		while (!temp.empty()){
			nodeStack.push(temp.top());
			temp.pop();
		}
	}
}
AVL::Iterator& AVL::Iterator::operator++(){
	if(this->nodeStack.size()!=1){
		this->nodeStack.pop();
	}
	else{
		this->nodeStack.top() = this->nodeStack.top()->getRight();
	}
	
	return *this;
}
AVL::Iterator AVL::Iterator::operator++(int a){
	Iterator save = *this; 
	++*this;
	return save;
}
string AVL::Iterator::operator*(){
	return this->nodeStack.top()->getElement();
}
bool AVL::Iterator::operator!=(Iterator it){
	if(this->nodeStack.top() != it.nodeStack.top()->getRight()){
		return true;
	}
	return false;
}
bool AVL::Iterator::operator==(Iterator it){
	if(this->nodeStack.top() == it.nodeStack.top()->getRight()){
		return true;
	}
	return false;
}
AVL::Iterator AVL::begin() const{
	Iterator* it = new Iterator(this->root);
	return it->nodeStack.top();
}
AVL::Iterator AVL::end() const{
	Iterator* it = new Iterator(this->root);
	
	while(it->nodeStack.size()!=1){
		it->nodeStack.pop();
	}
	return it->nodeStack.top();
}
