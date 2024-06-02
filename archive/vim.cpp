#include <vector>
#include <algorithm>
using namespace std;

struct DisjointSet {
    vector<int> parent;
    vector<int> size;

    DisjointSet(int maxSize) {
        parent.resize(maxSize);
        size.resize(maxSize);
        for (int i = 0; i < maxSize; i++) {
            parent[i] = i;
            size[i] = 1;
        }
    }

    int find_set(int v) {
        if (v == parent[v])
            return v;
        return parent[v] = find_set(parent[v]);
    }

    void union_set(int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            if (size[a] < size[b])
                swap(a, b);
            parent[b] = a;
            size[a] += size[b];
        }
    }
};

class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        DisjointSet S(edges.size());
        for(const auto& edge : edges){
            S.union_set(edge[0], edge[1]);
            if(S.find_set(edge[1]) != S.find_set(edge[0])) return edge;
        }
        return {-1, -1};
    }
};