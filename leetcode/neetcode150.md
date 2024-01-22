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

## 36. Valid Sudoku

Notes: unordered_map<int, unordered_set> to map index value from 0-8 of row, col, and "box" (row/3*3) + (col/3) for duplicates

```cpp
// O(N^2) Time Complexity
// O(N^2) Space Complexity

class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        unordered_map<int, unordered_set<char>> rows, cols, boxes; 
        //maps index to set , gives us O(N^2) space bc we have a hashmap of a hashset. 
        
        for(int r = 0; r < 9; ++r){
            for(int c = 0; c < 9; ++c){ //double for loop
                char val = board[r][c];
                if(val == '.')
                    continue;
                
                //check row by index
                if(rows[r].find(val) != rows[r].end()){
                    return false;
                }
                rows[r].insert(val);

                //check col by index
                if(cols[c].find(val) != cols[c].end()){
                    return false;
                }
                cols[c].insert(val);

                //check the box by indicies
                int boxRow = (r / 3) * 3; // should be 0,3,6 
                int boxCol = c / 3; // should be 0,1,2
                
                //now we created boxes 0-8, separated by row
                int boxIndex = boxRow + boxCol;  
                if(boxes[boxIndex].find(val) != boxes[boxIndex].end()){
                    return false;
                }
                boxes[boxIndex].insert(val);
            }
        }
        return true; // no duplicates found
    }
};
```

## 271. Encode and Decode Strings

Notes: encode using string length + "#" into single concatanated string, and decode once char == "#" and str = s.substr(j + 1, stoi(s.substr(i, j - i)));

```cpp
class Codec {
public:
  

// O(N) Time
// O(1) Space 
    
    // Encodes a list of strings to a single string.
    string encode(vector<string>& strs) {
        string res = "";
        for (int i = 0; i<strs.size(); i++){
            //encode using string length + hashtag concatanation
            res += to_string(strs[i].length()) + "#" + strs[i];
        }
        return res;
    }

    // Decodes a single string to a list of strings.
    vector<string> decode(string s) {
        vector<string> result;
        
        int i = 0;
        while (i < s.size()) {
            int j = i;
            while (s[j] != '#') {
                j++;
            }
            int length = stoi(s.substr(i, j - i));
            string str = s.substr(j + 1, length);
            result.push_back(str);
            i = j + 1 + length;
        }
        
        return result;
    }
};
```

## 128. Longest Consecutive Sequence

Notes: set, find bottom of sequence (s.find(curr-1)) and then while loop curr + 1 to find length of max sequence

```cpp
// O(N) Time
// O(N) Space
/*
    Because the while loop is reached only when currentNum marks
    the beginning of a sequence (i.e. currentNum-1 is not present in
    set), the while loop can only run for n iterations throughout the
    entire runtime of the algorithm. This means that despite looking like
    O(n⋅n) complexity, the nested loops actually run in O(n+n)=O(n)
*/

class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> set (nums.begin(), nums.end());
        int result = 0;

        for(int i = 0; i < nums.size(); ++i){
            int curr = nums[i];
            //we found the bottom of the sequence
            if(set.find(curr - 1) == set.end()){
                int currLength = 1;
                while(set.find(curr + 1) != set.end()){
                    currLength++;
                    set.erase(curr);
                    curr++;
                }
                result = max(result, currLength);
            }
        }
        return result;
    }
};
```

# Two Pointers

## 125. Valid Palindrome

Notes: isalnum(c), tolower(c), left = 0, right = s.size() - 1; two pointers

```cpp
// O(N) Time
// O(1) Space

class Solution {
public:
    bool isPalindrome(string str) {
        string s;
        for(const auto& c : str)
            if(isalnum(c)) s += tolower(c);
        
        int left = 0, right = s.size() - 1;
        while(left < right){
            if(s[left] != s[right]) return false;
            left++;
            right--;
        }
        return true;
    }
};
```

## 167. Two Sum II - Input Array Is Sorted

Notes: two pointers. if left + right < target: left++; else right--

```cpp
// O(N) Time
// O(1) Space

class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int left = 0, right = numbers.size() - 1;

        while(left < right){
            if(numbers[left] + numbers[right] == target){
                return {left + 1, right + 1};
            }
            else if(numbers[left] + numbers[right] < target){
                left++;
            }
            else{ //numbers[left] + numbers[right] > target
                right--;
            }
        }
        return {-1};
    }
};
```

## 15. 3Sum

Notes: for i=0, n - 2; int j = i + 1, k = n - 1 (and skip duplicates for i); two pointers j < k: if sum is 0: trips.pushback and skip duplicates for j & k (i.e. while (j < k && nums[j] == nums[j + 1]) j++;) j++; k--;. else if sum > 0: k--, else j++;

```cpp
// O(N^2) Time
// O(N) Space

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> trips; //O(N) space
        sort(nums.begin(), nums.end()); // O(nlogn) time

        for (int i = 0; i < nums.size() - 2; ++i) {
            // Skip duplicates for nums[i]
            if (i > 0 && nums[i] == nums[i - 1]) continue; 
            int j = i + 1, k = nums.size() - 1;
            
            while (j < k) {
                int sum = nums[i] + nums[j] + nums[k];
                if (sum == 0) {
                    trips.push_back({nums[i], nums[j], nums[k]});
                    // Skip duplicates for nums[j] and nums[k]
                    while (j < k && nums[j] == nums[j + 1]) j++;
                    while (j < k && nums[k] == nums[k - 1]) k--;
                    j++;
                    k--;
                } else if (sum > 0) {
                    k--;
                } else { //sum < 0
                    j++;
                }
            }
        }

        return trips;
    }
};
```

## 11. Container With Most Water

Notes: maxArea = max(min(height[left], height[right]) * (right - left), maxArea);

```cpp
// O(N) Time
// O(1) Space

class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0, right = height.size() - 1, maxArea = 0;

        while(left < right){
            maxArea = max(min(height[left], height[right]) * (right - left), maxArea);
            if(height[left] <  height[right]){
                left++;
            }
            else{
                right--;
            }
        }
        return maxArea;
    }
};
```

## 42. Trapping Rain Water

Notes: while (i < j) : if(maxLeft <= maxRight) maxLeft = max(maxLeft, height[++i]), result += maxLeft - height[i]; inverse for else. 

```cpp
// O(N) Time
// O(1) Space

class Solution {
public:
    int trap(vector<int>& height) {
        int i = 0, j = height.size() - 1;
        int maxLeft = height[i], maxRight = height[j];
        int result = 0;

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

# Sliding Window

## 121. Best Time to Buy and Sell Stock

Notes: profit = max(profit, prices[i] - curMin);

```cpp
// O(N) Time
// O(1) Space

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int profit = 0, curMin = INT_MAX;

        for(int i = 0; i < prices.size(); ++i){
            curMin = min(curMin, prices[i]);
            profit = max(profit, prices[i] - curMin);
        }
        return profit;
    }
};
```