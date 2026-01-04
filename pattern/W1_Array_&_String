# W1 Array & String

- Two Pointers
- Sliding Window

---

## 1. 核心原理與應用場景

### A. 雙指標 (Two Pointers)

**核心原理：** 利用兩個指標（在 C 中通常是陣列下標 `int i, j`）在陣列上移動，藉此將原本需要  的暴力解優化至 。

* **對撞指標 (Opposite Ends)：** 一左一右向中間靠攏。常用於**排序後的陣列**找兩數之和、翻轉字串。
* **快慢指標 (Fast & Slow)：** 兩個指標同向但速度不同。常用於**判斷鏈結串列是否有環**、**原地修改陣列**（如：移除重複項）。

**什麼時候該用？**

* 當題目要求「原地 (In-place)」修改陣列時（節省空間）。
* 在「已排序」的陣列中搜尋特定組合。

### B. 滑動視窗 (Sliding Window)

**核心原理：** 它是雙指標的變體，維護一個「視窗」，視窗的大小可以固定或動態變化。重點在於**視窗移動時，不要重新計算整個視窗的內容**，而是只處理「進入」和「離開」的元素。

**什麼時候該用？**

* 題目出現「子陣列 (Subarray)」或「子字串 (Substring)」關鍵字。
* 尋找符合某種條件（如：總和、長度、不重複字元）的最長或最短區間。

---

## 2. C 語言實作時最常見的 Bug

在 C 語言中，沒有自動邊界檢查，也沒有高階容器，所以這幾個地方最容易翻車：

1. **越界訪問 (Out of Bounds)：**
* 在 `while (right < size)` 的循環中，如果你內部還有 `while` 或是 `arr[right+1]`，極容易噴出 `Segmentation Fault`。
* **對策：** 永遠先檢查 `index < size`，再存取陣列。


2. **字串結尾 `\0`：**
* C 的字串是 `char` 陣列加 `\0`。在做 `reverse` 或 `window` 時，如果不小心改到了 `\0` 或者計算長度沒考慮它，會導致輸出亂碼或崩潰。


3. **整數溢位 (Integer Overflow)：**
* 在計算 `(left + right) / 2` 或是視窗內數值總和時，如果數值很大，記得用 `long long` 或改寫成 `left + (right - left) / 2`。


4. **指標更新時機：**
* 很多新手會忘記在 `while` 迴圈最後加上 `right++` 或 `left++`，導致死循環 (Infinite Loop)。



---

## 3. C 語言萬用模板

你可以直接把這兩段模板記在腦海裡，解題時像填空一樣填入邏輯。

### 雙指標模板（以對撞指標為例）

```c
void twoPointers(int* nums, int numsSize) {
    int left = 0;
    int right = numsSize - 1;

    while (left < right) {
        // 根據題目條件邏輯
        if (check(nums[left], nums[right])) {
            // 做點什麼，例如交換或記錄結果
            left++;
            right--;
        } else if (some_condition) {
            left++;
        } else {
            right--;
        }
    }
}

```

### 滑動視窗模板（最通用型）

```c
void slidingWindow(int* nums, int numsSize) {
    int left = 0, right = 0;
    int window_sum = 0; // 或是其他狀態，如計數器

    while (right < numsSize) {
        // 1. 【進門】：將右邊元素加入視窗
        window_sum += nums[right];
        
        // 2. 【檢查與縮減】：當視窗滿足條件（或不滿足時）
        while (window_sum > target) { 
            // 3. 【出門】：將左邊元素移出視窗
            window_sum -= nums[left];
            left++;
        }
        
        // 4. 【更新答案】：在這裡紀錄最大長度或最小區間
        // update_result(left, right);

        right++; // 指標右移
    }
}

```
