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

reference solution:
```py
from queue import PriorityQueue

class Solution:
    def minimumCost(self, source, target, original, changed, cost):
        graph = {}
        for i, elem in enumerate(original):
            if elem not in graph:
                graph[elem] = {}
            graph[elem][changed[i]] = min(cost[i], graph[elem].get(changed[i], float("inf")))

        total = 0
        memo = {} # memoizes the shortest path from source to target
        
        for i, letter in enumerate(source):
            if letter != target[i]:
                result = self.find_path(source[i], target[i], graph, memo)
                if result == -1:
                    return -1
                total += result
        return total
    
    
    def find_path(self, source, target, graph, memo):
        if (source, target) in memo:
            return memo[(source, target)]
        
        pq = PriorityQueue()
        pq.put((0, source))
        seen = set()
        
        while not pq.empty():
            cost, letter = pq.get()
            if letter == target:
                memo[(source, target)] = cost
                return cost
            if letter not in seen:
                seen.add(letter)
                memo[(source, letter)] = cost
                edges = graph.get(letter, [])
                for edge in edges:
                    pq.put((cost + edges[edge], edge))
        return -1
```

explanation video: 
https://www.youtube.com/watch?v=oP4iBpYZUiI&t=241s


```cpp
class Solution {
public:
    long long minimumCost(string source, string target, vector<char>& original, vector<char>& changed, vector<int>& cost) {
        // char -> char -> cost
        //construct our graph where our source letter maps to a dictionary, 
        // and our target letter maps to cost of transitioning from source -> target. 
        unordered_map<char, unordered_map<char, int>> graph;
        for (int i = 0; i < original.size(); ++i) {
            if (graph[original[i]].find(changed[i]) == graph[original[i]].end()) {
                graph[original[i]][changed[i]] = cost[i];
            } else {
                graph[original[i]][changed[i]] = min(cost[i], graph[original[i]][changed[i]]);
            }
        }

        //set total to 0 and iterate through our source word. 
        //if the letter differs from the index of target, 
        //we need to check graph to find a path. 
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


    //djikstas
    //lexically sort by cost, while pq not empty get the current cost and letter. 
    // if letter == to target we reached destination
    // else add each neighbor and cost to the pq 

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

from: https://leetcode.com/problems/minimum-cost-to-convert-string-ii/solutions/4449800/java-c-python-js-explained-in-detail-dynamic-programming/

Intuition
To solve this problem, we need to find the minimum cost to convert the string source to the string target using a series of operations. The operations involve replacing substrings in source with corresponding substrings in target, and each operation has an associated cost. However, there are constraints on the selection of substrings: either the substrings are disjoint, or they are identical. This constraint adds complexity to the problem.

Approach
Create an index mapping (index) for each unique substring in original and changed. This mapping is used to represent the substrings as integers, making it easier to work with.
Build a distance matrix (dis) to represent the costs of converting one substring to another. Initialize it with maximum values and update it based on the given costs.
Use Floyd-Warshall algorithm to calculate the shortest distances between all pairs of substrings in dis.
Initialize a set (set) to store the lengths of unique substrings in original.
Initialize an array dp to store the minimum cost to convert a prefix of target to a prefix of source. Initialize dp[0] as 0.
Iterate through each character in target and update dp based on the possible operations and their costs.
The final answer is stored in dp[target.length()]. If it is not possible to convert source to target, return -1.
Complexity
Let n be the length of the strings, and m be the maximum length of a substring in original and changed.
Building the index mapping takes O(len(original) + len(changed)) time.
Constructing the dis matrix using the Floyd-Warshall algorithm takes O(m^3) time.
Constructing the set takes O(len(original)) time.
Iterating through the characters of target and updating dp takes O(n * m) time.
Thus, the overall time complexity is O(m^3 + n * m).
The space complexity is O(m^2) for the dis matrix, O(len(original)) for the set, and O(n) for the dp array.

```cpp
class Solution {
public:
    long long minimumCost(string source, string target, vector<string>& original, vector<string>& changed, vector<int>& cost) {
      unordered_map<string, int> index;
        for (const string& o : original) {
            if (!index.count(o)) {
                index[o] = index.size();
            }
        }
        for (const string& c : changed) {
            if (!index.count(c)) {
                index[c] = index.size();
            }
        }

        int n = index.size();
        vector<vector<long>> dis(n, vector<long>(n, LONG_MAX));

        for (int i = 0; i < cost.size(); ++i) {
            dis[index[original[i]]][index[changed[i]]] = min(dis[index[original[i]]][index[changed[i]]], (long)cost[i]);
        }

        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                if (dis[i][k] < LONG_MAX) {
                    for (int j = 0; j < n; ++j) {
                        if (dis[k][j] < LONG_MAX) {
                            dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
                        }
                    }
                }
            }
        }

        unordered_set<int> substrLengths;
        for (const string& o : original) {
            substrLengths.insert(o.length());
        }

        vector<long> dp(target.length() + 1, LONG_MAX);
        dp[0] = 0;

        for (int i = 0; i < target.length(); ++i) {
            if (dp[i] == LONG_MAX) {
                continue;
            }

            if (target[i] == source[i]) {
                dp[i + 1] = min(dp[i + 1], dp[i]);
            }

            for (int t : substrLengths) {
                if (i + t >= dp.size()) {
                    continue;
                }

                string subSource = source.substr(i, t);
                string subTarget = target.substr(i, t);

                int c1 = index.count(subSource) ? index[subSource] : -1;
                int c2 = index.count(subTarget) ? index[subTarget] : -1;

                if (c1 >= 0 && c2 >= 0 && dis[c1][c2] < LONG_MAX) {
                    dp[i + t] = min(dp[i + t], dp[i] + dis[c1][c2]);
                }
            }
        }

        return dp[dp.size() - 1] == LONG_MAX ? -1L : dp[dp.size() - 1];
    }
};
```


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

### Question 2: 2997. Minimum Number of Operations to Make Array XOR Equal to K

explanation video: https://www.youtube.com/watch?v=Fhe2y3HgRvk

"The code calculates the minimum operations needed to make each bit of the binary representation of integers in the array equal to the corresponding bit of the target 'k'. It iterates through each bit, checking if XOR operation is required for matching bits, updating the answer accordingly."

3 line solution:
```cpp
// Count bits of XOR(A) ^ k
int minOperations(vector<int>& A, int k) {
    for (int a : A)
        k ^= a;
    return __builtin_popcount(k);
}
```

### Question 3: 2998. Minimum Number of Operations to Make X and Y Equal

```cpp
// Approach: We need to check for 5 ways in our recursive calls :

// 1. Just abs diff of x & y can be ans. So initialise res = abs(x - y)
// 2. Check for multiple of 5 which is smaller than x. This can be 
//achieved by just subtracting x%5 from x and divide x by 5. Here total 
//operations = x%5 ( this many time decreament ) + 1 (for division by 5).
// 3. Check for multiple of 5 which is larger than x. Adding (5 - x%5) 
//to x and then divide x by 5. Here total operations = 5 - x%5 
//(this many time increment ) + 1(for division by 5).
// 4. 11 which is smaller than x. 
// 5. 11 which is larger than x. 


//O(N) Time & Space

class Solution {
public:
    int minimumOperationsToMakeEqual(int x, int y) {
        vector<int> dp (10011, -1);
        return solve(x, y, dp);
    }

    int solve(int x, int y, vector<int>& dp){
        if(x <= y) return y - x; //increment by 1 until theyre equal
        if(dp[x] != -1) return dp[x]; //x has already been processed
        int result = abs(x - y);
        result = min(result, 1 + x%5 + solve(x/5, y, dp));
        result = min(result, 1 + (5-x%5) + solve(x/5 + 1, y, dp));
        result = min(result, 1 + x%11 + solve(x/11, y, dp));
        result = min(result, 1 + (11-x%11) + solve(x/11 + 1, y, dp));
        dp[x] = result;
        return result;
    }
};
```

### Question 4: 2999. Count the Number of Powerful Integer

https://leetcode.com/problems/count-the-number-of-powerful-integers/solutions/4518645/easy-fast-cpp-solution-0ms-100-beat-for-loop/

```cpp
class Solution {
public:
    long long numberOfPowerfulInt(long long start, long long finish, int limit, string s) {
        string start_ = to_string(start-1), finish_ = to_string(finish);

        return calculate(finish_, s, limit)-calculate(start_, s, limit);
    }

    long long calculate(string num, string s, int limit){
        // it will find powerful number less than or equal num

        if(num.length()<s.length())
            return 0;

        if(num.length()==s.length()){
            return num >= s ? 1 : 0;
        }

        string s_ = num.substr(num.length() - s.length(), s.length());
        //it is the suffix of num of length of s.length

        long long ans = 0;
        int n = num.length() - s.length();

        for (int i = 0; i < n;i++){
            if(limit<(num[i]-'0')){
                ans+=(long)pow(limit+1, n-i);
                return ans;
                //if current digit is larger than limit then we can take all combinations with limit+1 (including 0) digits
            }
            ans += (long)(num[i] - '0') * (long)pow(limit + 1, n - 1 - i);
            //0 to num[i]-1 have no problem with rest of digits [0->limit]
            //for num[i] we need to check the rest of digits to ensure all combinations do not exceed the num

        }
        if(s_>=s)
            ans++;
        return ans;
    }
};
```