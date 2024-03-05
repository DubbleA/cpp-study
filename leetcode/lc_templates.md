## Djikstra template:

### 1514. Path with Maximum Probability

```cpp
class Solution {
public:
    double maxProbability(int n, vector<vector<int>>& edges, vector<double>& succProb, int start_node, int end_node) {
        //Create a graph: node -> vector of pairs (neighboring node, probability)
       vector<vector<pair<int, double>>> graph (n);

        // Build the graph from the times vector
        for (int i = 0; i < edges.size(); ++i) {
            graph[edges[i][0]].emplace_back(edges[i][1], log2(succProb[i]));
            graph[edges[i][1]].emplace_back(edges[i][0], log2(succProb[i]));
        }

        // Min-heap to select the node with the minimum probability
        // Pair format: (probability, node)
        priority_queue<pair<double, int>> pq;

        // Start from node start_node with its initial probability
        pq.emplace(0, start_node);

        // Map to store the shortest time to reach each node
        vector<double> dist(n, -numeric_limits<double>::infinity()); // Use negative infinity to represent unvisited nodes
        dist[start_node] = 0; // Start node has a probability of 1, log2(1) = 0
        
        while(!pq.empty()){
            auto [prob, node] = pq.top(); pq.pop();
            // Skip if we have already found a min path to this node
            if (prob < dist[node]) continue; // Skip if we have found a better path already

            // Record the max prob to reach this node
            dist[node] = prob;

            // Update the probability for neighboring nodes
            for (const auto& [next, newProb] : graph[node]) {
                if (dist[next] < newProb + prob) {
                    dist[next] = newProb + prob;
                    pq.emplace(newProb + prob, next);
                }
            }
        }

        return dist[end_node] == -numeric_limits<double>::infinity() ? 0 : pow(2, dist[end_node]);
    }
};

// class Solution {
// public:
//     double maxProbability(int n, vector<vector<int>>& edges, vector<double>& succProb, int start_node, int end_node) {
//         // Create a graph: node -> vector of pairs (neighboring node, probability)
//         unordered_map<int, vector<pair<int, double>>> graph;

//         // Build the graph from the times vector
//         for (int i = 0; i < edges.size(); ++i) {
//             graph[edges[i][0]].push_back({edges[i][1], log2(succProb[i])});
//             graph[edges[i][1]].push_back({edges[i][0], log2(succProb[i])});
//         }

//         // Min-heap to select the node with the minimum probability
//         // Pair format: (probability, node)
//         priority_queue<pair<double, int>> pq;

//         // Start from node start_node with its initial probability
//         pq.push({0, start_node});

//         // Map to store the shortest time to reach each node
//         unordered_map<int, float> dist;

//         while(!pq.empty()){
//             auto [prob, node] = pq.top(); pq.pop();
//             // Skip if we have already found a min path to this node
//             if (dist.find(node) != dist.end()) continue;

//             // Record the max prob to reach this node
//             dist[node] = prob;

//             // Update the probability for neighboring nodes
//             for (const auto& [next, newProb] : graph[node]) {
//                 if (dist.find(next) == dist.end()) {
//                     pq.push({newProb + prob, next});
//                 }
//             }
//         }

//         return (dist.count(end_node) ? pow(2, dist[end_node]) : 0);
//     }
// };
```

### also this q:
https://leetcode.com/problems/network-delay-time/
