## 2/24 2709. Greatest Common Divisor Traversal

From solutions tab:
```cpp
class Graph {
private:
    int n;
    vector<int> parent, compSize;

    int getParent(int x) {
        if (parent[x] == x) {
            return x;
        }
        return parent[x] = getParent(parent[x]);
    }

    void unionSet(int x, int y) {
        int parx = getParent(x), pary = getParent(y);
        if (parx != pary) {
            if (compSize[parx] < compSize[pary]) {
                swap(parx, pary);
            }
            parent[pary] = parx;
            compSize[parx] += compSize[pary];
        }
    }

public:
    Graph(int n = 0) : n(n) {
        parent.resize(n);
        compSize.resize(n, 1);
        iota(parent.begin(), parent.end(), 0);
    }
    
    void addEdge(int x, int y) {
        unionSet(x, y);
    }

    bool isConnected() {
        return compSize[getParent(0)] == n;
    }
};

class Solution {
private:
    vector<int> getPrimeFactors(int x) {
        vector<int> primeFactors;
        for (int i = 2; i * i <= x; i++) {
            if (x % i == 0) {
                primeFactors.push_back(i);
                while (x % i == 0) {
                    x /= i;
                }
            }
        }
        if (x != 1) {
            primeFactors.push_back(x);
        }
        return primeFactors;
    }

public:
    bool canTraverseAllPairs(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) {
            return true;
        }
        Graph g(n);
        unordered_map<int, int> seen;
        for (int i = 0; i < n; i++) {
            if (nums[i] == 1) {
                return false;
            }
            vector<int> primeFactors = getPrimeFactors(nums[i]);
            for (int prime: primeFactors) {
                if (seen.find(prime) != seen.end()) {
                    g.addEdge(i, seen[prime]);
                } else {
                    seen[prime] = i;
                }
            }
        }
        return g.isConnected();
    }
};
```

TLE Solutions:

```cpp
struct DisjointSet{
private:
    std::vector<int> parent;
    std::vector<int> size;
    int cnt;
    int mx;
public:
    DisjointSet(int n): parent(n,0), size(n, 1), cnt{n}, mx(1) {
        std::iota(begin(parent), end(parent), 0);
    }
    int find(int n){
        if(n == parent[n]) return n;
        return parent[n] = find(parent[n]);
    }
    void combine(int x, int y){
        auto rootX = find(x);
        auto rootY = find(y);
        if(rootX == rootY) return;

        if(size[rootX] < size[rootY]) std::swap(rootX, rootY);
        size[rootX] += size[rootY];
        parent[rootY] = rootX;
        mx = std::max(mx, size[rootX]);
        cnt--;
    }
    bool connected(int x, int y){
        return find(x) == find(y);
    }
    int count() const {
        return cnt;
    }
    int biggest() const {
        return mx; 
    }
};

class Solution {
public:
    bool canTraverseAllPairs(vector<int>& nums) {
        int n = nums.size();
        DisjointSet ds(n);

        // Iterate through all pairs of indices
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                // If gcd(nums[i], nums[j]) > 1, merge their sets
                if (gcd(nums[i], nums[j]) > 1) {
                    ds.combine(i, j);
                }
            }
        }

        // If the count of distinct sets is 1, all pairs are connected
        return ds.count() == 1;
    }
};


//TLE
    bool canTraverseAllPairs(vector<int>& nums) {
        // Build a graph where edges exist between indices i and j if gcd(nums[i], nums[j]) > 1
        vector<vector<int>> graph(nums.size());
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = i + 1; j < nums.size(); ++j) {
                //c++17 gcd: 
                if (gcd(nums[i], nums[j]) > 1) {
                    graph[i].push_back(j);
                    graph[j].push_back(i);
                }
            }
        }

        // Check if all nodes are connected
        for (int i = 0; i < nums.size(); ++i) {
            vector<bool> visited(nums.size(), false);
            if (!bfs(graph, visited, i)) {
                return false;
            }
        }

        return true;
    }

    bool bfs(const vector<vector<int>>& graph, vector<bool>& visited, int start) {
        queue<int> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int current = q.front(); q.pop();

            for (int next : graph[current]) {
                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }

        // Check if all nodes were visited
        return all_of(visited.begin(), visited.end(), [](bool v) { return v; });
    }
```

## 2/23 2092. Find All People With Secret

```cpp
class Solution {
public:
    vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson) {
        //create the graph between ppl
        vector<vector<pair<int,int>>> graph(n);
        for(auto& mt: meetings){
            graph[mt[0]].push_back({mt[1], mt[2]}); //person 1 talks person 2 at t
            graph[mt[1]].push_back({mt[0], mt[2]}); //person 2 talks person 1 at t
        }

        //minheap sorting by time
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> q;

        q.push({0, firstPerson}); //person 0 tells first person time 0
        q.push({0, 0}); //person 0 knows at time 0

        vector<bool> visited(n, false);
        //mark person true if they have met and shared to all possible ppl

        while(!q.empty()){
            auto [time, person] = q.top(); q.pop();

            if(visited[person]) continue; //if person alr processed / shared
            visited[person] = true;

            //iterate all the meetings of this person and add the potential ppl in q;
            for(auto [neighbor, newTime] : graph[person]){
                if(!visited[neighbor] && time <= newTime){
                    q.push({newTime, neighbor});
                }
            }
        }
        vector<int> res;
        for(int i = 0; i < visited.size(); ++i) if(visited[i]) res.push_back(i);
        return res;

    }
};

//TLE
// class Solution {
// public:
//     vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson) {
//         map<int, vector<pair<int, int>>> graph; //timestamp p1 p2
//         // graph[0].push_back({0, firstPerson}); //person 0 tells first person
//         for(auto mtg : meetings) graph[mtg[2]].push_back({mtg[0], mtg[1]});

//         vector<bool> known(n, false);
//         known[0] = true; // Person 0 knows the secret
//         known[firstPerson] = true; // firstPerson knows the secret

//         for (const auto& [time, pairs] : graph) {
//             // Temporarily track who learns the secret in this round
//             vector<bool> tempKnown = known;
//             queue<int> q;
//             // Add everyone who already knows the secret to the queue
//             for (int i = 0; i < n; ++i) {
//                 if (known[i]) q.push(i);
//             }
            
//             while (!q.empty()) {
//                 int person = q.front(); q.pop();
//                 for (const auto& [p1, p2] : pairs) {
//                     if (p1 == person && !tempKnown[p2]) {
//                         tempKnown[p2] = true;
//                         q.push(p2);
//                     } else if (p2 == person && !tempKnown[p1]) {
//                         tempKnown[p1] = true;
//                         q.push(p1);
//                     }
//                 }
//             }
            
//             // Update the known status for the next timestamp
//             for (int i = 0; i < n; ++i) {
//                 if (tempKnown[i]) known[i] = true;
//             }
//         }

//         vector<int> res;
//         for(int i = 0; i < known.size(); ++i) {
//             if(known[i]) res.push_back(i);
//         }
//         return res;
//     }
// };
```

```cpp
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        unordered_map<int, vector<pair<int,int>>> graph; //from -> to, price

        for(auto& vec: flights){
            graph[vec[0]].push_back({vec[1], vec[2]});
        }

        // Queue for BFS: Each element is a tuple (node, cost, stops)
        queue<vector<int>> q;
        q.push({src, 0, 0});

        // Vector to track the minimum cost to reach each node
        vector<int> minCost(n, INT_MAX);
        minCost[src] = 0;

        while(!q.empty()){
            auto front = q.front(); q.pop();
            int node = front[0], cost = front[1], stops = front[2];
            // If the number of stops exceeds K, continue to the next iteration
            if (stops > k) continue;

            for(auto& [neighbor, price] : graph[node]){
                int nextCost = cost + price;
                if(nextCost < minCost[neighbor]){
                    minCost[neighbor] = nextCost;
                    q.push({neighbor, nextCost, stops + 1});
                }
            }
        }
        // Check if the destination is reachable within K stops
        return minCost[dst] == INT_MAX ? -1 : minCost[dst];
    }

};
```

## 2/22/24 997. Find the Town Judge

```cpp
class Solution {
public:
    int findJudge(int n, vector<vector<int>>& trust) {
        if(n <= 1) return n;
        unordered_set<int> s; //trustees
        unordered_map<int, int> m; //trust freq
        for(auto& vec: trust) {
            s.insert(vec[0]);
            m[vec[1]]++;
        };
        for(auto& vec: trust) if(s.find(vec[1]) == s.end()) {
            if(m[vec[1]] == n - 1) return vec[1];
        };
        return -1;

    }
};
```

## 2/21/24 201. Bitwise AND of Numbers Range

```cpp
class Solution {
public:
    int rangeBitwiseAnd(int left, int right) {
        int count = 0;
        //keep right shifting until the "prefix" of both numbers match
        while(left != right){
            left >>= 1; 
            right >>= 1;
            count++;
        }
        //then left shift the amount of times we shifted to get our "matching" #
        return left << count;
    }
};

TLE Solution:
// int rangeBitwiseAnd(int left, int right) {
//     int res = left;
//     while(left < right){
//         res &= left;
//         left++;
//     }
//     return res & right;
// }
```

## 2/20/24 268. Missing Number

```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        //gauss formula
        int tsum = 0, vsum = 0;
        int n = nums.size();
        for(int i = 0; i <= n; ++i) tsum += i;
        for(int i = 0; i < n; ++i) vsum += nums[i];
        return tsum - vsum;
    }

    int missingNumber(vector<int>& nums) {
        //bit manip
        //the surviving number will be our number
        int missing = nums.size();
        for (int i = 0; i < nums.size(); ++i) missing ^= i ^ nums[i];
        return missing;
    }
};
```

## 2/19/24 231. Power of Two
```cpp
class Solution {
public:
    bool isPowerOfTwo(int n) {
        return n > 0 and !(n & n-1);
    }
};

// since powers of two only have 1 one in the entire binary representation
// one minus a power of two is all 1's in its representation i.e. 8 (1000) 7 (111)
// so if (1000) & (0111) == 0000 since bitwise and needs both digits to be one
// so !(0000) == 1 therefore its a power of two


// 001 2
// 10000 16
// 11 3
// 100 8
// 110 6


// bool isPowerOfTwo(int n) {
//     if(n < 0) return false;
//     //count number of 1's in binary representation
//     unsigned int count = 0;
//     while (n) {
//         count += n & 1;
//         n = n >> 1;
//     }
//     return count == 1 ? true : false;
// }
```

## 2/18/24 2402. Meeting Rooms III

```cpp
class Solution {
public:
    int mostBooked(int n, vector<vector<int>>& meetings) {
        sort(meetings.begin(), meetings.end());
        // pq in asc order to manage next available time for each room
        // Pair: <next available time, room index>
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        
        // Initially, all rooms are available at time 0
        for (int i = 0; i < n; ++i) pq.push({0, i}); 

        // Count how many times each room is used
        vector<int> countVec(n, 0); 
        int maxCount = 0;

        for(auto& mtng: meetings){
            long long start = mtng[0], end = mtng[1];
            //necessary to make sure that nextAvailable is at least
            //at the start time
            while(pq.top().first < start){
                pq.push({start, pq.top().second}); pq.pop();
            }
            // Wait for the next available room
            auto [nextAvailable, roomId] = pq.top(); pq.pop();
            //the time has to be at least the start time + duration
            pq.push({nextAvailable + (end - start), roomId});
            countVec[roomId]++;
            maxCount = max(maxCount, countVec[roomId]);
        }

        for(int i = 0; i < n; ++i){
            if(countVec[i] == maxCount) return i;
        }
        return 0;
    }
};

//TLE SOLUTION:
// int mostBooked(int n, vector<vector<int>>& meetings) {
//     sort(meetings.begin(), meetings.end());
//     vector<int> qVec(n, -1), countVec(n, 0);
//     queue<pair<int, int>> q;
//     for(auto& vec : meetings) q.push({vec[0], vec[1] - vec[0]});
//     int time = 0, maxCount = 0;
//     while(!q.empty()){
//         for(int i = 0; i < n; ++i){
//             if(q.empty()) break;
//             if ((qVec[i] == -1 or qVec[i] <= time) && q.front().first <= time){
//                 auto [_, dur] = q.front(); q.pop();
//                 qVec[i] = time + dur;
//                 countVec[i]++;
//                 maxCount = max(maxCount, countVec[i]);
//             }
//         }
//         time++;
//     }
//     for(int i = 0; i < n; ++i){
//         if(countVec[i] == maxCount) return i;
//     }
//     return 0;
// }
```

## 2/17/24 1642. Furthest Building You Can Reach

```cpp
class Solution {
public:
    int furthestBuilding(vector<int>& heights, int bricks, int ladders) {
        //save a pq of ladders sorted in descending order of size ladders
        priority_queue<int> pq;

        for(int i = 0; i < heights.size() - 1; ++i){
            int j = i + 1;
            if(heights[i] >= heights[j]) continue; //same or less height can jump
            pq.push(heights[j] - heights[i]); // keep track of highest highs
            bricks -= heights[j] - heights[i];
            while(bricks < 0 && !pq.empty()) { //while we have ladders dec bricks
                bricks += pq.top(); pq.pop();
                ladders--;
            }
            if(ladders < 0 or bricks < 0) return i;
        }
        return (ladders < 0 or bricks < 0) ? - 1 : size(heights) - 1;
    }
};
```

## 2/16/24: 1481. Least Number of Unique Integers after K Removals

```cpp
class Solution {
public:
    int findLeastNumOfUniqueInts(vector<int>& arr, int k) {
        //k freq to pop?
        unordered_map<int, int> m; //num -> freq
        for(auto& n : arr) m[n]++;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int,int>>> pq; //freq, num; 
        for(auto [n , f] : m) pq.push({f, n});
        while(k > 0){
            auto [f, n] = pq.top();
            if(k >= f){
                pq.pop();
            }
            k -= f;
        }
        return pq.size();
    }
};
```

## 2/15/24: 2971. Find Polygon With the Largest Perimeter

```cpp
class Solution {
public:
    long long largestPerimeter(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        long long csum = 0;
        for(auto& n: nums) csum += n;
        int sides = 0;
        for(int i = nums.size() - 1; i >= 0; --i){
            if(nums[i] < csum - nums[i]){
                sides = i + 1; 
                break;
            }
            csum -= nums[i];
        }
        return sides >= 3 ? csum : -1;
    }
};
```

## 2/14/24: 2149. Rearrange Array Elements by Sign

```cpp
class Solution {
public:
    vector<int> rearrangeArray(vector<int>& nums) {
        queue<int> neg, pos;
        for(auto& n: nums) n >= 0 ? pos.push(n) : neg.push(n);
        vector<int> result;
        while(!neg.empty() && !pos.empty()){
            result.push_back(pos.front()); pos.pop();
            result.push_back(neg.front()); neg.pop();
        }
        return result;
    }
};
```
