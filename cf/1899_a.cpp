#include <iostream>
using namespace std;

void solve() {
    int n;
    cin >> n;

    if (n % 3) {
        cout << "First" << endl;
    } else {
        cout << "Second" << endl;
    }
}

int main(){
  ios_base::sync_with_stdio(0); cin.tie(0);
  int t; cin >> t; while (t--) solve();
  return 0;
}
