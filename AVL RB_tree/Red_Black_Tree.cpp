#include <iostream>
#include <cstdio>
using namespace std;

enum Colour
{
    red,
    black
};

template <typename T>
struct Node
{
    T key;
    Node *parent, *left, *right;
    Colour color;
    int sub_tree_size;

    Node(T k) : key(k), parent(nullptr), left(nullptr),
                right(nullptr), color(red), sub_tree_size(1) {}
};

template <typename T>
class RedBlackTree
{
private:
    Node<T> *root;
    Node<T> *TNULL; // sentinel node-> to represent null nodes

    void update_size(Node<T> *x)
    {
        if (x != TNULL)
        {
            x->sub_tree_size =
                1 + x->left->sub_tree_size + x->right->sub_tree_size;
        }
    }

    void update_upwards(Node<T> *x)
    {
        while (x != nullptr)
        {
            update_size(x);
            x = x->parent;
        }
    }

    void rotate_left(Node<T> *x)
    {
        Node<T> *y = x->right;
        x->right = y->left;

        if (y->left != TNULL)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;

        update_size(x);
        update_size(y);
    }

    void rotate_right(Node<T> *y)
    {
        Node<T> *x = y->left;
        y->left = x->right;

        if (x->right != TNULL)
            x->right->parent = y;

        x->parent = y->parent;

        if (y->parent == nullptr)
            root = x;
        else if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;

        x->right = y;
        y->parent = x;

        update_size(y);
        update_size(x);
    }

    void fix_insertion(Node<T> *z)
    {
        while (z->parent != nullptr && z->parent->color == red)
        {
            if (z->parent == z->parent->parent->left)
            {
                Node<T> *uncle = z->parent->parent->right;

                if (uncle->color == red)
                {
                    z->parent->color = black;
                    uncle->color = black;
                    z->parent->parent->color = red;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->right)
                    {
                        z = z->parent;
                        rotate_left(z);
                    }
                    z->parent->color = black;
                    z->parent->parent->color = red;
                    rotate_right(z->parent->parent);
                }
            }
            else
            {
                Node<T> *uncle = z->parent->parent->left;

                if (uncle->color == red)
                {
                    z->parent->color = black;
                    uncle->color = black;
                    z->parent->parent->color = red;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->left)
                    {
                        z = z->parent;
                        rotate_right(z);
                    }
                    z->parent->color = black;
                    z->parent->parent->color = red;
                    rotate_left(z->parent->parent);
                }
            }
        }
        root->color = black;
    }

    void rb_transplant(Node<T> *u, Node<T> *v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        v->parent = u->parent;
    }

    void fix_deletion(Node<T> *x)
    {
        while (x != root && x->color == black)
        {
            if (x == x->parent->left)
            {
                Node<T> *s = x->parent->right;

                if (s->color == red)
                {
                    s->color = black;
                    x->parent->color = red;
                    rotate_left(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == black &&
                    s->right->color == black)
                {
                    s->color = red;
                    x = x->parent;
                }
                else
                {
                    if (s->right->color == black)
                    {
                        s->left->color = black;
                        s->color = red;
                        rotate_right(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = black;
                    s->right->color = black;
                    rotate_left(x->parent);
                    x = root;
                }
            }
            else
            {
                Node<T> *s = x->parent->left;

                if (s->color == red)
                {
                    s->color = black;
                    x->parent->color = red;
                    rotate_right(x->parent);
                    s = x->parent->left;
                }

                if (s->right->color == black &&
                    s->left->color == black)
                {
                    s->color = red;
                    x = x->parent;
                }
                else
                {
                    if (s->left->color == black)
                    {
                        s->right->color = black;
                        s->color = red;
                        rotate_left(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = black;
                    s->left->color = black;
                    rotate_right(x->parent);
                    x = root;
                }
            }
        }
        x->color = black;
    }

    Node<T> *minimum(Node<T> *node)
    {
        while (node->left != TNULL)
            node = node->left;
        return node;
    }

    Node<T> *find_helper(T key)
    {
        Node<T> *curr = root;
        while (curr != TNULL)
        {
            if (key == curr->key)
                return curr;
            if (key < curr->key)
                curr = curr->left;
            else
                curr = curr->right;
        }
        return nullptr;
    }

public:
    RedBlackTree()
    {
        TNULL = new Node<T>(T());
        TNULL->color = black;
        TNULL->sub_tree_size = 0;
        TNULL->left = TNULL;
        TNULL->right = TNULL;
        TNULL->parent = nullptr;
        root = TNULL;
    }

    int insert(T key)
    {
        if (find_helper(key) != nullptr)
            return 0;

        Node<T> *node = new Node<T>(key);
        node->left = TNULL;
        node->right = TNULL;

        Node<T> *y = nullptr;
        Node<T> *x = root;

        while (x != TNULL)
        {
            y = x;
            x->sub_tree_size++;
            if (key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        node->parent = y;

        if (y == nullptr)
            root = node;
        else if (key < y->key)
            y->left = node;
        else
            y->right = node;

        if (node->parent == nullptr)
        {
            node->color = black;
            return 1;
        }

        if (node->parent->parent == nullptr)
            return 1;

        fix_insertion(node);
        return 1;
    }

    int delete_node(T key)
    {
        Node<T> *z = find_helper(key);
        if (z == nullptr)
            return 0;

        Node<T> *y = z;
        Node<T> *x;
        Colour y_original_color = y->color;

        if (z->left == TNULL)
        {
            x = z->right;
            rb_transplant(z, z->right);
        }
        else if (z->right == TNULL)
        {
            x = z->left;
            rb_transplant(z, z->left);
        }
        else
        {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z)
                x->parent = y;
            else
            {
                rb_transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rb_transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;

            update_size(y);
        }

        update_upwards(x->parent);

        if (y_original_color == black)
            fix_deletion(x);

        delete z;
        return 1;
    }

    int search(T key)
    {
        return find_helper(key) != nullptr ? 1 : 0;
    }

    int search_all_lesser_than(T key)
    {
        int count = 0;
        Node<T> *curr = root;

        while (curr != TNULL)
        {
            if (curr->key < key)
            {
                count += 1 + curr->left->sub_tree_size;
                curr = curr->right;
            }
            else
            {
                curr = curr->left;
            }
        }
        return count;
    }
};

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N;
    if (!(cin >> N))
        return 0;

    RedBlackTree<int> rbt;
    // cout << "HELLO\n";

    cout << N << endl;

    for (int i = 0; i < N; ++i)
    {
        int e, x;
        cin >> e >> x;

        if (e == 0)
        {
            int res = rbt.delete_node(x);
            cout << e << " " << x << " " << res << endl;
        }
        else if (e == 1)
        {
            int res = rbt.insert(x);
            cout << e << " " << x << " " << res << endl;
        }
        else if (e == 2)
        {
            int res = rbt.search(x);
            cout << e << " " << x << " " << res << endl;
        }
        else if (e == 3)
        {
            int res = rbt.search_all_lesser_than(x);
            cout << e << " " << x << " " << res << endl;
        }
    }

    // cout << "HELLO\n";
    return 0;
}
