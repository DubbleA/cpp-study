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
