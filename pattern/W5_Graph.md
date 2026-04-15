# W5 Graph (圖論結構)

* Adjacency Matrix & List (圖的表示法)
* DFS / BFS (深度與廣度搜尋)
* Cycle Detection (環狀偵測)
* Dijkstra / MST (最短路徑與生成樹基礎)
* Union Found

---

## 1. 核心原理與應用場景

### A. 核心原理：關聯與連接

圖是由 **頂點 (Vertex)** 與 **邊 (Edge)** 組成的結構。與樹最大的不同在於：圖可以有環 (Cycle)，且沒有固定的根節點。在 C 語言中，我們通常用兩種方式來「描述」這張網：

1. **相鄰矩陣 (Adjacency Matrix)：** 使用二維陣列 `adj[V][V]`。
* 優點：查 $A$ 到 $B$ 是否有邊極快 $O(1)$。
* 缺點：浪費空間 $O(V^2)$，適合頂點少、邊稠密的圖。


2. **相鄰串列 (Adjacency List)：** 使用指標陣列，每個元素掛載一個 Linked List。
* 優點：節省空間 $O(V + E)$。實作複雜，需頻繁處理 malloc 與 free。
* 缺點：實作複雜，需頻繁處理 `malloc` 與 `free`。



* **遍歷核心：** 為了防止在環中無限迴圈，必須搭配一個 `visited[]` 陣列來記錄走過的蹤跡。

### B. 什麼時候該用？

* **社交網路：** 朋友關係（我加你，你加他，他加我）。
* **地圖導航：** 城市為頂點，道路為邊，尋找最短路徑（Dijkstra）。
* **任務調度：** 像編譯器的檔案依賴關係（必須先編譯 A 才能編譯 B），這叫 **拓樸排序 (Topological Sort)**。
* **網路路由：** 封包在路由器之間的傳遞路徑。

---

## 2. C 語言實作時最常見的 Bug

Graph 的實作是指標的地獄，請務必留意：

1. **忘記標記 Visited：**
* **錯誤：** 在 DFS/BFS 中沒有在進入節點時標記 `visited[v] = 1`。
* **後果：** 如果圖中有環，你的程式會陷入無限遞迴或死迴圈，直到 Stack Overflow。


2. **記憶體釋放不完全：**
* **錯誤：** 使用相鄰串列時，只 `free(graph->array)`，卻沒遍歷每個索引把裡面的 Linked List 節點一個個 `free` 掉。
* **對策：** 寫一個 `destroyGraph` 函式，由內而外釋放。


3. **陣列越界 (Index Out of Bounds)：**
* **錯誤：** 圖的頂點通常是 $0$ 到 $V-1$。如果你的資料是從 $1$ 開始編號，直接當成陣列 index 會導致噴錯或存取到不明記憶體。


4. **靜態陣列過大：**
* **錯誤：** 在函式內宣告 `int matrix[10000][10000]`。
* **後果：** 區域變數佔用 Stack 空間，這會導致程式一執行就直接 Crash。請改用 `malloc` (Heap) 或宣告為全域變數。



---

## 3. C 語言萬用模板

### A. 相鄰串列結構定義

這是最通用的結構，能處理大規模但稀疏的圖。

```c
struct AdjListNode {
    int dest;
    struct AdjListNode* next;
};

struct AdjList {
    struct AdjListNode *head; 
};

struct Graph {
    int V; // 頂點數
    struct AdjList* array; // 指標陣列
};

```

### B. DFS (深度優先遍歷 - 遞迴版)

核心在於利用遞迴深入探索，並用 `visited` 擋下重複路徑。

```c
void DFSUtil(struct Graph* graph, int v, int visited[]) {
    visited[v] = 1; // 標記已拜訪
    printf("%d ", v);

    struct AdjListNode* curr = graph->array[v].head;
    while (curr) {
        if (!visited[curr->dest]) {
            DFSUtil(graph, curr->dest, visited);
        }
        curr = curr->next;
    }
}

void DFS(struct Graph* graph) {
    int visited[graph->V];
    for (int i = 0; i < graph->V; i++) visited[i] = 0;

    for (int i = 0; i < graph->V; i++) { // 處理斷連通圖 (Disconnected Graph)
        if (!visited[i]) DFSUtil(graph, i, visited);
    }
}

```

### C. BFS (廣度優先 - 橫向探索)

一樣要配合 W2 的 **Queue**。先拜訪所有鄰居，再拜訪鄰居的鄰居。

```c
void BFS(struct Graph* graph, int startVertex) {
    int visited[graph->V];
    for (int i = 0; i < graph->V; i++) visited[i] = 0;

    int queue[10000], front = 0, rear = 0;

    visited[startVertex] = 1;
    queue[rear++] = startVertex;

    while (front < rear) {
        int v = queue[front++]; // Dequeue
        printf("%d ", v);

        struct AdjListNode* curr = graph->array[v].head;
        while (curr) {
            if (!visited[curr->dest]) {
                visited[curr->dest] = 1; // 放入 Queue 前就先標記，避免重複放入
                queue[rear++] = curr->dest;
            }
            curr = curr->next;
        }
    }
}

```


### D. Union Found

```c
#include <stdlib.h>
int find(int* parent, int i) {
    //找我自己的老大，如果我是直接回傳，不是的話去問我老大的老大是誰
    return parent[i] == i ? i : (parent[i] = find(parent, parent[i]));
}

bool validPath(int n, int** edges, int edgesSize, int* edgesColSize, int source, int destination) {
    int parent[n];
    for (int i = 0; i < n; i++) parent[i] = i;

    for (int i = 0; i < edgesSize; i++) {
        int rootU = find(parent, edges[i][0]);
        int rootV = find(parent, edges[i][1]);
        //每個都去問最高層是誰，問到之後，每個人都直接對到他
        if (rootU != rootV) parent[rootU] = rootV;
    }

    return find(parent, source) == find(parent, destination);
}

```

---

### 補充筆記：

1. **無向圖 vs 有向圖：** 在 `addEdge` 時，無向圖要記得雙向連接（`u->v` 和 `v->u`）。
2. **空間折衷：** 如果頂點數量小於 1000，直接用 `matrix[V][V]` 寫起來最快也最不容易噴 Bug。
3. **入度 (In-degree) / 出度 (Out-degree)：** 在實作拓樸排序時，紀錄每個節點被多少人指向是解題關鍵。
4. **DFS visit 用法：**
`visit` 的狀態管理取決於你的**目的**。我們可以把情況分為三種：

#### 1. 單純的「連通性」與「遍歷」（維持 1）

**場景：** 判斷能不能從 A 走到 B、計算島嶼數量、或是單純走過所有點。

* **邏輯：** 只要這個點我曾經踏進去過，且發現它無法到達終點（或已經計算過），我就再也不進去了。
* **為什麼不恢復為 0：** 因為圖的結構是固定的。如果從  找不到終點，那麼下次從  也絕對找不到。
* **代表題：** 剛才這題 (1971. Find if Path Exists)、Number of Islands。

#### 2. 尋找「所有可能路徑」或「特定長度路徑」（恢復為 0）

**場景：** 這就是所謂的 **Backtracking（回溯）**。例如：「請列出從 A 到 B 的所有走法」。

* **邏輯：** 我現在走  這條路，為了避免在**這一次**的路徑中繞圈圈，我把  標記為 。但我退回到  改走  時， 對我來說是「新路徑」的一部分，所以我必須在離開  時把它恢復為 。
* **為什麼要恢復：** 為了讓其他路徑可以再次利用這個節點。
* **代表題：** [79. Word Search](https://leetcode.com/problems/word-search/)、所有 Permutation (排列) 題目。

#### 3. 判斷「有無環路」或「拓撲排序」（三色標記 0, 1, 2）

**場景：** 在 **有向圖** 中判斷有沒有環（Cycle Detection）。

* **邏輯：**
* **0 (White)：** 尚未造訪。
* **1 (Gray)：** 正在遞迴路徑中（遞迴還沒結束）。**如果這時又撞到標記為 1 的點，代表抓到環了！**
* **2 (Black)：** 這個點及其所有子孫都已經安全檢查完畢，確定沒環。


* **為什麼要 2：** 為了加速。如果我走到一個標記為 2 的點，表示之前已經有人檢查過它後面沒環了，我可以放心直接回傳 `false` (無環)。
* **代表題：** [207. Course Schedule](https://leetcode.com/problems/course-schedule/)。

#### 快速判斷表

| 目的 | `visit` 處理方式 | 核心思維 |
| --- | --- | --- |
| **找「能不能到」** | 標記為 1 後**不恢復** | 走過的死路永遠是死路 |
| **找「所有路徑」** | 離開節點時**恢復為 0** | 換條路走時，這點還能再用 |
| **找「有無環路」** | 使用 **0, 1, 2** 三態 | 區分「正在走」與「走完了」 |

### Dijkstra's Algorithm

Dijkstra's Algorithm 是一個用來尋找圖形中**「單一起點到其他所有節點的最短路徑」**（Single-Source Shortest Path）的演算法。

這個演算法會像水波紋一樣往外擴散，每次都先挑「目前離起點最近、且還沒確認過最短路線的路口」來探索，並順便看看從這個路口走到鄰近路口會不會比較快。

Dijkstra 的核心思想是「貪婪法 (Greedy)」，它永遠相信：目前找到最近的那一步，就是最好的那一步。

1. 初始化 (Initialization)：
- 把「起點」到「起點」的距離設為0。
- 把「起點」到「其他所有節點」的距離設為無限大（$\infty$）。
- 準備一個名單，記錄哪些節點是「還沒拜訪過」的。

2. 尋找最近節點 (Greedy Choice)：
- 從「還沒拜訪過」的節點清單中，挑選出一個目前距離起點最近的節點，我們稱它為節點 $U$。（在剛開始時，這個節點 $U$ 當然就是起點自己，因為它的距離是 0，其他都是 $\infty$

3. 更新鄰居距離 (Relaxation，又稱鬆弛操作)：站在節點 $U$ 上，看看它所有相鄰的節點 $V$。計算一條新路線：「目前走到 $U$ 的距離」加上「從 $U$ 走到 $V$ 的距離」。如果這條新路線的距離，比原本記錄的「走到 $V$ 的距離」還要短，那就更新 $V$ 的最短距離紀錄。

4. 標記已拜訪 (Mark as Visited)：鄰居都檢查完後，把節點 $U$ 標記為「已拜訪」。這代表我們已經百分之百確定起點走到 $U$ 的最短路徑了，以後都不用再回頭檢查它。重複執行 (Repeat)：重複步驟 2 到步驟 4，直到所有可以到達的節點都被標記為「已拜訪」為止。

舉例

假設一個有 4 個節點的網路 ($N = 4$)，並且從節點 1 ($K = 1$) 發送訊號。

```text
       (1)
      ↗   ↘
    1/     \4
    /       ↘
  (2) --2--> (3)
    \       ↗
    6\     /3
      ↘   /
       (4)

```

**題目給的輸入參數會是這樣：**

* `times = [[1,2,1], [1,3,4], [2,3,2], [2,4,6], [3,4,3]]` (格式為 `[起點, 終點, 時間]`)
* `n = 4` (總共 4 個節點)
* `k = 1` (訊號從節點 1 出發)

---

### Dijkstra 演算法逐步圖解

準備兩個陣列來記錄狀態：

1. **`dist` (最短時間)**：記錄從起點(1)走到各節點目前需要的最短時間。
2. **`visited` (已拜訪)**：記錄該節點的最短時間是否已經「完全確定」。

#### 初始狀態

起點到自己的距離是 0，其他都是無限大 ($\infty$)。
| 節點 | 1 | 2 | 3 | 4 |
| :--- | :--- | :--- | :--- | :--- |
| **`dist`** | **0** | $\infty$ | $\infty$ | $\infty$ |
| **`visited`** | 否 | 否 | 否 | 否 |

---

#### 第 1 回合

* **尋找最近節點：** 目前還沒拜訪的節點中，距離最短的是 **節點 1** (`dist` = 0)。
* **標記已拜訪：** 將節點 1 標記為「已確定」 (打 ✓)。
* **更新鄰居：** 站在節點 1，它有兩個鄰居：節點 2 和節點 3。
* 走到節點 2 的新時間：$0 + 1 = 1$。比原本的 $\infty$ 小，**更新！**
* 走到節點 3 的新時間：$0 + 4 = 4$。比原本的 $\infty$ 小，**更新！**



| 節點 | 1 | 2 | 3 | 4 |
| --- | --- | --- | --- | --- |
| **`dist`** | **0** | **1** | **4** | $\infty$ |
| **`visited`** | ✓ | 否 | 否 | 否 |

---

#### 第 2 回合

* **尋找最近節點：** 還沒拜訪的節點 (2, 3, 4) 中，距離最短的是 **節點 2** (`dist` = 1)。
* **標記已拜訪：** 將節點 2 標記為「已確定」 (打 ✓)。這代表從起點到節點 2 的最快時間絕對是 1，不可能更短了。
* **更新鄰居：** 站在節點 2，它有兩個鄰居：節點 3 和節點 4。
* 走到節點 3 的新時間：從節點 2 走過去是 $1 + 2 = 3$。這比原本記錄的 4 還要小，**更新！**(這就是 Dijkstra 找捷徑的威力)
* 走到節點 4 的新時間：$1 + 6 = 7$。比原本的 $\infty$ 小，**更新！**



| 節點 | 1 | 2 | 3 | 4 |
| --- | --- | --- | --- | --- |
| **`dist`** | 0 | **1** | **3** | **7** |
| **`visited`** | ✓ | ✓ | 否 | 否 |

---

#### 第 3 回合

* **尋找最近節點：** 還沒拜訪的節點 (3, 4) 中，距離最短的是 **節點 3** (`dist` = 3)。
* **標記已拜訪：** 將節點 3 標記為「已確定」 (打 ✓)。
* **更新鄰居：** 站在節點 3，它有一個鄰居：節點 4。
* 走到節點 4 的新時間：從節點 3 走過去是 $3 + 3 = 6$。這比原本記錄的 7 還要小，**更新！**



| 節點 | 1 | 2 | 3 | 4 |
| --- | --- | --- | --- | --- |
| **`dist`** | 0 | 1 | **3** | **6** |
| **`visited`** | ✓ | ✓ | ✓ | 否 |

---

#### 第 4 回合

* **尋找最近節點：** 剩下還沒拜訪的只有 **節點 4** (`dist` = 6)。
* **標記已拜訪：** 將節點 4 標記為「已確定」 (打 ✓)。
* **更新鄰居：** 節點 4 沒有往外的連線了，結束。

| 節點 | 1 | 2 | 3 | 4 |
| --- | --- | --- | --- | --- |
| **`dist`** | 0 | 1 | 3 | **6** |
| **`visited`** | ✓ | ✓ | ✓ | ✓ |

---

### 最終計算答案

現在我們得到了從起點 1 到所有節點的最短時間陣列：`[0, 1, 3, 6]`。

題目問的是：**「所有節點」都收到訊號需要多久？**

* 節點 2 在第 1 秒收到。
* 節點 3 在第 3 秒收到。
* 節點 4 在第 6 秒收到。

也就是說，整個網路要完全收到訊號，必須等最慢的那個人，也就是取這個陣列裡的**最大值**。
答案就是 **6**！

### DFS 排列組合

- 組合 (Combination) / 子集 (Subset)：與「順序無關」 ({1, 2} 和 {2, 1} 視為同一種)。我們需要使用 startIndex 來控制，確保每次往下層搜尋時，只能挑選「當前數字之後」的數字，避免產生倒退選取造成的重複。
- 排列 (Permutation)：與「順序有關」 ({1, 2} 和 {2, 1} 是不同的)。我們不需要 startIndex，每次都從頭 (index 0) 開始找，但需要一個 used (或 visited) 布林陣列，來記錄「這個數字在當前這條路徑中是不是已經被用過了」

以下整理四大情境與 DFS 範本：

---

### 情境一：子集 (Subsets)
**特徵**：收集樹狀結構中**所有的節點**（每個狀態都是答案）。

#### 1. 元素不重複 (LeetCode 78 - Subsets)
* **邏輯**：因為元素不重複，直接用 `startIndex` 往後找即可。
* **DFS 範本**：
```c
void dfs(int* nums, int numsSize, int startIndex, int* path, int pathLen) {
    // 1. 收集答案：不需要 return 條件，因為每個走過的節點都是一個合法的子集
    add_to_result(path, pathLen); 

    // 2. 展開選擇
    for (int i = startIndex; i < numsSize; i++) {
        path[pathLen] = nums[i]; // 做選擇
        // 進入下一層，startIndex 變成 i + 1 (不能選自己了)
        dfs(nums, numsSize, i + 1, path, pathLen + 1); 
        // 撤銷選擇：在 C 語言中，其實不需要特別做什麼，因為下次迴圈 pathLen 會直接覆蓋當前位置
    }
}
```

#### 2. 元素有重複 (LeetCode 90 - Subsets II)
* **邏輯**：為了避免選出重複的子集（例如兩個相同的 `2` 產生兩組 `{1, 2}`），**必須先排序**。接著在同一層迴圈中，遇到跟前一個一樣的數字就跳過（剪枝）。
* **DFS 範本**：
```c
// 主程式必須先呼叫 qsort(nums, numsSize, sizeof(int), cmp);
void dfs(int* nums, int numsSize, int startIndex, int* path, int pathLen) {
    add_to_result(path, pathLen);

    for (int i = startIndex; i < numsSize; i++) {
        // 剪枝：如果不是這層的第一個元素，且跟前一個元素相同，就跳過
        if (i > startIndex && nums[i] == nums[i - 1]) {
            continue;
        }
        
        path[pathLen] = nums[i];
        dfs(nums, numsSize, i + 1, path, pathLen + 1);
    }
}
```

---

### 情境二：組合 (Combinations)
**特徵**：收集樹狀結構中**葉子節點**的結果（通常有條件限制，如長度、總和）。

#### 1. 限制長度，不重複選 (LeetCode 77 - Combinations)
* **邏輯**：給定 `n` 個數字，選 `k` 個。使用 `startIndex`。
* **DFS 範本**：
```c
void dfs(int n, int k, int startIndex, int* path, int pathLen) {
    // 終止條件：路徑長度達到 k
    if (pathLen == k) {
        add_to_result(path, pathLen);
        return;
    }

    // 這裡可以做一個優化剪枝：如果剩下的元素不夠湊滿 k 個，就不用跑了
    // 條件： i <= n - (k - pathLen) + 1
    for (int i = startIndex; i <= n; i++) {
        path[pathLen] = i;
        dfs(n, k, i + 1, path, pathLen + 1);
    }
}
```

#### 2. 元素可無限重複選取 (LeetCode 39 - Combination Sum)
* **邏輯**：可以重複選自己！所以進入下一層時，傳入的不是 `i + 1`，而是 `i`。
* **DFS 範本**：
```c
void dfs(int* candidates, int candidatesSize, int target, int startIndex, int currentSum, int* path, int pathLen) {
    if (currentSum == target) {
        add_to_result(path, pathLen);
        return;
    }
    if (currentSum > target) { // 爆了，剪枝
        return; 
    }

    for (int i = startIndex; i < candidatesSize; i++) {
        path[pathLen] = candidates[i];
        // 關鍵：因為可以重複選當前數字，所以下一層 startIndex 還是 i
        dfs(candidates, candidatesSize, target, i, currentSum + candidates[i], path, pathLen + 1);
    }
}
```

---

### 情境三：排列 (Permutations / Sequences)
**特徵**：順序不同視為不同結果。**不用 `startIndex`，改用 `used` 陣列。**

#### 1. 元素不重複 (LeetCode 46 - Permutations)
* **邏輯**：每次迴圈都從 `0` 開始找，如果這個數字在當前路線用過了就跳過。
* **DFS 範本**：
```c
// bool* used 需要在主程式初始化為 false
void dfs(int* nums, int numsSize, bool* used, int* path, int pathLen) {
    if (pathLen == numsSize) { // 蒐集滿了
        add_to_result(path, pathLen);
        return;
    }

    for (int i = 0; i < numsSize; i++) { // 排列問題，永遠從 0 開始
        if (used[i]) continue; // 已經在路徑中，跳過

        used[i] = true; // 標記為使用過
        path[pathLen] = nums[i];
        
        dfs(nums, numsSize, used, path, pathLen + 1);
        
        used[i] = false; // 撤銷標記 (非常重要！)
    }
}
```

#### 2. 元素有重複 (LeetCode 47 - Permutations II)
* **邏輯**：最難的一種。一樣**必須先排序**。不僅要檢查是否 `used[i]`，還要進行同層去重。
* **DFS 範本**：
```c
// 主程式必須先呼叫 qsort
void dfs(int* nums, int numsSize, bool* used, int* path, int pathLen) {
    if (pathLen == numsSize) {
        add_to_result(path, pathLen);
        return;
    }

    for (int i = 0; i < numsSize; i++) {
        if (used[i]) continue;

        // 終極剪枝邏輯：如果跟前一個數字一樣，而且前一個數字「還沒被用過」
        // 代表前一個數字剛在「同一層」被拿來用過並且撤銷了，所以這輪如果再用會產生重複排列
        if (i > 0 && nums[i] == nums[i - 1] && !used[i - 1]) {
            continue;
        }

        used[i] = true;
        path[pathLen] = nums[i];
        
        dfs(nums, numsSize, used, path, pathLen + 1);
        
        used[i] = false; 
    }
}
```

---

### 總結速查表

| 問題類型 | 是否需要排序 | 迴圈起點 | 去重 / 防呆機制 | 下一層的遞迴參數 |
| :--- | :--- | :--- | :--- | :--- |
| **組合/子集** (無重複元素) | 否 | `startIndex` | 靠 `startIndex` 避免往前選 | `dfs(..., i + 1, ...)` |
| **組合/子集** (有重複元素) | **是** | `startIndex` | `if (i > startIndex && nums[i] == nums[i-1]) continue;` | `dfs(..., i + 1, ...)` |
| **組合** (元素可無限重用)| 否 | `startIndex` | 靠 `startIndex` 避免往前選 | `dfs(..., i, ...)` **(關鍵)** |
| **排列** (無重複元素) | 否 | `0` | `if (used[i]) continue;` | `dfs(..., used, ...)` |
| **排列** (有重複元素) | **是** | `0` | `if (used[i] \|\| (i > 0 && nums[i] == nums[i-1] && !used[i-1])) continue;` | `dfs(..., used, ...)` |
