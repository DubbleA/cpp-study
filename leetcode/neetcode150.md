# Arrays & Hashing


## 217. Contains Duplicate

Notes: set to check if new elem is alr processed

```cpp
// O(N) Time
// O(N) Space

class Solution {
public:
    bool containsDuplicate(vector<int>& nums) { 
        unordered_set<int> set; // O(N) Space
        for(const auto& num : nums){ //O(N)
            if (set.find(num) != set.end()) return true; //O(1) 
            //unordered_set is implemented as a hash table 
            //where these operations are generally constant time.
            set.insert(num); //O(1)
        }
        return false;
    }
};
```


## 242. Valid Anagram

Notes: unordered_map with char, int frequency. and check if theyre equal

```cpp  
// O(N) Time
// O(N) Space hashmap

class Solution {
public:
    bool isAnagram(string s, string t) {
        if(s.size() != t.size()) return false;
        unordered_map<char, int> sMap, tMap;
        for(int i = 0; i < s.size(); ++i){
            sMap[s[i]]++; // O(1) insertions
            tMap[t[i]]++;
        }
        return sMap == tMap; //O(N) Average Case
    }
};
```

## 1. Two Sum

Notes: unordered map to store "complement" value derived from subtracting target

```cpp
//O(N) Time
//O(N) Space

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int> m; //maps val -> index
        for(int i = 0; i < nums.size(); ++i){
            int additive = target - nums[i];
            if(m.find(additive) != m.end()){
                return {i, m[additive]};
            }
            m[nums[i]] = i;
        }
        return {-1, -1};
    }
};
```

## 49. Group Anagrams

Notes: make unordered map where key is sorted string, -> arr of strings

```cpp
// O(N * k log k) Time Complexity
// O(N * k) space complexity

class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        //The space complexity of the unordered_map is O(n * k), as it stores each string along with the sorted anagram key.
        unordered_map<string, vector<string>> m; // map anagram -> strings of anagram

        for(const auto& str : strs){ // O(N) for n strs
            string anagram = str; 
            // sorting string is O(k log k), k is the length of str string. 
            sort(anagram.begin(), anagram.end()); 
            m[anagram].push_back(str); // O(1) avg
        }
        vector<vector<string>> result;
        for(const auto& [key, vec] : m){
            result.push_back(vec);
        }
        return result;
    }
};
```

## 347. Top K Frequent Elements

Notes: unordered map to track num frequency, and pq to sort nums by freq

```cpp
// O(n log n) Time Complexity
// O(N) Space
 
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> m; // maps num -> freq // O(N) space
        for(const auto& num: nums) // O(N)
            m[num]++;
        
        priority_queue<pair<int, int>> pq; // O(N) space
        
        for(const auto& [num, freq] : m){ // O(n log n)
            pq.push({freq, num});
        }

        vector<int> result; // O(k) space
        while(k--){ // O (k log n)
            auto [freq, num] = pq.top(); pq.pop();
            result.push_back(num);
        }
        return result;
    }
};

```

## 238. Product of Array Except Self

Notes: Left `(n[i-1]*left[i-1])` `*` Right `(n[i+1]*right[i+1])`

```cpp
// O(N) Time
// O(N) Space
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> left (n, 1), right (n, 1);

        // left contains all the elements products to the left
        for(int i = 1; i < n; ++i)
            left[i] = nums[i-1] * left[i-1];
        
        //right contains all the elements products to the right
        for(int i = n - 2; i >= 0; --i)
            right[i] = nums[i+1] * right[i+1];
        
        vector<int> result(n);
        for(int i = 0; i < n; ++i)
            result[i] = left[i] * right[i];
        
        return result;
    }
};
```