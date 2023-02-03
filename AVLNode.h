#pragma once

//easily changeable, use inheritance to add more data to the node

template<class keyType, class T>
struct AVLNode{
    keyType key;
    T data;
    
    int bf;
    int height;

    AVLNode* left;
    AVLNode* right;

    AVLNode(keyType key, T data) : key(key), data(data), bf(0), height(0), left(nullptr), right(nullptr) {}

    ~AVLNode(){}
};