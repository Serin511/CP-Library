#ifndef PRIVATES_REAL_RAND
#define PRIVATES_REAL_RAND 1
#include <bits/stdc++.h>
using namespace std;
mt19937 rd((unsigned)chrono::steady_clock::now().time_since_epoch().count());
// uniform_int_distribution<int> rnd_int(l, r); // rnd_int(rd)
// uniform_real_distribution<double> rnd_real(0, 1);// rnd_real(rd)
#endif // PRIVATES_REAL_RAND