/**
    This is a program for Steiner Tree
    Using 4 heuristic algorithms
**/

#include <bits/stdc++.h>
#include <ctime>

using namespace std;

/** Edge class **/
struct Edge{
    int v_1, v_2, value;

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

/** Input Data class **/
struct DataIn{
    int n, m, l;
    vector <int> V;
    vector <Edge> E;
    vector <int> Terminal;
    set <int> Terminal_set; // đánh dấu mảng Terminal


    map <pair <int, int>, int> adjmatrix;   // dùng kiểm tra nhanh 2 đỉnh kề nhau

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

    int v1, v2, v;
    for (int i=1; i<=datain.m; i++){
        fi >> v1 >> v2 >> v;
        datain.E.push_back(Edge(v1, v2, v));
        datain.adjmatrix[make_pair(v1, v2)] = v;
        datain.adjmatrix[make_pair(v2, v1)] = v;
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
        set <int> hasht;
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
    set <int> hash;
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
        if (datain.adjmatrix.find(make_pair(u, a[i])) != datain.adjmatrix.end()){
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
    map <pair <int, int>, int> adjmatrix;
    map <int, int> adj_V;
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

        int T_v_z = datain.adjmatrix[make_pair(v, z)];

        V.push_back(v);
        E.push_back(Edge(v, z, T_v_z));
        adjmatrix[make_pair(v, z)] = T_v_z;
        adjmatrix[make_pair(z, v)] = T_v_z;
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
    /*set <pair <int, int> > hash;
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

////    set <Edge> hashe;
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

Tree EdgeAdd(Tree t, Edge e){

    set <int> L = datain.Terminal_set;

    t.T += e.value;
//    t.T += t.adjmatrix[make_pair(e.v_1, e.v_2)];
    t.adjmatrix[make_pair(e.v_1, e.v_2)] = e.value;
    t.adjmatrix[make_pair(e.v_2, e.v_1)] = e.value;
    t.adj_V[e.v_1]++;
    t.adj_V[e.v_2]++;

    // Chưa tối ưu
    bool v_1_bool = false;
    for (int i=0; i<t.V.size(); i++){
        if (t.V[i] == e.v_1){
            v_1_bool = true;
        }
    }
    if (!v_1_bool){
        t.V.push_back(e.v_1);
    }

    bool v_2_bool = false;
    for (int i=0; i<t.V.size(); i++){
        if (t.V[i] == e.v_2){
            v_2_bool = true;
        }
    }
    if (!v_2_bool){
        t.V.push_back(e.v_2);
    }

    t.E.push_back(e);
    //t.E.erase(find(t.E.begin(), t.E.end(), e));

    return t;
}

Tree EdgeRemove(Tree t, Edge e){

    set <int> L = datain.Terminal_set;

    t.T -= t.adjmatrix[make_pair(e.v_1, e.v_2)];
    t.adjmatrix[make_pair(e.v_1, e.v_2)] = 0;
    t.adjmatrix[make_pair(e.v_2, e.v_1)] = 0;
    if ((t.adj_V[e.v_1] == 1 && L.find(e.v_1) == L.end())){
        t.V.erase(find(t.V.begin(), t.V.end(), e.v_1));
    }
    else if ((t.adj_V[e.v_2] == 1 && L.find(e.v_2) == L.end())){
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

Tree EdgeRemove_Without_L(Tree t, Edge e){

    set <int> L = datain.Terminal_set;

    t.T -= t.adjmatrix[make_pair(e.v_1, e.v_2)];
    t.adjmatrix[make_pair(e.v_1, e.v_2)] = 0;
    t.adjmatrix[make_pair(e.v_2, e.v_1)] = 0;
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
    return t;
}

Tree EdgeDel(Tree t){
    map <int, int> count_edge;

    for (Edge e : t.E){
        count_edge[e.v_1]++;
        count_edge[e.v_2]++;
    }

    vector <Edge> E;
    set <int> L =  datain.Terminal_set;
    for (Edge e : t.E){
        if (!((count_edge[e.v_1] == 1 && L.find(e.v_1) == L.end()) || (count_edge[e.v_2] == 1 && L.find(e.v_2) == L.end()))){ // dỉnh treo không thuộc Terminal
            E.push_back(e);
        }
        else{
            t.T -= t.adjmatrix[make_pair(e.v_1, e.v_2)];
            t.adjmatrix[make_pair(e.v_1, e.v_2)] = 0;
            t.adjmatrix[make_pair(e.v_2, e.v_1)] = 0;
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
    t = EdgeDel(t);
    while (E_size != t.E.size()){
        E_size = t.E.size();
        t = EdgeDel(t);
    }
}

int ReduceTreeValue(Tree t){
    int E_size = t.E.size();
    t = EdgeDel(t);
    while (E_size != t.E.size()){
        E_size = t.E.size();
        t = EdgeDel(t);
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
    return false;
}

bool is_tree(Tree t){
    bool *visited = new bool[50007];
    for (int i = 1; i <= 50007; i++){
        visited[i] = false;
    }

    if (isCyclicUtil(1, visited, -1, t)){
        return false;
    }

    for (int e : t.V){
        if (!visited[e]){
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
    if (!is_tree(t)){
        return false;
    }

    set <int> T;
    for (int v : t.V){
        if (datain.Terminal_set.count(v) > 0){
            T.insert(v);
        }
    }

    if (T.size() != datain.Terminal_set.size()){
        return false;
    }

    return true;
}

Tree NeighSearch(Tree t, int k){
    Tree tb = t;
    for (int i=1; i<=k; i++){
        Edge e_del = t.E[rand()%t.E.size()];
        t = EdgeRemove_Without_L(t, e_del);
//        vector <Edge> sub_E = Complement_Edge(datain.E, t.E);
        Edge e_add = sub_E_t[rand()%sub_E_t.size()];
        t = EdgeAdd(t, e_add);


//        cout << e_del.value << ' ' << e_add.value << ' ';
//        cout << tb.T << ' ' << t.T << '\n';
        if (t.T < tb.T && is_steiner_tree(t)){
            tb = t;
            sub_E_t.push_back(e_del);
            for (int j=0; j<sub_E_t.size(); j++){
                if (sub_E_t[j] == e_add){
                    sub_E_t.erase(sub_E_t.begin() + j);
                    break;
                }
            }
        }
        else{
            t = tb;
        }
    }

    return tb;
}

Tree RandSearch(Tree t, int k){
    Tree tb = t;
    for (int i=1; i<=k; i++){
        Edge e_del = t.E[rand()%t.E.size()];
        t = EdgeRemove_Without_L(t, e_del);
//        vector <Edge> sub_E = Complement_Edge(datain.E, t.E);
        Edge e_add = sub_E_t[rand()%sub_E_t.size()];
        t = EdgeAdd(t, e_add);

        /* Closed
        if (t.T < tb.T && is_steiner_tree(t)){
            tb = t;
            sub_E_t.push_back(e_del);
            for (int j=0; j<sub_E_t.size(); j++){
                if (sub_E_t[j] == e_add){
                    sub_E_t.erase(sub_E_t.begin() + j);
                    break;
                }
            }
        }
        */
        if (ReduceTreeValue(t) < ReduceTreeValue(tb) && is_steiner_tree(t)){
            tb = t;
            sub_E_t.push_back(e_del);
            for (int j=0; j<sub_E_t.size(); j++){
                if (sub_E_t[j] == e_add){
                    sub_E_t.erase(sub_E_t.begin() + j);
                    break;
                }
            }
        }
    }

    return tb;
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
    if (k < 20){
        return randfromto(3*i/4 + 1, i-1);
    }
    else if (k < 30){
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

Tree Search_1(Tree t, int k){
    Tree tb = t;

    vector <Edge> E, sub_E;

    E = t.E;
    sub_E = Complement_Edge(datain.E, t.E);

    sort(E.begin(), E.end(), Compare_Edge_1);
    sort(sub_E.begin(), sub_E.end(), Compare_Edge_2);

    for (int i=1; i<=k; i++){
        Edge e_del = E[RatePoint(E.size())];
        t = EdgeRemove_Without_L(t, e_del);
        Edge e_add = sub_E[RatePoint(sub_E.size())];
        t = EdgeAdd(t, e_add);

//        cout << e_del.value << ' ' << e_add.value << ' ';
//        cout << tb.T << ' ' << t.T << '\n';
        if (ReduceTreeValue(t) < ReduceTreeValue(tb) && is_steiner_tree(t)){
            tb = t;
            E.erase(find(E.begin(), E.end(), e_del));
            sub_E.erase(find(sub_E.begin(), sub_E.end(), e_add));
//            E.push_back(e_add);
//            sub_E.push_back(e_del);
            push_back_priority_less(E, e_add);
            push_back_priority_greater(sub_E, e_del);
        }
        else{
            t = tb;
        }
    }

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
        Edge e_del = E[RatePoint(E.size())];
        t = EdgeRemove_Without_L(t, e_del);
        Edge e_add = sub_E[RatePoint(sub_E.size())];
        t = EdgeAdd(t, e_add);
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

    /*while (countt--){
        SortPopulation(200, 100, 50);
        if (tb.T < Bees_Trees[0].T){
            tb = Bees_Trees[0];
            countt = 100;
        }
        for (int i = 0; i < h_set.size(); i++){
            h_set[i] = NeighSearch(h_set[i], 100);
        }

        for (int i = 0; i <  p_h_set.size(); i++){
            p_h_set[i] = NeighSearch(p_h_set[i], 100);
        }

        for (int i = 0; i < N_p_set.size(); i++){
            //N_p_set[i] = RandSearch(N_p_set[i], 100);
        }
    }*/
    //SortPopulation(10000, 1000, 100);
    if (tb.T < Bees_Trees[0].T){
        tb = Bees_Trees[0];
    }
    cout << "Min : " << tb.T << '\n';
}

int TestRunner(string fi){

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
    else if (fi.substr(0, 6) == "steind"){
        ffi.open("./test file/TD/" + fi, ifstream::in);
    }
    else if (fi.substr(0, 6) == "steine"){
        ffi.open("./test file/TE/" + fi, ifstream::in);
    }
    else{
        ffi.open("./test file/TB/" + fi, ifstream::in);
    }



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
    Bees_Trees.push_back(LikePrim());
    cout << "Default Cost: " << Bees_Trees[0].T << '\n';
    fo_xls << Bees_Trees[0].T << '\t';
    foo << Bees_Trees[0].T << '\t';
//    cout << "sdsiakdlasjk" << Bees_Trees[0].T;
//    ReduceTree(Bees_Trees[0]);
//    cout << "asdjhsaokdhsalkdj" << Bees_Trees[0].T;



    /* NeighSearch */ /* Closed
    start = std::clock();
    sub_E_t = Complement_Edge(datain.E, Bees_Trees[0].E);
    Tree t_neighsearch = NeighSearch(Bees_Trees[0], 10000);
//    Tree tt = NeighSearch(t_neighsearch, 1000);
//    while (t_neighsearch.T != tt.T){
//        t_neighsearch = tt;
//        tt = NeighSearch(tt, 10000);
//        cerr << 123;
//    }
    cout << "NeighSearch Cost: " << t_neighsearch.T << '\n';
    foo << t_neighsearch.T << '\t';
    cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
    foo << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << '\t';
    ReduceTree(t_neighsearch);
    cout << "Reduced NeighSearch Cost: " << t_neighsearch.T << '\n';
    foo << t_neighsearch.T << '\t';
    cout << '\n';
    cerr << 1 << '\n';
    sub_E_t = Complement_Edge(datain.E, Bees_Trees[0].E);
//    PrintTree(t_neighsearch);
    */

    /* RandSearch */ /* Closed
    start = std::clock();
    sub_E_t = Complement_Edge(datain.E, Bees_Trees[0].E);
    Tree tRS = Bees_Trees[0];
    for (int i=1; i<=5000; i++){
        tRS = RandSearch(tRS, 10);
    }
    cout << "RandSearch Cost: " << tRS.T << '\n';
    fo_xls << tRS.T << '\t';
    foo << tRS.T << '\t';

    cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
    fo_xls << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << '\t';
    foo << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << '\t';
    ReduceTree(tRS);

    cout << "Reduced RandSearch Cost: " << tRS.T << '\n';
    fo_xls << tRS.T << '\n';
    foo << tRS.T << '\t';

    cout << '\n';
    fo_xls << '\n';
    cerr << 2 << '\n';
    sub_E_t = Complement_Edge(datain.E, Bees_Trees[0].E);
//    PrintTree(tRS);
    */


    /* Search 1 */ /* Closed
    start = std::clock();
    Tree t_search_1 = Search_1(Bees_Trees[0], 10000);

    cout << "Search 1 Cost: " << t_search_1.T << '\n';
    fo_xls << t_search_1.T << '\t';
    foo << t_search_1.T << '\t';

    cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
    fo_xls << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << '\t';
    foo << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << '\t';

//    PrintTree(t_search_1);
    ReduceTree(t_search_1);
//    PrintTree(t_search_1);
    cout << "Reduced Search 1 Cost: " << t_search_1.T << '\n';
    fo_xls << t_search_1.T << '\t';
    foo << t_search_1.T << '\t';
    cout << '\n';
    fo_xls << '\n';
    cerr << 3 << '\n';
//    PrintTree(t_search_1);
    */

    /* Search 2 */
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

//    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
//    //Bees_Steiner();
//    InputData();
//    //XuatInput();
//    Bees_Trees.push_back(LikePrim());
//    PrintTree(Bees_Trees[0]);
//    EdgeDelTrees();
//    PrintTree(Bees_Trees[0]);
//    Bees_Trees[0] = NeighSearch(Bees_Trees[0], 10000);
//    cout << "NeighSearch :" << Bees_Trees[0].T << '\n';
//    for (int i=1; i<=1000; i++){
//        Bees_Trees[0] = RandSearch(Bees_Trees[0], 100);
//    }
//
//    PrintTree(Bees_Trees[0]);

//    TestRunner("Steinb1.txt");
/*
    fo_xls << "Test 2" << endl;
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");
    TestRunner("Steinb2.txt");

    fo_xls << "Test 3" << endl;
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");
    TestRunner("Steinb3.txt");

    fo_xls << "Test 4" << endl;
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");
    TestRunner("Steinb4.txt");

    fo_xls << "Test 5" << endl;
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");
    TestRunner("Steinb5.txt");

    fo_xls << "Test 6" << endl;
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");
    TestRunner("Steinb6.txt");

    fo_xls << "Test 7" << endl;
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");
    TestRunner("Steinb7.txt");

    fo_xls << "Test 8" << endl;
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");
    TestRunner("Steinb8.txt");

    fo_xls << "Test 9" << endl;
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");
    TestRunner("Steinb9.txt");

    fo_xls << "Test 10" << endl;
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");
    TestRunner("Steinb10.txt");

    fo_xls << "Test 11" << endl;
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");
    TestRunner("Steinb11.txt");

    fo_xls << "Test 12" << endl;
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");
    TestRunner("Steinb12.txt");

    fo_xls << "Test 13" << endl;
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");
    TestRunner("Steinb13.txt");

    fo_xls << "Test 14" << endl;
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");
    TestRunner("Steinb14.txt");

    fo_xls << "Test 15" << endl;
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");
    TestRunner("Steinb15.txt");

    fo_xls << "Test 16" << endl;
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");
    TestRunner("Steinb16.txt");

    fo_xls << "Test 17" << endl;
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");
    TestRunner("Steinb17.txt");

    fo_xls << "Test 18" << endl;
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");
    TestRunner("Steinb18.txt");*/
//    TestRunner("Steinb19.txt");
//    TestRunner("Steinb20.txt");


    TestRunner("Steinc1.txt");
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


//    Tree t;
//    vector <Edge> e;
//    e.push_back(Edge(1, 2, 1));
//    e.push_back(Edge(2, 3, 2));
//    e.push_back(Edge(3, 5, 2));
//    e.push_back(Edge(1, 4, 6));
//    e.push_back(Edge(1, 6, 2));
//
//    map <pair <int, int>, int> adjmatrix;
//    adjmatrix[make_pair(1, 2)] = 1;
//    adjmatrix[make_pair(2, 3)] = 2;
//    adjmatrix[make_pair(3, 5)] = 2;
//    adjmatrix[make_pair(1, 4)] = 6;
//    adjmatrix[make_pair(1, 6)] = 2;
//
//    adjmatrix[make_pair(2, 1)] = 1;
//    adjmatrix[make_pair(3, 2)] = 2;
//    adjmatrix[make_pair(5, 3)] = 2;
//    adjmatrix[make_pair(4, 1)] = 6;
//    adjmatrix[make_pair(6, 1)] = 2;
//
//    vector <int> v;
//    v.push_back(1);
//    v.push_back(2);
//    v.push_back(3);
//    v.push_back(4);
//    v.push_back(5);
//    v.push_back(6);
//    t.E = e;
//    t.V = v;
//    t.adjmatrix = adjmatrix;
//
//
//    cerr << is_tree(t);

    /*PrintTree(Bees_Trees[0]);
    cout << "--\n";
    PrintTree(Bees_Trees[1]);
    cout << "--\n";
    PrintTree(Bees_Trees[2]);
    cout << "=====\n";
    EdgeDelTrees();

    PrintTree(Bees_Trees[0]);
    cout << "--\n";
    PrintTree(Bees_Trees[1]);
    cout << "--\n";
    PrintTree(Bees_Trees[2]);
    cout << "=====\n";*/

    cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
}
