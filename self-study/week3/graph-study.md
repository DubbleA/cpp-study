### 2146. K Highest Ranked Items Within a Price Range

```cpp
class Solution {
    using T = tuple<int, int, int, int>; //distance, price, row, col
public:
    vector<vector<int>> highestRankedKItems(vector<vector<int>>& grid, vector<int>& pricing, vector<int>& start, int k) {
        queue<tuple<int,int,int>> q; //bfs q
        priority_queue<T, vector<T>, greater<>> pq; //rank q;
        vector<vector<int>> visited (grid.size(), vector<int> (grid[0].size(), false));

        q.emplace(0, start[0], start[1]);
        
        while(!q.empty()){
            auto [dis, r, c] = q.front(); q.pop();
            if(r < 0 or c < 0 or r >= grid.size() or c >= grid[0].size()) continue;
            if(visited[r][c] or grid[r][c] == 0) continue;
            visited[r][c] = true;
            if(pricing[0] <= grid[r][c] and pricing[1] >= grid[r][c]) pq.emplace(dis, grid[r][c], r, c);;
            // pq.emplace(dis, grid[r][c], r, c);

            q.emplace(dis + 1, r + 1, c);
            q.emplace(dis + 1, r - 1, c);
            q.emplace(dis + 1, r, c + 1);
            q.emplace(dis + 1, r, c - 1);
        }
        vector<vector<int>> res;
        while(!pq.empty() and res.size() < k){
            auto [dis, price, r, c] = pq.top(); pq.pop();
            res.push_back({r, c});
        }
        return res;
    }
};
```

### 743. Network Delay Time

```cpp
class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        
        // Create a graph: node -> vector of pairs (neighboring node, time)
        vector<vector<pair<int, int>>> graph (n); 

        // Build the graph from the times vector
        for(int i = 0; i < times.size(); ++i){
            graph[times[i][0] - 1].emplace_back(times[i][1] - 1, times[i][2]);
            //graph[times[i][1] - 1].emplace_back(times[i][0] - 1, times[i][2]);
            //not two way graph
        }

        // Min-heap to select the node with the minimum delay time
        // Pair format: (time, node)
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq; //time, node
        
        // Start from node K with initial delay time 0
        pq.emplace(0, k-1);

        // Map to store the shortest time to reach each node
        vector<int> map (n, INT_MAX);
        map[k-1] = 0;

        while(!pq.empty()){
            auto [dis, node] = pq.top(); pq.pop();
            // Skip if we have already found a shorter path to this node
            if(map[node] < dis) continue; 
            map[node] = dis;
            // Update the time for neighboring nodes
            for(const auto& [next, newdist] : graph[node]){
                //only continue algo if its unprocessed node
                if(map[next] == INT_MAX) pq.emplace(dis + newdist, next);
            }
        }
        int res = 0;
        for(auto& n: map){
            if(n == INT_MAX) return -1;
            res = max(n, res);
        }
        return res;
    }
};
```

### 2247. Maximum Cost of Trip With K Highways

```cpp
class Solution {
    using T = tuple<int, int, int>;
public:
    int maximumCost(int n, vector<vector<int>>& highways, int k) {
        vector<vector<pair<int,int>>> graph (n);

        for(const auto& h : highways){
            graph[h[0]].emplace_back(h[1], h[2]);
            graph[h[1]].emplace_back(h[0], h[2]);
        }
        int maxVal = INT_MIN;
        
        for(int i = 0; i < n; ++i){ //to ensure we visit every node
            priority_queue<T> pq;
            pq.emplace(0, 0, i);//cost, k, node
            vector<int> map (n, -1);
            map[i] = 0;

            while(!pq.empty()){
                auto [cost, stops, node] = pq.top(); pq.pop();
                if(stops == k) {
                    maxVal = max(maxVal, cost);
                    continue;
                }
                if(map[node] > 0) continue; 
                map[node] = cost;
                for(const auto& [next, toll] : graph[node]){
                    if(map[next] == -1) pq.emplace(cost + toll, stops + 1, next);
                }

            }
        }
        return maxVal == INT_MIN ? -1 : maxVal;
    }
};

```

