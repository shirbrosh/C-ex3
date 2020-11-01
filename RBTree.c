#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "RBTree.h"

#define FAIL 0
#define SUCCESS 1

Node *findUncle(const Node *father, const Node *grandfather);

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree *myTree = (RBTree *) malloc(sizeof(RBTree));
    if (myTree == NULL)
    {
        return NULL;
    }
    myTree->root = NULL;
    myTree->compFunc = compFunc;
    myTree->freeFunc = freeFunc;
    myTree->size = 0;
    return myTree;
}

/**
 * initialize a new Node with given data and parent.
 * @param data- the data of the new Node
 * @param parent- the parent of the new Node
 * @return the new Node
 */
Node *initializeNode(Node *newNode, void *data, Node *parent)
{

    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = parent;
    newNode->data = data;
    newNode->color = RED;
    return newNode;
}


/**
 * operates a left rotation
 * @param root- the root of the tree to rotate
 * @param nodeToRotate- the node to rotate
 */
void leftRotation(Node *node)
{
    Node *newNode = node->right;
    Node *parent = node->parent;
    if (newNode == NULL)
    {
        return;
    }
    node->right = newNode->left;
    newNode->left = node;
    node->parent = newNode;
    if (node->right != NULL)
    {
        node->right->parent = node;
    }
    if (parent != NULL)
    {
        if (node == parent->left)
        {
            parent->left = newNode;
        }
        else if (node == parent->right)
        {
            parent->right = newNode;
        }
    }
    newNode->parent = parent;
}


/**
 * operates a right rotation
 * @param root- the root of the tree to rotate
 * @param nodeToRotate- the node to rotate
 */
void rightRotation(Node *node)
{
    Node *newNode = node->left;
    Node *parent = node->parent;
    if (newNode == NULL)
    {
        return;
    }
    node->left = newNode->right;
    newNode->right = node;
    node->parent = newNode;
    if (node->left != NULL)
    {
        node->left->parent = node;
    }
    if (parent != NULL)
    {
        if (node == parent->left)
        {
            parent->left = newNode;
        }
        else if (node == parent->right)
        {
            parent->right = newNode;
        }
    }
    newNode->parent = parent;
}


/**
 * This function balances the received tree after the insertion of a newNode
 * @param tree- the tree to balance
 * @param newNode- the Node that had been added to the tree
 */
void rotation(Node *newNode)
{
    //case 1
    if (newNode->parent == NULL)
    {
        newNode->color = BLACK;
        return;
    }
    Node *father = newNode->parent;

    //case 2
    if (father->color == BLACK)
    {
        return;
    }
    Node *grandfather = father->parent;

    //find the uncle
    Node *uncle = findUncle(father, grandfather);

    //case 3
    if (uncle != NULL && uncle->color == RED)
    {
        father->color = BLACK;
        uncle->color = BLACK;
        grandfather->color = RED;
        rotation(grandfather);
    }

        //case 4
    else if (uncle == NULL || uncle->color == BLACK)
    {
        if (newNode == father->right && father == grandfather->left)
        {
            leftRotation(father);
            newNode = newNode->left;
        }
        else if (newNode == father->left && father == grandfather->right)
        {
            rightRotation(father);
            newNode = newNode->right;
        }
        father = newNode->parent;
        grandfather = father->parent;
        if (newNode == father->left)
        {
            rightRotation(grandfather);
        }
        else
        {
            leftRotation(grandfather);
        }
        father->color = BLACK;
        grandfather->color = RED;
    }
}

/**
 * This function finds a nodes uncle
 * @param father- the parent of the node
 * @param grandfather- the parent of the parent of the node
 * @return the node's uncle
 */
Node *findUncle(const Node *father, const Node *grandfather)
{
    Node *uncle;
    if (grandfather->left == father)
    {
        uncle = grandfather->right;
    }
    else
    {
        uncle = grandfather->left;
    }
    return uncle;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int addToRBTree(RBTree *tree, void *data)
{
    if (tree == NULL)
    {
        return FAIL;
    }
    if (data == NULL)
    {
        return FAIL;
    }
    if (containsRBTree(tree, data) == SUCCESS)
    {
        return FAIL;
    }

    Node *newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL)
    {
        return FAIL;
    }
    Node *curNode = tree->root;

    //in case the tree is empty
    if (curNode == NULL)
    {
        newNode = initializeNode(newNode, data, NULL);
        newNode->color = BLACK;
        tree->root = newNode;
        tree->size++;
        return SUCCESS;
    }
    //if the data is already in the tree
    while (true)
    {

        //if the data of the item is bigger thant the data of the node go to the right
        if (tree->compFunc(curNode->data, data) < 0)
        {
            if (curNode->right == NULL)
            {
                newNode = initializeNode(newNode, data, curNode);
                curNode->right = newNode;
                break;
            }
            curNode = curNode->right;
        }
            //if the data of the item is bigger thant the data of the node go to the left
        else
        {
            if (curNode->left == NULL)
            {
                newNode = initializeNode(newNode, data, curNode);
                curNode->left = newNode;
                break;
            }
            curNode = curNode->left;
        }
    }
    rotation(newNode);
    tree->size++;
    tree->root = newNode;
    while (tree->root->parent != NULL)
    {
        tree->root = tree->root->parent;
    }
    return SUCCESS;
}

/**
 * check whether the tree contains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int containsRBTree(RBTree *tree, void *data)
{
    Node *curNode = tree->root;
    while (curNode != NULL)
    {
        int compRes = tree->compFunc(curNode->data, data);
        if (compRes == 0)
        {
            return SUCCESS;
        }
        if (compRes < 0)
        {
            curNode = curNode->right;
        }
        else
        {
            curNode = curNode->left;
        }
    }
    return FAIL;
}

/**
 * This function operates the recursion for forEachRBTree function, it moves throw all the nodes
 * and operates the received func on each one
 * @param node- the start node
 * @param func- the function to operate on the nodes
 * @param args- the args for the given func (optional)
 * @return 0 for failure or 1 for success
 */
int forEachRBTreeRecursion(Node *node, forEachFunc func, void *args)
{
    //move to the left
    if (node->left != NULL)
    {
        if (forEachRBTreeRecursion(node->left, func, args) == FAIL)
        {
            return FAIL;
        }
    }
    if (!func(node->data, args))
    {
        return FAIL;
    }

    //move to the right
    if (node->right != NULL)
    {
        if (forEachRBTreeRecursion(node->right, func, args) == FAIL)
        {
            return FAIL;
        }
    }
    return SUCCESS;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(RBTree *tree, forEachFunc func, void *args)
{
    if (tree == NULL || tree->root == NULL)
    {
        return FAIL;
    }
    return forEachRBTreeRecursion(tree->root, func, args);
}

/**
 * This function moves throw all the nodes in recursion ans frees each data and each node
 * @param tree- the tree from which to free all the nodes.
 * @param node- the node to start the function from- will be the root
 * @return 0 for failure or 1 for success
 */
int freeRBTreeForEachNode(RBTree *tree, Node **node)
{
    if ((*node) == NULL)
    {
        return FAIL;
    }

    //move to the left
    if ((*node)->left != NULL)
    {
        if (freeRBTreeForEachNode(tree, &(*node)->left) == FAIL)
        {
            return FAIL;
        }
        else
        {
            (*node)->left = NULL;
        }
    }

    //move to the right
    if ((*node)->right != NULL)
    {
        if (freeRBTreeForEachNode(tree, &(*node)->right) == FAIL)
        {
            return FAIL;
        }
        else
        {
            (*node)->right = NULL;
        }
    }

    //if we found a node without children- free it
    if ((*node)->right == NULL && (*node)->left == NULL)
    {
        if ((*node)->data != NULL)
        {
            tree->freeFunc((*node)->data);
        }
        free((*node));
        (*node) = NULL;
        return SUCCESS;
    }
    return SUCCESS;
}


/**
 * free all memory of the data structure.
 * @param tree: the tree to free.
 */
void freeRBTree(RBTree *tree)
{
    freeRBTreeForEachNode(tree, &tree->root);
    free(tree);
    tree = NULL;
}

