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
	while(aux->left != NULL){
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
		node->pair->key = aux->pair->key;
		node->pair->value = aux->pair->value;
		//node->pair = aux->pair;

		if (aux->parent->left == aux){
			aux->parent->left = aux->right;
		} else {
			aux->parent->right = aux->right;
		}

		if (aux->right != NULL){
			aux->right->parent = aux->parent;
		}

		
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
/*
	retorna el Pair con clave igual a key.
	En caso de no encontrarlo retorna el primer par asociado a una clave mayor o igual a key
	Para implementarla puede realizar una búsqueda normal y usar un puntero a nodo auxiliar ub_node que vaya guardando el nodo con la menor clave mayor o igual a key. Finalmente retorne el par del nodo ub_node (aux).
	
*/
	
	if (tree == NULL || tree->root == NULL) return NULL;
	
	tree->current = tree->root;
	TreeNode* aux = NULL;
	
	while(tree->current != NULL){
		
		if ((tree->lower_than(key, tree->current->pair->key))){
			aux = tree->current;
			tree->current = tree->current->left;
		}
		else if(tree->lower_than(tree->current->pair->key, key)){
			tree->current = tree->current->right;
		
		} else {
			return tree->current->pair;
		}
	}
	if (aux != NULL){
		return aux->pair;
	} else {
		return NULL;
	}
}

Pair * firstTreeMap(TreeMap * tree) {
	if (tree == NULL || tree->root == NULL) return NULL;

	TreeNode* aux = tree->root;
	while(aux->left != NULL){
		aux = aux->left;
	}

	return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
	if (tree == NULL || tree->root == NULL) return NULL;

  tree->current = tree->root;
	//Si hay subarbol derecho
	if (tree->current->right != NULL){
		tree->current = tree->current->right;
		
		while(tree->current->left != NULL){
				tree->current = tree->current->left;
		}
		return tree->current->pair;
	} else {
		// si no hay subarbol derecho
		TreeNode* aux = tree->current;
		while(aux->parent != NULL && aux->parent->right == aux){
			aux = aux->parent;
		}
		return aux->parent->pair;
	}
	
	return NULL;
	
}
