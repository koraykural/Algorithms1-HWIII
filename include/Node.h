/**
 * Node Class for Red-Black Tree.
 */

#ifndef NODE_H
#define NODE_H

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

#endif
