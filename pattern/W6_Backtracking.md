# W6 Backtracking (回溯法)

* Decision Tree (決策樹)
* State Restoration (狀態重置)
* Pruning (剪枝技術)
* Combinations & Permutations (組合與排列)

---

## 1. 核心原理與應用場景

### A. 核心原理：試錯與回頭

回溯法本質上是一種**深度優先搜尋 (DFS)** 的變體。它的核心思想是：**「一條路走到黑，走不通就退回上一步，換條路再試。」**

在 C 語言的實作中，這通常表現為一個遞迴函式。我們在進入下一層遞迴前「做出選擇」，在遞迴返回後「撤銷選擇」。

* **路徑 (Path)：** 已經做出的選擇。
* **選擇列表 (Choices)：** 當前可以做的選擇。
* **終止條件 (Goal)：** 到達決策樹的葉子節點，找到了一個可行解。

### B. 什麼時候該用？

回溯法通常用於解決「找尋所有可能」或「是否存在某種組合」的問題：

* **組合問題 (Combinations)：** $N$ 個數裡面按一定規則找出 $K$ 個數的集合。
* **排列問題 (Permutations)：** $N$ 個數按一定規則全排列。
* **切割問題 (Partitioning)：** 一個字串按一定規則切割。
* **子集問題 (Subsets)：** 一個集合的所有子集。
* **棋盤問題：** N-Queens、解數獨。

---

## 2. C 語言實作時最常見的 Bug

在 C 語言中，沒有高階語言的自動記憶體管理和動態陣列，這讓回溯法變得格外危險：

1. **忘記撤銷狀態 (The "Backtrack" Step)：**
* 這是回溯法的靈魂。如果你修改了全域變數或傳入的陣列（例如標記 `visited[i] = 1`），但在遞迴結束後忘記改回 `0`，後續的搜尋分支會因為「誤以為已經拜訪過」而跳過正確答案。


2. **二維陣列結果的記憶體分配 (`**returnColumnSizes`)：**
* 在 LeetCode 的 C 語言題目中，這通常是新手的地獄。你必須手動 `malloc` 每一列的大小。如果計算錯誤或忘記分配，會直接噴 `Memory Limit Exceeded` 或 `Heap Buffer Overflow`。


3. **指標傳遞與區域變數：**
* 在遞迴中，如果你把區域陣列的指標傳給下一層，而該層又依賴這個指標存入最終結果，當遞迴彈棧 (Pop) 時，該指標指向的內容可能會失效。
* **對策：** 存入結果時，務必使用 `memcpy` 將內容複製到動態分配的空間。


4. **剪枝邏輯（Pruning）放錯位置：**
* 剪枝是為了提早結束無效的分支。如果剪枝判斷放在「做出選擇」之後太久，優化效果會大打折扣；如果放太早，可能會漏掉解。



---

## 3. C 語言萬用模板 (Recursion with Pruning)

這個模板包含了 **「剪枝 (Pruning)」** 邏輯，能有效提升效能。

```c
/**
 * Backtracking 萬用模板
 * @param res         存儲最終所有結果的陣列
 * @param path        當前路徑（已選的元素）
 * @param pathSize    當前路徑的大小
 * @param startIdx    搜尋的起始位置（防止重複組合）
 * @param candidates  題目給定的候選陣列
 */
void backtrack(int** res, int* resSize, int* path, int pathSize, int startIdx, int* candidates, int n) {
    // 1. 終止條件 (Base Case)
    if (pathSize == target_condition) {
        // 拷貝當前路徑到結果中
        res[*resSize] = (int*)malloc(sizeof(int) * pathSize);
        memcpy(res[*resSize], path, sizeof(int) * pathSize);
        // 如果是 LeetCode，通常還需要處理 (*returnColumnSizes)[*resSize] = pathSize;
        (*resSize)++;
        return;
    }

    // 2. 遍歷選擇列表
    for (int i = startIdx; i < n; i++) {
        
        // 3. 剪枝 (Pruning)
        // 範例：如果剩下的元素加起來也不夠，或是數值已超過目標
        if (should_prune(candidates[i])) {
            continue; // 跳過此分支
            // 或 break; (如果 candidates 是排序過的，後面的都不用看了)
        }

        // 4. 做選擇 (Make Choice)
        path[pathSize] = candidates[i];
        
        // 5. 進入下一層遞迴
        // 注意：如果是排列問題，startIdx 通常不變或傳 visited 陣列
        // 注意：如果是組合問題，傳 i + 1 以避免重複使用同一元素
        backtrack(res, resSize, path, pathSize + 1, i + 1, candidates, n);

        // 6. 撤銷選擇 (Backtrack / Undo Choice) - 重點！！
        // 在 C 語言中，如果是操作 path 陣列，通常只要讓 pathSize 自然減少即可
        // 如果有修改 visited[] 或 全域變數，這裡一定要改回來
    }
}

```

### 具體案例：組合總和 (Combination Sum) 剪枝範例

```c
// 假設已對 candidates 進行排序
for (int i = startIdx; i < n; i++) {
    // 【剪枝】：如果當前數字已經大於剩下的目標值，後面的數字更大，直接斷尾
    if (target - candidates[i] < 0) break; 

    path[pathSize] = candidates[i];
    backtrack(..., target - candidates[i], i); // 遞迴
    // 這裡不需額外動作，因為下一輪迴圈會直接覆蓋 path[pathSize]
}

```

---

### 補充建議：

1. **先排序再剪枝：** 大部分的回溯題，如果能先對輸入陣列進行 `qsort`，剪枝效率會從 $O(2^N)$ 逼近到接近 $O(N^k)$。
2. **畫圖調試：** 回溯法就是走樹狀圖，Bug 通常發生在你以為路徑已經「回頭」了，但某個變數還留在原地。
3. **注意 Stack 空間：** C 的遞迴深度受限，如果題目規模極大（如 $N > 1000$），請考慮這是否真的適合用回溯法，還是應該用動態規劃 (DP)。

