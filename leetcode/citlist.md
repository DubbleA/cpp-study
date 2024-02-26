## 355. Design Twitter
Notes: 
```cpp
unordered_map<int, unordered_set<int>> m; //maps followerId -> followeeID
priority_queue<pair<int, pair<int, int>>> pq; // pair of (timestamp, (tweetId, userId))
int time; // Global timestamp
```

```cpp
class Twitter {
public:
    unordered_map<int, unordered_set<int>> m; //maps followerId -> followeeID
    priority_queue<pair<int, pair<int, int>>> pq; // Now using a pair of (timestamp, (tweetId, userId))
    int time; // Global timestamp
    Twitter() {
        
    }
    
    void postTweet(int userId, int tweetId) {
        pq.push({++time, {tweetId, userId}});
    }
    
    vector<int> getNewsFeed(int userId) {
        priority_queue<pair<int, pair<int, int>>> pqTemp = pq;
        unordered_set<int> followedUsers = m[userId];
        vector<int> result;
        followedUsers.insert(userId); // Including user's own tweets

        while(!pqTemp.empty() && result.size() < 10){
            auto [_, tweetInfo] = pqTemp.top(); pqTemp.pop();
            auto [tweetId, userId] = tweetInfo;
            if(followedUsers.find(userId) != followedUsers.end()){
                result.push_back(tweetId);
            }
        }

        return result;
    }
    
    void follow(int followerId, int followeeId) {
        m[followerId].insert(followeeId);
    }
    
    void unfollow(int followerId, int followeeId) {
        m[followerId].erase(followeeId);
    }
};
```

Design Twitter | maps, User should have linked list of tweets, get feed similar
to merge k sorted lists

Alien Dict (Page 1) | Topo Sort, Use the words to get graph

Best time to buy/sell stock (k times, 2 times ) (Page 1) | DP + Variable for counter, State: (index, cap, isBuy)

median of 2 sorted arrays (Page 1) | Binary search, partition both arrays, if odd more elem on left

k smallest elements (Page 2) | nth_element in stl

N Queens (Page 1) | Backtraking by row, checking is impl heavy can be done in one loop

2 sum problem with doubles | Use epsilon/brute force 1e-9

Sort the list with the order of encountering 

different ways to add parentheses | Split on Operator eval(first), eval(rest) then combine

Longest matching parenthesis (Page 1)

traversing a matrix in a spiral | Right, Down, Left, Up, repeat

jump game

rotate array | 3x Reverse

word search | DFS

min stack | 2 stacks one for data, one for mins, push/pop on min after

comparing values

expression add operators

longest increasing path | DFS + DP, Can be solved with topo sort

merge intervals | Sort By start, prev.end >= curr.start then merge

overlapping intervals | Equiv to schedule max meetings (sort by finish time)

least frequently used cache X
Map of freq -> LRU Cache, keep track of min freq, Map key -
> Node pointer/ittr
autocompleter
circular buffer X Keep track of L/R or Head and count
Skyline X
Sort points, use multiset, add new point on height change,
multiset like pq
Find cycle in graph X Union Find (better) can be DFS / Topo sort for directed
2812. Find the Safest Path in a Grid X Use BFS to preprocess them dijkstras or BSF + binary search
union of 2 lists ?
Search in Rotated Sorted Array REDO
Rotting Oranges X BFS, Keep track of fresh and max time
Trap rainwater X
 water(i) = min(max_left, max_right) - hi. O(N) space using
prefix/suffix, O(1) space using 2 pointer
https://leetcode.com/problems/spiral-matrix/ X
Edge case when matrix is just one row/rect. Add check for top
<= bottom, and left <= right. in last 2 loops
https://leetcode.com/problems/spiral-matrix-ii/ X
https://leetcode.com/problems/rotate-image/
https://leetcode.com/problems/diagonal-traverse/
https://leetcode.com/problems/reshape-the-matrix/
https://leetcode.com/problems/toeplitz-matrix/
https://leetcode.com/problems/image-overlap/
https://leetcode.com/problems/transpose-matrix/
Find closest palindrome X
If single digit return n - 1. Otherwise there are 5 cases all 9s,
1(0)*1, prefix-1, prefix, prefix + 1 return the closest
Buy Sell stock cool down X Same as buy sell stock II but skip a day if sell
move 0s to front of array X Pointer to last 0, swap at non-zero and increment pointer
Thread pool X
mutex + Queue/pq for tasks. Atomic Stop variable for
stopping, CV for waiting on queue or stop variable. Vector to
store threads
point lies inside a triangle
String class with big 5
virtual inheritance X
big endian/little endian
Merge k sorted lists X
min priority queue to keep head pointers to all lists. Push top
of heap to merge
Insert Delete Get Random X Map from value => index, move to back for O(1) remove