## 4/14 404. Sum of Left Leaves

```cpp
class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        int res = 0;
        dfs(root, res);
        return res;
    }
    void dfs(TreeNode* root, int& csum){
        if(!root) return;
        if(root->left) {
            if(!root->left->left and !root->left->right) csum += root->left->val;
        }
        dfs(root->left, csum);
        dfs(root->right, csum);
    }
};
```

## 4/12 42. Trapping Rain Water

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int i = 0, j = height.size() - 1, result = 0;
        int maxLeft = height[i], maxRight = height[j];

        while(i < j){
            if(maxLeft <= maxRight){
                maxLeft = max(maxLeft, height[++i]);
                result += maxLeft - height[i];
            }
            else{
                maxRight = max(maxRight, height[--j]);
                result += maxRight - height[j];
            }
        }
        return result;
    }
};
```

## 4/10 950. Reveal Cards In Increasing Order

```cpp
class Solution {
public:
    vector<int> deckRevealedIncreasing(vector<int>& deck) {
        priority_queue<int> pq (deck.begin(), deck.end());
        vector<int> res;
        res.emplace_back(pq.top()); pq.pop();

        //put last index first then put new value at the top
        while(!pq.empty()){
            res.insert(res.begin(), res.back());
            res.pop_back();
            res.insert(res.begin(), pq.top()); pq.pop();
        }

        return res;
    }
};
```

## 4/9 2073. Time Needed to Buy Tickets

```cpp
class Solution {
public:
    int timeRequiredToBuy(vector<int>& tickets, int k) {
        int pivot = tickets[k];
        int total = 0;
        for(int i = 0; i < tickets.size(); ++i){
            if(i <= k){
                total += min(tickets[i], tickets[k]);
            }
            else{
                total += min(tickets[i], tickets[k] - 1);
            }
        }
        return total;
    }
};
```

## 4/8 1700. Number of Students Unable to Eat Lunch

```cpp
class Solution {
public:
    int countStudents(vector<int>& students, vector<int>& sandwiches) {
        map<int, int> freqmap;
        for(auto n : students) freqmap[n]++;

        for(int i = 0; i < sandwiches.size(); ++i){
            if(freqmap[sandwiches[i]] == 0) return sandwiches.size() - i;
            freqmap[sandwiches[i]]--;
        }
        return 0;
    }
};
```

## 4/7 678. Valid Parenthesis String

```cpp
class Solution {
public:
    bool checkValidString(string s) {
        int unmatchedLeft = 0, rightNeeded = 0;
        for(char c : s){
            if(c == '(') unmatchedLeft++, rightNeeded++;
            else if(c == ')') rightNeeded--, unmatchedLeft = max(unmatchedLeft-1,0);
            else if(c == '*') rightNeeded++, unmatchedLeft = max(unmatchedLeft-1,0);
            if(rightNeeded < 0) return false; //found extra right parenthesis
        }
        return unmatchedLeft == 0;
    }
};
```

## 4/6 1249. Minimum Remove to Make Valid Parentheses

```cpp
class Solution {
public:
    string minRemoveToMakeValid(string s) {
        stack<pair<char, int>> stack;
        int i = 0;
        while(i < s.size()){
            if(s[i] == ')'){
                if(stack.empty()){
                    s.erase(i, 1);
                    continue;
                }
                auto [paren, _] = stack.top();
                if(paren == '(') stack.pop();
                else s.erase(i, 1);
            }
            else if (s[i] == '(') stack.emplace(s[i], i);
            ++i;
        }
        while(!stack.empty()){
            auto [_, index] = stack.top(); stack.pop();
            s.erase(index, 1);
        }   
        return s;
    }
};
```

## 4/5 1544. Make The String Great

```cpp
class Solution {
public:
    bool good(char a, char b){
        return (tolower(a) == tolower(b) && (isupper(a) != isupper(b)));
    }
    string makeGood(string s) {
        int i = 0;
        while(i < s.size() - 1){
            if(s.empty()) return "";
            if(good(s[i], s[i+1])){
                // Directly erase the current and next character
                s.erase(i, 2);
                //step back one to check if we can process the new adjacent
                if(i > 0) --i;
            }
            else ++i;
        }
        return s;
    }
};
```

## 4/4 1614. Maximum Nesting Depth of the Parentheses

```cpp
class Solution {
public:
    int maxDepth(string s) {
        int maxLeft = 0;
        int curLeft = 0;
        for(auto c : s){
            if(c == '(') curLeft++;
            else if(c == ')') curLeft--;
            maxLeft = max(maxLeft, curLeft);
        }
        return maxLeft;
    }
};
```

## 4/3 79. Word Search

```cpp
// O(M×N×L) Time where MxN is the board dimensions and L is length of the word
// O(L + K) Space where L is the recursive call stack and K for the length of the trie

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool end;
    string word;
    TrieNode() : end(false), word("") {}
};

class Solution {
public:
    TrieNode* root = new TrieNode();

    void insert(const string& word){
        TrieNode* temp = root;
        for(const auto& c : word){
            if(temp->children.find(c) == temp->children.end()){
                temp->children[c] = new TrieNode();
            }
            temp = temp->children[c]; //iterate to next node
        }
        temp->end = true;
        temp->word = word;
    }

    void dfs(vector<vector<char>>& board, int i, int j, TrieNode* curr, bool& ans){
        if(ans) return;
        if(i < 0 or i >= board.size() or j < 0 or j >= board[0].size()) return;
        if(board[i][j] == '#' or curr->children.find(board[i][j]) == curr->children.end()) return;
        char c = board[i][j];
        curr = curr->children[c];
        if(curr->end){
            ans = true;
            return;
        }
        board[i][j] = '#';
        dfs(board, i + 1, j, curr, ans);
        dfs(board, i - 1, j, curr, ans);
        dfs(board, i, j + 1, curr, ans);
        dfs(board, i, j - 1, curr, ans);
        board[i][j] = c; //backtrack
    }


    bool exist(vector<vector<char>>& board, string word) {
        insert(word);
        bool ans = false;
        for(int i = 0; i < board.size(); ++i){
            for(int j = 0; j < board[0].size(); ++j){
                dfs(board, i, j, root, ans);
                if(ans) return true;
            }
        }
        return false;
    }
};
```

## 4/2 205. Isomorphic Strings

```cpp
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        unordered_map<char, char> sMap,tMap;
        for(int i = 0; i < s.size(); ++i){
            if(sMap[s[i]] and sMap[s[i]] != t[i]) return false;
            if(tMap[t[i]] and tMap[t[i]] != s[i]) return false;
            sMap[s[i]] = t[i];
            tMap[t[i]] = s[i];
        }
        return true;
    }
};
```

## 4/1 58. Length of Last Word

```cpp
class Solution {
public:
    int lengthOfLastWord(string s) {
        s.erase(s.find_last_not_of(" \n\r\t")+1);
        return s.substr(s.find_last_of(" ") + 1 ).size();
    }
};
```

## 3/12 2485. Find the Pivot Integer

```cpp
class Solution {
public:
    int pivotInteger(int n) {
        int l = 1, r = n;

        int csum = 0, tsum = 0;
        for(int i = 1; i <= n; ++i) tsum += i;

        while(l <= r){
            csum += l;
            if(csum == tsum) return l;
            tsum -= l;
            l++;
        }

        return -1;
    }
};
```

## 3/11 1171. Remove Zero Sum Consecutive Nodes from Linked List

```cpp
class Solution {
public:
    ListNode* removeZeroSumSublists(ListNode* head) {
        //This solution first goes through the list 
        //to record the latest node for each cumulative sum.
        // Then, it traverses the list again, using the map 
        //to skip over any nodes that are part of a zero-sum 
        //sequence, effectively removing them by adjusting the next pointers.
        ListNode* dummy = new ListNode(0, head);
        ListNode* curr = dummy;
        unordered_map<int, ListNode*> prefixSumMap;
        prefixSumMap[0] = curr;
        
        int sum = 0;
        // Calculate the prefix sum for each node and add to the hashmap
        // Duplicate prefix sum values will be replaced
        while (curr) {
            sum += curr->val;
            prefixSumMap[sum] = curr;
            curr = curr->next;
        }

        curr = dummy;
        sum = 0;
        // Delete zero sum consecutive sequences 
        // by setting node before sequence to node after
        while (curr) {
            sum += curr->val;
            curr->next = prefixSumMap[sum]->next;
            curr = curr->next;
        }
        
        ListNode* res = dummy->next;
        delete dummy; // Clean up dummy node
        return res;
    }
};
```

## 3/10 791. Custom Sort String

```cpp
class Solution {
public:
    string customSortString(string order, string s) {
        map<char, int> m; 
        for(char c : s) m[c]++;
        string res;
        for(char c : order){
            res.append(m[c], c); // Append the character c, freq[c] times
            m[c] = 0; // Set the frequency to 0 to mark it as processed
        }
        for(auto [ch, freq] : m){
            res.append(freq, ch);
        }
        return res;
    }
};
```

## 3/9 349. Intersection of Two Arrays

```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> n1 (nums1.begin(), nums1.end()), n2 (nums2.begin(), nums2.end());
        vector<int> res;
        for(const auto& n : n1) if(n2.find(n) != n2.end()) res.emplace_back(n);
        return res;
    }
};
```

## 3/8 2540. Minimum Common Value

```cpp
class Solution {
public:
    int getCommon(vector<int>& nums1, vector<int>& nums2) {
        int n1 = 0, n2 = 0;
        int n1len = nums1.size(), n2len = nums2.size();

        while(n1 < n1len and n2 < n2len){
            if(nums1[n1] == nums2[n2]) return nums1[n1];
            else if(nums1[n1] < nums2[n2]) n1++;
            else if(nums1[n1] > nums2[n2]) n2++;
        }
        return -1;
    }
};
```

## 3/7 3005. Count Elements With Maximum Frequency

```cpp
class Solution {
public:
    int maxFrequencyElements(vector<int>& nums) {
        map<int, int> m; 
        int maxFreq = 1;
        for(auto& n : nums) {
            m[n]++;
            maxFreq = max(maxFreq, m[n]);
        }
        int res = 0;
        it wa
        for(auto [k, f] : m) if(f == maxFreq) res += f;
        return res;
    }
};
```

## 3/6 876. Middle of the Linked List

```cpp
class Solution {
public:
    ListNode* middleNode(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while(fast and fast->next){
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }
};
```

## 3/5 141. Linked List Cycle

```cpp
// O(N) Space
// O(1) Time
class Solution {
public:
    bool hasCycle(ListNode *head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while(fast and fast->next){
            slow = slow->next;
            fast = fast->next->next;
            if(slow == fast) return true;
        }
        return false;
    }
};
```

## 3/4 1750. Minimum Length of String After Deleting Similar Ends

```cpp
class Solution {
public:
    int minimumLength(string s) {
        int left = 0, right = s.size() - 1;
        while(left < right){
            if(s[left] == s[right]){
                while(s[left] == s[left+1] && left < right) left++;
                while(s[right] == s[right-1] && left < right) right--;
                left++;
                right--;
            }
            else return right - left + 1 > 0 ? right - left + 1 : 0;
        }
        return right - left + 1 > 0 ? right - left + 1 : 0;
    }
};
```

## 3/3 948. Bag of Tokens

```cpp
class Solution {
public:
    int bagOfTokensScore(vector<int>& tokens, int power) {
        sort(tokens.begin(), tokens.end());

        int score = 0;
        int left = 0, right = tokens.size() - 1;

        while(left <= right){
            //play lowest card
            if(power >= tokens[left]){
                power -= tokens[left];
                score++;
                left++;
            }
            //we need power use most score
            else if(score > 0 and left < right){
                score--;
                power += tokens[right];
                right--;
            }
            //cooked
            else break;
        }
        return score;
    }
};
```

## 3/2 19. Remove Nth Node From End of List

```cpp
// O(N) Time
// O(1) Space

class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        //slow fast method for n stops

        //first get a lead of size n with right pointer
        ListNode* right = head;
        for(int i = 0; i < n; ++i){
            right = right->next;
        }
        //if right is null then remove end of list
        if(!right) return head->next;
        ListNode* prev = nullptr;
        ListNode* left = head;

        //iterate one by one until end
        while(right){
            prev = left;
            left = left->next;
            right = right->next;
        }

        //skip nth node
        if(prev and left) prev->next = left->next;
        
        return head;
    }
};
```

## 3/1 977. Squares of a Sorted Array

```cpp
class Solution {
public:
    vector<int> sortedSquares(vector<int>& nums) {
        vector<int> result(nums.size());
        int left = 0, right = nums.size() - 1;
        for(int i = nums.size() - 1; i >= 0; i--){
            if(abs(nums[left]) > abs(nums[right])){
                result[i] = nums[left] * nums[left];
                left++;
            }
            else{ //right greater
                result[i] = nums[right] * nums[right];
                right--;
            }
        }
        return result;
    }
};
```

## 2/29 2864. Maximum Odd Binary Number

```cpp
class Solution {
public:
    string maximumOddBinaryNumber(string s) {
        int ones = 0;
        for(auto c : s) if (c == '1') ones++;
        if(ones == 1) return string(s.size() - 1, '0') + "1";
        
        string str = string(s.size() - 1, '0') + "1";
        ones--;
        int i = 0;
        while(ones > 0){
            str[i] = '1';
            i++;
            ones--;
        }
        return str;  
    }
};
```

## 2/28 1609. Even Odd Tree

```cpp
using T = tuple<TreeNode*, int>; //node, height
class Solution {
public:
    bool isEvenOddTree(TreeNode* root) {
        queue<T> q;
        q.push({root, 0});

        int prev = -1; // Initialize prev outside 
        int currHeight = 0;

        while(!q.empty()){
            auto [node, height] = q.front(); q.pop();
            if(!node) continue; //node is nullptr

            if(currHeight != height){
                currHeight = height;
                prev = (height % 2 == 0) ? -1 : INT_MAX; // Reset prev based on level
            }

            if(height % 2 == 0){ //even rules
                if(node->val % 2 != 1 or node->val <= prev) return false;
            }
            else{ //odd rules
                if(node->val % 2 != 0 or node->val >= prev) return false;
            }
            prev = node->val;

            q.push({node->left, height + 1});
            q.push({node->right, height + 1});
        }
        return true;
    }
};
```

## 2/27 513. Find Bottom Left Tree Value

```cpp
class Solution {
public:
    int findBottomLeftValue(TreeNode* root) {
        vector<vector<int>> heights;
        dfs(root, 0, heights);
        return heights[heights.size() - 1][0];
    }
    void dfs(TreeNode* root, int height, vector<vector<int>>& heights){
        if(!root) return;
        if(heights.size() == height) heights.push_back({});
        dfs(root->left, height + 1, heights);
        heights[height].push_back(root->val);
        dfs(root->right, height + 1, heights);
    }
};
```

## 2/26 543. Diameter of Binary Tree

```cpp
class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        //max height or max left + right
        int diameter = 0;
        dfs(root, diameter);
        return diameter;
    }
    int dfs(TreeNode* root, int& diameter){
        if(!root) return 0;
        int left = dfs(root->left, diameter);
        int right = dfs(root->right, diameter);
        diameter = max(diameter, left + right);
        return max(left, right) + 1;
    }
};
```

## 2/25 100. Same Tree

```cpp
class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        return dfs(p, q);
    }

    bool dfs(TreeNode* p, TreeNode* q){
        if(!p and !q) return true;
        if(p and !q or !p and q) return false;
        if(p->val != q->val) return false;
        return dfs(p->left, q->left) and dfs(p->right, q->right); 
    }
};
```

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
