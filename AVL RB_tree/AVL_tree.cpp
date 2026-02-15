#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

template <typename T>
struct Node
{
    T key;
    Node *left;
    Node *right;
    int height;

    Node(T k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

template <typename T>
class AVLTree
{
private:
    Node<T> *root;

    int height(Node<T> *node)
    {
        return node ? node->height : 0;
    }

    int getBalance(Node<T> *n)
    {
        return n ? height(n->left) - height(n->right) : 0;
    }

    Node<T> *rotate_right(Node<T> *y)
    {
        Node<T> *x = y->left;
        Node<T> *T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = 1 + max(height(y->left), height(y->right));
        x->height = 1 + max(height(x->left), height(x->right));
        return x;
    }

    Node<T> *rotate_left(Node<T> *x)
    {
        Node<T> *y = x->right;
        Node<T> *T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = 1 + max(height(x->left), height(x->right));
        y->height = 1 + max(height(y->left), height(y->right));
        return y;
    }

    Node<T> *minValueNode(Node<T> *node)
    {
        Node<T> *current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    Node<T> *insert(Node<T> *node, T key, bool &success)
    {
        if (node == nullptr)
        {
            success = true;
            return new Node<T>(key);
        }

        if (key < node->key)
            node->left = insert(node->left, key, success);
        else if (key > node->key)
            node->right = insert(node->right, key, success);
        else
        {
            success = false; // Duplicate
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        // Rotations
        if (balance > 1 && key < node->left->key)
            return rotate_right(node);
        if (balance < -1 && key > node->right->key)
            return rotate_left(node);
        if (balance > 1 && key > node->left->key)
        {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }
        if (balance < -1 && key < node->right->key)
        {
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }
        return node;
    }

    Node<T> *remove(Node<T> *node, T key, bool &success)
    {
        if (node == nullptr)
        {
            success = false;
            return nullptr;
        }

        if (key < node->key)
            node->left = remove(node->left, key, success);
        else if (key > node->key)
            node->right = remove(node->right, key, success);
        else
        {
            success = true;
            if (!node->left || !node->right)
            {
                Node<T> *temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }
            else
            {
                Node<T> *temp = minValueNode(node->right);
                node->key = temp->key;
                node->right = remove(node->right, temp->key, success);
            }
        }

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0)
            return rotate_right(node);
        if (balance > 1 && getBalance(node->left) < 0)
        {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }
        if (balance < -1 && getBalance(node->right) <= 0)
            return rotate_left(node);
        if (balance < -1 && getBalance(node->right) > 0)
        {
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }
        return node;
    }

    void preOrder(Node<T> *n)
    {
        if (n)
        {
            cout << n->key << " ";
            preOrder(n->left);
            preOrder(n->right);
        }
    }
    void inOrder(Node<T> *n)
    {
        if (n)
        {
            inOrder(n->left);
            cout << n->key << " ";
            inOrder(n->right);
        }
    }
    void postOrder(Node<T> *n)
    {
        if (n)
        {
            postOrder(n->left);
            postOrder(n->right);
            cout << n->key << " ";
        }
    }
    void levelOrder(Node<T> *n)
    {
        if (!n)
            return;
        queue<Node<T> *> q;
        q.push(n);
        while (!q.empty())
        {
            Node<T> *curr = q.front();
            q.pop();
            cout << curr->key << " ";
            if (curr->left)
                q.push(curr->left);
            if (curr->right)
                q.push(curr->right);
        }
    }

public:
    AVLTree() : root(nullptr) {}
    bool insert(T key)
    {
        bool s;
        root = insert(root, key, s);
        return s;
    }
    bool remove(T key)
    {
        bool s;
        root = remove(root, key, s);
        return s;
    }
    void traverse(int type)
    {
        if (!root)
            return;
        if (type == 1)
            preOrder(root);
        else if (type == 2)
            inOrder(root);
        else if (type == 3)
            postOrder(root);
        else if (type == 4)
            levelOrder(root);
        cout << endl;
    }
    bool isEmpty() { return root == nullptr; }
};

int main()
{
    // Corrected File Handling
    if (fopen("input_avl.txt", "r"))
    {
        freopen("input_avl.txt", "r", stdin);
        freopen("output_avl.txt", "w", stdout);
    }

    int N;
    if (!(cin >> N))
        return 0;
    cout << N << endl;

    AVLTree<int> tree;
    for (int i = 0; i < N; i++)
    {
        int e, x;
        cin >> e >> x;
        if (e == 1)
            cout << "1 " << x << " " << tree.insert(x) << endl;
        else if (e == 0)
            cout << "0 " << x << " " << tree.remove(x) << endl;
        else if (e == 2)
            tree.traverse(x);
    }
    return 0;
}