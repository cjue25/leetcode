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
