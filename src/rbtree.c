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
                z -> parent -> parent -> color = RBTREE_RED;
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
    z -> key = key;
    //^이전 위치 저장 -> 현재 노드의 부모 노드 위치 정보 저장
    z -> parent = y;
    //^현재 노드가 루트 노드 또는 노드가 아무것도 없는 상태 -> while문에 걸리지 않고 바로 내려올 수도 있음
    if (y == t -> nil) {
        t -> root = z;
    } else if(z -> key < y -> key){
        y -> left = z;
    }else{
        y -> right = z;
    }
    z -> left = t -> nil;
    z -> right = t -> root;
   
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
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
    //^ u : 삭제하려는 노드 v : 삭제하는 노드의 자리를 채워줄 노드
    //^ u의 부모노드가 없다면 -> 삭제하려는 노드가 nil이라면
    if (u -> parent == t -> nil) {
        //^ 루트노드를 v로 바꾸어준다
        t -> root = v;

    } else if (u == u -> parent -> left) {//^u가 u의 부모의 왼쪽 자식이면
        //^ v를 u의 자리에 넣어준다
        u -> parent -> left = v;
    } else {
        u -> parent -> right = v;
    }
    //^ u의 부모를 v의 부모로 넣어준다
    v -> parent = u -> parent;
}

void rbtree_delete(rbtree *t, node_t *p) {
    node_t *x; //^fix_up의 기준이 될 노드 ptr, successor의 값의 자리를 저장해두는 포인터 변수
    node_t *y = p; //^삭제될 노드
    color_t y_origin_color = y -> color; //^변경 전 y의 색 저장

    //&왼쪽 노드가 nil일때 -> 자식이 하나이거나 없는 케이스, 자식이 있다면 오른쪽 자식일테니
    //&지워지는 노드 대신 오른쪽 노드가 지워진 자리에 올라감
    if(p -> left == t -> nil) {
        //^삭제될 노드의 오른쪽 자식을 x에 저장
        x = p -> right;
        //^ p를 삭제하고, p의 오른쪽 자식을 p의 원래 자리에 넣어준다.
        rbtree_transplant(t, p, p -> right);
    //&오른쪽 노드가 nil일때
    }else if(p -> right == t -> nil) {
        x = p -> left;
        rbtree_transplant(t, p, p -> left);
    }else {//&자식이 둘일때
        //^자식이 둘일때는 삭제되는 색은 삭제되는 노드의 succ색
        //^ minimum 함수로 succ를 찾아서 y에 저장
        y = rbtree_successor(t, p -> right);
        //^삭제되는 색이 삭제되는 노드의 succ색이기 때문에, 즉 y의 색이 삭제 되기 때문에
        //^y의 색 저장
        y_origin_color = y -> color;
        //^x는 y의 오른쪽 자식을 가리킴
        x = y -> right;
        if ( y != p -> right) {
            rbtree_transplant(t, y -> right);
            y -> right = z -> right;
            y -> right -> parent = y;
        } else{
            x -> parent = y;
        }
        rbtree_transplant(t, z, y);
        y -> left = z -> left;
        y -> left -> p = y;
        y -> color = z -> color;
    }
    if (y_origin_color == RBTREE_BLACK){
        rbtree_delete_fixup(t, x);
    }
}
//^succ찾아주는 함수 오른쪽 자식이 있으면 오른쪽 자식 중 minimum을 찾기 위해 오른쪽 자식 -> 오른쪽 자식의 왼쪽 자식으로
//^왼쪽 자식이 없을때까지 내려가서 가장 아래에 있는 왼쪽 자식을 반환한다
node_t *rbtree_minimum(const rbtree *t) {
    node_t *x = t -> root; //^x는 트리의 루트
    while(x -> left != t -> nil) { //^끝까지 내려갔을때까지
        x = x -> left; //^왼쪽이 제일 작은 값
    }
    return x;
} 
//^succ찾아주는 함수 왼쪽 자식이 있으면 왼쪽 자식 중 max를 찾기 위해 왼쪽 자식 -> 오른쪽 자식으로가서
//^오른쪽 자식이 없을때 까지 내려가서 가장 아래에 있는 오른쪽 자식을 반환한다.
node_t *rbtree_maximum(const rbtree *t){
    node_t *x = t -> root; //^시작은 트리의 루트 노드
    while(x -> right != t -> nil) {
        x = x -> right;
    }
    return x;
}

node_t *rbtree_successor(rbtree *t, node_t *x){
    node_t *y = x;
    while (y -> left != t -> nil) {
        y = y -> left;
    }
    return y;
}

void rbtree_delete_fixup(rbtree *t, node_t* x){
    node_t *w;
    //^ fix가 시작되는 조건 : 삭제한 x의 자식이 
        //^&case 1
        //^doubly black의 형제가 red일 때
        //^doubly black의 형제룰 black으로 만든 후 case 2, 3, 4 중 하나로 해결
        if(x == x -> parent -> left) {
            //^w는 x의 오른쪽 형제 노드
            w = x -> parent -> right;
            //^x : doubly black 
            //^doubly black의 형제가 red일 때
            if (w -> color == RBTREE_RED) {
                //^형제 노드를 black으로 바꿔준다
                w -> color = RBTREE_BLACK;
                //^x의 부모 노드를 red로 바꿔준다
                x -> parent -> color = RBTREE_RED;
                //^ x의 부모를 기준으로 left roate
                rbtree_left_rotate(t, x -> parent); 
                w = x -> parent -> right; //^ case 1 end
            } if (w->left->color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK) {
                //&case2 : doubly black의 형제가 black이고 그 형제의 두 자녀 모두 black일 경우
                w -> color = RBTREE_RED; //^형제만 red로 만든다
                x = x -> parent; //^ case 2 end
            } else{
                //^ 자식 중 최소 1개는 red - case 2의 else
                //& case 3 : doubly black의 형제 노드가 black이고, 형제의 왼쪽 자식이 red 오른쪽 자식이 black일때
                if(w -> right -> color == RBTREE_BLACK){
                    w -> left -> color = RBTREE_BLACK;//^왼쪽 자식을 black으로 만들어준다
                    w -> color = RBTREE_RED; //^형제를 red로 만들어준다
                    rbtree_right_rotate(t, w); //^형제를 기준으로 오른쪽으로 회전
                    w = x -> parent -> right;//^case 3 end
                }
                //& case 4 : doubly black의 오른쪽 형제가 black이고
                //& 그 형제의 오른쪽 자녀가 red일때
                w -> color = x -> parent -> color;//^오른쪽 형제를 부모의 색으로
                x -> parent -> color = RBTREE_BLACK;//^부모의 색을 black으로
                w -> right -> color = RBTREE_BLACK;//^형제의 오른쪽 자식의 색을 black으로
                rbtree_left_rotate(t, x -> parent);//^x의 부모를 기준으로 왼쪽 회전
                x = t -> root;//^while 종료 조건
                } 

            }
        //^기준이 되는 노드가 오른쪽 일때 - 방향만 반대로
        else {
                //^w는 x의 오른쪽 형제 노드
                node_t *w = x -> parent -> right;
                if(w -> color == RBTREE_RED) { //^형제의 색이 red일때
                    w -> color = RBTREE_BLACK; //^형제의 색을 black으로 만들어준다
                    x -> parent -> color = RBTREE_RED; //^x의 부모를 red로
                    rbtree_right_rotate(t, x -> parent);//^부모를 기준으로 우회전
                    w = x -> parent -> left;//^부모의 왼쪽 자식이 형제
                }if(w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK){
                    w -> color = RBTREE_RED;
                    x = x -> parent;
                }else {
                    if(w -> left -> color == RBTREE_BLACK) {
                        w -> right -> color = RBTREE_BLACK;
                        w -> color = RBTREE_RED;
                        rbtree_left_rotate(t, w);
                        w = x -> parent -> left;
                        
                    }
                    w -> color = x -> parent -> color;
                    x -> parent -> color = RBTREE_BLACK;
                    w -> left -> color = RBTREE_BLACK;
                    
                    rbtree_right_rotate(t, x -> parent);
                    x = t -> root;
                }
            }
            x ->color = RBTREE_BLACK;
            return;
    }
    
//^트리 중위순회 함수
int rbtree_inorder(node_t *nil, node_t *root, key_t *arr, const size_t n, int index) {
    if (root == nil) { //^root가 nil이면 
        return index; //^index반환
    }
    //^재귀 종료 조건
    if (index == n) {
        return index;
    }
    index = rbtree_inorder(nil, root -> left, arr, n, index); //^root의 왼쪽 자식으로 재귀
    if (index < n) {//^index가 n보다 작을때만 arr에 값을 추가
        arr[index] = root -> key; //^arr[현재 index] = 현재 노드의 key값

    }
    index = rbtree_inorder(nil, root->right, arr, n, index); //^루트의 오른쪽 자식으로 재귀
    return index;
}

//^트리를 이용한 오름차순 arr 구현
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    if(t -> root == t -> nil) {
        //^트리의 루트가 nil이면 반환
        return 0;
    }
    rbtree_inorder(t->nil, t->root, arr, n, 0); //^중위 순회
    return 0;
}