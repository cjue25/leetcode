# W7 Dynamic Programming (動態規劃)

* Overlapping Subproblems (重疊子問題)
* Optimal Substructure (最佳子結構)
* Memoization (Top-Down / 記憶化搜索)
* Tabulation (Bottom-Up / 遞迴轉迭代)
* State Transition Equation (狀態轉移方程式)

---

## 1. 核心原理與應用場景

### A. 核心原理：記憶與查表

動態規劃本質上是**「分治法 + 記憶體」**。當我們把一個大問題拆成許多小問題時，如果發現這些小問題會被「重複計算」（重疊子問題），我們就可以拿空間換取時間，把算過的結果存進一個陣列（DP Table）裡，下次遇到直接查表拿答案。

在實作上，DP 分為兩大派系：
1. **Top-Down (由上而下 - 記憶化搜索)：** 從大問題開始，搭配遞迴，算過的值存入 `memo` 陣列。思維直觀，就是帶有外掛的 DFS。

2. **Bottom-Up (由下而上 - 遞迴轉迭代)：** 找出最小的 Base Case，用 `for` 迴圈一路推導到大問題。這是 C 語言中最常見也最有效率的寫法。

* 核心三靈魂：**定義狀態**（`dp[i]` 代表什麼）、**找出轉移方程式**（`dp[i]` 怎麼從 `dp[i-1]` 推過來）、**初始化 Base Case**。

### B. 什麼時候該用？

如果你在題目中看到以下關鍵字，而且發現用回溯法 (DFS) 會 Time Limit Exceeded (TLE)，九成九就是 DP：

* **求極值：** 「最大」利潤、「最長」共同子字串、「最短」路徑、「最少」步數。
* **求方法數：** 走到右下角有「幾種」走法、湊成目標金額有「幾種」組合。
* **求存在性：** 「能否」跳到最後一格、「是否」能分割成兩個和相等的子集。

---

## 2. C 語言實作時最常見的 Bug

在 C 語言寫 DP，陣列的操作與邊界控管是生死關鍵：

1. **陣列越界 (Index Out of Bounds)：**
* **錯誤：** 狀態轉移方程式寫了 `dp[i] = dp[i-1] + dp[i-2]`，但迴圈卻從 `i = 0` 開始，直接存取到 `dp[-1]` 導致 Segmentation Fault。
* **對策：** 迴圈的起點必須從 Base Case 的下一個開始（例如 `i = 2`）。

2. **記憶體大小開錯 (`N` vs `N+1`)：**
* **錯誤：** 題目要求長度 $N$ 的字串，你宣告 `int dp[N]`，但你的狀態定義如果是「前 $i$ 個字元」，其實你需要 `dp[N+1]` 的空間來容納 0 的狀態。
* **對策：** 寧可多開一格，宣告 `int dp[n + 1]`。

3. **忘記初始化或初始化錯誤 (Uninitialized Memory)：**
* **錯誤：** 使用 `malloc` 開啟 DP 陣列卻沒用 `memset` 歸零，裡面全是垃圾值。或者要求「最小值」時，把陣列初始化為 0（應該初始化為非常大的數，如 `10000` 或 `INT_MAX`）。
* **對策：** 求極小值時要小心 `INT_MAX` 加 1 會變負數（Integer Overflow），建議用一個安全的極大值，例如 `0x3f3f3f3f`。

4. **二維陣列空間爆掉 (Memory Limit Exceeded)：**
* **錯誤：** LeetCode 上遇到 $10000 \times 10000$ 的二維 DP，直接 `int dp[10000][10000]` 會瞬間吃爆 Stack 或是超出記憶體限制。
* **對策：** 使用 `malloc` 放 Heap，或者進階一點使用「空間最佳化（Rolling Array / 狀態壓縮）」。

---

## 3. C 語言萬用模板

### A. 1D DP (一維動態規劃 - Bottom-Up)

這是最標準的迭代寫法，以經典的「爬樓梯 (Climbing Stairs)」或「費式數列」為例。

```c
int solveDP(int n) {
    // 1. 處理極端情況 (Edge cases)
    if (n == 0) return 0;
    if (n == 1) return 1;

    // 2. 宣告 DP 陣列 (注意大小通常是 n + 1)
    int* dp = (int*)malloc(sizeof(int) * (n + 1));
    // 如果需要全歸零： memset(dp, 0, sizeof(int) * (n + 1));

    // 3. 初始化 Base Case
    dp[0] = 0; 
    dp[1] = 1; 
    // dp[2] = 2; // 視題目而定

    // 4. 迴圈推導狀態轉移方程式
    for (int i = 2; i <= n; i++) {
        // 狀態轉移方程式 (State Transition Equation)
        dp[i] = dp[i - 1] + dp[i - 2]; 
    }

    // 5. 取出最終答案並釋放記憶體
    int result = dp[n];
    free(dp);
    
    return result;
}
```

### B. 2D DP (二維動態規劃)

常見於網格路徑問題 (Grid) 或是字串比對 (LCS)。
```c
int minPathSum(int** grid, int gridSize, int* gridColSize) {
    int m = gridSize;
    int n = gridColSize[0];

    // 動態配置二維 DP 陣列
    int** dp = (int**)malloc(sizeof(int*) * m);
    for (int i = 0; i < m; i++) {
        dp[i] = (int*)malloc(sizeof(int) * n);
    }

    // 初始化起點
    dp[0][0] = grid[0][0];

    // 初始化第一列 (只能從左邊來)
    for (int i = 1; i < n; i++) dp[0][i] = dp[0][i-1] + grid[0][i];
    
    // 初始化第一欄 (只能從上面來)
    for (int i = 1; i < m; i++) dp[i][0] = dp[i-1][0] + grid[i][0];

    // 填表
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            // 狀態轉移：選上面和左邊比較小的，加上自己這格的代價
            int min_prev = (dp[i-1][j] < dp[i][j-1]) ? dp[i-1][j] : dp[i][j-1];
            dp[i][j] = min_prev + grid[i][j];
        }
    }

    int res = dp[m-1][n-1];

    // 記得釋放記憶體
    for (int i = 0; i < m; i++) free(dp[i]);
    free(dp);

    return res;
}
```

### C. 空間最佳化 (Space Optimization - 滾動陣列)

當你發現 `dp[i]` **只依賴前幾個狀態**（例如只看 `i-1` 和 `i-2`），你根本不需要保留整個陣列，只需幾個變數就能將空間複雜度從 $O(N)$ 降到 $O(1)$。

```c
// 空間最佳化版的爬樓梯
int climbStairsOpt(int n) {
    if (n <= 2) return n;

    int prev2 = 1; // 相當於 dp[i-2]
    int prev1 = 2; // 相當於 dp[i-1]
    int curr = 0;  // 相當於 dp[i]

    for (int i = 3; i <= n; i++) {
        curr = prev1 + prev2; // 轉移
        prev2 = prev1;        // 狀態滾動：舊的 i-1 變成新的 i-2
        prev1 = curr;         // 狀態滾動：當前的 i 變成新的 i-1
    }
    return curr;
}
```

---

### 補充筆記：

1. ** `dp[i]` 的定義：** 寫下任何一行 Code 之前，先在註解寫下「`dp[i]` 代表長度為 `i` 的最大利潤」或是「`dp[i][j]` 代表字串 A 的前 `i` 個與字串 B 的前 `j` 個的最長長度」。定義錯了，方程式一定錯。
2. **一定要畫表格：** 遇到不會推的 DP，請拿出一張紙，手動把 `N=1` 到 `N=4` 的表格畫出來。人類大腦很難直接想出轉移方程式，但擅長從表格找規律。
3. **Debug 技巧：** 如果答案不對，把迴圈裡的 `printf("%d ", dp[i]);` 打開，去比對你印出來的 DP 陣列跟你在紙上畫的表格哪一格開始不一樣，Bug 就在那裡。