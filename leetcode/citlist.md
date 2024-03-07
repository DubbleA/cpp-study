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