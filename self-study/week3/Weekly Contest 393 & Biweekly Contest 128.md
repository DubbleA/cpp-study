# Weekly Contest 393

## Q3 3116. Kth Smallest Amount With Single Denomination Combination

https://leetcode.com/problems/kth-smallest-amount-with-single-denomination-combination/solutions/5019598/binary-search-bitmasking/

```cpp
class Solution {
public:
    long long findKthSmallest(vector<int>& coins, int k) {
        long long kk = k;
        long long l = 1, r = 1e11, ans = 0;
        while (l <= r) {
            long long m = l + (r - l) / 2;
            long long cnt = 0;
            cnt = pie(coins, m);
            if (cnt < kk) l = m + 1;
            else {
                ans = m;
                r = m - 1;
            }
        }
        return ans;
    }

    //calculates the count of values less than or equal to x 
    //which are divisible by at least one number from the coins.
    long long pie(vector<int> &coins, long long x)
    {
        auto m = coins.size();
        auto nn = (1 << m); //equal to 2^m (total number of combinations of coins)
        long long cnt = 0;
        for (int i = 1; i < nn; i++) {
            auto lcmm = 1;
            //for each combination calculate current least common multiple
            for (int j = 0; j < m; j++) if (i & (1 << j)) lcmm = lcm(lcmm, coins[j]);
            //__builtin_popcount(i) returns the number of bits set in i 
            //(i.e., the number of selected coins)
            if (__builtin_popcount(i) & 1) cnt += x / lcmm;

            //If the count is even, it subtracts from cnt 
            //the same quantity to correct for over-counting.
            else cnt -= x / lcmm; 
        }
        return cnt;
    }
};
```

## Q4 3117. Minimum Sum of Values by Dividing Array

