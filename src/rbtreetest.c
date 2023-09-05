#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

//^초기화
rbtree *new_rbtree(void){
    printf("Hello\n");
    //^트리 생성
    rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
    node_t *nil_node = (node_t*)calloc(1, sizeof(node_t));
    nil_node -> color = RBTREE_BLACK;
    t -> nil = nil_node;
    t -> root = nil_node;
    return t;
}

//^노드 메모리 해제
void delete_node(node_t *node, rbtree *t) {
    //^현재 노드가 nil이면 return : 아무 값이 없어서 할당될 것도 없음
    if (node == t -> nil) {return;}
    //^재귀호출로 끝까지 간 후 해제
    delete_node(node -> left, t);
    delete_node(node -> right, t);
    free(node);
    node = NULL;
    return;
}
//^트리 메모리 해제
void delete_rbtree(rbtree *t){
    //^트리 없으면 return
    if (t == NULL) { return;}
    delete_node(t -> root, t); //^생성된 노드들의 공간 해제
    free(t -> nil); //^nil노드 해제
    t -> nil = NULL; //^할당 해제 후 nil값 NULL 값으로 초기화
    free(t);
    t = NULL;
    return;
}
void rbtree_insert_fixup(rbtree *t, node_t *z) {
    //^부모 노드가 red인 경우
    while (z -> parent -> color == RBTREE_RED){
        //^
        if (z -> parent == z -> parent -> parent -> left) {
            //^ 현재 노드의 삼촌 노드
            node_t *uncle = z -> parent -> parent -> right;
            
            //^삼촌이 RED일때 -> recoloring
            if(uncle->color == RBTREE_RED)
            {//& CASE 1
                //^부모노드와 삼촌노드의 색을 black으로 바꾸어준다
                z -> parent -> color = RBTREE_BLACK;
                uncle -> color = RBTREE_BLACK;
                //^할아버지 노드를 red로 바꾸어준다
                z -> parent -> parent = RBTREE_RED;
                //^할아버지 기준으로 재정렬해야하기 때문에 
                z = z -> parent -> parent;
            } else { //& CASE 2
                if(z == z -> parent -> right){
                    z = z -> parent;
                    rbtree_left_rotate(t, z);
                } //& CASE 3
                z -> parent -> color = RBTREE_BLACK;
                z -> parent -> parent -> color = RBTREE_RED;
                rbtree_right_rotate(t, z -> parent -> parent);
            }
        } else {
            node_t *uncle = z -> parent -> parent -> left;
            if (uncle -> color == RBTREE_RED) {
                z -> parent -> color = RBTREE_BLACK;
                uncle -> color = RBTREE_BLACK;
                z -> parent -> parent -> color = RBTREE_RED;
                z = z -> parent -> parent;
            } else{
                if (z == z -> parent -> left) {
                    z = z -> parent;
                    rbtree_right_rotate(t, z);

                }
                z -> parent -> color = RBTREE_BLACK;
                z -> parent -> parent -> color = RBTREE_RED;
                rbtree_left_rotate(t, z -> parent -> parent);
            }
        }
    }
    t -> root -> color = RBTREE_BLACK;
    return;
}
void rbtree_left_rotate(rbtree *t, node_t *x){
    node_t *y = x -> right;
    x -> right = y -> left;
    if (y -> left != t -> nil) {
        y -> left -> parent = x;
    }
    y -> parent = x -> parent;
    //^x가 루트노드라면
    if (x -> parent == t -> nil) {
        //^y가 새로운 루트 노드가 된다.
        t -> root = y;

    }
    else if(x == x -> parent -> left) {
        x -> parent -> left = y;
    
    }else{
        x -> parent -> right = y;

    }
    y -> left = x;
    x -> parent = y;
    return;
}
void rbtree_right_rotate(rbtree *t, node_t *x){
    node_t *y = x -> left;
    x -> left = y -> right;
    if (y -> right != t -> nil) {
        //^y의 오른쪽 자식을 x에 붙인다
        y -> right -> parent = x;
    }
    y -> parent = x -> parent;
    if ( x -> parent == t -> nil) {
        t -> root = y;
    } else if(x == x -> parent -> right) {
        x -> parent -> right = y;
    }
    y -> right = x;
    x -> parent = y;
    return;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {
    //^t는 tree의 정보를 담고 있는 rbtree 구조체 포인터
    //^x는 t의 root 즉, x는 root
    node_t *x = t -> root;
    //^y는 nil을 가리키고 있다. y는 x의 parent라고 생각하면 됨
    node_t *y = t -> nil;
    node_t *z = (node_t*)calloc(1, sizeof(node_t));
    while(x != t -> nil) {
        y = x;
        //^만약 새로 넣을 key값이 현재 노드의 key값보다 작다면 x(==커서라고 생각하자)를 왼쪽 자식으로 이동한다
        if (x -> key > key) {
            x = x -> left;
        }
        else{
            x = x -> right;
        }
        
    }
    //^이전 위치 저장 -> 현재 노드의 부모 노드 위치 정보 저장
    x -> parent = y;
    //^현재 노드가 루트 노드 또는 노드가 아무것도 없는 상태 -> while문에 걸리지 않고 바로 내려올 수도 있음
    if (y == t -> nil) {
        z = t -> root;
    } else if(key < y -> key){
        z = y -> left;
    }else{
        z = y -> right;
    }
    z -> left = t -> nil;
    z -> right = t -> root;
    z -> key = key;
    z -> color = RBTREE_RED;
    rbtree_insert_fixup(t, z);
    return t -> root;
}
//^왜 얘만 const?
node_t *rbtree_find(const rbtree *t, const key_t key) {
    node_t *x = t -> root; //^x는 이진탐색트리에서 cur역할과 동일하다 생각하면 됨. root에서부터 탐색하니까 초기에 root를 넣어줌
    while (x != t -> nil && key != x -> key) {
        //^x가 nil노드가 아니거나, cur(x)의 key가 찾는 값이 아닐때
        //^현재 cur의 key값이 찾고자하는 key값보다 작을때, 즉 찾고자 하는key 값이 더 클때
        if(x -> key < key) {
            //^오른쪽으로 이동
            x = x -> right;
        } else {
            x = x -> left;
        }
    }
    //^ 원하는 값을 찾지 못했을 경우
    if (x == t -> nil) {
        return NULL;

    }
    //^찾았을때
    return x;

}