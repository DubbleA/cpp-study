## Weekly Contest 377 12/23/23

### Question 1: 2974. Minimum Number Game

You are given a 0-indexed integer array nums of even length and there is also an empty array arr. Alice and Bob decided to play a game where in every round Alice and Bob will do one move. The rules of the game are as follows:

Every round, first Alice will remove the minimum element from nums, and then Bob does the same.
Now, first Bob will append the removed element in the array arr, and then Alice does the same.
The game continues until nums becomes empty.
Return the resulting array arr.

```cpp
class Solution {
public:
    vector<int> numberGame(vector<int>& nums) {
        vector<int> result; 
        stack<int> s;
        multiset<int> ms (nums.begin(), nums.end());
        int i = 0;
        for(auto it = ms.begin(); it != ms.end(); it++){
            if(i % 2 == 0 && !s.empty()){
                result.push_back(s.top()); s.pop();
                result.push_back(s.top()); s.pop();
            }
            s.push(*it);
            i++;
            
        }
        
        while(!s.empty()){
            result.push_back(s.top()); s.pop();

        }
        
        return result;
        
    }
};
```

### Question 2: 2975. Maximum Square Area by Removing Fences From a Field

Notes: Greedy + Impl

Time Complexity:

1. **Sorting Fences**:
   - Sorting `hFences` and `vFences` requires `O(H log H)` and `O(V log V)` time respectively, where `H` is the number of horizontal fences and `V` is the number of vertical fences.

2. **Generating Gaps**:
   - The nested loops for generating `hgaps` and `vgaps` run in `O(V^2)` and `O(H^2)` time respectively. This is because for each element in the fence lists, the loops compare it with every other fence.

3. **Sorting Gaps and Searching**:
   - Sorting `vgaps` is done in `O(H^2 log H^2)` time. The subsequent search for a common gap in `hgaps` and `vgaps` has a worst-case time complexity of `O(H^2)`.

Overall, the time complexity of the entire function is dominated by the gap generation and sorting steps, leading to a complexity of `O(H^2 log H^2 + V^2 log V^2)`.

Space Complexity:

1. **Storing Gaps**:
   - Additional space is used for storing `hgaps` and `vgaps`, each of which can have a maximum size of `O(H^2)` and `O(V^2)` respectively.

2. **Additional Data Structures**:
   - An `unordered_map` is used for storing `hgaps`, which requires `O(H^2)` space.

Hence, the overall space complexity is `O(H^2 + V^2)`, considering the additional space required for storing the gaps and the hash map.

```cpp
class Solution {
public:
    int maximizeSquareArea(int m, int n, vector<int>& hFences, vector<int>& vFences) {
        
        vector<int> vgaps; // To store vertical gaps
        vector<int> hgaps; // To store horizontal gaps
        
        // Including boundaries as fences
        hFences.push_back(m); 
        hFences.push_back(1);
        vFences.push_back(n); 
        vFences.push_back(1);
        
        // Sorting the fences
        sort(vFences.begin(), vFences.end());
        sort(hFences.begin(), hFences.end());
                
        // Finding all possible horizontal gaps
        for(int i = 0; i < vFences.size() - 1; ++i)
            for(int j = i + 1; j < vFences.size(); ++j)            
                hgaps.push_back(vFences[j] - vFences[i]);
        
        // Finding all possible vertical gaps
        for(int i = 0; i < hFences.size() - 1; ++i)
            for(int j = i + 1; j < hFences.size(); ++j)
                vgaps.push_back(hFences[j] - hFences[i]);
        
        // Sorting vertical gaps for binary search
        sort(vgaps.begin(), vgaps.end());
        
        // Hash map to store horizontal gaps
        unordered_map<int, int> hm;
        for(int i = 0; i < hgaps.size(); ++i)
            hm[hgaps[i]] = 1;
        
        long long res = -1; // Result initialization
        
        // Checking for the largest common gap
        for(int i = vgaps.size() - 1; i >= 0; --i) {
            if(hm[vgaps[i]] == 1) { // If common gap found
                long long l = vgaps[i];
                res = l * l; // Calculate square area
                res = res % 1000000007; // Modulo operation
                break;
            }
        }

        return res;
    }
};
```

### Question 3: 2976. Minimum Cost to Convert String I

explanation video: 
https://www.youtube.com/watch?v=oP4iBpYZUiI&t=241s

```cpp
class Solution {
public:
    long long minimumCost(string source, string target, vector<char>& original, vector<char>& changed, vector<int>& cost) {
        // char -> char -> cost
        
        unordered_map<char, unordered_map<char, int>> graph;
        for (int i = 0; i < original.size(); ++i) {
            if (graph[original[i]].find(changed[i]) == graph[original[i]].end()) {
                graph[original[i]][changed[i]] = cost[i];
            } else {
                graph[original[i]][changed[i]] = min(cost[i], graph[original[i]][changed[i]]);
            }
        }

        long long total = 0;

        // memoize the shortest path
        unordered_map<char, unordered_map<char, int>> memo;

        for (int i = 0; i < source.length(); ++i) {
            if (source[i] != target[i]) {
                int result = findPath(source[i], target[i], graph, memo);
                if (result == -1) return -1; //path doesnt exist
                total += result;
            }
        }

        return total;
    }

    long long findPath(char source, char target, unordered_map<char, unordered_map<char, int>>& graph, unordered_map<char, unordered_map<char, int>>& memo) {
        if (memo[source].find(target) != memo[source].end()) {
            return memo[source][target];
        }

        priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>> pq;
        pq.push({0, source});
        unordered_map<char, int> costs;
        costs[source] = 0;

        while (!pq.empty()) {
            auto [cost, letter] = pq.top();
            pq.pop();

            if (letter == target) {
                memo[source][target] = cost;
                return cost;
            }

            if (graph.find(letter) != graph.end()) {
                for (const auto& edge : graph[letter]) {
                    char nextLetter = edge.first;
                    int nextCost = cost + edge.second;

                    if (!costs.count(nextLetter) || nextCost < costs[nextLetter]) {
                        costs[nextLetter] = nextCost;
                        pq.push({nextCost, nextLetter});
                    }
                }
            }
        }

        memo[source][target] = -1; // No path found
        return -1;
    }
};
```

### Question 4: 2977. Minimum Cost to Convert String II



## Biweekly Contest 121

### Question 1: 10031. Smallest Missing Integer Greater Than Sequential Prefix Sum

Return the smallest integer x missing from nums such that x is greater than or equal to the sum of the longest sequential prefix.

```cpp
class Solution {
public:
    int missingInteger(vector<int>& nums) {
        int csum = nums[0];
        for(int i = 1; i < nums.size(); ++i){
            if(nums[i] - nums[i-1] != 1) break;
            csum += nums[i];
        }
        unordered_set<int> s (nums.begin(), nums.end());
        while(true){
            if(s.find(csum) == s.end()) return csum;
            csum++;
        }
        return -1;
    }
};
```
