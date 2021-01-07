/**
 * RedBlackTree class.
 */

#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "Node.h"

template <class Data, class Key>
class RedBlackTree
{
private:
    Node<Data, Key> *root;

    /**
     * Finds sibling of a node. Returns NULL if uncle does not exists.
     * 
     * @param ptr {Node*} Sibling of the node to be searched.
     * 
     * @return {Node*} Sibling of the given node.
     */
    static Node<Data, Key> *get_sibling(Node<Data, Key> *&ptr)
    {
        Node<Data, Key> *parent = ptr->parent;
        if (parent == NULL || parent->left == NULL || parent->right == NULL)
            return NULL;

        if (parent->left->key == ptr->key)
            return parent->right;
        else
            return parent->left;
    }

    /**
     * BST search on a subtree. Recursive
     * 
     * @param root {Node*} Root of the subtree.
     * @param key {Key} Key attrubute to check on nodes.
     * 
     * @return {Node*} NULL or node with the given key.
     */
    static Node<Data, Key> *BSTsearch(Node<Data, Key> *&root, Key key)
    {
        if (root == NULL || root->key == key)
        {
            return root;
        }
        if (root->key < key)
        {
            return BSTsearch(root->right, key);
        }
        else
        {
            return BSTsearch(root->left, key);
        }
    }

    /**
     * BST insert operation on a subtree. Recursive
     * 
     * @param root {Node*} Root of the subtree.
     * @param ptr {Node*} Pointer to the Node to be inserted.
     */
    static void BSTinsert(Node<Data, Key> *&root, Node<Data, Key> *&ptr)
    {
        // Edge case, first node is root
        if (root == NULL)
        {
            root = ptr;
        }

        else if (root->key < ptr->key)
        {
            if (root->right == NULL)
            {
                root->right = ptr;
                ptr->parent = root;
                return;
            }

            // Recursive call on right subtree
            BSTinsert(root->right, ptr);
        }
        else
        {
            if (root->left == NULL)
            {
                root->left = ptr;
                ptr->parent = root;
                return;
            }

            // Recursive call on left subtree
            BSTinsert(root->left, ptr);
        }
    }

    /**
     * Left rotate on a subtree.
     * 
     * @param ptr {Node*} Root of the subtree. Will be left child of the root after rotation.
     */
    void rotate_left(Node<Data, Key> *&ptr)
    {
        Node<Data, Key> *parent = ptr->parent;
        Node<Data, Key> *rchild = ptr->right;

        if (parent != NULL)
        {
            if (parent->right && parent->right->key == ptr->key)
                parent->right = rchild;
            else
                parent->left = rchild;
        }
        ptr->right = rchild->left;
        if (ptr->right != NULL)
            ptr->right->parent = ptr;
        rchild->left = ptr;
        ptr->parent = rchild;
        rchild->parent = parent;

        if (root->key == ptr->key)
        {
            root = rchild;
        }
    }

    /**
     * Right rotate on a subtree.
     * 
     * @param ptr {Node*} Root of the subtree. Will be right child of the root after rotation.
     */
    void rotate_right(Node<Data, Key> *&ptr)
    {
        Node<Data, Key> *lchild = ptr->left;
        Node<Data, Key> *parent = ptr->parent;

        if (parent != NULL)
        {
            if (parent->right && parent->right->key == ptr->key)
                parent->right = lchild;
            else
                parent->left = lchild;
        }

        ptr->left = lchild->right;
        if (ptr->left != NULL)
            ptr->left->parent = ptr;
        lchild->right = ptr;
        ptr->parent = lchild;
        lchild->parent = parent;

        if (root->key == ptr->key)
        {
            root = lchild;
        }
    }

    /**
     * Finds route from grandparent of a node to it. Returns -1 if no route exists
     * 
     * @param ptr {Node*} Grandchild node.
     * 
     * @return {int} Route from grandparent of the given node to it.
     * LeftLeft = 0, LeftRight = 1, RightLeft = 2, RightRight = 3,
     */
    static int get_route_from_grandparent(Node<Data, Key> *&ptr)
    {
        Node<Data, Key> *parent = ptr->parent;
        if (parent == NULL)
        {
            return -1;
        }

        Node<Data, Key> *grandparent = ptr->parent->parent;

        if (grandparent == NULL)
        {
            return -1;
        }

        int route = 0; // Initially LeftLeft

        if (parent->right && parent->right->key == ptr->key)
        {
            route += 1;
        }
        if (grandparent->right && grandparent->right->key == parent->key)
        {
            route += 2;
        }

        return route;
    }

    /**
     * Maintains red-black property after insertion of a new node.
     * 
     * @param ptr {Node*} Pointer to the newly added node. Should be color of RED.
     */
    void fix_insert(Node<Data, Key> *&ptr)
    {
        Node<Data, Key> *parent = ptr->parent;

        // If parent is NULL or BLACK, no violation
        if (parent == NULL || parent->color == BLACK)
        {
            return;
        }

        Node<Data, Key> *grandparent = ptr->parent->parent;
        Node<Data, Key> *uncle = get_sibling(ptr->parent);

        /* --- If uncle is NULL or BLACK --- */
        if (uncle == NULL || uncle->color == BLACK)
        {
            // Route from grandparent to violating node
            int route = get_route_from_grandparent(ptr);
            switch (route)
            {
            case 0:
                // LeftLeft
                rotate_right(grandparent);
                grandparent->toggle_color();
                parent->toggle_color();
                break;
            case 1:
                // LeftRight
                rotate_left(parent);
                fix_insert(parent);
                break;
            case 2:
                // RightLeft
                rotate_right(parent);
                fix_insert(parent);
                break;
            case 3:
                // RightRight
                rotate_left(grandparent);
                grandparent->toggle_color();
                parent->toggle_color();
                break;
            default:
                // No grandparent or parent exists
                break;
            }
        }

        /* --- If uncle is RED --- */
        else
        {
            Node<Data, Key> *gparent = ptr->parent->parent;

            // Recolor: parent, uncle, grandparent
            ptr->parent->toggle_color();
            uncle->toggle_color();
            gparent->toggle_color();

            // Recursive call on grandparent
            fix_insert(gparent);
        }

        // Root is always BLACK
        root->color = BLACK;
    }

    static void BSTpreorder_print(Node<Data, Key> *root, int depth)
    {
        if (root == NULL)
            return;

        for (int i = 0; i < depth; i++)
            cout << "-";

        if (root->color == BLACK)
            cout << "(BLACK) ";
        else
            cout << "(RED) ";

        cout << root->key << endl;
        BSTpreorder_print(root->left, depth + 1);
        BSTpreorder_print(root->right, depth + 1);
    }

public:
    /**
     * Default constructor.
     */
    RedBlackTree()
    {
        root = NULL;
    }

    /**
     * Deallocates memory of the nodes.
     */
    ~RedBlackTree()
    {
        delete_subtree(root);
    }

    /**
     * Deallocates memory of the nodes in a subtree. Recursive
     * 
     * @param ptr {Node*} Root of the subtree
     */
    void delete_subtree(Node<Data, Key> *ptr)
    {
        if (ptr == NULL)
        {
            return;
        }

        delete_subtree(ptr->left);
        delete_subtree(ptr->right);
        delete ptr;
    }

    /**
     * Search for a node in the tree.
     * 
     * @param key {Key} Key to be used in comparison.
     */
    Node<Data, Key> *search(Key key)
    {
        return BSTsearch(root, key);
    }

    /**
     * Inserts a new node into the tree.
     * 
     * Rearranges colors and positions of nodes if needed.
     * 
     * @param node {Node*} Pointer to the node to be inserted.
     */
    void insert(Node<Data, Key> *node)
    {
        // BST Insertion, initial color is red
        BSTinsert(root, node);

        if (root->key == node->key)
        {
            node->color = BLACK;
        }

        // Fix any insert violation
        fix_insert(node);
    }

    void preorder_print()
    {
        BSTpreorder_print(root, 0);
    }
};

#endif
