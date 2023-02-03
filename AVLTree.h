#pragma once

/*
    Modular AVLTree implementation
    use inheritance to overload AVLNode, AVLTree::update
    to easily change this into a tree with extra data in the nodes

    D'tor will free all the nodes, but not the data.
    use freeAllData() to free the data as well.
*/

#include <iostream>
#include "AVLNode.h"

template<class keyType, class T>
class AVLTree{
public:
    using AVLNode = AVLNode<keyType, T>;

AVLNode* root;
int size;

public:

    AVLTree() : root(nullptr), size(0) {};

    //frees only the nodes. not the data
    virtual ~AVLTree(){
        freeAllNodes(root);
    }

    //calls d'tor of all the data in the nodes
    void freeAllData(){
        freeAllData(this->root);
    }

    AVLNode* search(keyType key){
        return search(this->root, key);
    }

    AVLNode* insert(keyType key, T data){
        AVLNode* node = search(this->root, key);
        if(node == nullptr){
            this->root = insert(this->root, key, data);
            this->size++;
            return node;
        }
        return nullptr;
    }

    AVLNode* remove(keyType key){
        AVLNode* node = search(this->root, key);
        if(node != nullptr){
            this->root = remove(this->root, key);
            size--;
            return root;
        }
        return nullptr;
    }

protected:

    void freeAllNodes(AVLNode* node){
        if(node == nullptr) return;
        freeAllNodes(node->left);
        freeAllNodes(node->right);
        delete node;
    }

    void freeAllData(AVLNode* node){
        if(node == nullptr) return;
        freeAllData(node->left);
        freeAllData(node->right);
        delete node->data;
    }
    
    static AVLNode* search(AVLNode* curr, keyType key){  
        if(curr == nullptr) return nullptr;

        if(key < curr->key) return search(curr->left, key);

        if(curr->key < key) return search(curr->right, key);

        return curr;
    }

    static AVLNode* insert(AVLNode* node, keyType key, T data){
        if(node == nullptr) return new AVLNode(key, data);

        if(key < node->key){
            node->left = insert(node->left, key, data);
        } else{
            node->right = insert(node->right, key, data);
        }

        update(node);

        return balance(node);
    }

    static AVLNode* remove(AVLNode* node, keyType key){
        if(node == nullptr) return nullptr;

        if(key < node->key){
            node->left = remove(node->left, key);
        }
        
        else if(node->key < key){
            node->right = remove(node->right, key);
        }

        //found the node we want to remove
        else{
            //node has (maybe) only a right child
            if(node->left == nullptr){
                AVLNode* right = node->right;
                delete node;
                return right;
            }
            //node has (maybe) only a left child
            else if(node->right == nullptr){
                AVLNode* left = node->left;
                delete node;
                return left;
            }
            //node has both right and left childs.
            //look for it's successor, switch between them
            //and then remove the node of(one of) the successor(s).
            //the successor is the smallest(biggest) element in the
            //right(left) subtree. we will search in the right subtree.
            else{
                AVLNode* successor = findMin(node->right);
                node->data = successor->data;
                node->key = successor->key;
                node->right = remove(node->right, successor->key);
            }
        }

        update(node);

        return balance(node);
    }

    static void update(AVLNode* node){
        int leftHeight = (node->left == nullptr) ? -1 : node->left->height; 
        int rightHeight = (node->right == nullptr) ? -1 : node->right->height;

        node->height = 1 + max(leftHeight, rightHeight);

        node->bf = leftHeight - rightHeight;
    }

    static AVLNode* balance(AVLNode* node){
        //left heavy
        if(node->bf == 2){
            //Left-Left
            if(node->left->bf >= 0){
                return LeftLeft(node);
            }
            //Left-Right
            else{
                return LeftRight(node);
            }
        }
        //right heavy
        else if(node->bf == -2){
            //Right-Right
            if(node->right->bf <= 0){
                return RightRight(node);
            }
            //Right-Left
            else{
                return RightLeft(node);
            }
        }

        //node is balanced
        return node;
    }

    static AVLNode* LeftLeft(AVLNode* node){
        return rotateRight(node);
    }

    static AVLNode* RightRight(AVLNode* node){
        return rotateLeft(node);
    }

    static AVLNode* LeftRight(AVLNode* node){
        node->left = rotateLeft(node->left);
        return LeftLeft(node);
    }

    static AVLNode* RightLeft(AVLNode* node){
        node->right = rotateRight(node->right);
        return RightRight(node);
    }

    static AVLNode* rotateRight(AVLNode* node){
        AVLNode* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;

        update(node);
        update(newRoot);

        return newRoot;
    }

    static AVLNode* rotateLeft(AVLNode* node){
        AVLNode* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;

        update(node);
        update(newRoot);

        return newRoot;
    }

    static AVLNode* findMin(AVLNode* node){
        if(node == nullptr) return nullptr;
        while(node->left != nullptr){
            node = node->left;
        }
        return node;
    }

    static AVLNode* findMax(AVLNode* node){
        if(node == nullptr) return nullptr;
        while(node->right != nullptr){
            node = node->right;
        }
        return node;
    }
};