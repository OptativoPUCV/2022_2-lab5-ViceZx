#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* mapa = (TreeMap *) malloc (sizeof(TreeMap));
    mapa->root= NULL;
    mapa->current= NULL;
    mapa->lower_than= lower_than;
    return mapa;
}


void insertTreeMap(TreeMap * tree, void* key, void * value){
  TreeNode *aux = tree->root;
  while(aux != NULL){
    tree->current = aux;
    if (tree->lower_than(key, aux->pair->key) == 1){
      aux = aux->left;
    }
    else if (tree->lower_than(key, aux->pair->key) == 0){
      aux = aux->right;
    }else{
      return;
    }
  }
  TreeNode *new= createTreeNode(key, value);
  new->parent = tree->current;
  aux = tree->current;
  if(tree->lower_than(key, aux->pair->key) == 1){
    aux->left = new;
  }
  else if (tree->lower_than(aux->pair->key, key) == 1){
    aux->right = new;
  }
  tree->current = new;
  
}

TreeNode * minimum(TreeNode * x){
  TreeNode *aux = x;
  while (aux->left != NULL){
    aux = aux->left;
  }
  return aux;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  TreeNode* aux = node;
  if (node->left == NULL && node->right == NULL){
    if(node->parent->left == node){
      node->parent->left = NULL;
    }
    if(node->parent->right == node){
      node->parent->right = NULL;
    }
    free(node);
    return;
  }
  if (node->left == NULL && node->right != NULL){
    if (node == node->parent->right){
      aux = node->right;
      node->parent->right = aux;
    }
    if (node->parent->left == node){
      aux = node->right;
      node->parent->left = aux;
    }
    aux->parent = node->parent;
    free(node);
    return;
  }
  if (node->left != NULL && node->right == NULL){
    if (node == node->parent->right){
      aux = node->left;
      node->parent->right = aux;
    }
    if (node->parent->left == node){
      aux = node->left;
      node->parent->left = aux;
    }
    aux->parent= node->parent;
    free(node);
    return;
  }
  if (node->left != NULL && node->right != NULL){
    aux = minimum(node->right);
    node->pair = aux->pair;
    removeNode(tree, aux);
  }
  
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode* aux = tree->root;
    while (aux != NULL){
        if (tree->lower_than(key, aux->pair->key) == 1){
          aux = aux->left;
        } 
        else if (tree->lower_than(aux->pair->key, key) == 1){
          aux = aux->right;
        }else{
          tree->current = aux;
          return aux->pair;
        }
    }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode* aux = minimum(tree->root);
  Pair* min = aux->pair;
  return min;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* aux = tree->current;
    if (aux->right != NULL){
        aux = minimum(tree->current->right);
        tree->current = aux;
        return aux->pair;
    }else{
      TreeNode* x = tree->current->parent;
      while (x != NULL){
        if (tree->lower_than(aux->pair->key, x->pair->key) == 1){
          tree->current = x;
          return x->pair;
        }
          x = x->parent;
      }
    }
  return NULL;
}
