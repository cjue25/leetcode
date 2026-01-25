# W4 Tree (樹狀結構)

* Binary Tree Traversal (DFS/BFS)
* Recursion (遞迴思維)
* LCA (最近公共祖先)
* 記憶體與指標管理

---

## 1. 核心原理與應用場景

### A. 核心原理：階層與分治

樹是一種由節點（Node）組成的階層式結構。每個節點最多有一個父節點，但可以有多個子節點。在 C 中，我們透過 `struct` 定義節點，並用指標連接它們。樹的許多問題都可以拆解為：「處理當前節點 + 遞迴處理左子樹 + 遞迴處理右子樹」。

* 時間複雜度：大部分平衡樹的操作（搜尋、插入）為 。若退化成鏈結串列則為 。
* 遍歷方式：
    - 深度優先 (DFS)：往深處走，分為前序、中序、後序。
    - 廣度優先 (BFS)：按層前進，通常配合 Queue 實作。


### B. 什麼時候該用？

* **階層式資料：** 如檔案系統、組織架構。
* **快速搜尋與排序：** 如 Binary Search Tree (BST) 或平衡樹（AVL, Red-Black Tree）。
* **表達式解析：** 編譯器利用語法樹（Syntax Tree）來解析程式碼。
* **路徑尋找：** 在決策樹中尋找最優解。

---

## 2. C 語言實作時最常見的 Bug

在 C 語言中處理樹時，指標的操作是萬惡之源：

1. **存取空指標 (NULL Pointer Dereference)：**

* **錯誤：** 直接存取 `root->left` 而沒有先檢查 `if (root == NULL)`。這是最常見的 `Segmentation Fault` 來源。
* **對策：** 遞迴的第一行永遠要是 `if (!root) return;`。

2. **記憶體洩漏 (Memory Leak)：**

* **錯誤：** 刪除節點或整棵樹時，先釋放了父節點，導致子節點的指標丟失，無法 `free`。
* **對策：** 釋放整棵樹必須使用**後序遍歷 (Post-order)**，先處理小孩，再處理自己。

3. **指標傳遞錯誤：**

* **錯誤：** 在函式中修改 `root = NULL` 但沒使用傳入指標的指標（`Node**`），導致函式外部的指標依然指向原處（野指標）。

4. **Stack Overflow (遞迴過深)：**

* 如果樹極度不平衡且深度達到數萬層，系統 Stack 會噴掉。
* **對策：** 對於極端情況，考慮改用迭代法（搭配自定義 Stack）。

---

## 3. C 語言萬用模板

### A. 基礎節點定義與遞迴 (Recursion)

遞迴是樹的靈魂。記住：**「定義好終止條件，剩下的交給子問題」**。

```c
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// 遞迴基本邏輯
void solve(struct TreeNode* root) {
    if (root == NULL) return; // 1. 終止條件
    
    // 2. 處理當前節點 (Pre-order 邏輯放這)
    solve(root->left);        // 3. 往左走
    // (In-order 邏輯放這)
    solve(root->right);       // 4. 往右走
    // (Post-order 邏輯放這)
}

```

### B. DFS (深度優先遍歷 - 遞迴版)

以最常見的「前、中、後序」為例：

```c
void traversal(struct TreeNode* root) {
    if (!root) return;
    
    // printf("%d ", root->val); // 前序 (Pre-order): 根 -> 左 -> 右
    traversal(root->left);
    // printf("%d ", root->val); // 中序 (In-order): 左 -> 根 -> 右
    traversal(root->right);
    // printf("%d ", root->val); // 後序 (Post-order): 左 -> 右 -> 根
}

```

### C. BFS (廣度優先 / 層序遍歷)

在 C 中，BFS 必須搭配W2的 **Queue**。

```c
void BFS(struct TreeNode* root) {
    if (!root) return;
    
    struct TreeNode* queue[10000]; // 簡單用陣列模擬 Queue
    int front = 0, rear = 0;
    
    queue[rear++] = root;
    
    while (front < rear) {
        struct TreeNode* node = queue[front++]; // Pop
        
        printf("%d ", node->val);
        
        if (node->left) queue[rear++] = node->left;   // Push Left
        if (node->right) queue[rear++] = node->right; // Push Right
    }
}

```

### D. LCA (最近公共祖先)

這是一個非常經典的遞迴應用，邏輯在於「向上回傳狀態」。

```c
struct TreeNode* lowestCommonAncestor(struct TreeNode* root, struct TreeNode* p, struct TreeNode* q) {
    // 1. 碰到 NULL 或碰到目標之一，就回傳當前節點
    if (root == NULL || root == p || root == q) return root;

    // 2. 探查左右子樹
    struct TreeNode* left = lowestCommonAncestor(root->left, p, q);
    struct TreeNode* right = lowestCommonAncestor(root->right, p, q);

    // 3. 結果判斷
    if (left != NULL && right != NULL) return root; // 左右都有，我就是 LCA
    return (left != NULL) ? left : right;           // 哪邊有就傳哪邊
}

```

---

### 補充：

1. **畫圖：** 寫 Tree 題如果不畫圖，就是在跟自己的大腦過不去。
2. **保護措施：** 只要看到 `->` 符號，腦中要反射性檢查左邊的東西是否可能是 `NULL`。
3. **Base Case：** 遞迴的第一行永遠寫 `if (!root)`。
