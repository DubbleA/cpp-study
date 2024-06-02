#include <vector>

using namespace std;

vector<int> calculateSubsets(const int& numTeams, vector<int>& scores){
    vector<pair<int, int>> v(scores.size()); //score, idx

    for(int i = 0; i < scores.size(); ++i) v[i] = {scores[i], i};
    sort(v.begin(), v.end());

    //1 group diversity score = v.back() - v[0]
    //2 group diversity score = max(r1 - l1, r2 - l2)
    ...
    //group cannot be formed


}
