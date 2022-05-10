
#ifndef _GRAPH_HPP_ 
#define _GRAPH_HPP_

#include <list>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include<bits/stdc++.h>


#include "NegativeGraphCycle.hpp"

using namespace std;

template<typename T>
struct ListT{
	T info;
	list< pair<T,int> > mylist;
	T prev;
	int dist;

	ListT(T f): info(f) {}
};

template<typename T>
struct Edge {
	T from;
	T to;
	int dist;
	Edge(T f, T t, int d): from(f), to(t), dist(d) {
	}
	bool operator<(const Edge<T>& e) const;
	bool operator>(const Edge<T>& e) const;
	template<typename U>
	friend ostream& operator<<(ostream& out, const Edge<U>& e);
};

template<typename T>
ostream& operator<<(ostream& out, const Edge<T>& e) {
	out << e.from << " -- " << e.to << " (" << e.dist << ")";
	return out;
}

template <typename T>
class Graph {
	bool isDirected;
	vector< ListT<T> > adjList;
public:
	Graph(bool isDirectedGraph): isDirected(isDirectedGraph) {}
	
	bool contains(const T& info){
		if(adjList.empty()){
			return false;
		}
		
		for(auto it = adjList.begin(); it != adjList.end(); it++) {
			if((*it).info == info){
				return true;
			}
		}
	
		return false;
	}

	bool addVtx(const T& info){
		if(contains(info)){
			return false;
		}
		adjList.emplace(adjList.end(), info);
	
		return true;
	}

	bool rmvVtx(const T& info) {
		if(!contains(info)){
			return false;
		}
		for(auto it = adjList.begin(); it != adjList.end(); it++) {
			if(!((*it).info == info)){
				for(auto itList = (*it).mylist.begin(); itList != (*it).mylist.end(); itList++) {
					if((*itList).first == info){
						itList = (*it).mylist.erase(itList);
					}
				}
			}
			else{
				it = adjList.erase(it);
				if(it == adjList.end()){break;}
			}
		}
		return true;
	}

	bool addEdg(const T& from, const T& to, int distance) {
		if(!contains(from) || !contains(to)){
			return false;
		}
		
		for(auto it = adjList.begin(); it != adjList.end(); it++) {
			if((*it).info == from){
				for(auto i = (*it).mylist.begin(); i != (*it).mylist.end(); i++){
					if((*i).first == to){
						return false;
					}
				}
				
				
				if((*it).mylist.empty()){
					(*it).mylist.emplace((*it).mylist.begin(), to, distance);
				}
				else{
					list< pair<T,bool> > prev;
					
					for(auto i = (*it).mylist.begin(); i != (*it).mylist.end(); i++){
						prev.emplace(prev.end(), (*i).first, false);
					}
					auto i = adjList.begin();
					auto j = prev.begin();
					while(!((*i).info == to)){
						if(j!=prev.end() && (*i).info == (*j).first){
							(*j).second = true;
							j++;
						}
						i++;
					}
					j = prev.begin();
					while((*j).second){
						j++;
						if(j == prev.end()){
							(*it).mylist.emplace((*it).mylist.end(), to, distance);
							break;
						}
					}
					for(auto i = (*it).mylist.begin(); i != (*it).mylist.end(); i++){
						if(j!=prev.end() && (*i).first == (*j).first){
							(*it).mylist.emplace(i, to, distance);
						}
					}
				}
			}
		}
		
		if(!isDirected){
			for(auto it = adjList.begin(); it != adjList.end(); it++) {
				if((*it).info == to){
					for(auto i = (*it).mylist.begin(); i != (*it).mylist.end(); i++){
						if((*i).first == from){
							return false;
						}
					}
					
					if((*it).mylist.empty()){
						(*it).mylist.emplace((*it).mylist.begin(), from, distance);
					}
					else{
						list< pair<T,bool> > prev;
						
						for(auto i = (*it).mylist.begin(); i != (*it).mylist.end(); i++){
							prev.emplace(prev.end(), (*i).first, false);
						}
						auto i = adjList.begin();
						auto j = prev.begin();
						while(!((*i).info == from)){
							if(j!=prev.end() && (*i).info == (*j).first){
								(*j).second = true;
								j++;
							}
							i++;
						}
						j = prev.begin();
						while((*j).second){
							j++;
							if(j == prev.end()){
								(*it).mylist.emplace((*it).mylist.end(), from, distance);
								break;
							}
						}
						for(auto i = (*it).mylist.begin(); i != (*it).mylist.end(); i++){
							if(j!=prev.end() && (*i).first == (*j).first){
								(*it).mylist.emplace(i, from, distance);
							}
						}
					}
				
				}
			}
		}
		return true;
	}

	bool rmvEdg(const T& from, const T& to) {
		if(!contains(from) || !contains(to)){
			return false;
		}
		
		for(auto it = adjList.begin(); it != adjList.end(); it++) {
			if((*it).info == from){
				for(auto itList = (*it).mylist.begin(); itList != (*it).mylist.end(); itList++) {
					if((*itList).first == to){
						itList = (*it).mylist.erase(itList);
						//if(isDirected){ return true; }
					}
				}
			}
		}
		
		if(!isDirected){		
			for(auto it = adjList.begin(); it != adjList.end(); it++) {
				if((*it).info == to){
					for(auto itList = (*it).mylist.begin(); itList != (*it).mylist.end(); itList++) {
						if((*itList).first == from){
							itList = (*it).mylist.erase(itList);
							//return true;
						}
					}
				}
			}
		}
		return true;
	}
	
	list<T> dfs(const T& info) const{		
		list< pair<T,bool> > visited;
		list<T> listTReturn;
		for(auto it = adjList.begin(); it != adjList.end(); it++) {
			visited.emplace(visited.end(), (*it).info, false);
		}
		
		auto it = visited.begin();
		for(it = visited.begin(); it != visited.end(); it++) {
			if((*it).first == info){
				break;
			}
		}
		while(!(it == visited.end())){
			if (!(*it).second){
				stack<T> stackT;
				stackT.push((*it).first);
				
				while (!stackT.empty()) {
					T node = stackT.top();
					stackT.pop();
					
					for(auto it = visited.begin(); it != visited.end(); it++){
						if (!(*it).second && (*it).first == node) { 
							listTReturn.emplace(listTReturn.end(), (*it).first);
							(*it).second = true;
						}
					}
					auto itVisited = visited.begin();
					for (auto itAdj = adjList.begin(); itAdj != adjList.end(); itAdj++){
						if( node == (*itAdj).info){
							auto itList = (*itAdj).mylist.end();
							while (itList != (*itAdj).mylist.begin()){
								 --itList;
								while(!((*itVisited).first == (*itList).first)){
									itVisited++;
								}
								if(!(*itVisited).second){
									stackT.push((*itVisited).first);
								}

							}
						}
					}
				}
			}
			it++;
		}
		
		return listTReturn;
	}
	
	/*void PrintStack(stack<T> s) const{ 
		// If stack is empty then return 
		if (s.empty())  
			return; 
	  
		T x = s.top(); 
	  
		// Pop the top element of the stack 
		s.pop(); 
	  
		// Recursively call the function PrintStack 
		PrintStack(s); 
	  
		// Print the stack element starting 
		// from the bottom 
		cout << x << " "; 
	  
		// Push the same element onto the stack 
		// to preserve the order 
		s.push(x); 
	}*/
	
	list<T> bfs(const T& info) const{
		list< pair<T,bool> > visited;
		list<T> listTReturn;
		for(auto it = adjList.begin(); it != adjList.end(); it++) {
			visited.emplace(visited.end(), (*it).info, false);
		}
		
		auto it = visited.begin();
		for(it = visited.begin(); it != visited.end(); it++) {
			if((*it).first == info){
				break;
			}
		}
		
		while(!(it == visited.end())){
			if(!(*it).second){
				stack<T> stackT;
				stackT.push((*it).first);
				
				while (!stackT.empty()) {
					T node = stackT.top();
					stackT.pop();
					
					for(auto it = visited.begin(); it != visited.end(); it++){
						if (!(*it).second && (*it).first == node) { 
							listTReturn.emplace(listTReturn.end(), (*it).first);
							(*it).second = true;
						}
					}
					auto itVisited = visited.begin();
					for (auto itAdj = adjList.begin(); itAdj != adjList.end(); itAdj++){
						if( node == (*itAdj).info){
							list<T> nodesInStack;
							while (!stackT.empty()) {
								nodesInStack.emplace(nodesInStack.begin(), stackT.top());
								stackT.pop();
							}
							auto itList = (*itAdj).mylist.end();
							while (itList != (*itAdj).mylist.begin()){
								 --itList;
								while(!((*itVisited).first == (*itList).first)){
									itVisited++;
								}
								if(!(*itVisited).second){
									stackT.push((*itVisited).first);
								}
							}
							
							while (!nodesInStack.empty()) {
								stackT.push(*nodesInStack.begin());
								nodesInStack.erase(nodesInStack.begin());
							}
						}
					}
				}
			}
			it++;
		}
		
		return listTReturn;
	}
	
	list<Edge<T>> mst(){
		list<Edge<T>> edge;
		list<Edge<T>> mst;
		list< T > mstSet;
		if(isDirected){
			return mst;
		}
		
		/*for(auto it = adjList.begin(); it != adjList.end(); it++) {
			cout << (*it).info << ": ";
			for(auto itList = (*it).mylist.begin(); itList != (*it).mylist.end(); itList++){
				cout << (*itList).first << "||" << (*itList).second << "-> ";
			}
			cout << endl;
		}*/
		
		for(auto it = adjList.begin(); it != adjList.end(); it++) {
			for(auto itList = (*it).mylist.begin(); itList != (*it).mylist.end(); itList++){
				if(edge.empty()){
					edge.emplace(edge.begin(), (*it).info, (*itList).first, (*itList).second);
				}
				else{
					auto itMst = edge.begin();
					bool skip = false;
					while(itMst != edge.end() && (*itList).second >= (*itMst).dist ){
						if((*itMst).from == (*itList).first && (*itMst).to == (*it).info){// && (*itMst).from == (*itList).first &&){
							skip = true;
							break;
						}
						itMst++;
					}
					if(!skip)
						edge.emplace(itMst, (*it).info, (*itList).first, (*itList).second);
				}
			}
		}
		/*cout << endl;
		
		for(auto it = edge.begin(); it != edge.end(); it++) {
			cout << (*it).from << " --- " << (*it).to << endl;
		}*/
		
		auto itMst = edge.begin();
		while(mst.size() < adjList.size()-1 && itMst != edge.end()){
			if(mst.empty()){
				mst.emplace(mst.end(), (*itMst).from, (*itMst).to, (*itMst).dist);
				mstSet.emplace(mstSet.end(), (*itMst).from);
				mstSet.emplace(mstSet.end(), (*itMst).to);
			}
			else{
				if(find(begin(mstSet), end(mstSet), (*itMst).from) != end(mstSet) && 
					find(begin(mstSet), end(mstSet), (*itMst).to) != end(mstSet)){}
				else{
					mst.emplace(mst.end(), (*itMst).from, (*itMst).to, (*itMst).dist);
					if(find(begin(mstSet), end(mstSet), (*itMst).from) != end(mstSet)){
					}
					else{
						mstSet.emplace(mstSet.end(), (*itMst).from);
					}
					
					if(find(begin(mstSet), end(mstSet), (*itMst).to) != end(mstSet)){
					}
					else{
						mstSet.emplace(mstSet.end(), (*itMst).to);
					}
				}
			}
			itMst++;
			cout << endl;
			for(auto i = mst.begin(); i != mst.end(); i++) {
				cout << (*i).from << " --- " << (*i).to << endl;
			}
			cout << endl;
		}
		
		cout << endl;
		
		return mst;
	}
	
	bool print2DotFile(const char *filename) const {
		ofstream file;
		file.open(filename);
		if (!file.is_open()) {
			return false;
		}
		
		list< pair<T,T> > check;//
		bool add = true;//
		if(isDirected){
			file << "digraph AVL{\n";
			file << "{ shape=ellipse color=black }\n";
		
		}
		else{
			file << "graph AVL{\n";
			file << "{ shape=ellipse color=black }\n";
		
		}
		for(auto it = adjList.begin(); it != adjList.end(); it++) {
			file << (*it).info;
			file << "[label="; 
			file << (*it).info;
			file << "];\n";
			for(auto itList = (*it).mylist.begin(); itList != (*it).mylist.end(); itList++){
				if(!isDirected){//
					for(auto itC = check.begin(); itC != check.end(); itC++){//
						if((*itC).first == (*itList).first && (*itC).second == (*it).info){//
							add = false;
						}
					}
				}
				if(add){//
					file << (*it).info;
					if(isDirected){
						file << " -> ";
					}
					else{
						file << " -- ";
					}
					file << "{";
					file << (*itList).first;
					file << "} ";
					file << "[label=";
					file << (*itList).second;
					file << "];\n";
					check.emplace(check.end(), (*it).info, (*itList).first);//
				}//
				add = true;//
			}
		}
		file << "}\n";
		file.close();
		
		return true;
	}

	static bool myfunction(const pair<int,T> &a, const pair<int,T> &b)  { 
		return (a.first < b.first); 
	} 

	list<T> dijkstra(const T& from, const T& to){
		list<T> unvisited;
		list<T> listTReturn;
		
		//ADJLIST print
		/*for(auto it = adjList.begin(); it != adjList.end(); it++) {
			cout << (*it).info << ": ";
			for(auto itList = (*it).mylist.begin(); itList != (*it).mylist.end(); itList++){
				cout << (*itList).first << "||" << (*itList).second << "-> ";
			}
			cout << endl;
		}*/

		auto itData = adjList.begin();	
		for(auto it = adjList.begin(); it != adjList.end(); it++) {
			unvisited.emplace(unvisited.end(), (*it).info);
			(*it).dist = INT_MAX;
			if((*it).info == from) {
				(*it).dist = 0;
				itData = it;				
			}
		}
		
		while(true){
			vector<pair<int, T>> pq;
			auto itList = (*itData).mylist.begin();
			while(itList != (*itData).mylist.end()) {
				int curDist = (*itList).second + (*itData).dist;
				
				for(auto itD = adjList.begin(); itD != adjList.end(); itD++){
					if((*itD).info == (*itList).first && (*itD).dist > curDist){
						(*itD).dist = curDist;
						(*itD).prev = (*itData).info;
					}
				}
				itList++;
			}

			for(auto it = unvisited.begin(); it != unvisited.end(); it++){
				if((*it) == (*itData).info){
					unvisited.erase(it);
					break;
				}
			}

			if(unvisited.empty())
				break;

			for(auto& it : adjList) {
				if(find(begin(unvisited), end(unvisited), it.info) != end(unvisited)) {
					pq.push_back(make_pair(it.dist, it.info));
				}
			}
			sort(pq.begin(), pq.end(), myfunction);

			for(itData = adjList.begin(); itData!=adjList.end(); itData++) {			
				if((*itData).info == (*(pq.begin())).second) {
					break;
				}
			}
			if((*itData).dist == INT_MAX)
				break;
		}

		T prev;
		do {
			for(auto j = adjList.begin(); j != adjList.end(); j++){
				if(listTReturn.empty() && (*j).info == to){
					if((*j).dist==INT_MAX){
						return listTReturn;
					}
					listTReturn.emplace(listTReturn.begin(), (*j).info);
					prev = (*j).prev;
					break;
				}
				else if(!listTReturn.empty() && (*j).info == prev){
					if((*j).dist == INT_MAX){
						listTReturn.clear();
						return listTReturn;
					}
					listTReturn.emplace(listTReturn.begin(), (*j).info);
					prev = (*j).prev;
					break;
				}
			}
		} while(*(listTReturn.begin()) != from);

		return listTReturn;
	}
	
	list<T> bellman_ford(const T& from, const T& to){
		
	}
	
};


#endif
