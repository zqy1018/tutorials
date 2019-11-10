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
    int siz, v, lch, rch;
    double k;
};
Tr tr[400005];
int S, root, max_root_siz;
int n, m, a[100005];
int stck[400005], top;
const double alpha = 0.75;
void init_env(){
    S = root = 0;
    max_root_siz = 0;
    tr[0].siz = 0;
}
int tree_new(double k, int v){
    ++S;
    tr[S].siz = 1;
    tr[S].k = k, tr[S].v = v;
    tr[S].lch = tr[S].rch = 0;
    return S;
}
bool is_unbalanced(int x){
    return tr[tr[x].lch].siz > alpha * tr[x].siz + 5 || tr[tr[x].rch].siz > alpha * tr[x].siz + 5;
}
void maintain(int x){
    tr[x].siz = 1 + tr[tr[x].lch].siz + tr[tr[x].rch].siz;
}
void traverse(int x){
    if (!x) return ;
    traverse(tr[x].lch);
    stck[top++] = x;
    traverse(tr[x].rch);
}
int divide(int l, int r, double lk, double rk){
    if (r < l) return 0;
    int mid = (l + r) >> 1;
    int rt = stck[mid];
    double midk = (lk + rk) * 0.5;
    tr[rt].k = midk;
    tr[rt].lch = divide(l, mid - 1, lk, midk);
    tr[rt].rch = divide(mid + 1, r, midk, rk);
    maintain(rt);
    return rt;
}
void rebuild(int &x){
    top = 0;
    traverse(x);
    double lk = tr[stck[0]].k;
    double rk = tr[stck[top - 1]].k;
    x = divide(0, top - 1, lk, rk);
}
bool Ins(double k, int v, int &x){
    if (!x){
        x = tree_new(k, v);
        return false;
    }
    bool has_rebuilt;
    if (k > tr[x].k) has_rebuilt = Ins(k, v, tr[x].rch);
    else has_rebuilt = Ins(k, v, tr[x].lch);
    maintain(x);
    if (!has_rebuilt && is_unbalanced(x)) {
        rebuild(x);
        has_rebuilt = true;
    }
    return has_rebuilt;
}
void Insert(double k, int v){
    int targ = Ins(k, v, root);
    if (tr[root].siz > max_root_siz)
        max_root_siz = tr[root].siz;
}
void Del(int k){
    int t = root, *p = &root;
    while (t > 0){
        int left_size = tr[tr[t].lch].siz;
        if (left_size >= k) --tr[t].siz, p = &tr[t].lch, t = tr[t].lch;
        else if (left_size + 1 == k){
            if (!tr[t].rch) {
                *p = tr[t].lch;
            }else {
                --tr[t].siz;
                p = &tr[t].rch;
                while (tr[*p].lch > 0)
                    --tr[*p].siz, 
                    p = &tr[*p].lch;
                tr[t].v = tr[*p].v;
                tr[t].k = tr[*p].k;
                *p = tr[*p].rch;
            }
            return ;
        }else
            k -= left_size + 1, --tr[t].siz, p = &tr[t].rch, t = tr[t].rch;
    }
}
void Delete(int k){
    Del(k);
    if (tr[root].siz < alpha * max_root_siz){
        rebuild(root);
        max_root_siz = tr[root].siz;
    }
}
double Get_Kth(int x, int k){
    if (k < 1) return 0;
    if (k > tr[x].siz) return 1;
    int res;
    while (x > 0){
        int left_size = tr[tr[x].lch].siz;
        if (left_size >= k)
            x = tr[x].lch;
        else if (left_size + 1 == k){
            res = x;
            break;
        }else {
            k -= left_size + 1;
            x = tr[x].rch;
        }
    }
    return tr[res].k;
}
int build_tree(int l, int r, double lk, double rk){
    if (r < l) return 0;
    int mid = (l + r) >> 1;
    double midk = (lk + rk) * 0.5;
    int cur = tree_new(midk, a[mid]);
    tr[cur].lch = build_tree(l, mid - 1, lk, midk);
    tr[cur].rch = build_tree(mid + 1, r, midk, rk);
    maintain(cur);
    return cur;
}
void inorder(int x){
    if (!x) return ;
    inorder(tr[x].lch);
    printf("%d ", tr[x].v);
    inorder(tr[x].rch);
}
void init(){
    init_env();
    n = read(), m = read();
    for (int i = 1; i <= n; ++i)
        a[i] = read();
    root = build_tree(1, n, 0, 1.0);
}
void solve(){
    while (m--){
        int opt = read(), k = read();
        if (opt == 1){
            int v = read();
            /* not a good method */
            double lk = Get_Kth(root, k);
            double rk = Get_Kth(root, k + 1);
            Insert((lk + rk) * 0.5, v);
        }else {
            Delete(k);
        }
    }
    inorder(root);
}
int main(){
    init();
    solve();
    return 0;
}