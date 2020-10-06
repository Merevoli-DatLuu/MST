#include <bits/stdc++.h>
#include <ctime>

using namespace std;

struct Edge{
    int v_1, v_2, value;

    Edge(int _v_1, int _v_2, int _value){
        v_1 = _v_1;
        v_2 = _v_2;
        value = _value;
    }

    inline bool operator > (const Edge& f){
        return value > f.value;
    }

    inline bool operator == (const Edge& f){
        return value == f.value;
    }

    inline bool operator < (const Edge& f){
        return value < f.value;
    }
};

struct Tree{
    vector <int> V;
    vector <Edge> E;

    int T = 0;
    map <pair <int, int>, int> adjmatrix;   // dùng kiểm tra nhanh 2 đỉnh kề nhau
    map <int, int> adj_V; // đếm số cạnh kề với đỉnh u

    Tree(){

    }

    Tree(vector <int> _V, vector <Edge> _E){
        V = _V;
        E = _E;
    }

    Tree(vector <int> _V, vector <Edge> _E, map <pair <int, int>, int> _adjmatrix, int _T, map <int, int> _adj_V){
        V = _V;
        E = _E;
        adjmatrix = _adjmatrix;
        T = _T;
        adj_V = _adj_V;
    }

};

int main(){
    cout << (Edge(1, 2, 3) > Edge(2,3, 1) );
}
