# W2 Stack & Queue

- LIFO (Stack) vs FIFO (Queue)
- Monotonic Stack (單調堆疊)
- Heap (Priority Queue)

---

## 1. 核心原理與應用場景

### A. Stack (堆疊)

**核心原理：** **LIFO (Last In, First Out)**，後進先出。就像洗碗時疊盤子，最後疊上去的盤子必須最先拿走。在 C 中通常用陣列加上一個 `top` 下標來實作。

**什麼時候該用？**

* **具有「回溯」性質的問題：** 如 DFS（深度優先搜尋）、撤銷操作（Undo）。
* **匹配問題：** 括號匹配、成對符號檢查。
* **函數調用：** 電腦底層處理遞迴就是利用系統 Stack。

### B. Queue (佇列)

**核心原理：** **FIFO (First In, First Out)**，先進先出。就像排隊買票，先排隊的人先離開。在 C 中實作時，為了節省空間，常用**環狀陣列 (Circular Queue)**。

**什麼時候該用？**

* **具有「公平性」或「層次性」的問題：** 如 BFS（廣度優先搜尋）。
* **緩衝處理：** 任務調度、印表機任務、網路封包緩衝。

### C. Heap

Complete Binary Tree：每一層的節點數都是上一層的 2 倍
- 第 1 層：$1$ 個節點 (根) $2^0$
- 第 2 層：$2$ 個節點      $2^1$
- 第 3 層：$4$ 個節點      $2^2$
- 第 4 層：$8$ 個節點      $2^3$
- 第 d 層：$2^(d-1)$ 個

第 1 層到第 $d-1$ 層的總節點數 $S$

$$
S = 2^0 + 2^1 + 2^2 + \dots + 2^{d-2}
$$

(首項 $a_1 = 1$，公比 $r = 2$，項數 $n = d-1$ 的等比數列。)

$$
S_n = \frac{a_1(r^n - 1)}{r - 1}\\
= \frac{1 \times (2^{d-1} - 1)}{2 - 1} = \frac{2^{d-1} - 1}{1} = 2^{d-1} - 1
$$

所以第d層的第一個編號就是(0-based)

$$
2^{d-1} - 1
$$

假設現在在 d 層，第 k 個節點，index = i

$$
i = (2^{d-1} - 1) + (k - 1) \quad (1)
$$


再找節點 $i$ 的左子節點。這個子節點一定位在第 $d+1$ 層。

在第 $d$ 層中，節點 $i$ 之前有 $k-1$ 個節點。

這 $k-1$ 個節點每個都有 2 個小孩，所以它們一共佔據了第 $d+1$ 層前面的 $2(k-1)$ 個位置。
因此，節點 $i$ 的左子節點，就是第 $d+1$ 層的第 $2(k-1)$ 個節點。

所以總共節點數字為

$$
L_i = (2^d - 1) + (2k - 2)\\
L_i = 2^d + 2k - 3 \quad (2)
$$


$$
k = i - (2^{d-1} - 1) + 1 = i - 2^{d-1} + 2
$$

$$
L_i = 2^d + 2i - 2^d + 4 - 3  \\
L_i = 2^d + 1
$$

右邊就是

$$
L_R = 2^d + 2
$$


#### 左右子點的公式

- 若從索引 0 開始存，左子點是 `2i + 1` (i<<1) + 1，右子點是 `2i + 2` (i<<1)+2。


#### 例子

[1, 2, 6, 3, 8, 7]
      1 (idx:0)
    /   \
   2     6   (idx:1, 2)
  / \   /
 3   8 7     (idx:3, 4, 5)


#### 找到沒有小孩的節點

從 (size - 2) / 2 開始

在 0-based 索引中，最後一個元素的索引是 size - 1。
根據父節點公式 (i - 1) / 2，最後一個元素的父親就是 ((size - 1) - 1) / 2 = (size - 2) / 2。
這代表 build_heap 的時候可以選這個，跳過了所有沒有小孩的「葉子節點」


#### 最底層游到根部是 $O(\log n)$ 

層數與總節點數的公式：假設樹的高度為 $H$。

第 1 層有 $2^0 = 1$ 個節點。
第 2 層有 $2^1 = 2$ 個節點。
第 $H$ 層（最底層）有 $2^{H-1}$ 個節點。

總節點數 $n$ 為：$n = 2^0 + 2^1 + \dots + 2^{H-1} = 2^H - 1$。

反推高度：由 $n = 2^H - 1$ 可以得知 $2^H = n + 1$。

左右取 $\log$，得到 $H = \log_2(n + 1)$。

步數計算：最底層的節點要游到根部，最多就是經過這棵樹的高度 $H-1$ 步。既然 $H$ 與 $\log n$ 成正比，那麼這段路徑長度就是 $O(\log n)$。

#### Floyd's Algorithm (底層建立法) (向下)

先把 $N$ 個數字全部排進陣列，然後從倒數第二層（最後一個有小孩的人）開始，往回走到根節點，對每個人執行 heapify (向下調整)。

堆積的最底層（葉子節點）佔了總數的 $1/2$，他們完全不需要移動。
倒數第二層佔了 $1/4$，最多只需要向下移動 1 層。
倒數第三層佔了 $1/8$，最多只需要向下移動 2 層。...


越往上，節點越少，雖然移動距離變長，但影響力變小。


把所有節點的「最大移動步數」加總起來：

$$
\sum (\text{節點數} \times \text{移動步數}) = \frac{N}{4} \cdot 1 + \frac{N}{8} \cdot 2 + \frac{N}{16} \cdot 3 \dots
$$

這個數列的極限會收斂到 $O(N)$。


($$\text{最後一個非葉子節點} = \frac{(size - 1) - 1}{2} = \frac{size - 2}{2}$$)

#### 逐一插入 (向上)

從空堆積開始，來一個數字就 heap_add 一個

每個新來的數字都是放在最底層，然後執行向上調整。

第 1 個數字：不移動。
第 $N/2$ 到 $N$ 個數字：這 $1/2$ 的數字都在底層，他們每個人「向上爬」的潛在距離都是 $\log N$。

大部分的數字（底層那一大群）都被迫要參與長距離的搬運。總步數大約是 $N \times \log N$，

所以複雜度是 $O(N \log N)$。
---

## 2. C 語言實作時最常見的 Bug

在 C 語言中，Stack 與 Queue 的邏輯不難，但記憶體管理與邊界條件極易出錯：

1. **下溢 (Underflow) 忘記檢查：**
* 在呼叫 `pop()` 或 `top()` 之前，沒有檢查 `isEmpty()`。這會導致存取 `arr[-1]`，直接造成程序崩潰。


2. **環狀佇列 (Circular Queue) 的滿位判斷：**
* 如果用 `(rear + 1) % size == front` 判斷滿位，會「浪費一個空間」。如果不浪費空間，則需要額外的 `count` 變數來區分「空」與「滿」（因為兩者都會滿足 `front == rear`）。


3. **動態記憶體洩漏 (Memory Leak)：**
* 如果你用 Linked List 實作 Stack/Queue，在 `pop` 節點時若忘記 `free(temp)`，長時間運行會耗盡記憶體。


4. **Heap 的索引計算錯誤：**
* 若從索引 0 開始存，左子點是 `2i + 1`，右子點是 `2i + 2`。
* 若從索引 1 開始存，左子點是 `2i`，右子點是 `2i + 1`。混用兩者會導致樹狀結構大亂。



---

## 3. C 語言萬用模板

### Stack
```c
typedef struct _STACK {
    int capacity;
    char * data;
    int top;
} STACK;

STACK * create_stack(int size) {
    STACK * s = malloc(sizeof(STACK));
    s->capacity = size;
    s->data = malloc(sizeof(char)*size);
    s->top = -1;
    return s;
}

bool is_empty(STACK * s) {
    return (s->top == -1);
}

bool is_full(STACK * s) {
    return (s->capacity == (s->top - 1));
}

void stack_push(STACK * s, char c) {
    if (!is_full(s)) {
        s->data[++(s->top)] = c;
    }
}

char stack_pop(STACK *s) {
    if (!is_empty(s)) {
        char c = s->data[(s->top)--];
        return c;
    }
    return -1;
}

char stack_peak(STACK *s) {
    if (!is_empty(s)) {
        return s->data[s->top];
    }
    return -1;
}

void stack_free(STACK * s) {
    free(s->data);
    free(s);
}
```

### 單調堆疊模板 (Monotonic Stack)

常用於解決「找尋下一個更大/更小元素」的問題。

```c
// 以「找尋右邊第一個比自己大」的元素索引為例
void monotonicStack(int* nums, int numsSize) {
    int* stack = (int*)malloc(sizeof(int) * numsSize);
    int top = -1;
    int* nextGreater = (int*)malloc(sizeof(int) * numsSize);

    for (int i = 0; i < numsSize; i++) {
        // 當前元素 > 堆疊頂端元素，代表找到了頂端元素的「下一個更大值」
        while (top >= 0 && nums[i] > nums[stack[top]]) {
            int prevIndex = stack[top--];
            nextGreater[prevIndex] = nums[i];
        }
        stack[++top] = i; // 壓入當前索引
    }
    
    // 剩餘在 stack 中的元素代表右邊沒有比它大的
    while (top >= 0) nextGreater[stack[top--]] = -1;
}

//739
int* dailyTemperatures(int* temperatures, int temperaturesSize, int* returnSize) {
    *returnSize = temperaturesSize;

    int * ans = calloc(sizeof(int), temperaturesSize);

    int stack[temperaturesSize];
    int top = -1;

    for (int i = 0; i < temperaturesSize; i++) {
        int temp = temperatures[i];
        while (top >= 0 && (temp > temperatures[stack[top]])) {
            ans[stack[top]] = i - stack[top];
            top--;
        }
        top++;
        stack[top] = i;
    }

    while (top >= 0) {
        ans[stack[top].index] = 0;
        top--;
    }
    return ans;
}
```

### 最小堆疊模板 (Min-Heap)

C 語言實作 Heap 最穩定的方式是使用陣列（1-based indexing）。

```c
typedef struct {
    int *data;
    int size;
    int capacity;
} MinHeap;

// 向上調整
void swim(MinHeap* h, int k) {
    while (k > 1 && h->data[k] < h->data[k/2]) {
        int temp = h->data[k];
        h->data[k] = h->data[k/2];
        h->data[k/2] = temp;
        k /= 2;
    }
}

// 向下調整
void sink(MinHeap* h, int k) {
    while (2 * k <= h->size) {
        int j = 2 * k;
        if (j < h->size && h->data[j] > h->data[j+1]) j++; // 找較小的子節點
        if (h->data[k] <= h->data[j]) break;
        int temp = h->data[k];
        h->data[k] = h->data[j];
        h->data[j] = temp;
        k = j;
    }
}

void push(MinHeap* h, int val) {
    h->data[++(h->size)] = val;
    swim(h, h->size);
}

int pop(MinHeap* h) {
    int min = h->data[1];
    h->data[1] = h->data[h->size--];
    sink(h, 1);
    return min;
}

```

### Max-Heap

```c
typedef struct _HEAP {
    int capacity;
    int * data;
} HEAP;

HEAP * heap_alloc(int size) {
    HEAP * h = malloc(sizeof(HEAP));
    h->capacity = size;
    h->data = malloc(sizeof(int) * size);
    return h;
}

bool need_swap(int root, int next) {
    if (root < next) {
        return 1;
    }
    return 0;
}

void swap(int * a, int * b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void heapify(HEAP * h, int i) {
    int index = i;
    while (1) {
        int left = (index<<1) + 1;
        int right = (index<<1) + 2;
        int max_idx = index;
        if (left < h->capacity && need_swap(h->data[max_idx], h->data[left])) {
            max_idx = left;
        }

        if (right < h->capacity && need_swap(h->data[max_idx], h->data[right]))  {
            max_idx = right;
        }

        if (max_idx != index) {
            swap(&h->data[max_idx], &h->data[index]);
            index = max_idx;
        } else {
            break;
        }
    }

}

void heap_free(HEAP * h) {
    free(h->data);
    free(h);
}

void build_heapify(HEAP * h) {
    int size = (h->capacity - 2)/ 2;
    for (int i = size; i >=0; i--) {
        heapify(h, i);
    }
}

```
### Linked List

```c
struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode * curr = head;
    struct ListNode * prev = NULL;
    while (curr != NULL) {
        struct ListNode * next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    return prev;
}
```