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
	TreeMap* map = (TreeMap *)malloc(sizeof(TreeMap));
	map->root = NULL;
	map->current = NULL;
	map->lower_than = lower_than;
	return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
	if (searchTreeMap(tree, key) != NULL) return; //si ya existe no se inserta

	TreeNode* inserted_node = createTreeNode(key, value);
	if (inserted_node == NULL) return;

	TreeNode* parent = NULL;
	TreeNode* current = tree->root;
	while(current != NULL){
		parent = current;
		if (tree->lower_than(key, current->pair->key)){
			current = current->left;
		} else { 
			current = current->right;
		}
	}

	
	inserted_node->parent = parent;
	if (parent == NULL){
		tree->root = inserted_node;
	} else if (tree->lower_than(key, parent->pair->key)){
		parent->left = inserted_node;
	} else {
		parent->right = inserted_node;
	}
	tree->current = inserted_node;
}

TreeNode * minimum(TreeNode * x){
	if (x == NULL) return NULL;
	if (x->left == NULL) return x;
	
	TreeNode* aux = x;
	while(aux != NULL){
		aux = aux->left;
	}
	return aux;
}


void removeNode(TreeMap * tree, TreeNode* node) {
	if (node == NULL || tree == NULL) return;

	//Si el nodo no tiene hijos
	if(node->right == NULL && node->left == NULL){
		
		if(node->parent == NULL){
			tree->root = NULL;
			
		} else if(node->parent->left == node){
			node->parent->left = NULL;
			
		} else{
			node->parent->right = NULL;
		}
		
		free(node->pair);
		free(node);
	}

	//Si el nodo so tiene un hijo
	else if(node->right == NULL || node->left == NULL){
		TreeNode* aux; //Auxiliar para almacenar el nodo hijo
		
		if (node->right != NULL){
			aux = node->right;
		} else {
			aux = node->left;
		} 
		
		if(node->parent == NULL){
			tree->root = aux;
			aux->parent = NULL;
					
		} else if (node->parent->left == node){
			node->parent->left = aux;
			aux->parent = node->parent;
		} else {
			node->parent->right = aux;
			aux->parent = node->parent;
		}
		free(node->pair);
		free(node);
	}

	//Si el nodo tiene los 2 hijos

	else{
		TreeNode* aux = minimum(node->right);
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
	if (tree == NULL || tree->root == NULL) return NULL;

	TreeNode* aux = tree->root;
	while(aux != NULL){
		if (is_equal(tree, key, aux->pair->key)){
			tree->current = aux;
			return aux->pair;
		} else if(tree->lower_than(key, aux->pair->key)){
			aux = aux->left;
		} else {
			aux = aux->right;
		}
	}
	
	return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
	return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
	return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
	return NULL;
}
