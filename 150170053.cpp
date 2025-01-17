/**
 * Compile: g++ -std=c++11 -Wall 150170053.cpp
 * Run:     ./a.out filename.csv
 * 
 * @author Koray Kural
 * @date 09/01/2021
 */
#include <iostream>
#include <fstream> // ifstream

using namespace std;

enum Color
{
    RED,
    BLACK,
};

template <class Data, class Key>
struct Node
{
    Node *parent, *left, *right;
    Color color;
    Key key;
    Data data;

    /**
     * Node constructor. Default color is red.
     * 
     * @param node_data {Data} Data of the node.
     * @param node_key {Key} Key of the node. Will be used to identify the node.
     */
    Node(Data node_data, Key node_key, Color node_color = RED)
    {
        data = node_data;
        key = node_key;
        color = node_color;
        parent = NULL;
        left = NULL;
        right = NULL;
    }

    /**
     * Compares keys of two node.
     */
    bool operator==(const Node &r)
    {
        return key == r.key;
    }

    /**
     * Switches color of node between red and black.
     */
    void toggle_color()
    {
        if (color == BLACK)
            color = RED;
        else
            color = BLACK;
    }

    /**
     * Returns whether node is black or not.
     * 
     * @return {bool} True if node is black
     */
    bool is_black()
    {
        return color == BLACK;
    }

    /**
     * Returns whether node is red or not.
     * 
     * @return {bool} True if node is red
     */
    bool is_red()
    {
        return color == RED;
    }
};

struct PlayerData
{
public:
    string team;
    int point;
    int total_point;
    int rebound;
    int total_rebound;
    int assist;
    int total_assist;

    PlayerData()
    {
    }

    PlayerData(string team, int _point, int _rebound, int _assist)
        : team(team), point(_point), total_point(_point), rebound(_rebound),
          total_rebound(_rebound), assist(_assist), total_assist(_assist)
    {
    }

    PlayerData(const PlayerData &v)
    {
        team = v.team;
        point = v.point;
        total_point = v.total_point;
        rebound = v.rebound;
        total_rebound = v.total_rebound;
        assist = v.assist;
        total_assist = v.total_assist;
    }

    friend std::ostream &
    operator<<(std::ostream &os, const PlayerData &val)
    {
        os << val.point;
        return os;
    }

    void update(int _point, int _assist, int _rebound)
    {
        total_point += _point;
        total_assist += _assist;
        total_rebound += _rebound;
        point = _point;
        assist = _assist;
        rebound = _rebound;
    }
};

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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "File name is not given as argument" << endl;
        return EXIT_FAILURE;
    }

    string filename = argv[1];
    ifstream file;
    file.open(filename);

    if (!file)
    {
        cerr << "File cannot be opened!";
        exit(1);
    }

    RedBlackTree<PlayerData, string> tree;
    string current_season = "";
    int max_point = 0;
    string max_point_name = "";
    int max_assist = 0;
    string max_assist_name = "";
    int max_rebound = 0;
    string max_rebound_name = "";

    // Read header line into dummy variable
    string line;
    getline(file, line);

    while (getline(file, line, ','))
    {
        string season = line;

        string name;
        getline(file, name, ',');

        string team;
        getline(file, team, ',');

        int rebound, assist, point;

        getline(file, line, ',');
        rebound = stoi(line);

        getline(file, line, ',');
        assist = stoi(line);

        getline(file, line, '\n');
        point = stoi(line);

        // Check if season is changed
        if (season != current_season)
        {
            if (current_season.length() != 0)
            {
                // Print the situation
                cout << "End of the " << current_season << " Season" << endl;

                // Print max's
                cout << "Max Points: " << max_point << " - Player Name: " << max_point_name << endl;
                cout << "Max Assists: " << max_assist << " - Player Name: " << max_assist_name << endl;
                cout << "Max Rebs: " << max_rebound << " - Player Name: " << max_rebound_name << endl;
            }

            tree.preorder_print();

            // Update current season
            current_season = season;
        }

        // Search for the player in the tree
        Node<PlayerData, string> *node = tree.search(name);

        // Total scores of player
        int t_point, t_assist, t_rebound;

        if (node == NULL)
        {
            // User is not found in the tree, will be inserted
            PlayerData player_data(team, point, rebound, assist);
            node = new Node<PlayerData, string>(player_data, name);
            tree.insert(node);
            t_point = point;
            t_assist = assist;
            t_rebound = rebound;
        }
        else
        {
            // User is found in the tree, will be updated
            node->data.update(point, assist, rebound);
            t_point = node->data.total_point;
            t_assist = node->data.total_assist;
            t_rebound = node->data.total_rebound;
        }

        // Update max point, rebound and assit
        if (t_assist > max_assist)
        {
            max_assist = t_assist;
            max_assist_name = name;
        }
        if (t_point > max_point)
        {
            max_point = t_point;
            max_point_name = name;
        }
        if (t_rebound > max_rebound)
        {
            max_rebound = t_rebound;
            max_rebound_name = name;
        }
    }

    // Print last season data
    cout << "End of the " << current_season << " Season" << endl;

    // Print max's
    cout << "Max Points: " << max_point << " - Player Name: " << max_point_name << endl;
    cout << "Max Assists: " << max_assist << " - Player Name: " << max_assist_name << endl;
    cout << "Max Rebs: " << max_rebound << " - Player Name: " << max_rebound_name << endl;

    tree.preorder_print();

    file.close();
    return EXIT_SUCCESS;
}
