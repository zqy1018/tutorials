/**
 * 
 * A tutorial for Scapegoat tree.
 * Based on Luogu 3369. 
 * 
 * Author: zqy1018
 * Last modified: 2019/11/04
 * Encoded in UTF-8
 * 
 */

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
    int siz, k, lch, rch;
};
Tr tr[400005];
int S, root, max_root_siz;
const double alpha = 0.75;
void init_env(){
    S = root = 0;
    max_root_siz = 0;
    tr[0].siz = 0;
}
int tree_new(int k){
    ++S;
    tr[S].siz = 1;
    tr[S].k = k;
    tr[S].lch = tr[S].rch = 0;
    return S;
}
bool is_unbalanced(int x){
    return tr[tr[x].lch].siz > alpha * tr[x].siz + 5 || tr[tr[x].rch].siz > alpha * tr[x].siz + 5;
}
void maintain(int x){
    tr[x].siz = 1 + tr[tr[x].lch].siz + tr[tr[x].rch].siz;
}
int stck[200005], top;
void traverse(int x){
    if (!x) return ;
    traverse(tr[x].lch);
    stck[top++] = x;
    traverse(tr[x].rch);
}

int divide(int l, int r){
    if (r <= l) return 0;
    int mid = (l + r) / 2;
    int rt = stck[mid];
    tr[rt].lch = divide(l, mid);
    tr[rt].rch = divide(mid + 1, r);
    maintain(rt);
    return rt;
}
void rebuild(int &x){
    top = 0;
    traverse(x);
    x = divide(0, top);
}
bool Ins(int k, int &x){
    if (!x){
        x = tree_new(k);
        return false;
    }
    bool has_rebuilt;
    if (k > tr[x].k) has_rebuilt = Ins(k, tr[x].rch);
    else has_rebuilt = Ins(k, tr[x].lch);
    maintain(x);
    if (!has_rebuilt && is_unbalanced(x)) {
        rebuild(x);
        has_rebuilt = true;
    }
    return has_rebuilt;
}
void Insert(int k){
    int targ = Ins(k, root);
    if (tr[root].siz > max_root_siz)
        max_root_siz = tr[root].siz;
}
void Del(int x){
    int t = root, *p = &root;
    while (t > 0){
        if (x < tr[t].k) --tr[t].siz, p = &tr[t].lch, t = tr[t].lch;
        else if (x > tr[t].k) --tr[t].siz, p = &tr[t].rch, t = tr[t].rch;
        else {
            if (!tr[t].rch) {
                *p = tr[t].lch;
            }else {
                --tr[t].siz;
                p = &tr[t].rch;
                while (tr[*p].lch > 0)
                    --tr[*p].siz, 
                    p = &tr[*p].lch;
                tr[t].k = tr[*p].k;
                *p = tr[*p].rch;
            }
            return ;
        }
    }
}
void Delete(int k){
    Del(k);
    if (tr[root].siz < alpha * max_root_siz){
        rebuild(root);
        max_root_siz = tr[root].siz;
    }
}

int Get_Predecessor(int x, int k){
    int res = -2147483648; 
    while (x > 0){
        if (tr[x].k < k){
            res = tr[x].k;
            x = tr[x].rch;
        }else x = tr[x].lch;
    }
    return res;
}
int Get_Successor(int x, int k){
    int res = 2147483647;  
    while (x > 0){
        if (tr[x].k > k){
            res = tr[x].k;
            x = tr[x].lch;
        }else x = tr[x].rch;
    }
    return res;
}
int Get_Kth(int x, int k){
    if (k < 1 || k > tr[x].siz) 
        return 0;
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
int Get_Rank(int x, int k){
    int res = 0;
    while (x > 0){
        if (tr[x].k >= k)
            x = tr[x].lch;
        else {
            res += tr[tr[x].lch].siz + 1;
            x = tr[x].rch;
        }
    }
    return res + 1;
}
int Lookup(int x, int k){
    while (x > 0){
        if (tr[x].k > k)
            x = tr[x].lch;
        else if (tr[x].k < k)
            x = tr[x].rch;
        else return x;
    }
    return 0;
}
void init(){
    init_env();
}
void solve(){
    int n = read();
	int opr, x;
	while (n--){
		opr = read(), x = read();
		if (opr == 1) Insert(x);
		if (opr == 2) Delete(x);
		if (opr == 3) printf("%d\n", Get_Rank(root, x));
		if (opr == 4) printf("%d\n", Get_Kth(root, x));
		if (opr == 5) printf("%d\n", Get_Predecessor(root, x));
		if (opr == 6) printf("%d\n", Get_Successor(root, x));
	}
}
int main(){
    init();
    solve();
    return 0;
}