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