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

## 22. Generate Parentheses

Notes: backtrack. if openN < n: backtrack(n, openN + 1, closedN, curr + "(", res), if closedN < openN : add ")" paren

```cpp
// O( 4^n / sqrt(n)) Time 
// the total number of valid parentheses strings ^ (Catalan Number of possible combinations)
// O(N) space 
//The space complexity of a recursive call depends on the maximum depth of the recursive call stack, 
//which is 2n. As each recursive call either adds a left parenthesis or a right parenthesis, 
//and the total number of parentheses is 2n. Therefore, at most O(n) levels of recursion

class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        backtrack(n, 0, 0, "", res);
        return res;
    }
    void backtrack(int n, int openN, int closedN, string curr, vector<string>& res){
        if(curr.length() == n * 2) {
            res.push_back(curr);
            return;
        }
        
        if(openN < n) {
            backtrack(n, openN + 1, closedN, curr + "(", res);
        }
        
        if(closedN < openN) {
            backtrack(n, openN, closedN + 1,curr + ")", res);
        }
    }
    
};
```

## 739. Daily Temperatures

Notes: while(!stack.empty() && temperatures[stack.top()] < currentTemp){ result[stack.top()] = i - stack.top(); stack.pop(); }

```cpp
// O(N) Time Complexity

//At first glance, it may look like the time complexity of this algorithm should be O(N^2), 
// because there is a nested while loop inside the for loop. However, each element 
//can only be added to the stack once, which means the stack is limited to N pops. 
//Every iteration of the while loop uses 1 pop, which means the while loop will not 
//iterate more than N times in total, across all iterations of the for loop.

// O(N) Space Complexity

class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        stack<int> stack;
        int n = temperatures.size();
        vector<int> result (n, 0);

        //Iterate through temperatures. At each index currTemp
        for(int i = 0; i < n; ++i){
            int currentTemp = temperatures[i];
            //If the stack is not empty, that means there are previous 
            //days for which we have not yet seen a warmer day.
            //if the vales in the stack are colder than current temp, we need to update
            //result array with the diff in index
            while(!stack.empty() && temperatures[stack.top()] < currentTemp){
                result[stack.top()] = i - stack.top(); stack.pop();
            }
            //push current day onto stack to be processed
            stack.push(i);
        }

        return result;
    }
};
```

## 853. Car Fleet

Notes: sort(pair<position[i], float(target - position[i]) / speed[i])> and put into stack, then while : if car cant catch up or no one in front, fleets++,
else make sure that the cars have the slower time once merging to become a fleet

```cpp
// O(nlogn) Time because we have to sort it
// O(N) Space

class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        vector<pair<int, float>> vec; // pos, target - position / speed (unit distance)
        for(int i = 0; i < size(position); ++i) {
            vec.push_back({position[i], float(target - position[i]) / speed[i]});
        }
        sort(vec.begin(), vec.end());
        stack<pair<int, float>> s;
        for(auto pair: vec) s.push(pair);
        int fleets = 0;
        while(!s.empty()){
            auto [pos, time] = s.top(); s.pop();
            if(s.empty() || time < s.top().second) { // If no cars in front or can't catch up
                fleets++; // This car forms a new fleet.
            } else {
                // If the current car can catch up to the car in front, merge them into one fleet.
                // we only increase `fleets` when we find a car that can't catch up.
                // Adjust the time of the car in front if the current car takes longer to reach the target.
                if(time > s.top().second) s.top().second = time; 
                    // Update the time of the car in front to reflect this car joining the fleet, 
                    // ensuring the fleet's speed is dictated by the slowest car.
            }
        }
        return fleets;
    }
};
```

## 84. Largest Rectangle in Histogram

Notes: stack store values in inc order (indexes); whenever pop we account for max area of rectangle (while s.top != -1 and h[i] <= h[s.top]); (rightlimit(i) - leftlimit(s.top) - 1) * currHeight(heights[stack.top()]); if we r left with entries pop them using n as right limit

```cpp
// O(N) Time
// O(N) Space

class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        stack<int> stack; 
        stack.push(-1); //left limit
        int maxArea = heights[0];
        for(int i = 0; i < heights.size(); ++i){
            while(stack.top() != -1 and heights[i] <= heights[stack.top()]){
                int rightlimit = i;
                int currHeight = heights[stack.top()]; stack.pop();
                int leftlimit = stack.top();
                maxArea = max(maxArea, (rightlimit - leftlimit - 1) * currHeight);
            }
            stack.push(i);
        }

        while(stack.top() != -1){
            int rightlimit = i;
            int currHeight = heights[stack.top()]; stack.pop();
            int leftlimit = stack.top();
            maxArea = max(maxArea, (rightlimit - leftlimit - 1) * currHeight);
        }

        return maxArea;
    }
};
```

# Binary Search

## 704. Binary Search

Notes: Binary search: while l <= r : if mid < target, l = mid + 1 else r = mid - 1

```cpp
// O(logn) time
// O(1) space

class Solution {
public:
    int search(vector<int>& nums, int target) {
        int l = 0, r = nums.size() - 1;
        while(l <= r){
            int mid = (l + r) / 2;
            if(nums[mid] == target) return mid;
            if(nums[mid] < target) l = mid + 1;
            else r = mid - 1; //nums[mid] > target
        }
        return -1;
    }
};
```

## 74. Search a 2D Matrix

Notes: First find correct row. while rowLeft <= rowRight : if(m[i][0] <= target and target <= m[i][n]) we found row.
else if(m[i][n] <= target) l = i + 1; else r = i - 1; then just regular binary search on the found row m[i][]

```cpp
// O(log n * m) or O(log n + log m) Time
// O(1) Space
 
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int rowLeft = 0, rowRight = matrix.size() - 1, rowIdx = 0;
        while(rowLeft <= rowRight){
            rowIdx = (rowLeft + rowRight) / 2;
            if (matrix[rowIdx][0] <= target and target <= matrix[rowIdx][matrix[0].size() - 1]) break; //we found the row
            else if (matrix[rowIdx][matrix[0].size() - 1] <= target) rowLeft = rowIdx + 1;
            else rowRight = rowIdx - 1; // its in a lower row
        }
        int left = 0, right = matrix[0].size() - 1;
        while(left <= right){
            int mid = (left + right) / 2;
            if(matrix[rowIdx][mid] == target) return true;
            if(matrix[rowIdx][mid] < target) left = mid + 1;
            else right = mid - 1;
        }
        return false;
    }
};
```

## 875. Koko Eating Bananas

Notes: sort piles, l = 1, r = p[n-1]. while (l <= r) binary search. unsigned int rate = kokoSim(piles, h, m); if(rate <= h) minrate update, r = m - 1; else l = m + 1

```cpp
// O(nlogn+nlogk) -> O(nlogn) bc we sort + binary search on k eating speeds
// O(1) space

class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        sort(piles.begin(), piles.end());
        int left = 1, right = piles[piles.size() - 1];
        int minrate = INT_MAX;

        while(left <= right){
            int mid = (left + right) / 2;
            unsigned int rate = kokoHelper(piles, h, mid);
            if(rate <= h){
                minrate = min(minrate, mid);
                right = mid - 1;
            }
            else left = mid + 1;
        }
        return minrate;


    }
    int kokoHelper(vector<int>& piles, int h, int rate){
        unsigned int thour = 0;
        for(auto& n : piles){
            if(rate>=n) thour++;
            else{
                thour += n / rate;
                if(n % rate > 0) thour++;
            }
        }

        return thour;
    }
};
```

## 153. Find Minimum in Rotated Sorted Array

Notes: if the mid value > nums right (the minimum can only be to the right, bc of the rotate), then the minimum is on the right partition, otherwise its in the left partition.

```cpp
//O(log n) Time 
//O(1) Space

class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0, right = nums.size() - 1, minVal = INT_MAX;
        while(left <= right){
            int mid = (left + right) / 2;
            // min is on right partition
            if(nums[mid] > nums[right]) left = mid + 1;
            else{ // min on left partition
                minVal = min(minVal, nums[mid]);
                right = mid - 1;
            }
        }
        return minVal;
    }
};
```

## 33. Search in Rotated Sorted Array

Notes: if(n[m] > n[r]) //part of rotation is on right, check target in left partition
        if(n[l] <= tgt < n[m]) r = m - 1; else l = m + 1; 
       else //part of rotation is on left, check target in right partition
        if(n[m] < tgt <= n[r]) l = m + 1; else r = m - 1;

```cpp
// O(logn) time
// O(1) space

class Solution {
public:
    int search(vector<int>& n, int target) {
        int l = 0, r = n.size() - 1;
        while(l <= r){
            int m = (l + r) / 2;
            if(n[m] == target) return m;
            //part of the rotation is on the right
            if(n[m] > n[r]){
                //target might be in left partition
                if(n[l] <= target and target < n[m]) r = m - 1;
                else l = m + 1;
            }
            else{
                //target might be in the right partition
                if(n[m] < target and target <= n[r]) l = m + 1;
                else r = m - 1;
            }
        }
        return -1;
    }
};
```

## 981. Time Based Key-Value Store

Notes: map<str key, map<int time, str value>> m; for get .upper_bound(timestamp). if it == .begin() no elem leq to timestamp return "", else --it (step back 1) and return it->second.
```cpp
// Time Complexity:
// O(log m) per operation for set & get functions. 

// m is the number of timestamp-value pairs for the key.

// O(k x m) space where m is same as above and k is the # of keys. bc of our map of maps

class TimeMap {
public:
    std::map<string, std::map<int, string>> m; //str key -> <int time, str value>
    TimeMap() {
        
    }
    
    void set(string key, string value, int timestamp) {
        m[key].insert({timestamp, value});
    }
    
    string get(string key, int timestamp) {
        //std::upper_bound searches for the first element in a sorted range 
        //that is greater than a given value. 
        //If no such element exists (i.e., all elements in the range are 
        //leq to the given value), std::upper_bound 
        //returns an iterator to the end of the range. (i.e. m[key].end())
        //therefore if its still at m[key].begin(), we did not find anything leq
        
        if(m.find(key) == m.end()) return ""; // key not found
        //finds first greater elem than timestamp
        auto it = m[key].upper_bound(timestamp); 
        if(it == m[key].begin()) return ""; //no elem leq timestamp
        // Since 'upper_bound' finds the first greater element, we step back to find the lesser or equal
        --it;
        return it->second;
    }
};
```

## 4. Median of Two Sorted Arrays

Notes: if (A size > B size) swap(A, B)
int total, l = 0, r = a size() [dont subtract 1]
double result;

while l <= r
partition A i = l + (r - l) / 2; to avoid overflow
j = (total + 1) / 2 - i; B partition (half point - rest)

partition A top, A next, B top, B next. (A[i-1]) for left A[i] for next. use INT_MAX / INT_MIN 
for left and right bounds

if(Atop leq Bnext and Btop leq Anext) partition is correct return max(atop, btop) if odd else
add max(atop,btop) and min(anext,bnext) / 2.
else if Atop > Bnext : r = m - 1
else l = m + 1

https://www.youtube.com/watch?v=q6IEA26hvXc

```cpp
// O(log(min(n, m))) Time
// O(1) Space

class Solution {
public:
    double findMedianSortedArrays(vector<int>& A, vector<int>& B) {
        if (A.size() > B.size()) swap(A, B);
        
        int total = A.size() + B.size();
        int l = 0, r = A.size();
        double result = 0.0;
        
        while (l <= r) {
            // A
            int i = l + (r - l) / 2;
            // B partition (half point - rest)
            int j = (total + 1) / 2 - i;
            
            int Aleft = (i > 0) ? A[i - 1] : INT_MIN;
            int Aright = (i < A.size()) ? A[i] : INT_MAX;
            int Bleft = (j > 0) ? B[j - 1] : INT_MIN;
            int Bright = (j < B.size()) ? B[j] : INT_MAX;
            
            // partition is correct
            if (Aleft <= Bright && Bleft <= Aright) {
                // even
                if (total % 2 == 0) {
                    result = (max(Aleft, Bleft) + min(Aright, Bright)) / 2.0;
                // odd
                } else {
                    result = max(Aleft, Bleft);
                }
                break;
            } else if (Aleft > Bright) {
                r = i - 1;
            } else {
                l = i + 1;
            }
        }
        
        return result;
    }
};
```

# Linked List

## 206. Reverse Linked List

Notes: swap curr->next and prev, and return prev;

```cpp
// O(N) Time
// O(1) Space

class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;

        while(curr){
            ListNode* temp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = temp;
        }

        return prev;
    }
};
```

## 21. Merge Two Sorted Lists

Notes: new ListNode(), while l1 and l2 if(l1->val <= l2val) curr->next = l1; return head->next

```cpp
// O(N + M) Time
// O(1) Space
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode* head = new ListNode(0);
        ListNode* curr = head;

        while(list1 && list2){
            if(list1->val <= list2->val){
                curr->next = list1;
                list1 = list1->next;
            }
            else{
                curr->next = list2;
                list2 = list2->next;
            }

            curr = curr->next;
        }
        if(list1) curr->next = list1;
        if(list2) curr->next = list2;
        return head->next;
    }
};
```

## 143. Reorder List

Notes: //split list (slow, fast method), reverse second list, merge lists

```cpp
// O(N) Time
// O(1) Space
class Solution {
public:
    void reorderList(ListNode* head) {
        if (!head or !head->next) return;
        //split list, reverse second list, merge lists
        ListNode* slow = head;
        ListNode* fast = head;
        //split list
        ListNode* prev = nullptr;
        while(fast && fast->next){
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        prev->next = nullptr;

        //reverse second list
        prev = nullptr;
        while(slow){
            ListNode* temp = slow->next;
            slow->next = prev;
            prev = slow;
            slow = temp;
        }
        
        //merge lists
        ListNode* l1 = head;
        ListNode* l2 = prev;
        ListNode* newHead = new ListNode(0);
        while (l1 && l2){
            newHead->next = l1;
            l1=l1->next;
            newHead = newHead->next;
            newHead->next = l2;
            l2 = l2->next;
            newHead = newHead->next;
        }
        head = newHead->next;
    }   
};
```

## 19. Remove Nth Node From End of List

Notes: for(i in range n) right = right->next; if !right return head->next. then iterate left and right. if left and prev: prev->next = left->next

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

## 138. Copy List with Random Pointer

Notes: First pass copy all nodes into u_map<node* orig, node* copy>. in second pass update links (m[curr]->next = m[curr->next]) etc

```cpp
//O(N) Time
//O(N) Space

class Solution {
public:
    Node* copyRandomList(Node* head) {
        unordered_map<Node*, Node*> m;

        // First pass: Copy all nodes and store the mapping
        Node* curr = head;
        while(curr){
            m[curr] = new Node(curr->val);
            curr = curr->next;
        }

        // Second pass: Assign next and random pointers
        curr = head;
        while(curr){
            m[curr]->next = m[curr->next];
            m[curr]->random = m[curr->random];
            curr = curr->next;
        }
        return m[head];
    }
};
```

## 2. Add Two Numbers

Notes: while(l1 or l2 or carry != 0): int val = l1val + l2val + carry, carry = val/10, curr->next = new Node(val%10)

```cpp
// O(N) Time
// O(1) Space

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* newHead = new ListNode(0);
        ListNode* curr = newHead;
        int carry = 0;

        while(l1 or l2 or carry != 0){
            int l1val = l1 ? l1->val : 0;
            int l2val = l2 ? l2->val : 0;
            int val = l1val + l2val + carry;
            carry = val / 10;
            curr->next = new ListNode(val % 10);
            l1 = l1 ? l1->next : nullptr;
            l2 = l2 ? l2->next : nullptr;
            curr = curr->next;
        }
        return newHead->next;
    }
};
```

## 141. Linked List Cycle

Notes: slow and fast

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

## 287. Find the Duplicate Number

Notes: Floyd's slow and fast: do while (s != f) s = n[s], f = n[n[f]] to find intersection point. then find entrance to cycle s = n[0] and while(...) s=n[s],f=n[f] return s

```cpp
//O(N) Time
//O(1) Space
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        // Find the intersection point of the two runners.
        int slow = nums[0];
        int fast = nums[0];

        // In a "do-while" loop, the loop's body is executed first, and then the condition is evaluated.
        do {
            slow = nums[slow];
            fast = nums[nums[fast]];
        } while (slow != fast);

        // Find the "entrance" to the cycle.
        slow = nums[0];
        while (slow != fast) {
            slow = nums[slow];
            fast = nums[fast];
        }

        return slow;
    }
};
```

## 146. LRU Cache

Notes: 
setup: Node struct, private: capacity, size, Node* head, tail, map<key, Node*>, move_to_front and remove_tail functions

get: if not in map return -1; else move to front and return m[key]->val
put: if(m[key] in map) update key value pair and move to front. else if max capacity remove tail and erase from map. then create new node as new head

```cpp
//O(1) Time
//O(N) Space

struct Node{
    int key;
    int val;
    Node* prev;
    Node* next;
};

class LRUCache {
private: 
    int capacity;
    int size;
    Node* head;
    Node* tail;
    std::unordered_map<int, Node*> map;

public:
    LRUCache(int capacity) : capacity(capacity), size(0), head(nullptr), tail(nullptr) {}
    
    int get(int key) {
        if(map.find(key) == map.end()) return -1;
        Node* node = map[key];
        move_to_front(node);
        return node->val;
    }
    
    void put(int key, int value) {
        //update key value pair and move to front
        if(map.find(key) != map.end()){
            Node* node = map[key];
            node->val = value;
            move_to_front(node);
            return;
        }
        if(size == capacity){
            map.erase(tail->key);
            remove_tail();
            --size;
        }

        Node* newNode = new Node{key, value, nullptr, head}; //key, val, prev, next
        if(!head and !tail) tail = newNode;
        else head->prev = newNode;
        head = newNode;

        map[key] = newNode;
        ++size;
    }

    void move_to_front(Node* node){
        //already at front
        if(node == head) return;

        //disconnect node from list
        node->prev->next = node->next;
        //update prev value
        if(node->next) node->next->prev = node->prev;
        //if node is the tail then move tail back one
        if(node == tail) tail = node->prev;

        //move node to front;
        node->prev = nullptr;
        node->next = head;
        head->prev = node;
        head = node;
    }

    void remove_tail(){
        if(!tail) return;

        //if only one node is present
        if(tail == head){
            delete tail;
            head = nullptr;
            tail = nullptr;
            return;
        }
        Node* temp = tail;
        tail = tail->prev;
        tail->next = nullptr;
        delete temp;
    }
};
```

## 23. Merge k Sorted Lists

Notes: m2s + for loop merge lists 1 at a time. optimized soln is using pq + custom comparator

```cpp
//O(N*k) Time where N is number of nodes k is # of lists
//O(1) Space

class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode* dummy = new ListNode();
        dummy = dummy->next;
        for(auto l : lists){
            if(!l) continue;
            dummy = m2s(dummy, l);
        }
        return dummy;
    }
    ListNode* m2s(ListNode* l1, ListNode* l2){
        ListNode* dummy = new ListNode(0);
        ListNode* head = dummy;
        while(l1 && l2){
            if(l1->val <= l2->val){
                head->next = l1;
                l1 = l1->next;
            }
            else{
                head->next = l2;
                l2 = l2->next;
            }
            head = head->next;
        }
        if(l1) head->next = l1;
        if(l2) head->next = l2;
        return dummy->next;
    }
};
```

## 

Notes:
```
while curr
    container of stacks of size k
    if stack.size() == k push to container
    anything not pushed to container pushed to leftovers

for stack in container
    reverse stack on main linked list

append leftovers to end of list
```

```cpp
// O(N) Time
// O(N) Space

class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        
        vector<vector<int>> container;
        vector<int> stack;
        ListNode* curr = head;
        while(curr){
            stack.emplace_back(curr->val);
            curr = curr->next;
            if(stack.size() == k){
                container.emplace_back(stack);
                stack.clear(); //clear stack
            }
        }

        ListNode* dummy = new ListNode(0);
        curr = dummy;

        for(auto& st : container){
            for(int i = st.size() - 1; i >= 0; --i){
                curr->next = new ListNode(st[i]);
                curr = curr->next;
            }
        }
        
        for(int i = 0; i < stack.size(); ++i) {
            curr->next = new ListNode(stack[i]);
            curr = curr->next;
        }

        return dummy->next;

    }
};
```

Follow up: O(1) Space?

```cpp
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* dummy = new ListNode(0);
        dummy->next = head;
        ListNode* prevGroupTail = dummy;

        while(head) {
            ListNode* groupStart = head;
            ListNode* groupEnd = getGroupEnd(head, k);

            if(!groupEnd) break; //remaining nodes are less than k so no need to reverse

            ListNode* nextGroupStart = groupEnd->next;
            groupEnd->next = nullptr; //separate the group to be reversed

            //Reverse the group
            prevGroupTail->next = reverseList(groupStart);
            groupStart->next = nextGroupStart;

            prevGroupTail = groupStart;
            head = nextGroupStart;
        }
        return dummy->next;

    }
    ListNode* getGroupEnd(ListNode* head, int k){
        while(head && k > 1){
            head = head->next;
            k--;
        }
        return head;
    }
    ListNode* reverseList(ListNode* head){
        ListNode* prev = nullptr;
        ListNode* curr = head;

        while(curr){
            ListNode* temp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = temp;
        }
        return prev;
    }
};
```

# Trees

## 226. Invert Binary Tree

Notes: swap(root->right, root->left), dfs(left), dfs(right)

```cpp
//O(N) Time
//O(h) Space where h is the height of the tree.

class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        dfs(root);
        return root;
    }
    void dfs(TreeNode* root){
        if(!root) return;
        swap(root->left, root->right);
        dfs(root->left);
        dfs(root->right);
    }
};
```

## 104. Maximum Depth of Binary Tree

Notes: dfs() rightHeight = dfs(root->right) + 1; leftHeight = dfs(root->left) + 1. return max(r, l)

```cpp
//O(N) Time
//O(h) Space where h is the height of the tree.

class Solution {
public:
    int maxDepth(TreeNode* root) {
        return dfs(root);
    }
    int dfs(TreeNode* root){
        if(!root) return 0;
        int rHeight = dfs(root->right) + 1;
        int lHeight = dfs(root->left) + 1;

        return max(rHeight, lHeight);
    }
};
```

## 543. Diameter of Binary Tree
 
Notes: left & right = dfs(root->dir, width), width = max(width, left + right), return max(left,right) + 1, return width

```cpp
// O(N) Time
// O(h) Space where h is the height of the tree.

class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int width = 0;
        dfs(root, width);
        return width;
    }
    int dfs(TreeNode* root, int& width){
        if(!root) return 0;
        //find longest path in left & right child
        int left = dfs(root->left, width);
        int right = dfs(root->right, width);
        //update diameter if left_path + right_path is larger
        width = max(width, left + right);
        //return the longest path + 1 including parent
        return max(left, right) + 1;
    }
};
```

## 110. Balanced Binary Tree

Notes: dfs height, modify to return -1 if (rHeight/lHeight == -1 or abs(rHeight - lHeight) > 1)

```cpp
//O(N) Time
//O(h) Space where h is the height of the tree.
class Solution {
public:
    bool isBalanced(TreeNode* root) {
        if(!root) return true;
        return dfs(root) != -1;
    }

    int dfs(TreeNode* root){
        if(!root) return 0;
        int rHeight = dfs(root->right);
        int lHeight = dfs(root->left);

        if(rHeight == -1 or lHeight == -1 or abs(rHeight - lHeight) > 1) return -1;
        return max(rHeight, lHeight) + 1;
    }
};
```

## 100. Same Tree

Notes: dfs check false cases (!p or !q) !vals equal etc. then dfs on right and left

```cpp
// O(N) Time
// O(h) Space where h is the height of the tree.

class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) { 
        return dfs(p, q);
    }

    bool dfs(TreeNode* p, TreeNode* q){
        if(!p and !q) return true;
        if(!p or !q) return false;
        if(p->val != q->val) return false;
        return dfs(p->left, q->left) and dfs(p->right, q->right);
    }
};
```

## 572. Subtree of Another Tree

Notes: isSameTree + dfs until root and subroot vals match if(root->val == subRoot->val and isSameTree(root, subRoot)) return true;

```cpp
//O(N*M) Time
//O(h) Space where h is the height of the tree.

class Solution {
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        return dfs(root, subRoot);
    }
    bool dfs(TreeNode* root, TreeNode* subRoot){
        if(!root) return false;
        if(root->val == subRoot->val and isSameTree(root, subRoot)) return true;
        return dfs(root->left, subRoot) or dfs(root->right, subRoot);
    }
    bool isSameTree(TreeNode* p, TreeNode* q){
        if(!p and !q) return true;
        if(!p or !q) return false;
        if(p->val != q->val) return false;
        return isSameTree(p->left, q->left) and isSameTree(p->right, q->right);
    }
};

//O(N) Solution is serializing the tree and seeing if the subroot string is in pos:
bool isSubtree(TreeNode* root, TreeNode* subRoot) {
    string rootStr = serialize(root);
    string subRootStr = serialize(subRoot);
    
    return rootStr.find(subRootStr) != string::npos;
}

string serialize(TreeNode* node) {
    if (!node)
        return "null";
    
    return "#" + to_string(node->val) + " " + serialize(node->left) + " " + serialize(node->right);
}
```

## 235. Lowest Common Ancestor of a Binary Search Tree

Notes: return when root leq max(pval, qval) and geq min(pval, qval)

```cpp 
//O(N) Time
//O(h) Space where h is the height of the tree.

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        return dfs(root, p, q);
    }
    TreeNode* dfs(TreeNode* root, TreeNode* p, TreeNode* q){
        if(!root) return nullptr;
        //we want to be inbetween p and q
        if(root->val <= max(p->val, q->val) and root->val >= min(p->val, q->val)) return root;
        else if(root->val < q->val and root->val < p->val) return dfs(root->right, p, q);
        return dfs(root->left, p, q); //ancestor in left
    }
};
```

## 102. Binary Tree Level Order Traversal

Notes: dfs with 2d vector passed by reference to keep track of values. if index (height) == size of 2d vector push back blank vector. then dfs left, right

```cpp
//O(N) Time
//O(N) for storing the output. The space required for the recursion stack is O(h), where h is the height of the tree.

class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> levels;
        dfs(root, levels, 0);
        return levels;
    }

    void dfs(TreeNode* root, vector<vector<int>>& levels, int height){
        if(!root) return;
        if(height == levels.size()) levels.push_back({});
        levels[height].emplace_back(root->val);
        dfs(root->left, levels, height + 1);
        dfs(root->right, levels, height + 1);
    }
};
```

## 199. Binary Tree Right Side View

Notes: level order traversal but just return last val in each level of vector

```cpp
//O(N) Time
//O(N) for storing the output. The space required for the recursion stack is O(h), where h is the height of the tree.

class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<vector<int>> levels;
        dfs(root, levels, 0);
        vector<int> res;
        for(auto& v : levels) res.push_back(v.back());
        return res;
    }
    void dfs(TreeNode* root, vector<vector<int>>& levels, int height){
        if(!root) return;
        if(levels.size() == height) levels.push_back({});
        levels[height].emplace_back(root->val);
        dfs(root->left, levels, height + 1);
        dfs(root->right, levels, height + 1);
    }
};
```

## 1448. Count Good Nodes in Binary Tree

Notes: dfs (root, prevMax, int& goodNodes)

```cpp
//O(N) Time
//O(h) Space
class Solution {
public:
    int goodNodes(TreeNode* root) {
        int goodNodes = 0;
        dfs(root, INT_MIN, goodNodes);
        return goodNodes;
    }

    void dfs(TreeNode* root, int prevMax, int& goodNodes){
        if(!root) return;
        if(root->val >= prevMax){
            goodNodes++;
        }
        prevMax = max(prevMax, root->val);
        dfs(root->left, prevMax, goodNodes);
        dfs(root->right, prevMax, goodNodes);
    }
};
```

## 98. Validate Binary Search Tree

Notes: return dfs(root->left, lVal, root->val) and dfs(root->right, root->val, rVal);

```cpp
//O(N) Time
//O(h) Space

class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return dfs(root, LONG_MIN, LONG_MAX);
    }
    bool dfs(TreeNode* root, long long lVal, long long rVal){
        if(!root) return true;
        if(lVal >= root->val or root->val >= rVal) return false;
        return dfs(root->left, lVal, root->val) and dfs(root->right, root->val, rVal);
    }
};
```

## 230. Kth Smallest Element in a BST

Notes: inorder dfs. dfs left, process root, dfs right

```cpp
//O(Nlogk) Time
//O(N) Space
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        priority_queue<int, vector<int>, greater<>> pq;
        queue<TreeNode*> q;
        q.push(root);

        while(!q.empty()){
            auto node = q.front(); q.pop();
            if(!node) continue;
            pq.emplace(node->val);
            q.emplace(node->left);
            q.emplace(node->right);
        }

        while(--k) pq.pop();

        return pq.top();
    }
};

// could u give me a O(N) sol?

class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        vector<int> levels;
        dfs(root, levels, k);
        return levels.back();
    }
    void dfs(TreeNode* root, vector<int>& levels, int k){
        if(!root) return;
        //inorder traversal
        dfs(root->left, levels, k);
        if(levels.size() == k) return;
        levels.emplace_back(root->val);
        dfs(root->right, levels, k);
    }
};
```

## 105. Construct Binary Tree from Preorder and Inorder Traversal

Notes: 

preorder ALWAYS has the node first. But you don't know the size of either branch.
inorder ALWAYS has the left branch to the left of the node, and right branch right of the node. So now you know the size of each branch.
Take those information and break the arrays into subproblems, based on the size.
```
def buildTree(self, preorder, inorder):
    if inorder:
        ind = inorder.index(preorder.pop(0))
        root = TreeNode(inorder[ind])
        root.left = self.buildTree(preorder, inorder[0:ind])
        root.right = self.buildTree(preorder, inorder[ind+1:])
        return root
```

```cpp
// O(N^2) Time
// O(N) Space
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        int rootIndex = 0;
        return arrayToTree(preorder, inorder, rootIndex, 0, inorder.size() - 1);
    }

    TreeNode* arrayToTree(vector<int>& preorder, vector<int>& inorder, int& rootIndex, int left, int right){
        if(left > right) return nullptr;
        int pivot = left;

        while(preorder[rootIndex] != inorder[pivot]) pivot++;
        TreeNode* newNode = new TreeNode(preorder[rootIndex]);
        ++rootIndex;
        newNode->left = arrayToTree(preorder, inorder, rootIndex, left, pivot-1);
        newNode->right = arrayToTree(preorder, inorder, rootIndex, pivot+1, right);
        return newNode;
    }

};
```

## 124. Binary Tree Maximum Path Sum

Notes:
```
case 1: left_max + Node
case 2: right_max + Node
case 3: just the Node
curMax = max(c1, c2, c3)
case 4: left_max + Node + right_max
result = max(res, curMax, case4)
return curMax
```

```cpp
//O(N) Time
//O(N) Space

class Solution {
public:
    int maxPathSum(TreeNode* root) {
        int result = INT_MIN;
        dfs(root, result);
        return result;
    }

    int dfs(TreeNode* root, int& result) {
        if(!root) return 0;
        int leftMax = dfs(root->left, result);
        int rightMax = dfs(root->right, result);
        int currentMax = max(leftMax + root->val ,max(rightMax + root->val, root->val));
        result = max(result, max(currentMax, root->val + rightMax + leftMax));
        // Return the maximum path sum from the current node to either left or right child
        return currentMax;
    }
};
```

## 297. Serialize and Deserialize Binary Tree

Notes: 
```
serialize: 
if(!root) return "null,"
return to_string(root->val) , s(left) + s(right)

deserialize:
for c in data: if char == ',' push s to q, else s+=c

makeTree:
s = q.front()
t* = new TreeNode(stoi(s));
t->right & t->left = makeTree(q)
```

```cpp
// O(N) Time
// O(N) Space

class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        if(!root) return "null,";
        return to_string(root->val) + "," + serialize(root->left) + serialize(root->right);
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        queue<string> q;
        string s = "";
        for(const char& c : data){
            if(c == ',') {
                q.emplace(s);
                s.clear();
            }
            else s += c;
        }
        return makeTree(q);
    }

    TreeNode* makeTree(queue<string>& q){
        string s = q.front(); q.pop();
        if(s == "null") return nullptr;
        TreeNode* t = new TreeNode(stoi(s));
        t->left = makeTree(q);
        t->right = makeTree(q);
        return t;
    }
};
```

# Tries

## 208. Implement Trie (Prefix Tree)

Notes: custom TrieNode (map<char, TN*> children, bool end, string word). insert function to create trie. two forloops to iterate to see if you find your prefix/word.

```cpp
// O(L) Time (length of word)
// O(N * L) Space (# of words * length)

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool end;
    string word;
    TrieNode() : end(false), word("") {}
};
class Trie {
public:
    TrieNode* root = new TrieNode();
    
    void insert(string word) {
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
    
    bool search(string word) {
        TrieNode* temp = root;
        for(int i = 0; i < word.size(); ++i){
            if(temp->children.find(word[i]) == temp->children.end()) return false;
            temp = temp->children[word[i]];
        }
        return temp->end;  // Return whether the current node marks the end of a word
    }
    
    bool startsWith(string prefix) {
        TrieNode* temp = root;
        for(int i = 0; i < prefix.size(); ++i){
            if(temp->children.find(prefix[i]) == temp->children.end()) return false;
            temp = temp->children[prefix[i]];
        }
        return true; // All characters found, return true
    }
};
```

## 211. Design Add and Search Words Data Structure

Notes: default TrieNode struct setup + dfs for search. If its a dot use for loop to dfs check all children, else proceed as normal. 

```cpp
// O(L) Time
// O(N * L) Space (# of words in dict * length of words)
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool end;
    string word;
    TrieNode() : end(false), word("") {}
};

class WordDictionary {
public:
    TrieNode* root = new TrieNode();

    void addWord(string word) {
        TrieNode* temp = root;
        for(const auto& c : word) {
            if(temp->children.find(c) == temp->children.end()){
                temp->children[c] = new TrieNode();
            }
            temp = temp->children[c];
        }
        temp->end = true;
        temp->word = word;
    }
    
    bool search(string word) {
        TrieNode* temp = root;
        return dfs(word, temp, 0);
    }
    bool dfs(const string& word, TrieNode* curr, int i){
        if(i == word.size()) return curr->end;
        char c = word[i];
        if(c != '.'){ //proceed as regular
            if(curr->children.find(c) == curr->children.end()) return false;
            return dfs(word, curr->children[c], i + 1);
        }
        else{ //its a dot check all children
            for(const auto [_, node] : curr->children){
                if(dfs(word, node, i + 1)) return true;
            }
        }
        return false; // no match found
    }
};
```

# Heap/Priority Queue

### 703. Kth Largest Element in a Stream

Notes: pq<greater> of size k, return top (pop if pq size > k)

```cpp
// O(nlogn) time
// O(k) space

class KthLargest {
public:
    priority_queue<int, vector<int>, greater<>> pq;
    int k;
    KthLargest(int k, vector<int>& nums) : pq(nums.begin(), nums.end()), k(k) {
        while(pq.size() > k) pq.pop();
    }
    
    int add(int val) {
        pq.emplace(val); 
        if(pq.size() > k) pq.pop();
        return pq.top();
    }
};
```

### 1046. Last Stone Weight

Notes: pq regular while pq.size() > 1

```cpp
// nlogn time
// n space

class Solution {
public:
    int lastStoneWeight(vector<int>& stones) {
        priority_queue<int> pq(stones.begin(), stones.end());
        while(pq.size() > 1){
            auto x = pq.top(); pq.pop();
            auto y = pq.top(); pq.pop();
            pq.emplace(abs(x-y));
        }
        return pq.top();
    }
};
```

### 973. K Closest Points to Origin

Notes: using T = tuple<double, int, int>; //distance, x, y. pq<T> of size k

```cpp
// nlognk time
// k space

class Solution {
    using T = tuple<double, int, int>; //distance, x, y
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        priority_queue<T> pq; //maxHeap which will be size k
        for(auto point : points) {
            double distance = sqrt(point[0] * point[0] + point[1] * point[1]);
            pq.emplace(distance, point[0], point[1]);
            if(pq.size() > k) pq.pop();
        }
        vector<vector<int>> res;
        while(!pq.empty()){
            auto [_, x, y] = pq.top(); pq.pop();
            res.push_back({x, y});
        }

        return res;
    }
};
```

### 215. Kth Largest Element in an Array

Notes: minheap using greater<> 

```cpp
//nlogn time
//n space
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        //minheap
        priority_queue<int, vector<int>, greater<>> pq (nums.begin(), nums.end());
        while(pq.size() > k) pq.pop();
        return pq.top();
    }
};
```

### 621. Task Scheduler 

Notes: freqMap, maxheap pq<int freq>, queue<<int freq, int time>> while(!pq or !q empty): if(!pq) if freq > 1 q.emplace(--freq, time + n). if(!q) if timestamp == time, pq.emplace(curFreq) q.pop()

```cpp
// Time Complexity: O(TlogU) where T is the # tasks and U is # of unique tasks
// Space Complexity O(T + U)

class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        map<char, int> m; 
        for(auto& c: tasks) m[c]++; //freqMap

        //maxheap for frequency (we want most freq to get processed first)
        priority_queue<int> pq; 
        queue<pair<int,int>> q; //freq, time

        //we dont need char val, each value in pq is a char
        for(auto [_, freq] : m) pq.emplace(freq); 

        int time = 0;
        while(!pq.empty() or !q.empty()){
            ++time;
            if(!pq.empty()){
                auto freq = pq.top(); pq.pop();
                if(freq > 1) { //doesnt make to process freq 0 nodes
                    q.emplace(--freq, time + n);
                }
            }
            if(!q.empty()){
                auto [curFreq, timeStamp] = q.front();
                if(timeStamp == time){
                    pq.emplace(curFreq);
                    q.pop();
                } 
            }
        }
        
        return time;
    }
};
```

## 355. Design Twitter

Notes: pq<tuple<time, tweetId, userId>>, map<followerId, set<followeeId>>, int time;

```cpp
// O(nlogn) time
// O(N) Space
 
class Twitter {
    using T = tuple<int, int, int>; // time, tweetId, userId
public:
    unordered_map<int, unordered_set<int>> m; //maps followerId -> followeeID
    priority_queue<T> pq; // time, tweetId, userId
    int time; // Global timestamp
    Twitter() : time(0) {}
    
    void postTweet(int userId, int tweetId) {
        pq.emplace(++time, tweetId, userId);
    }
    
    vector<int> getNewsFeed(int userId) {
        vector<int> result;
        priority_queue<T> feed = pq;
        while(!feed.empty() and result.size() < 10){
            auto [time, tweetId, userPosted] = feed.top(); feed.pop();
            if(m[userId].find(userPosted) != m[userId].end() or userId == userPosted){
                result.emplace_back(tweetId);
            }
            if(result.size() == 10) return result;
        }
        return result;
    }
    
    void follow(int followerId, int followeeId) {
        m[followerId].emplace(followeeId);
    }
    
    void unfollow(int followerId, int followeeId) {
        m[followerId].erase(followeeId);
    }
};
```

## 295. Find Median from Data Stream

Notes: 2 pq: maxheap for leftside, minheap for rightSide. when you add rebalance pqs otherwise return top of each to get the median

```cpp
// O(logn), O(1) Time
// O(n) Space

class MedianFinder {
public:
    priority_queue<int> leftheap; //maxheap
    priority_queue<int, vector<int>, greater<>> rightheap; //minheap

    /*
    we want larger numbers on right side (min heap) where top is smallest
    smaller numbers on left side (max heap) where the top is our largest
    */
    
    void addNum(int num) {
        if(leftheap.empty()){
            leftheap.emplace(num);
            return;
        }
        if(leftheap.top() <= num) rightheap.emplace(num);
        else leftheap.emplace(num);

        if(leftheap.size() < rightheap.size()){
            leftheap.emplace(rightheap.top()); rightheap.pop();
        }
        if(leftheap.size() - rightheap.size() > 1) {
            rightheap.emplace(leftheap.top()); leftheap.pop();
        }

    }
    
    double findMedian() {
        if((leftheap.size() + rightheap.size()) % 2 == 0){
            return (leftheap.top() + rightheap.top()) / 2.0;
        }
        return leftheap.top();
    }
};
```

single pass approach (not as intuitive)

```cpp
void addNum(int num) {
    maxHeap.push(num);
    minHeap.push(maxHeap.top());
    maxHeap.pop();

    if(minHeap.size() > maxHeap.size()){
        maxHeap.push(minHeap.top());
        minHeap.pop();
    }
}
```

# Backtracking

## 78. Subsets

Notes: in backtrack, #1 push back curr vector to result. #2 for loop i = start #3 curr.p_b(n[i]) #4 backtrack i + 1 #5 curr.pop_back()

```cpp
// O(N * 2^N) Time to generate all subsets and copy to output list
// O(N * 2^N) Space to store all subsets

class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> curr;
        backtrack(result, curr, 0, nums);
        return result;
    }

    void backtrack(vector<vector<int>>& result, vector<int>& curr, int idx, vector<int>& nums){
        //push back immediately
        result.push_back(curr);
        for(int i = idx; i < nums.size(); ++i){
            //push back number
            curr.push_back(nums[i]);
            //run dfs style backtrack
            backtrack(result, curr, i + 1, nums);
            //delete last addition before moving onto next number
            curr.pop_back();
        }
    }
};
```

## 39. Combination Sum

Notes: backtrack: if csum > target return; if(csum == target) add curr vec to res and return; for(int i = start) 1. curr.add c[i], 2. backtrack(i as start bc we want duplicates), 3. curr.pop_back()

```cpp
// O(N ^ (T/M + 1)) Time where T is target value, M is the minimum value among candidates
// O(T/M) Space

class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> curr;
        backtrack(result, curr, 0, 0, candidates, target);
        return result;
    }

    void backtrack(vector<vector<int>>& result, vector<int>& curr, int start, int cSum, vector<int>& candidates, int target){
        if(cSum > target) return;
        if(cSum == target) {
            result.emplace_back(curr);
            return;
        }
        for(int i = start; i < candidates.size(); ++i){
            curr.emplace_back(candidates[i]);
            backtrack(result, curr, i, cSum + candidates[i], candidates, target);
            curr.pop_back();
        }
    }
};
```

## 46. Permutations

Notes: backtrack, but instead of passing in int start, pass in vector<visited>. for i (0, n) if(!visited) visited = 1, curr.add n[i], backtrack, curr.pop, visited = 0

```cpp
//O(n!) time
//O(n * n!) space

class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> curr;
        vector<int> visited (nums.size(), 0);
        backtrack(res, curr, visited, nums);
        return res;
    }

    void backtrack(vector<vector<int>>& res, vector<int>& curr, vector<int>& visited, vector<int>& nums){
        if(curr.size() == nums.size()) {
            res.emplace_back(curr);
            return;
        }
        for(int i = 0; i < nums.size(); ++i){
            if(visited[i] == 0){
                visited[i] = 1;
                curr.emplace_back(nums[i]);
                backtrack(res, curr, visited, nums);
                curr.pop_back();
                visited[i] = 0;
            }
        }
    }
};
```

## 90. Subsets II

Notes: default backtrack setup to skip duplicate elems with 

if (i != start && nums[i] == nums[i - 1]) continue;

```cpp
// O(n * 2^n) Time
// O(n) Space

class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> curr;
        sort(nums.begin(), nums.end());
        backtrack(result, nums, curr, 0);
        return result;
    }

    void backtrack(vector<vector<int>>& result, vector<int>& nums, vector<int>& curr, int start){
        result.emplace_back(curr);
        for(int i = start; i < nums.size(); ++i){
            // If the current element is a duplicate, ignore.
            if (i != start && nums[i] == nums[i - 1]) {
                continue;
            }
            curr.emplace_back(nums[i]);
            backtrack(result, nums, curr, i + 1);
            curr.pop_back();
        }
    }

};
```

## 40. Combination Sum II

Notes: default backtrack setup with duplicate check `if(i != start && candidates[i]==candidates[i-1]) continue;`

```cpp
// Time Complexity: O(2^n), In the worst case, we generate all possible combinations
// O(N) Space

class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> curr;
        sort(candidates.begin(), candidates.end());
        dfs(result, curr, candidates, 0, 0, target);
        return result;
    }

    void dfs(vector<vector<int>>& result, vector<int>& curr, vector<int>& candidates, int start, int cSum, int target){
        if(cSum > target) return;
        if(cSum == target) {
            result.emplace_back(curr);
            return;
        }

        for(int i = start; i < candidates.size(); ++i){
            //to avoid picking up the same combnations i.e. 
            //we don't pick same element for certain kth position of a combination 
            if(i != start && candidates[i]==candidates[i-1]) continue;
            curr.emplace_back(candidates[i]);
            dfs(result, curr, candidates, i + 1, cSum + candidates[i], target);
            curr.pop_back();
        }
    }   
};
```

## 79. Word Search

Notes: TrieNode setup + dfs

```cpp
// Building the Trie: O(L), where L is the length of the word.
// Searching in the grid: O(M * N * 4^L), where M is the number of rows, 
// N is the number of columns in the board, and L is the length of the word. 
// In the worst case, for each cell, we explore all four directions, and the 
// depth of exploration is bounded by the length of the word.
// So, the overall time complexity is O(L + M * N * 4^L).

// O(L) Space

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    string word;
    bool end;
    TrieNode() : word(""), end(false) {}
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
            temp = temp->children[c];
        }
        temp->end = true;
        temp->word = word;
    }

    void dfs(vector<vector<char>>& board, int i, int j, TrieNode* curr, bool& ans){
        if(i < 0 or i >= board.size() or j < 0 or j >= board[0].size()) return;
        if(board[i][j] == '#' or ans) return;

        char c = board[i][j];
        if(curr->children.find(c) == curr->children.end()) return; // not valid

        board[i][j] = '#';
        curr = curr->children[c];
        if(curr->end){
            ans = true;
            return;
        }

        dfs(board, i + 1, j, curr, ans);
        dfs(board, i - 1, j, curr, ans);
        dfs(board, i, j + 1, curr, ans);
        dfs(board, i, j - 1, curr, ans);
        board[i][j] = c;
    }

    bool exist(vector<vector<char>>& board, string word) {
        insert(word);
        TrieNode* temp = root;
        bool ans = false;
        for(int i = 0; i < board.size(); ++i){
            for(int j = 0; j < board[0].size(); ++j){
                if(temp->children.find(board[i][j]) != temp->children.end()){
                    dfs(board, i, j, temp, ans);
                    if(ans) return ans;
                }
            }
        }
        return ans;
    }
};
```

## 131. Palindrome Partitioning

Notes: std backtrack except in for loop maintain temp string char by char

```
temp += s[i];
if(isPalindrome(temp)){
    curr.push_back(temp);
    dfs(res, curr, i + 1, s);
    curr.pop_back();
}
```

```cpp
// O(N * 2^N) Time
// O(N) Space

class Solution {
public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> res;
        vector<string> curr;
        dfs(res, curr, 0, s);
        return res;
    }

    void dfs(vector<vector<string>>& res, vector<string>& curr, int start, string& s){
        if(start == s.size()) {
            res.emplace_back(curr);
            return;
        }
        string temp = "";
        for(int i = start; i < s.size(); ++i){
            temp += s[i];
            if(isPalindrome(temp)){
                curr.push_back(temp);
                dfs(res, curr, i + 1, s);
                curr.pop_back();
            }
        }
    }
    bool isPalindrome(const string& s){
        string rev = s;
        reverse(rev.begin(), rev.end());
        return s == rev;
    }
};
```

## 17. Letter Combinations of a Phone Number

Notes: standard backtrack setup + map to map char ['x'] = {"x", "y", "z"}; then in dfs `vector<string> temp = m[digits[start]];` and loop on each index

```
for(const auto& str : temp){
    curr += str;
    dfs(result, curr, m, digits, start + 1);
    curr.pop_back();
}
```

```cpp
// Time Complexity: O(4^N * N) where 4 comes from in the worst case, 
// each digit maps to 4 letters, and we have 4 choices at each step.

// O(N) Space
class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if(digits.empty()) return {};
        unordered_map<char, vector<string>> m;
        m['2'] = {"a", "b", "c"};
        m['3'] = {"d", "e", "f"};
        m['4'] = {"g", "h", "i"};
        m['5'] = {"j", "k", "l"};
        m['6'] = {"m", "n", "o"};
        m['7'] = {"p", "q", "r", "s"};
        m['8'] = {"t", "u", "v"};
        m['9'] = {"w", "x", "y", "z"};

        vector<string> result;
        string curr = "";
        dfs(result, curr, m, digits, 0);
        return result;
    }

    void dfs(vector<string>& result, string curr, unordered_map<char, vector<string>>& m, string& digits, int start){
        if(curr.size() == digits.size()){
            result.emplace_back(curr);
            return;
        } 

        vector<string> temp = m[digits[start]];
        for(const auto& str : temp){
            curr += str;
            dfs(result, curr, m, digits, start + 1);
            curr.pop_back();
        }
    }
};
```

# Graphs

## 200. Number of Islands

Notes: dfs left right up down

```cpp
// O(R * C) Time (rows x cols)
// O(R * C) Space (call stack during recursion)

class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int numIslands = 0;
        for(int i = 0; i < grid.size(); ++i){
            for(int j = 0; j < grid[0].size(); ++j){
                if(grid[i][j] == '1'){
                    dfs(grid, i, j);
                    numIslands++;
                }
            }
        }
        return numIslands;
    }

    void dfs(vector<vector<char>>& grid, int i, int j){
        if(i < 0 or i >= grid.size() or j < 0 or j >= grid[0].size()) return;
        if(grid[i][j] != '1') return;
        grid[i][j] = '0';
        dfs(grid, i + 1, j);
        dfs(grid, i - 1, j);
        dfs(grid, i, j + 1);
        dfs(grid, i, j - 1);
    }
};
```

## 133. Clone Graph

Notes: similar to deep copy of linked list, two pass (dfs to copy in map first pass, update neighbors in second pass)

```cpp
// O(V+E): Where V # vertices (nodes) and E is # edges (connections)
// O(V) Space

class Solution {
public:
    Node* cloneGraph(Node* node) {
        unordered_map<Node*, Node*> copyMap;
        dfs(node, copyMap);
        return copyMap[node];
        
    }
    void dfs(Node* node, unordered_map<Node*, Node*>& copyMap){
        if (!node) return;
        if (copyMap.find(node) == copyMap.end()) {
            copyMap[node] = new Node(node->val);
            // first pass to clone all nodes (similar to link list)
            for (auto neighbor : node->neighbors) {
                dfs(neighbor, copyMap); // Ensure neighbor is cloned
            }
            //second pass to add all nodes
            for (auto neighbor : node->neighbors) {
                // Now add the cloned neighbors to the cloned node's neighbors
                copyMap[node]->neighbors.emplace_back(copyMap[neighbor]);
            }
        }
    }

};
```

## 695. Max Area of Island

Notes: num islands but keep int curr size

```cpp
// O(R * C) Time (rows x cols)
// O(R * C) Space (call stack during recursion)

class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int maxArea = 0;
        for(int i = 0; i < grid.size(); ++i){
            for(int j = 0; j < grid[0].size(); ++j){
                if(grid[i][j] == 1) {
                    int cMax = 0;
                    dfs(grid, i, j, cMax);
                    maxArea = max(maxArea, cMax);
                }
            }
        }
        return maxArea;
    }
    void dfs(vector<vector<int>>& grid, int i, int j, int& curr){
        if(i < 0 or i >= grid.size() or j < 0 or j >= grid[0].size()) return;
        if(grid[i][j] != 1) return;
        curr++;
        grid[i][j] = 0; // mark as visited
        dfs(grid, i + 1, j, curr);
        dfs(grid, i - 1, j, curr);
        dfs(grid, i, j + 1, curr);
        dfs(grid, i, j - 1, curr);
    }
};
```

## 417. Pacific Atlantic Water Flow

Notes: 2 dfs's Pacific and Atlantic. Vector bools for each to mark as visited. if(heights[i][j] < prev) return.

```cpp
// O(R * C) Time
// O(R * C) Space

class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        vector<vector<bool>> pacific (heights.size(), vector<bool> (heights[0].size(), false));
        vector<vector<bool>> atlantic (heights.size(), vector<bool> (heights[0].size(), false));
        for(int i = 0; i < heights.size(); ++i){ // left and right
            dfs(i, 0, INT_MIN, pacific, heights);
            dfs(i, heights[0].size() - 1, INT_MIN, atlantic, heights);
        }

        for(int j = 0; j < heights[0].size(); ++j){ // top and bottom
            dfs(0, j, INT_MIN, pacific, heights);
            dfs(heights.size() - 1, j, INT_MIN, atlantic, heights);
        }
        vector<vector<int>> res;
        for(int i = 0; i < heights.size(); ++i){
            for(int j = 0; j < heights[0].size(); ++j){
                if(atlantic[i][j] and pacific[i][j]) res.push_back({i, j});
            }
        }
        return res;
    }

    void dfs(int i, int j, int prev, vector<vector<bool>>& valid, vector<vector<int>>& heights){
        if(i < 0 or j < 0 or i >= heights.size() or j >= heights[0].size()) return;
        if(valid[i][j]) return; // alr visited
        if(heights[i][j] < prev) return;
        valid[i][j] = true;
        dfs(i + 1, j, heights[i][j], valid, heights);
        dfs(i - 1, j, heights[i][j], valid, heights);
        dfs(i, j + 1, heights[i][j], valid, heights);
        dfs(i, j - 1, heights[i][j], valid, heights);
    }
};
```

## 130. Surrounded Regions

Notes: 1st pass dfs on edges of rectangle to convert all O's to Y's (unvisitable). then in second for loop turn all Y's back to O's and any surviving O's into X's

```cpp
// O(n*m) time
// O(n*m) space
class Solution {
public:
    void solve(vector<vector<char>>& board) {
        //take all the o's on the edge and turn them into y's
        for(int i = 0; i < board.size(); ++i){
            for(int j = 0; j < board[0].size(); ++j){
                if(i == 0 or i == board.size() - 1 or j == 0 or j == board[0].size() - 1){
                    if(board[i][j] == 'O') dfs(i, j, board);
                }
            }
        }

        //all nodes marked at this point are unsurroundable. so unmarked nodes can be X'd

        for(int i = 0; i < board.size(); ++i){
            for(int j = 0; j < board[0].size(); ++j){
                if(board[i][j] == 'O') board[i][j] = 'X';
                else if(board[i][j] == 'Y') board[i][j] = 'O';
            }
        }

    }
    void dfs(int i, int j, vector<vector<char>>& board){
        if(i < 0 or j < 0 or i >= board.size() or j >= board[0].size()) return;
        if(board[i][j] != 'O') return; //visited
        board[i][j] = 'Y';
        dfs(i + 1, j, board);
        dfs(i - 1, j, board);
        dfs(i, j + 1, board);
        dfs(i, j - 1, board);
    }
};
```

## 994. Rotting Oranges

Notes: BFS using T = i, j, time. first pass for loop if grid[i][j] == 2, add 4 dirs to q. else if 1 inc fresh count. do bfs and inc max time. return fresh == 0 ? maxTime : -1; 

```cpp
class Solution {
public:
    using T = tuple<int, int, int>; //time, i, j
    int orangesRotting(vector<vector<int>>& grid) {
        queue<T> q; 
        int fresh = 0;
        for(int i = 0; i < grid.size(); ++i){
            for(int j = 0; j < grid[0].size(); ++j){
                if(grid[i][j] == 2){
                    //put neighbors in queue at time 0
                    q.emplace(i + 1, j, 0);
                    q.emplace(i - 1, j, 0);
                    q.emplace(i, j + 1, 0);
                    q.emplace(i, j - 1, 0);
                }
                else if(grid[i][j] == 1) fresh++;
            }
        }
        int maxTime = 0;
        while(!q.empty()){
            auto [i, j, time] = q.front(); q.pop();
            if(i < 0 or j < 0 or i >= grid.size() or j >= grid[0].size()) continue;
            if(grid[i][j] != 1) continue; //we have no fresh orange to process
            grid[i][j] = 2; // mark as rotten
            fresh--;
            maxTime = max(maxTime, time + 1);
            q.emplace(i + 1, j, time + 1);
            q.emplace(i - 1, j, time + 1);
            q.emplace(i, j + 1, time + 1);
            q.emplace(i, j - 1, time + 1);
        }
        return fresh == 0 ? maxTime : -1;
    }
};
```

