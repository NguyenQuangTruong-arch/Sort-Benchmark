#include <bits/stdc++.h>
#define endl '\n'
#define ll long long
using namespace std;

mt19937_64 rng(25120460);
const ll LOW = -2147483648LL;
const ll HIGH = 2147483647LL;

ll rnd(ll l, ll r) { return rng() % (r - l + 1) + l; }

int main() {
    ofstream inp("test001.in");
    int n = 100000;
    inp << n << endl;
    for (int i = 0; i < n; i++) {
        inp << (rng() % 2 == 0 ? -2147483648LL : 2147483647LL) << endl;
    }
    inp.close();
    for (int i = 2; i <= 5; i++) {
        ofstream inp("test00" + to_string(i) + ".in");
        int n = rnd(1, 100000);
        inp << n << endl;
        for (int j = 0; j < n; j++) {
            inp << rnd(LOW, HIGH) << endl;
        }
        inp.close();
    }
    return 0;
}
