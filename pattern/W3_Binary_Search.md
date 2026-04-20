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

**建議將「左閉右開 `[left, right)`」作為未來的統一寫法。**

雖然「左閉右閉 `[left, right]`」在剛開始學的時候最符合人類直覺，但如果你未來想在 C/C++ 的生態系中走得更遠，**左閉右開** 會為你省下非常多麻煩。

以下比較兩者的差異、各自的模板。

---

### 1. 左閉右閉 `[left, right]` (最直覺)

這個寫法的意思是：我們要在包含 `left` 且包含 `right` 的實體區間內尋找目標。

* **初始狀態：** `left = 0`, `right = n - 1` (兩個指標都指向陣列內實際存在的元素)。
* **迴圈條件：** `while (left <= right)` (因為 `left == right` 時，區間內還有 1 個元素需要檢查，是有意義的)。
* **區間縮小：** 既然 `mid` 已經檢查過了不是我們要的，下一次搜尋就不需要包含 `mid`。
    * 目標在左半邊：`right = mid - 1`
    * 目標在右半邊：`left = mid + 1`

**C 語言模板：**
```c
int binary_search_closed(int arr[], int n, int target) {
    int left = 0;
    int right = n - 1; // 重點 1：指向最後一個元素

    while (left <= right) { // 重點 2：有等號
        int mid = left + (right - left) / 2; // 防止整數溢位
        
        if (arr[mid] == target) {
            return mid; // 找到了
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1; // 重點 3：mid 已經檢查過，右邊界退一步
        }
    }
    return -1; // 找不到
}
```
* **優點：** 邏輯非常直接，我們就是在找一個確切的範圍。
* **缺點：** 計算區間長度時要加一（`長度 = right - left + 1`），有時容易算錯。

---

### 2. 左閉右開 `[left, right)` (強烈推薦)

這個寫法的意思是：區間包含 `left`，但**不包含** `right`。`right` 永遠指向我們搜尋範圍「最後一個元素的下一個位置」（一個無效的邊界）。

* **初始狀態：** `left = 0`, `right = n` (`right` 指向陣列外)。
* **迴圈條件：** `while (left < right)` (因為當 `left == right` 時，區間 `[left, left)` 裡面沒有任何元素，迴圈應該終止)。
* **區間縮小：**
    * 目標在左半邊：因為右邊界本來就是「不包含」的開區間，所以直接將右邊界設為剛才檢查過的 `mid` 即可。`right = mid`。
    * 目標在右半邊：左邊界是閉區間，`mid` 檢查過了不要包含，所以 `left = mid + 1`。

**C 語言模板：**
```c
int binary_search_half_open(int arr[], int n, int target) {
    int left = 0;
    int right = n; // 重點 1：指向陣列長度 (越界位置)

    while (left < right) { // 重點 2：沒有等號
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid; // 重點 3：直接等於 mid，不減 1
        }
    }
    return -1;
}
```

---

### 為什麼推薦「左閉右開」作為統一寫法？

1.  **完美契合 C/C++ 的底層基因：** C 語言的陣列索引是從 `0` 到 `n-1`。當你傳遞一個陣列大小 `n` 時，`[0, n)` 這個左閉右開區間剛好完美涵蓋整個陣列，不需要你手動去寫 `n - 1`。未來的 C++ 標準模板庫 (STL) 中的迭代器（如 `.begin()` 和 `.end()`）也全都是採用左閉右開的設計。
2.  **區間長度計算超簡單：** 在 `[left, right)` 區間中，元素的個數永遠是 `right - left`。不用像左閉右閉那樣去糾結要不要 `+1`。
3.  **無縫分割區間：** 如果你要把一個區間從 `mid` 切成兩半，左閉右開可以優雅地切成 `[left, mid)` 和 `[mid, right)`，兩者的交集為空，聯集為原區間。如果是左閉右閉，你得寫成 `[left, mid]` 和 `[mid+1, right]`，看起來較為破碎。
4.  **找 Lower Bound / Upper Bound 更方便：** 單純找一個值很簡單，但實務上常常需要找「第一個大於等於 target 的位置」或「第一個大於 target 的位置」。使用左閉右開的模板，在處理這類進階二分法時，邏輯會高度統一，不容易出錯。


### B. 進階範本：尋找左邊界 (Lower Bound)

在一個排序好的陣列中，找到『第一個大於或等於 (>=) target』的元素位置

- 如果陣列裡有 target，它會找到第一個出現的 target。
- 如果陣列裡沒有 target，它會找到第一個比它大的數字。
- 如果 target 比陣列裡所有數字都大，它會回傳陣列長度 n（越界位置）。

```c
int lower_bound(int arr[], int n, int target) {
    int left = 0;
    int right = n; // [left, right)

    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] < target) {
            // mid 本身小於 target，所以「大於等於 target」的第一個元素一定在右邊
            left = mid + 1; 
        } else {
            // arr[mid] >= target
            // 關鍵：mid 可能是答案，或者答案在 mid 更左邊。
            // 我們把 right 設為 mid，代表「mid 以後（含 mid）我都找過了，它們都 >= target」。
            // 接下來只要專心找 [left, mid) 這個區間就好。
            right = mid; 
        }
    }
    
    // 當 left == right 時迴圈結束。
    // left (或 right) 就是「第一個 >= target」的索引位置。
    return left; 
}

```

Upper Bound 的定義是：「找到『第一個嚴格大於 (>) target』的元素位置。」

```c
int upper_bound(int arr[], int n, int target) {
    int left = 0;
    int right = n;

    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] <= target) { // 唯一的差別：這裡變成 <=
            // mid 本身小於或等於 target，所以「嚴格大於」的元素一定在右邊
            left = mid + 1; 
        } else {
            // arr[mid] > target
            right = mid; 
        }
    }
    return left;
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

$$N = 2^k$$

$$\log_2 N = k$$


這代表在最差的情況下，我們只需要 $\log_2 N$步就能找到答案或確定不存在。在演算法的大$O$表示法中，對數的底數通常省略，記作 $O(\log N)$。

---

2. 決策樹的結構

二分搜尋的過程可以想像成一棵**平衡二元樹**。每次比較中間值（Mid），就是在決定要往左子樹還是右子樹走。

根據先前學習 Heap 的筆記內容：

- 一個擁有 $n$ 個節點的完全二元樹，其高度 $H$ 與總節點數 $n$ 的關係為 $H = \log_2(n + 1)$。
- 從根節點走到最底層的葉子節點，最多經過的步數就是樹的高度 $H-1$。
- 由於 $H$ 與 $\log n$ 成正比，這段搜尋路徑的長度就是 $O(\log n)$。

