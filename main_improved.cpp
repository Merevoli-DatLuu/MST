/**
    This is a program for Steiner Tree
    Using 4 heuristic algorithms
**/

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ctime>

using namespace std;
using namespace __gnu_pbds;

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1, T2> const &pair) const
    {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);

        return h1 ^ h2;
    }
};

/** Edge class **/
struct Edge{
    int v_1, v_2, value;

    Edge(){}

    Edge(int _v_1, int _v_2, int _value){
        v_1 = _v_1;
        v_2 = _v_2;
        value = _value;
    }

    inline bool operator > (const Edge& f) const{
        return value > f.value;
    }

    inline bool operator == (const Edge& f) const{
        return (v_1 == f.v_1 && v_2 == f.v_2) || (v_1 == f.v_2 && v_2 == f.v_1);
    }

    inline bool operator < (const Edge& f) const{
        return value < f.value;
    }
};

/** Tree class **/
struct Tree{
    vector <int> V;
    vector <Edge> E;

    int T = 0;
//    unordered_map<pair <int, int>, int, pair_hash> adjmatrix;   // dùng kiểm tra nhanh 2 đỉnh kề nhau
    vector < vector <int > > adjmatrix;
//    gp_hash_table <int, int> adj_V; // đếm số cạnh kề với đỉnh u
    vector < int > adj_V;

    Tree(){

    }

    Tree(vector <int> _V, vector <Edge> _E){
        V = _V;
        E = _E;
        adjmatrix.resize(V.size() + 1);
        for (int i=1; i<=V.size(); i++){
            adjmatrix[i].resize(V.size() + 1, 0);
        }
        adj_V.resize(V.size() + 1, 0);
    }

    Tree(vector <int> _V, vector <Edge> _E, vector < vector <int > > _adjmatrix, int _T, vector < int > _adj_V){
        V = _V;
        E = _E;
        adjmatrix = _adjmatrix;
        T = _T;
        adj_V = _adj_V;
    }

};

/** Input Data class **/
struct DataIn{
    int n, m, l;
    vector <int> V;
    vector <Edge> E;
    vector <int> Terminal;
    unordered_set<int> Terminal_set; // đánh dấu mảng Terminal


//    unordered_map<pair <int, int>, int, pair_hash> adjmatrix;   // dùng kiểm tra nhanh 2 đỉnh kề nhau
    vector < vector <int > > adjmatrix;

    DataIn(){

    }

    DataIn(int _n, int _m, int _l, vector <int> _Terminal, vector <Edge> _E){
        n = _n;
        m = _m;
        l = _l;
        Terminal = _Terminal;
        E = _E;
    }
};



vector <Tree> Bees_Trees;
DataIn datain;
vector <Edge> sub_E_t;

ofstream fo_xls("MST_xls.txt");

/* Hàm dùng để nhập dữ liệu đầu vào */
void InputData(ifstream &fi){
    fi >> datain.n >> datain.m;


    datain.adjmatrix.resize(datain.n + 1);
    for (int i=1; i<=datain.n; i++){
        datain.adjmatrix[i].resize(datain.n + 1, 0);
    }

    int v1, v2, v;
    for (int i=1; i<=datain.m; i++){
        fi >> v1 >> v2 >> v;
        datain.E.push_back(Edge(v1, v2, v));
        datain.adjmatrix[v1][v2] = v;
        datain.adjmatrix[v2][v1] = v;
    }

    fi >> datain.l;

    int t;
    for (int i=1; i<=datain.l; i++){
        fi >> t;
        datain.Terminal.push_back(t);
        datain.Terminal_set.insert(t);
    }

    for (int i=1; i<=datain.n; i++){
        datain.V.push_back(i);
    }
    fi.close();
}


void XuatInput(){
    cout << datain.n << ' ' << datain.m << '\n';

    for (Edge ed : datain.E){
        cout << ed.v_1 << ' ' << ed.v_2 << ' ' << ed.value << '\n';
    }

    cout << datain.l << '\n';
    for (int t : datain.Terminal){
        cout << t << ' ';
    }

    cout << endl;
}

/* Kiểm tra a là tập con của b */
bool Subset_check(vector <int> a, vector <int> b){
    if (a.size() > b.size()){
        return false;
    }
    else{
        unordered_set<int> hasht;
        for (int i=0; i<b.size(); i++){
            hasht.insert(b[i]);
        }

        for (int i=0; i<a.size(); i++){
            if (hasht.find(a[i]) == hasht.end()){
                return false;
            }
        }
        return true;
    }
}

/* Trả về phần bù b trong a (a - b) */
vector <int> Complement(vector <int> a, vector <int> b){
    unordered_set<int> hash;
    for (int i=0; i<b.size(); i++){
        hash.insert(b[i]);
    }
    vector <int> re;
    for (vector<int>::iterator it=a.begin(); it!=a.end(); it++){
        if (hash.find(*it) == hash.end()){
            re.push_back(*it);
        }
    }

    return re;
}

/* Hàm dùng để kiểm tra đỉnh kề với đỉnh u */
int adjacency_check(int u, vector <int> a){
    for (int i=0; i<a.size(); i++){
        if (datain.adjmatrix[u][a[i]] != 0){
            return a[i];
        }
    }
    return -1;
}

/* Hàm LikePrim để sinh ngẫu nhiên cây Steiner */
Tree LikePrim(){
    //srand(time(NULL));

    vector <int> V;
    vector <Edge> E;
//    unordered_map<pair <int, int>, int, pair_hash> adjmatrix;
    vector < vector <int > > adjmatrix;
    adjmatrix.resize(datain.n + 1);
    for (int i=1; i<=datain.n; i++){
        adjmatrix[i].resize(datain.n + 1, 0);
    }
//    gp_hash_table <int, int> adj_V;
    vector < int > adj_V;
    adj_V.resize(datain.n + 1, 0);
    int T = 0;
	int u = datain.Terminal[rand() % datain.l];
    V.push_back(u);

    while (!Subset_check(datain.Terminal, V)){
        vector <int> com_set = Complement(datain.V, V);
        int v = com_set[rand() % com_set.size()];
        int z = adjacency_check(v, V);
        while (z == -1){
            v = com_set[rand() % com_set.size()];
            z = adjacency_check(v, V);
//            cout << V.size() << V[0] << endl;
        }

        int T_v_z = datain.adjmatrix[v][z];

        V.push_back(v);
        E.push_back(Edge(v, z, T_v_z));
        adjmatrix[v][z] = T_v_z;
        adjmatrix[z][v] = T_v_z;
        T += T_v_z;
        adj_V[v]++;
        adj_V[z]++;

    }

    return Tree(V, E, adjmatrix, T, adj_V);
}

/* Xuất cây Steiner */
void PrintTree(Tree x){
    cout << x.V.size() << ' ' << x.E.size() << '\n';
    for (int e : x.V){
        cout << e << ' ';
    }
    cout << '\n';
    for (Edge e : x.E){
        cout << e.v_1 << ' ' << e.v_2 << ' ' << e.value << '\n';
    }
    cout << x.T << '\n';
}

/* Tạo quần thể cây Steiner ban đầu */
void InitPopulation(int num){
    for (int i=1; i<=num; i++){
        Bees_Trees.push_back(LikePrim());   //? Op : loại bỏ những cây giống nhau
    }
}

vector <Edge> Complement_Edge(vector <Edge> a, vector <Edge> b){
    /*unordered_set<pair <int, int> > hash;
    for (int i=0; i<b.size(); i++){
        hash.insert(make_pair(b[i].v_1, b[i].v_2));
    }*/
    vector <Edge> re;
    /*for (vector<Edge>::iterator it=a.begin(); it<a.end(); it++){
        if (hash.find(make_pair(*(it).v_1, *(it).v_2)) == hash.end() || hash.find(make_pair(*(it).v_2, *(it).v_1)) == hash.end()){
            re.push_back(*it);
        }
    }*/

//    cerr << "sadsad\n";
//    for (Edge e_1 : a){
//        cerr << e_1.v_1 << ' ' << e_1.v_2 << ' ' << e_1.value << '\n';
//    }
//    cerr << "\n";
//    for (Edge e_1 : b){
//        cerr << e_1.v_1 << ' ' << e_1.v_2 << ' ' << e_1.value << '\n';
//    }
    for (Edge e_1 : a){
        bool f = false;
        for (Edge e_2 : b){
            if ((e_1.v_1 == e_2.v_1 && e_1.v_2 == e_2.v_2) || (e_1.v_2 == e_2.v_1 && e_1.v_1 == e_2.v_2)){
                f = true;
                break;
            }
        }
        if (!f){
            re.push_back(e_1);
        }
    }

////    unordered_set<Edge> hashe;
////    for (Edge e_2 : b){
////        hashe.insert(e_2);
////    }
////    cerr << hashe.size();
////    cerr << 'a';
////    for (Edge e_1 : a){
////        if (hashe.find(e_1) == hashe.end()){
////            re.push_back(e_1);
////        }
////    }
////
////    cerr << 'b';
////    cerr << re.size();

    return re;
}

void EdgeAdd(Tree &t, Edge e){

    //unordered_set<int> L = datain.Terminal_set;

    t.T += e.value;
//    t.T += t.adjmatrix[make_pair(e.v_1, e.v_2)];
    t.adjmatrix[e.v_1][e.v_2] = e.value;
    t.adjmatrix[e.v_2][e.v_1] = e.value;

    if (t.adj_V[e.v_1] == 0){
        t.V.push_back(e.v_1);
    }

    if (t.adj_V[e.v_2] == 0){
        t.V.push_back(e.v_2);
    }

    t.adj_V[e.v_1]++;
    t.adj_V[e.v_2]++;

    /* [Wrong?]
    // Chưa tối ưu
    bool v_1_bool = false;
    for (int i=0; i<t.V.size(); i++){
        if (t.V[i] == e.v_1){
            v_1_bool = true;
            break;
        }
    }
    if (!v_1_bool){
        t.V.push_back(e.v_1);
    }

    bool v_2_bool = false;
    for (int i=0; i<t.V.size(); i++){
        if (t.V[i] == e.v_2){
            v_2_bool = true;
            break;
        }
    }
    if (!v_2_bool){
        t.V.push_back(e.v_2);
    }
    */

    t.E.push_back(e);
    //t.E.erase(find(t.E.begin(), t.E.end(), e));
}

Tree EdgeRemove(Tree t, Edge e){

    //unordered_set<int> L = datain.Terminal_set;

    /* [Wrong?]
    t.T -= t.adjmatrix[make_pair(e.v_1, e.v_2)];
    */

    t.T -= e.value;
    t.adjmatrix[e.v_1][e.v_2] = 0;
    t.adjmatrix[e.v_2][e.v_1] = 0;
    if ((t.adj_V[e.v_1] == 1 && datain.Terminal_set.find(e.v_1) == datain.Terminal_set.end())){
        t.V.erase(find(t.V.begin(), t.V.end(), e.v_1));
    }
    else if ((t.adj_V[e.v_2] == 1 && datain.Terminal_set.find(e.v_2) == datain.Terminal_set.end())){
        t.V.erase(find(t.V.begin(), t.V.end(), e.v_2));
    }
    t.adj_V[e.v_1]--;
    t.adj_V[e.v_2]--;

    for (int i=0; i<t.E.size(); i++){
        if (((t.E[i].v_1 == e.v_1 && t.E[i].v_2 == e.v_2) || (t.E[i].v_1 == e.v_2 && t.E[i].v_2 == e.v_1)) && t.E[i].value == e.value){
            t.E.erase(t.E.begin() + i);
            break;
        }
    }
    //t.E.erase(find(t.E.begin(), t.E.end(), e));
    return t;
}

void EdgeRemove_Without_L(Tree &t, Edge e){

    //unordered_set<int> L = datain.Terminal_set;

    /* [Wrong?]
    t.T -= t.adjmatrix[make_pair(e.v_1, e.v_2)];
    */

    t.T -= e.value;
    t.adjmatrix[e.v_1][e.v_2] = 0;
    t.adjmatrix[e.v_2][e.v_1] = 0;
    if (t.adj_V[e.v_1] == 1){
        t.V.erase(find(t.V.begin(), t.V.end(), e.v_1));
    }
    else if (t.adj_V[e.v_2] == 1){
        t.V.erase(find(t.V.begin(), t.V.end(), e.v_2));
    }
    t.adj_V[e.v_1]--;
    t.adj_V[e.v_2]--;

    for (int i=0; i<t.E.size(); i++){
        if (((t.E[i].v_1 == e.v_1 && t.E[i].v_2 == e.v_2) || (t.E[i].v_1 == e.v_2 && t.E[i].v_2 == e.v_1)) && t.E[i].value == e.value){
            t.E.erase(t.E.begin() + i);
            break;
        }
    }
    //t.E.erase(find(t.E.begin(), t.E.end(), e));
}

Tree EdgeDel(Tree t){
    gp_hash_table <int, int> count_edge;

    for (Edge e : t.E){
        count_edge[e.v_1]++;
        count_edge[e.v_2]++;
    }

    vector <Edge> E;
    unordered_set<int> L =  datain.Terminal_set;
    for (Edge e : t.E){
        if (!((count_edge[e.v_1] == 1 && L.find(e.v_1) == L.end()) || (count_edge[e.v_2] == 1 && L.find(e.v_2) == L.end()))){ // dỉnh treo không thuộc Terminal
            E.push_back(e);
        }
        else{
            /* [Wrong?]
            t.T -= t.adjmatrix[make_pair(e.v_1, e.v_2)];
            */

            t.T -= e.value;
            t.adjmatrix[e.v_1][e.v_2] = 0;
            t.adjmatrix[e.v_2][e.v_1] = 0;
            t.adj_V[e.v_1]--;
            t.adj_V[e.v_2]--;
            if ((count_edge[e.v_1] == 1 && L.find(e.v_1) == L.end())){
                t.V.erase(find(t.V.begin(), t.V.end(), e.v_1));
            }
            else if ((count_edge[e.v_2] == 1 && L.find(e.v_2) == L.end())){
                t.V.erase(find(t.V.begin(), t.V.end(), e.v_2));
            }
        }
    }
    t.E = E;
    return t;
}

void EdgeDel2(Tree &t){
    vector <Edge> E;

//    for (Edge e : t.E){
//        if (!((t.adj_V[e.v_1] == 1 && datain.Terminal_set.find(e.v_1) == datain.Terminal_set.end()) || (t.adj_V[e.v_2] == 1 && datain.Terminal_set.find(e.v_2) == datain.Terminal_set.end()))){ // dỉnh treo không thuộc Terminal
//            E.push_back(e);
//        }
//        else{
//            /* [Wrong?]
//            t.T -= t.adjmatrix[make_pair(e.v_1, e.v_2)];
//            */
//
//            t.T -= e.value;
//            t.adjmatrix[e.v_1][e.v_2] = 0;
//            t.adjmatrix[e.v_2][e.v_1] = 0;
//            if (t.adj_V[e.v_1] == 1){
//                t.V.erase(find(t.V.begin(), t.V.end(), e.v_1));
//            }
//            else if (t.adj_V[e.v_2] == 1){
//                t.V.erase(find(t.V.begin(), t.V.end(), e.v_2));
//            }
//            t.adj_V[e.v_1]--;
//            t.adj_V[e.v_2]--;
//        }
//    }
//
    for (int i=0; i<t.E.size(); i++){
        if (!((t.adj_V[t.E[i].v_1] == 1 && datain.Terminal_set.find(t.E[i].v_1) == datain.Terminal_set.end()) || (t.adj_V[t.E[i].v_2] == 1 && datain.Terminal_set.find(t.E[i].v_2) == datain.Terminal_set.end()))){ // dỉnh treo không thuộc Terminal
            E.push_back(t.E[i]);
        }
        else{

            t.T -= t.E[i].value;
            t.adjmatrix[t.E[i].v_1][t.E[i].v_2] = 0;
            t.adjmatrix[t.E[i].v_2][t.E[i].v_1] = 0;
            if (t.adj_V[t.E[i].v_1] == 1){
                t.V.erase(find(t.V.begin(), t.V.end(), t.E[i].v_1));
            }
            else if (t.adj_V[t.E[i].v_2] == 1){
                t.V.erase(find(t.V.begin(), t.V.end(), t.E[i].v_2));
            }
            t.adj_V[t.E[i].v_1]--;
            t.adj_V[t.E[i].v_2]--;
        }
    }
    t.E = E;
}

// Xóa cạnh dư thừa của quần thể cây Steiner
void EdgeDelTrees(){
    for (int i=0; i<Bees_Trees.size(); i++){
        int E_size = Bees_Trees[i].E.size();
        Bees_Trees[i] = EdgeDel(Bees_Trees[i]);
        while (E_size != Bees_Trees[i].E.size()){
            E_size = Bees_Trees[i].E.size();
            Bees_Trees[i] = EdgeDel(Bees_Trees[i]);
        }
    }
}

void ReduceTree(Tree &t){
    int E_size = t.E.size();
    EdgeDel2(t);
    while (E_size != t.E.size()){
        E_size = t.E.size();
        EdgeDel2(t);
    }
}

int ReduceTreeValue(Tree t){
    int E_size = t.E.size();
    EdgeDel2(t);
    while (E_size != t.E.size()){
        E_size = t.E.size();
        EdgeDel2(t);
    }

    return t.T;
}

// Tìm xem u thuộc cây nào
int Find(int u, int cha[]) {
    if (cha[u] != u) cha[u] = Find(cha[u], cha);
    return cha[u];
}

// Hợp nhất 2 cây chứ u và v,
// Trả về false nếu không thể hợp nhất
bool Join(int u, int v, int hang[], int cha[]) {
    u = Find(u, cha); v = Find(v, cha);
    if (u == v) return false;
    if (hang[u] == hang[v]) hang[u]++;
    if (hang[u] < hang[v]) cha[u] = v;
    else cha[v]=u;
    return true;
}

bool isCyclicUtil(int v, bool visited[], int parent, Tree t){
    visited[v] = true;

//    list<int>::iterator i;
//    for (i = adj[v].begin(); i != adj[v].end(); ++i){
//        if (!visited[*i]){
//           if (isCyclicUtil(*i, visited, v))
//              return true;
//        }
//        else if (*i != parent){
//           return true;
//        }
//    }
    // Chưa tối ưu
    for (int h : t.V){
        if (t.adjmatrix[v][h] != 0){
            //cerr << h << ' ' << visited[h] << '\n';
            if (!visited[h]){
               if (isCyclicUtil(h, visited, v, t))
                  return true;
            }
            else if (h != parent){
               return true;
            }
        }
    }

    /* Closed
    for (int i=0; i < t.V.size(); i++){
        int h = t.V[i];
        if (t.adjmatrix[make_pair(v, h)] != 0){
            //cerr << h << ' ' << visited[h] << '\n';
            if (!visited[h]){
               if (isCyclicUtil(h, visited, v, t))
                  return true;
            }
            else if (h != parent){
               return true;
            }
        }
    }
    */
    return false;
}

bool is_tree(Tree t){
    bool *visited = new bool[1007];
    for (int i = 1; i <= 1006; i++){
        visited[i] = false;
    }

    if (isCyclicUtil(1, visited, -1, t)){
        cerr << "b\n";
        return false;
    }

    for (int e : t.V){
        if (!visited[e]){
            cerr << "c\n";
            return false;
        }
    }

    return true;
}

bool is_steiner_tree(Tree t){
//    int cha[1007], hang[1007];
//    for (int i=1; i<=1007; i++) {
//        cha[i] = i;
//        hang[i] = 0;
//    }
//
//    for (Edge e : t.E){
//        if (!Join(e.v_1, e.v_2, hang, cha)){
//            return false;
//        }
//    }

    int T = 0;
    for (int v : t.V){
        if (datain.Terminal_set.find(v) != datain.Terminal_set.end()){
            T++;
        }
    }

    if (T != datain.Terminal_set.size()){
        cerr << "a\n";
        return false;
    }

    if (!is_tree(t)){
        return false;
    }

    return true;
}

int Check_ke_a_add(Edge e, Tree t){
    if (t.adj_V[e.v_1] == 0 && t.adj_V[e.v_2] == 0){
        return 0;
    }
    else if (t.adj_V[e.v_1] == 0 || t.adj_V[e.v_2] == 0){
        return 1;
    }
    else{
        return 2;
    }
}

int Check_ke_a_del(Edge e, Tree t){
    if ((t.adj_V[e.v_1] == 1 && t.adj_V[e.v_2] != 1) || (t.adj_V[e.v_2] == 1 && t.adj_V[e.v_1] != 1)){
        return 1;
    }
    else{
        return 2;
    }
}

bool Compare_Edge_1(Edge a, Edge b){
    return a.value > b.value;
}

bool Compare_Edge_2(Edge a, Edge b){
    return a.value < b.value;
}

//bool Compare_Edge_3(Edge a, Edge b){
//    if ()
//}

int randfromto(int a, int b){
    return rand()%(b - a + 1) + a;
}

int RatePoint(int i){
//    srand(time(NULL));
    int k = rand()%100;
    if (k < 10){
        return randfromto(3*i/4 + 1, i-1);
    }
    else if (k < 25){
        return randfromto(i/2 + 1, 3*i/4);
    }
    else if (k < 45){
        return randfromto(i/4 + 1, i/2);
    }
    else if (k < 100){
        return randfromto(0, i/4);
    }
    return randfromto(0, i-1);
}

void push_back_priority_greater(vector <Edge> &E, Edge a){
    int mid = (E.size() >> 1);
    int L = 0;
    int R = E.size() - 1;
    int k = 0;
    while (L < R){
        if (E[mid].value < a.value){
            L = min((int)E.size()-1, mid + 1);
        }
        else if (E[mid].value > a.value){
            R = max(0, mid - 1);
        }
        else {
            k = mid;
            break;
        }
        mid = (L + R) >> 1;
    }
    if (L >= R){
        if (E[mid].value < a.value){
            E.insert(E.begin() + mid + 1, a);
        }
        else{
            E.insert(E.begin() + mid, a);
        }
    }
    else{
        E.insert(E.begin() + k, a);
    }
}

void push_back_priority_less(vector <Edge> &E, Edge a){
    int mid = (E.size() >> 1);
    int L = 0;
    int R = E.size() - 1;
    int k = 0;
    while (L < R){
        if (E[mid].value > a.value){
            L = min((int)E.size()-1, mid + 1);
        }
        else if (E[mid].value < a.value){
            R = max(0, mid - 1);
        }
        else {
            k = mid;
            break;
        }
        mid = (L + R) >> 1;
    }
    if (L >= R){
        if (E[mid].value > a.value){
            E.insert(E.begin() + mid + 1, a);
        }
        else{
            E.insert(E.begin() + mid, a);
        }
    }
    else{
        E.insert(E.begin() + k, a);
    }
}

bool check_tree(Tree t, int u_1, int u_2){
    queue <int> Q;
    Q.push(u_1);
    bool *visited = new bool[1007];
    for (int i = 1; i <= 1006; i++){
        visited[i] = false;
    }
    visited[u_1] = true;
    while (!Q.empty()){
        int u = Q.front();
        Q.pop();

        for (int i=1; i<=datain.V.size(); i++){
            if (t.adjmatrix[i][u] && visited[i] == false){
                Q.push(i);
                visited[i] = true;
                if (i == u_2){
                    return false;
                }
            }
        }
    }
    return true;
}


Tree Search_1(Tree t, int k){
    Tree tb = t;

    vector <Edge> E, sub_E;

    E = t.E;
    sub_E = Complement_Edge(datain.E, t.E);

    vector <Edge> sub_E_1, sub_E_2;
    for (int i=0; i<sub_E.size(); i++){
        if (Check_ke_a_add(sub_E[i], t) == 1){
            sub_E_1.push_back(sub_E[i]);
        }
        else if (Check_ke_a_add(sub_E[i], t) == 2){
            sub_E_2.push_back(sub_E[i]);
        }
    }


    sort(E.begin(), E.end(), Compare_Edge_1);
    sort(sub_E_1.begin(), sub_E_1.end(), Compare_Edge_2);
    sort(sub_E_2.begin(), sub_E_2.end(), Compare_Edge_2);

    int k_e_del;
    Edge e_del;
    int k_e_add;
    Edge e_add;


    int tb_T = ReduceTreeValue(tb);
    int updatec = 0;
    cerr << sub_E_1.size() << ' ' << sub_E_2.size() << '\n';
    int u1 = 0;
    int u2 = 0;
    int u3 = 0;
    for (int i=1; i<=k; i++){
        if (i % 10000 == 0){
            cerr << i << '\t' << tb_T << '\n';
            if (!is_steiner_tree(tb)){
                cerr << updatec << ' ' << u1 << ' ' << u2 << ' ' << u3 << endl;
                cerr << "BREAK \n";
                cout << "BREAK \n";
                return tb;
            }
        }
        k_e_del = RatePoint(E.size());
        e_del = E[k_e_del];
        int t2 = Check_ke_a_del(e_del, t);
        if (t2 == 1){
            if (sub_E_1.size() == 0){
                continue;
            }
            k_e_add = RatePoint(sub_E_1.size());
            e_add = sub_E_1[k_e_add];
        }
        else{
            if (sub_E_2.size() == 0){
                continue;
            }
            k_e_add = RatePoint(sub_E_2.size());
            e_add = sub_E_2[k_e_add];
        }

        if (e_del.value <= e_add.value){
            continue;
        }

        if (t2 == 1){
            u2++;

            // Wrong
            if (e_del.v_1 == e_add.v_1 || e_del.v_1 == e_add.v_2 || e_del.v_2 == e_add.v_1 || e_del.v_2 == e_add.v_2){
                continue;
            }
            if ((t.adj_V[e_del.v_1] == 1 && datain.Terminal_set.find(e_del.v_1) != datain.Terminal_set.end()) || (t.adj_V[e_del.v_2] == 1 && datain.Terminal_set.find(e_del.v_2) != datain.Terminal_set.end())){
                continue;
            }
            EdgeRemove_Without_L(t, e_del);
            EdgeAdd(t, e_add);
            int t_T = ReduceTreeValue(t);

            if (t_T < tb_T){
                updatec++;
                tb = t;
                tb_T = t_T;
    //            E.erase(find(E.begin(), E.end(), e_del));
                E.erase(E.begin() + k_e_del);
    //            sub_E.erase(find(sub_E.begin(), sub_E.end(), e_add
                sub_E_1.erase(sub_E_1.begin() + k_e_add);
    //            E.push_back(e_add);
    //            sub_E.push_back(e_del);
                push_back_priority_less(E, e_add);
                push_back_priority_greater(sub_E_1, e_del);
            }
            else{
                t = tb;
            }
        }
        else{
            u3++;
            EdgeRemove_Without_L(t, e_del);
            if (!check_tree(t, e_add.v_1, e_add.v_2)){
                EdgeAdd(t, e_del);
                continue;
            }
            EdgeAdd(t, e_add);
            int t_T = ReduceTreeValue(t);

            if (t_T < tb_T){
                updatec++;
                tb = t;
                tb_T = t_T;
                cout << tb_T << ' ';
    //            E.erase(find(E.begin(), E.end(), e_del));
                E.erase(E.begin() + k_e_del);
    //            sub_E.erase(find(sub_E.begin(), sub_E.end(), e_add
                sub_E_2.erase(sub_E_2.begin() + k_e_add);
    //            E.push_back(e_add);
    //            sub_E.push_back(e_del);
                push_back_priority_less(E, e_add);
                push_back_priority_greater(sub_E_2, e_del);
            }
            else{
                t = tb;
            }
        }
    }

    if (!is_steiner_tree(tb)){
        cerr << "adsad\n";
        cout << "adsad\n";
    }
    cout << '\n';
    cerr << updatec << ' ' << u1 << ' ' << u2 << ' ' << u3 << endl;
    return tb;
}


//void Check_Edge_ke(vector <pair <Edge, bool> > &E, Tree t){
//    for (int i = 0; i<E.size(); i++){
//        if (find(t.V.begin(), t.V.end(), E.first[i].v_1) || find(t.V.begin(), t.V.end(), E.first[i].v_2)){
//            E.second.push_back(true);
//        }
//        else{
//            E.second.push_back(false);
//        }
//    }
//}

bool Check_ke(Edge e, Tree t){
    if (find(t.V.begin(), t.V.end(), e.v_1) != t.V.end() || find(t.V.begin(), t.V.end(), e.v_2) != t.V.end()){
        return true;
    }
    else{
        return false;
    }
}



Tree Search_2(Tree t, int k){
    Tree tb = t;

    vector <Edge> E;
    vector <Edge> sub_E, sub_E_temp;

    E = t.E;
    for (Edge e : Complement_Edge(datain.E, t.E)){
        if (Check_ke(e, t)){
            sub_E.push_back(e);
        }
//        else{
//            sub_E_temp.push_back(e);
//        }
    }

    sort(E.begin(), E.end(), Compare_Edge_1);
    sort(sub_E.begin(), sub_E.end(), Compare_Edge_2);
//    sort(sub_E_temp.begin(), sub_E_temp.end(), Compare_Edge_2);
//    sub_E.insert(sub_E.begin(), sub_E_temp.begin(), sub_E_temp.end());

    for (int i=1; i<=k; i++){
        cerr << i << '\n';
        Edge e_del = E[RatePoint(E.size())];
        EdgeRemove_Without_L(t, e_del);
        Edge e_add = sub_E[RatePoint(sub_E.size())];
        EdgeAdd(t, e_add);
//        cout << e_del.value << ' ' << e_add.value << ' ';
//        cout << tb.T << ' ' << t.T << '\n';
        if (ReduceTreeValue(t) < ReduceTreeValue(tb) && is_steiner_tree(t)){
            tb = t;
            E.erase(find(E.begin(), E.end(), e_del));
            sub_E.erase(find(sub_E.begin(), sub_E.end(), e_add));
            E.push_back(e_add);
//            sub_E.push_back(e_del);
            push_back_priority_greater(sub_E, e_del);
        }
        else{
            t = tb;
        }
    }

    return tb;
}

void Bees_Steiner(){
//    InputData();
    InitPopulation(20000);
    EdgeDelTrees();
    Tree tb;
    int countt = 100;


    for (Tree t : Bees_Trees){
        cout << t.T << ' ';
    }

    if (tb.T < Bees_Trees[0].T){
        tb = Bees_Trees[0];
    }
    cout << "Min : " << tb.T << '\n';
}

void TestRunner(string fi){

    /* Input Data File */
//    freopen(("./test file/TB/" + fi).c_str(), "r", stdin);
    ifstream ffi;

    srand(time(NULL));
    if (fi.substr(0, 6) == "Steinb"){
        ffi.open("./test file/TB/" + fi, ifstream::in);
    }
    else if (fi.substr(0, 6) == "Steinc"){
        ffi.open("./test file/TC/" + fi, ifstream::in);
    }
    else if (fi.substr(0, 6) == "Steind"){
        ffi.open("./test file/TD/" + fi, ifstream::in);
    }
    else if (fi.substr(0, 6) == "steine"){
        ffi.open("./test file/TE/" + fi, ifstream::in);
    }
    else{
        ffi.open("./test file/TB/" + fi, ifstream::in);
    }

    cout << '\n';



    /* init */
    Bees_Trees.clear();
//    Bees_Trees = vector <Tree>();
    datain = DataIn();
    //datain.clear();
    clock_t start;

    ofstream foo("re.txt", std::ios_base::app);
    foo << fi << '\t';
    cout << fi << '\n';



    /* Input Data */
    InputData(ffi);
    Tree tree_init = LikePrim();
    int rcost_init = ReduceTreeValue(tree_init);
    for (int i=1; i<=3000; i++){
        Tree t = LikePrim();
        int rcost_t = ReduceTreeValue(t);
        if (rcost_t < rcost_init){
            tree_init = t;
            rcost_init = rcost_t;
        }
    }

    Bees_Trees.push_back(tree_init);
    cout << "Default Cost: " << Bees_Trees[0].T << '\n';
    fo_xls << Bees_Trees[0].T << '\t';
    foo << Bees_Trees[0].T << '\t';

    /* Search 1 */ /* Closed */
    start = std::clock();
    Tree t_search_1 = Search_1(Bees_Trees[0], 1500000);

    cout << "Search 1 Cost: " << t_search_1.T << '\n';
    fo_xls << t_search_1.T << '\t';
    foo << t_search_1.T << '\t';

    cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
    fo_xls << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << '\t';
    foo << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << '\t';

    ReduceTree(t_search_1);
    cout << "Reduced Search 1 Cost: " << t_search_1.T << '\n';
    fo_xls << t_search_1.T << '\t';
    foo << t_search_1.T << '\t';
    cout << '\n';
    fo_xls << '\n';
    cerr << 3 << '\n';

    /* Search 2 */ /* Closed
    start = std::clock();
    Tree t_search_2 = Search_2(Bees_Trees[0], 10000);
    cout << "Search 2 Cost: " << t_search_2.T << '\n';
    fo_xls << t_search_2.T << '\t';
    foo << t_search_2.T << '\t';

    cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
    fo_xls << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << '\t';
    foo << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << '\t';
    ReduceTree(t_search_2);

    cout << "Reduced Search 2 Cost: " << t_search_2.T << '\n';
    fo_xls << t_search_2.T << '\t';
    foo << t_search_2.T << '\t';
    cout << '\n';
    fo_xls << '\n';
    cerr << 4 << '\n';
//    PrintTree(t_search_2);
    */

    /* All */ /* Closed
    start = std::clock();
    sub_E_t = Complement_Edge(datain.E, Bees_Trees[0].E);
    Tree tta = NeighSearch(Bees_Trees[0], 10000);
    sub_E_t = Complement_Edge(datain.E, Bees_Trees[0].E);
////    bool st = false;
////    while (!st){
////        int T = tta.T;
////        bool sn = false;
////        while (!sn){
////            int t = tta.T;
////            tta = NeighSearch(tta, 10000);
////            if (tta.T == t){
////                sn = true;
////            }
////        }
////        sn = false;
////        while (!sn){
////            int t = tta.T;
            for (int i=1; i<=10000; i++){
                tta = RandSearch(tta, 5);
            }
////            if (tta.T == t){
////                sn = true;
////            }
////        }
////        sn = false;
////        while (!sn){
////            int t = tta.T;
            tta = Search_1(tta, 10000);
////            if (tta.T == t){
////                sn = true;
////            }
////        }
////        sn = false;
////        while (!sn){
////            int t = tta.T;
            tta = Search_2(tta, 10000);
////            if (tta.T == t){
////                sn = true;
////            }
////        }
////        if (T == tta.T){
////            st = true;
////        }
////    }
    cout << "All Cost: " << tta.T << '\n';
    foo << tta.T << '\t';
    cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
    foo << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << '\t';
    ReduceTree(tta);
    cout << "Reduced All Cost: " << tta.T << '\n';
    foo << tta.T << '\n';
    cout << '\n';
    */


    /* Print Output */
    cerr << "Done " << fi << '\n';
    cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
    cout << '\n';
    ffi.close();
    foo.close();
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    clock_t start;
    start = std::clock();

    srand(time(NULL));

    freopen("outputNS1.txt", "a", stdout);

//    TestRunner("Steinb1.txt");
//    TestRunner("Steinb2.txt");
//    TestRunner("Steinb3.txt");
//    TestRunner("Steinb4.txt");
//    TestRunner("Steinb5.txt");
//    TestRunner("Steinb6.txt");
//    TestRunner("Steinb7.txt");
//    TestRunner("Steinb8.txt");
//    TestRunner("Steinb9.txt");
//    TestRunner("Steinb10.txt");
//    TestRunner("Steinb11.txt");
//    TestRunner("Steinb12.txt");
//    TestRunner("Steinb13.txt");
//    TestRunner("Steinb14.txt");
//    TestRunner("Steinb15.txt");
//    TestRunner("Steinb16.txt");
//    TestRunner("Steinb17.txt");
//    TestRunner("Steinb18.txt");
//    TestRunner("Steinb19.txt");
//    TestRunner("Steinb20.txt");


//    TestRunner("Steinc1.txt");
//    TestRunner("Steinc2.txt");
//    TestRunner("Steinc3.txt");
//    TestRunner("Steinc4.txt");
//    TestRunner("Steinc5.txt");
//    TestRunner("Steinc6.txt");
//    TestRunner("Steinc7.txt");
//    TestRunner("Steinc8.txt");
//    TestRunner("Steinc9.txt");
//    TestRunner("Steinc10.txt");
//    TestRunner("Steinc11.txt");
//    TestRunner("Steinc12.txt");
//    TestRunner("Steinc13.txt");
//    TestRunner("Steinc14.txt");
//    TestRunner("Steinc15.txt");
//    TestRunner("Steinc16.txt");
//    TestRunner("Steinc17.txt");
//    TestRunner("Steinc18.txt");
//    TestRunner("Steinc19.txt");
//    TestRunner("Steinc20.txt");

    TestRunner("Steind15.txt");

    cerr << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
    cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
}
