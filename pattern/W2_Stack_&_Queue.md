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
