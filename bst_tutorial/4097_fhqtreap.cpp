#include <bits/stdc++.h>
#define INF 2000000000
using namespace std;
typedef long long ll;
int read(){
    int f = 1, x = 0;
    char c = getchar();
    while(c < '0' || c > '9'){if(c == '-') f = -f; c = getchar();}
    while(c >= '0' && c <= '9')x = x * 10 + c - '0', c = getchar();
    return f * x; 
}
struct Tr {
    int siz, v, prio, lch, rch;
};
Tr tr[400005];
int S = 0, root = 0;
int n, m, ans[200005], tot = 0;
void maintain(int x){
    tr[x].siz = 1 + tr[tr[x].lch].siz + tr[tr[x].rch].siz;
}
int tree_new(int k){
    ++S;
    tr[S].siz = 1, tr[S].v = k, 
    tr[S].prio = rand(), 
    tr[S].lch = tr[S].rch = 0;
    return S;
}
void Split_K(int now, int k, int &x, int &y){
    if (!now) x = y = 0;
    else {
        if (k > tr[tr[now].lch].siz){
            x = now, Split_K(tr[now].rch, k - tr[tr[now].lch].siz - 1, tr[now].rch, y);
        }else {
            y = now, Split_K(tr[now].lch, k, x, tr[now].lch);
        }
        maintain(now);
    }
}
int Merge(int x, int y){
    if (!x || !y) return x + y;
    if (tr[x].prio < tr[y].prio){
        tr[x].rch = Merge(tr[x].rch, y);
        maintain(x);
        return x;
    }else{
        tr[y].lch = Merge(x, tr[y].lch);
        maintain(y);
        return y;
    }
} 
void Insert_K(int k, int v){
    int x, y, z = tree_new(v);
    Split_K(root, k, x, y);
    root = Merge(Merge(x, z), y);
}
void Del(int k){
    int x, y, w, z;
    Split_K(root, k - 1, x, y);
    Split_K(y, 1, w, z);
    root = Merge(x, z);
}
void Traverse(int cur){
    if (!cur) return ;
    Traverse(tr[cur].lch);
    ans[++tot] = tr[cur].v;
    Traverse(tr[cur].rch);
}

void init(){
    n = read(), m = read();
    tr[0].siz = 0;
    srand(time(NULL));
    for (int i = 1; i <= n; ++i){
        int u = read();
        Insert_K(i - 1, u);
    }
}
void solve(){
    while (m--){
        int opt = read(), k = read();
        if (opt == 1){
            int v = read();
            Insert_K(k, v);
        }else {
            Del(k);
        }
    }
    Traverse(root);
    for (int i = 1; i <= tot; ++i)
        printf("%d ", ans[i]);
}
int main(){
    init();
    solve();
    return 0;
}