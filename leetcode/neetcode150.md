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
// Because the while loop is reached only when currentNum marks
// the beginning of a sequence (i.e. currentNum-1 is not present in
// set), the while loop can only run for n iterations throughout the
// entire runtime of the algorithm. This means that despite looking like
// O(n⋅n) complexity, the nested loops actually run in O(n+n)=O(n)


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

## 3. Longest Substring Without Repeating Characters

Notes: unordered set to keep track of seen chars. if new char then longest = right - left. else erase s[left] & increment left.

```cpp
// O(N) Time
// O(N) Space

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_set<char> set;
        int longest = 0, left = 0, right = 0;

        while(right < s.size()){
            if(set.find(s[right]) == set.end()){
                set.insert(s[right]);
                right++;
                longest = max(longest, right - left);
            }
            else{ //process duplicate and slide window
                set.erase(s[left]);
                left++;
            }
        }
        return longest;
    }
};
```

## 424. Longest Repeating Character Replacement

Notes: unordered map char->freq to keep track of frequency of characters. sliding window with maxFreq (maxFreq, count[s[right]]) to keep track of max recurring letter. if right - left + 1 - maxFreq > k, move left window. outside of if stmt update longest length.

```cpp
// O(N) Time
// O(N) Space

class Solution {
public:
    int characterReplacement(string s, int k) {
        int left = 0, right = 0, maxFreq = 0, longest = 0;
        unordered_map<char, int> count; // char -> freq

        while(right < s.size()){
            count[s[right]]++;
            maxFreq = max(maxFreq, count[s[right]]);
            
            // Check if the current window is valid
            if(right - left + 1 - maxFreq > k){
                // If the window is not valid, move the left pointer
                count[s[left]]--;
                left++;
            }

            // Update the longest length after the check
            longest = max(longest, right - left + 1);
            right++;
        }

        return longest;
    }
};
```

## 567. Permutation in String

Notes: char freq map for s1 and s2. while right < s2 size, increase char freq in s2map. if right - left + 1 == s1 size (window is size of s1) check if the maps are equal. also check if --s2Map[s2[left]] == 0 then erase s2 left from the s2map.

```cpp
// O(n + m * k) Time, where n is the length of s1, m is 
// the length of s2, and k is the size of the character set

// O(k) Space, where k is the number of unique characters in s1.
// since k is small (english letters) it can be argued that it is O(1)


class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        unordered_map<char, int> s1Map, s2Map; //char -> freq
        for(const char& c: s1) s1Map[c]++; //O(N)
        
        int left = 0, right = 0;

        while(right < s2.size()){
            char c = s2[right];
            s2Map[c]++;
            if(right - left + 1 == s1.size()){
                if(s1Map == s2Map) return true; //O(1) avg, O(k) worst case
                if(--s2Map[s2[left]] == 0) s2Map.erase(s2[left]); //O(1) avg
                left++;
            }
            right++;
        }
        return false;
    }
};
```

### 76. Minimum Window Substring

Notes: sliding window. char freq of tMap, sMap. if tMap.find(c), increment sMap count. if sMap and tMap count of letter is equal increment validLetters. in nested while(validL == tMap.size()): if right - left < minLen, then start = left and update minLen. then check if s[left] in tMap and if its equal to count of tMap. if it is decrement valid letters. finally decremenent sMap[s[left]]. return "" or s.substr(start, len)

```cpp
// O(N + M) Time Complexity
// where N is the length of string s and M is the length of string t. 
// The outer loop runs for each character of s, and the inner loop processes
// each character only once due to the two-pointer approach. Therefore, 
// it's linear in the size of s


// O(M) Space Complexity 
// where M is the length of string t, as we are storing counts of 
// characters in tMap and sMap. The size of these maps will not 
// exceed the number of unique characters in t, which is at most the length of t.

class Solution {
public:
    string minWindow(string s, string t) {
        int left = 0, right = 0, validLetters = 0, minLen = INT_MAX, start = 0;
        unordered_map<char, int> sMap, tMap;
        
        // Build a frequency map for characters in t
        for(char c : t) tMap[c]++;
        
        // Expand the window with right pointer
        while(right < s.size()){
            char c = s[right++];
            
            // Check if character is in t
            if(tMap.find(c) != tMap.end()){
                sMap[c]++; // Increment frequency in sMap
                // If counts match, increment validLetters
                if(sMap[c] == tMap[c]) validLetters++;
            }

            // Contract the window from the left and update result
            while(validLetters == tMap.size()){
                if(right - left < minLen){
                    start = left;
                    minLen = right - left;
                }
                char d = s[left++];
                // Check if the character is in t
                if(tMap.find(d) != tMap.end()){
                    // Reduce validLetters count if a required char is removed
                    if(sMap[d] == tMap[d]) validLetters--;
                    sMap[d]--;
                }
            }
        }
        // Return the minimum length window or empty string if no valid window found
        return minLen == INT_MAX ? "" : s.substr(start, minLen);
    }
};
```

## 239. Sliding Window Maximum

Notes: deque<int> to store indicies of window. while(j < n.size()): pop smaller value(s) from dq (while(!empty && n[dq.back()] < n[right]) dq.pop_back()). add current index to dq and remove left value from index ((if left index > q index) dq.pop_front()). finally if right + 1 >= k add n[dq.front] to result vector and inc left; right. 

```cpp
// O(N) Time
// O(k) Space

class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> result;
        int left = 0, right = 0;
        //gonna store the index 
        deque<int> dq; // O(k) space bc thats the max size of the deque
        while(right < nums.size()){
            //pop smaller value from q
            //each elem can only be added to deque once
            while(!dq.empty() && nums[dq.back()] < nums[right]) dq.pop_back();
            dq.push_back(right);

            //remove left value from index (if left index > q index)
            if(left > dq.front()) dq.pop_front();

            if(right + 1 >= k){
                result.push_back(nums[dq.front()]);
                left++;
            }
            right++;
        }
            
        return result;
    }
};
```

# Stack

## 20. Valid Parentheses

Notes: Create a stack for open parenthesis. If the closing parenthesis doesnt equal the top of stack, its not valid. 

```cpp
// O(N) Time
// O(N) Space

class Solution {
public:
    bool isValid(string s) {
        stack<char> stack; // O(N) space

        for(auto c: s){
            if(c == '(' or c == '[' or c == '{') stack.push(c);
            else if(stack.empty()) return false;
            else if(c == ')' && stack.top() != '(') return false;
            else if(c == ']' && stack.top() != '[') return false;
            else if(c == '}' && stack.top() != '{') return false;
            else stack.pop();
        
        }
        if(!stack.empty()) return false;
        return true;
    }
};
```

## 155. Min Stack

Notes: stack<pair<int, int>> stack; //val, minVal at time. when u push if stack empty: push{x, x} else push x, min(x, cMin)

```cpp
//O(1) Operation Time
//O(N) Space

class MinStack {
public:
    stack<pair<int, int>> stack; //val, minVal at time
    MinStack() {
        
    }
    
    void push(int val) {
        if(stack.empty()){
            stack.push({val, val});
            return;
        }
        int currentMin = stack.top().second;
        stack.push({val, min(currentMin, val)});
    }
    
    void pop() {
        stack.pop();
    }
    
    int top() {
        return stack.top().first;
    }
    
    int getMin() {
        return stack.top().second;
    }
};
```

## 150. Evaluate Reverse Polish Notation

Notes: isOperator function to check if str is operator. stack<int>. if str in tokens isOperator pop first 2 off stack and push result onto stack. else push(stoi(str)) to stack. 

```cpp
// O(N) Time
// O(N) Space

class Solution {
public:
    bool isOperator(const string& c){
        return (c == "+" or c == "-" or c == "*" or c == "/");
    }
    int evalRPN(vector<string>& tokens) {
        stack<int> stack;
        for(string& str: tokens){
            if(isOperator(str)){
                auto second = stack.top(); stack.pop();
                auto first = stack.top(); stack.pop();
                if(str == "+") stack.push(first + second);
                else if(str == "-") stack.push(first - second);
                else if(str == "/") stack.push(first / second);
                else if(str == "*") stack.push(first * second);
            }
            else{
                stack.push(stoi(str));
            }
        }
        return stack.top();
    }
};
```