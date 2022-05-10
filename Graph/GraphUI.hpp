
#ifndef _GRAPH_UI_
#define _GRAPH_UI_
#include <iostream>

template <typename T>
int graphUI() {
	
	string option, line;
	int distance;
	bool digraph = false;
	
	cin >> option;
	if(!option.compare("digraph"))
		digraph = true;
	Graph<T> g(digraph);
	
	while(true) {
		
		std::stringstream stream;
		cin >> option;

		if(!option.compare("av")) {
			getline(std::cin, line);
			stream << line;
			T vtx(stream);
			if(g.addVtx(vtx))
				cout << "av " << vtx << " OK\n";
			else
				cout << "av " << vtx << " NOK\n";
		}
		else if(!option.compare("rv")) {
			getline(std::cin, line);
			stream << line;
			T vtx(stream);
			if(g.rmvVtx(vtx))
				cout << "rv " << vtx << " OK\n";
			else
				cout << "rv " << vtx << " NOK\n";
		}
		else if(!option.compare("ae")) {
			getline(std::cin, line);
			stream << line;
			T from(stream);
			T to(stream);
			
			int cost;
			stream >> cost;

			if(g.addEdg(from, to, cost))
				cout << "ae " << from << " " << to << " OK\n";
			else
				cout << "ae " << from << " " << to << " NOK\n";
		}
		else if(!option.compare("re")) {
			getline(std::cin, line);
			stream << line;
			T from(stream);
			T to(stream);
			
			if(g.rmvEdg(from, to))
				cout << "re " << from << " " << to << " OK\n";
			else
				cout << "re " << from << " " << to << " NOK\n";
		}
		else if(!option.compare("dot")) {
			
			std::getline(std::cin, line);
			const char *filename = line.data();
			if(g.print2DotFile(filename)){
				cout << "dot " << filename << " OK\n";
			}
			else{
				cout << "dot " << filename << " NOK\n"; 
			}
		}
		else if(!option.compare("bfs")) {
			getline(std::cin, line);
			stream << line;
			T vtx(stream);
			cout << "\n----- BFS Traversal -----\n";
			list<T> out = g.bfs(vtx);
			auto it = out.begin();
			while(it != out.end()){
				cout << *it;
				it++;
				if(it != out.end()){
					cout << " -> ";
				}
			}
			
			cout << "\n-------------------------\n";
		}
		else if(!option.compare("dfs")) {
			getline(std::cin, line);
			stream << line;
			T vtx(stream);
			cout << "\n----- DFS Traversal -----\n";
			list<T> out = g.dfs(vtx);
			auto it = out.begin();
			while(it != out.end()){
				cout << *it;
				it++;
				if(it != out.end()){
					cout << " -> ";
				}
			}
			cout << "\n-------------------------\n";
		}
		else if(!option.compare("dijkstra")) {
			getline(std::cin, line);
			stream << line;
			T from(stream);
			T to(stream);
			
			cout << "Dijkstra (" << from << " - " << to <<"): ";
			
			list<T> out = g.dijkstra(from, to);
			auto it = out.begin();
			while(it != out.end()){
				cout << *it;
				it++;
				if(it != out.end()){
					cout << ", ";
				}
			}
			cout << endl;
		}
		else if(!option.compare("bellman-ford")) {
			/* getline(std::cin, line);
			stream << line;
			T from(stream);
			T to(stream);
			
			cout << "Bellman-Ford (" << from << " - " << to <<"): ";
			list<T> out = g.bellman_ford(from, to);
			auto it = out.begin();
			while(it != out.end()){
				cout << *it;
				it++;
				if(it != out.end()){
					cout << ", ";
				}
			}
			cout << endl; */
		}
		else if(!option.compare("mst")) {
			
			cout << "\n--- Min Spanning Tree ---\n";
			
			list<Edge<T>> mst = g.mst();
			auto it = mst.begin();
			distance = 0;
			while(it != mst.end()){
				cout << (*it).from << " -- " << (*it).to << " (" << (*it).dist << ")" << endl;
				distance += (*it).dist;
				it++;
			}
			cout << "MST Cost: " << distance << endl;
		}
		else if(!option.compare("q")) {
			cerr << "bye bye...\n";
			return 0;
		}
		else if(!option.compare("#")) {
			string line;
			getline(cin,line);
			cerr << "Skipping line: " << line << endl;
		}
		else {
			cout << "INPUT ERROR\n";
			return -1;
		}
	}
	return -1;  
}

#endif
