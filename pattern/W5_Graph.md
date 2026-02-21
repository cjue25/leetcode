# W5 Graph (圖論結構)

* Adjacency Matrix & List (圖的表示法)
* DFS / BFS (深度與廣度搜尋)
* Cycle Detection (環狀偵測)
* Dijkstra / MST (最短路徑與生成樹基礎)

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

