#include<bits/stdc++.h>
using namespace std;

struct Node
{
	int puz[3][3];
	int ex;
	int ey;
	int depth;
	int w;
	int c;
	list<int> wtsVal;
	list<char> dirVal;
};

list<char> ans_dir;
list<struct Node> nodes;

int empty = 9;
int maxDepth = 15;
int mat[3][3];

//int soln[3][3] = {1,2,3,4,5,6,7,8,empty}; //Ascending
int soln[3][3] = {1,2,3,8,empty,4,7,6,5}; //Circular

void printPuz(int ar[3][3]) {
	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			if(ar[i][j] == empty)
				cout<<" ";
			else
				cout<<ar[i][j];
			cout<<" ";
		}
		cout<<"\n";
	}
}

pair<int,int> findEmpty(int puz[3][3]) {
	int ex, ey;
	bool flag = true;
	for(int i=0; flag && i < 3; i++) {
		for(int j=0; flag && j < 3; j++) {
			if(puz[i][j] == empty) {
				ex = i;
				ey = j;
				flag = false;
				break;
			}
		}
	}
	return make_pair(ex,ey);
}

void showSolution() {
	cout<<"\n\nOriginal:\n";
	printPuz(mat);
	list<char>::iterator it;
	pair<int,int> ecoords = findEmpty(mat);
	int ex = ecoords.first;
	int ey = ecoords.second;
	int i;
	for(it = ans_dir.begin(), i = 1 ; it != ans_dir.end(); ++it, ++i)  {
        cout << "\n\nStep "<<i<<": "<<*it<<"\n";
        switch(*it) {
        	case 'u': 	mat[ex][ey] = mat[ex-1][ey];
						mat[ex-1][ey] = empty;
						ex = ex-1;
						break;
        	case 'r':	mat[ex][ey] = mat[ex][ey+1];
						mat[ex][ey+1] = empty;
						ey = ey+1;
						break;
        	case 'd': 	mat[ex][ey] = mat[ex+1][ey];
						mat[ex+1][ey] = empty;
						ex = ex+1;
						break;
        	case 'l':	mat[ex][ey] = mat[ex][ey-1];
						mat[ex][ey-1] = empty;
						ey = ey-1;
						break;
        }
        printPuz(mat);
        /*
        ecoords = findEmpty(mat);
		ex = ecoords.first;
		ey = ecoords.second;
		*/
	}
}


int getWt(int ar[3][3]) {
	int wt = 0;
	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			wt += (((3*i)+j+1)*ar[i][j]);
		}
	}
	return wt;
}

bool search(int ele, list<int> wts) {
	list<int>::iterator it = find(wts.begin(), wts.end(), ele);
	if(it != wts.end())
		return true;
	return false;
}

bool isDone(int ar[3][3]) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			if(ar[i][j] != soln[i][j])
				return false;
		}
	}
	return true;
}

int getHeuristic(int ar[3][3]) {
	int notInPosition = 0;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			if(ar[i][j] != soln[i][j])
				notInPosition++;
		}
	}
	return notInPosition;
}

void removeOpenNode(int w_Find) {
	for (list<struct Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		if((*it).w == w_Find) {
			nodes.erase(it);
			break;
		}
	}
}

void addOpenNode(int puz[3][3], int ex, int ey, int depth, int w, list<int> wts, list<char> dir, int c) {
	struct Node n;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			n.puz[i][j] = puz[i][j];
		}
	}
	n.ex = ex;
	n.ey = ey;
	n.depth = depth;
	n.w = w;
	n.c = c;
	n.wtsVal.merge(wts);
	n.dirVal.merge(dir);
	nodes.push_back(n);
}

struct Node getMinOpen() {
	struct Node min_n;
	int min = INT_MAX;
	for (list<struct Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		if((*it).c < min) {
			min_n = *it;
			min = (*it).c;
		}
	}
	return min_n;
}

int solve(int puz[3][3], int &moves, int ex, int ey, int depth, int w, list<int> wts, list<char> dir) {
	system("cls");
	cout<<moves<<"\n\n";
	depth++;
	cout<<depth<<"\n\n";
	moves++;
	//printPuz(puz);
	//system("pause");
	if(depth > maxDepth) {
		dir.pop_back();
		return 0;
	}
	if(isDone(puz)) {
		ans_dir.merge(dir);
		showSolution();
		cout<<"\n\nSolved the puzzle in "<<moves<<" traversals!\n";
		cout<<"\n\n";
		//showdir();
		exit(0);
	}
	//cout<<w<<endl;
	wts.push_back(w);
	int c;
	int f = depth;
	int h;
	int toExploreIndex = -1;
	int temp_wt;
	removeOpenNode(w);
	if(ex-1 > -1) { 
		puz[ex][ey] = puz[ex-1][ey];
		puz[ex-1][ey] = empty;
		h = getHeuristic(puz);
		temp_wt = getWt(puz);
		c = f + h;
		dir.push_back('u');
		if(!search(temp_wt, wts)) {
			addOpenNode(puz, ex-1, ey, depth, temp_wt, wts, dir, c);
		}
		dir.pop_back();
		puz[ex-1][ey] = puz[ex][ey];
		puz[ex][ey] = empty;
	} //UP
	if(ey+1 < 3) { 
		puz[ex][ey] = puz[ex][ey+1];
		puz[ex][ey+1] = empty;
		h = getHeuristic(puz);
		temp_wt = getWt(puz);
		c = f + h;
		dir.push_back('r');
		if(!search(temp_wt, wts)) {
			addOpenNode(puz, ex, ey+1, depth, temp_wt, wts, dir, c);
		}
		dir.pop_back();
		puz[ex][ey+1] = puz[ex][ey];
		puz[ex][ey] = empty;
	} //RIGHT
	if(ex+1 < 3) { 
		puz[ex][ey] = puz[ex+1][ey];
		puz[ex+1][ey] = empty;
		h = getHeuristic(puz);
		temp_wt = getWt(puz);
		c = f + h;
		dir.push_back('d');
		if(!search(temp_wt, wts)) {
			addOpenNode(puz, ex+1, ey, depth, temp_wt, wts, dir, c);
		}
		dir.pop_back();
		puz[ex+1][ey] = puz[ex][ey];
		puz[ex][ey] = empty;
	} //DOWN
	if(ey-1 > -1) { 
		puz[ex][ey] = puz[ex][ey-1];
		puz[ex][ey-1] = empty;
		h = getHeuristic(puz);
		temp_wt = getWt(puz);
		c = f + h;
		dir.push_back('l');
		if(!search(temp_wt, wts)) {
			addOpenNode(puz, ex, ey-1, depth, temp_wt, wts, dir, c);
		}
		dir.pop_back();
		puz[ex][ey-1] = puz[ex][ey];
		puz[ex][ey] = empty;
	} //LEFT
	struct Node min_node = getMinOpen();
	solve(min_node.puz, moves, min_node.ex, min_node.ey, min_node.depth, min_node.w, min_node.wtsVal, min_node.dirVal);
	
	wts.pop_back();	
	system("cls");
	cout<<moves<<"\n\n";
	depth++;
	cout<<depth<<"\n\n";
	//system("pause");
	return 0;
}

int main() {
	int puz[3][3];
	cout<<"Input Puzzle initial state: (for empty spot enter "<<empty<<")\n";
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			cout<<"Enter puz["<<i<<"]"<<"["<<j<<"]: ";
			cin>>puz[i][j];
			mat[i][j] = puz[i][j];
		}
	}
	//int puz[3][3] = {1,2,3,8,6,empty,7,5,4};//{ {2,7,6}, {1,empty,5}, {3,4,8} };
	cout<<"Initial Puzzle State:\n";
	printPuz(puz);
	cout<<"\nEnter Max Depth to explore: ";
	cin>>maxDepth;
	int moves = 0;
	int ex, ey;
	pair<int,int> ecoords = findEmpty(puz);
	int w = getWt(puz);
	list<int> l1;
	list<char> l2;
	int a = solve(puz, moves, ecoords.first, ecoords.second, 0, w, l1, l2);
	cout<<"\nCould not solve puzzle in "<<maxDepth<<" depth! in "<<moves<<" moves!!!\n";
	exit(0);
	return 0;
}