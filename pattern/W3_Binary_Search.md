# W3 Binary Search (二分搜尋)

- $O(\log N)$ 時間複雜度
- 邊界條件 (Boundary Conditions)
- 搜尋範圍：左閉右閉 vs 左閉右開
- 二分搜尋答案 (Binary Search on Answer)

---

## 1. 核心原理與應用場景

### A. 核心原理：排除法

二分搜尋的核心並不是「尋找」，而是**「排除」**。在一個**具有單調性 (Monotonicity)** 的集合中，透過每次檢查中間元素，我們可以一次排除掉一半的搜尋範圍。

* **時間複雜度：** 每次範圍砍半，故為$O(\log N)$ 。
* **前提條件：** 資料必須是**已排序**的，或者具有某種二段性（一半滿足條件，另一半不滿足）。

### B. 什麼時候該用？

* **有序陣列中找特定值：** 最基礎的應用。
* **尋找邊界值：** 找第一個大於等於$X$的位置（Lower Bound），或最後一個小於等於$X$的位置。
* **搜尋答案 (Binary Search on Answer)：** 當問題要求一個「最大值的最小值」或「最小值的最大值」，且答案範圍固定時。
    * *例子：* 給定固定時間，求工人最少需要多大的搬運能力。
* **數值逼近：** 如求平方根 $\sqrt{x}$ ，或在連續函數中找根。

---

## 2. C 語言實作時最常見的 Bug

二分搜尋邏輯看似簡單，但邊界處理極其容易導致死迴圈或索引越界：

1. **整數溢位 (Integer Overflow)：**
* **錯誤：** `int mid = (left + right) / 2;` 如果 `left` 和 `right` 都很大，相加會超出 `int` 範圍。
* **正確：** `int mid = left + (right - left) / 2;`


2. **死迴圈 (Infinite Loop)：**
* 最常發生在 `while (left < right)` 且更新為 `left = mid` 時。若 `left` 與 `right` 只差 1，`mid` 會一直等於 `left`，導致範圍不再縮小。


3. **邊界判斷錯誤 (Off-by-one Error)：**
* 搞不清楚 `right` 初始該給 `n` 還是 `n - 1`。
* 搞不清楚 `while` 條件該用 `<=` 還是 `<`。


4. **未排序就搜尋：**
* 在 C 語言中，若傳入 `bsearch()` 或自定義函數前忘記先用 `qsort()` 排序，結果會完全錯誤。



---

## 3. C 語言萬用模板

為了避免邏輯混亂，建議在 C 語言中統一使用 **「左閉右閉 [left, right]」** 的思維，這是最直覺且不容易出錯的寫法。

### A. 基礎範本：尋找精確值

```c
int binarySearch(int* nums, int numsSize, int target) {
    int left = 0;
    int right = numsSize - 1; // [left, right]

    while (left <= right) { // 當 left > right 時終止
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            left = mid + 1; // 排除左半邊
        } else {
            right = mid - 1; // 排除右半邊
        }
    }
    return -1; // 找不到
}

```

### B. 進階範本：尋找左邊界 (Lower Bound)

常用於 `nums` 中有重複數字，要找「第一個」出現的位置，或第一個  target 的位置。

```c
int findFirst(int* nums, int numsSize, int target) {
    int left = 0;
    int right = numsSize - 1;
    int ans = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] >= target) {
            ans = mid;      // 暫存可能是答案的位置
            right = mid - 1; // 繼續往左邊找，看有沒有更小的索引
        } else {
            left = mid + 1;
        }
    }
    return ans;
}

```

### C. 搜尋答案模板 (Binary Search on Answer)

用於優化問題，假設我們要找滿足 `check()` 條件的「最小整數值」。

```c
bool check(int val, int* data, int size) {
    // 根據題目判斷 val 是否可行
    // 返回 true 或 false
}

int solve(int* data, int size, int min_range, int max_range) {
    int left = min_range;
    int right = max_range;
    int ans = max_range;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (check(mid, data, size)) {
            ans = mid;      // 此值可行，嘗試找找看有沒有更小的
            right = mid - 1;
        } else {
            left = mid + 1; // 此值不可行，必須增加數值
        }
    }
    return ans;
}

```

### D. 標準庫 `bsearch` 範例

C 語言 `<stdlib.h>` 內建了 `bsearch`，但僅限於精確值搜尋。

```c
#include <stdlib.h>

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// 使用方式
int* item = bsearch(&target, nums, numsSize, sizeof(int), compare);
if (item != NULL) {
    int index = item - nums; // 透過指針位移計算索引
}

```

### 時間複雜度補充

二分搜尋的時間複雜度為$O(\log N)$ ，核心原因在於其**「搜尋範圍縮減的速度」**。

從數學推導與結構邏輯兩個面向來理解：

1. 數學推導：從 $N$ 縮減到 $1$

假設資料總量為 $N$ ，我們每進行一次比較（一步），搜尋範圍就會縮小為原來的一半。

* **第 0 步：** 剩下 $N$ 個元素。
* **第 1 步：** 剩下 $\frac{N}{2}$ 個元素。
* **第 2 步：** 剩下 $\frac{N}{2^2} = \frac{N}{4}$ 個元素。
* **第  步：** 剩下 $\frac{N}{2^k}$ 個元素。

當我們找到目標值或是搜尋範圍縮小到只剩下 $1$個元素時，搜尋停止。因此我們可以令：

$$\frac{N}{2^k} = 1$$

解出 $k$（步數）：

$N = 2^k$$

$$\log_2 N = k$$


這代表在最差的情況下，我們只需要 $\log_2 N$步就能找到答案或確定不存在。在演算法的大$O$表示法中，對數的底數通常省略，記作 $O(\log N)$。

---

2. 決策樹的結構

二分搜尋的過程可以想像成一棵**平衡二元樹**。每次比較中間值（Mid），就是在決定要往左子樹還是右子樹走。

根據先前學習 Heap 的筆記內容：

- 一個擁有 $n$ 個節點的完全二元樹，其高度 $H$ 與總節點數 $n$ 的關係為 $H = \log_2(n + 1)$。
- 從根節點走到最底層的葉子節點，最多經過的步數就是樹的高度 $H-1$。
- 由於 $H$ 與 $\log n$ 成正比，這段搜尋路徑的長度就是 $O(\log n)$。

