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

int main(){
    cout << (Edge(1, 2, 3) > Edge(2,3, 1) );
}
