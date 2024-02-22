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
