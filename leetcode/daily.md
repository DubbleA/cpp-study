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

