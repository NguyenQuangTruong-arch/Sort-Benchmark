#include <bits/stdc++.h>
#define endl '\n'
#define ll long long
using namespace std;

mt19937_64 rng(25120460);

ll rnd(ll l, ll r) { return rng() % (r - l + 1) + l; }

int main() {
    for (int i = 1; i <= 5; i++) {
        ofstream inp("test00" + to_string(i) + ".in");
        int n = rnd(1, 10000);
        inp << n << endl;
        for (int j = 0; j < n; j++) {
            int m = rnd(10, 100);
            for (int k = 0; k < m; k++) {
                inp << char(rnd(97, 122));
            }
            inp << endl;
        }
        inp.close();
    }
    return 0;
}
