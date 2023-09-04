#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

// 초기화
rbtree *new_rbtree(void) {
  printf("Hello\n");
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree)); // 트리 생성 p
  // TODO: initialize struct if needed
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t)); // nil 노드 생성
  nil_node->color = RBTREE_BLACK; // 모든 리프 노드는 검정색
  t -> nil = nil_node; 
  t -> root = nil_node; // 부모 노드도 nil을 가리켜야 함
  return t;
}

// 노드 메모리 해제
void delete_node(node_t *node, rbtree *t) {
  // 현재 노드가 nil이면 return : 아무 값이 없어서 할당될 것도 없음
  if (node == t -> nil) { return; }
  // 재귀호출로  끝까지 간 후 해제
  delete_node(node -> left, t);
  delete_node(node -> right, t);
  free(node);
  node = NULL; //할당 해제 후 현재 노드 값을 null로 초기화함
  return;
}

// 트리 메모리 해제
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  // 트리 없으면 return
  if (t == NULL) { return; }

  delete_node(t ->root, t); //생성된 노드들의 공간 해제
  free(t->nil); //nil 노드 해제
  t->nil = NULL; //할당 해제 후 nil 값 NULL 값으로 초기화 : 예기치않은 오류와 안정성 확보를 위함
  free(t);
  t = NULL;
  return;
}

// 좌회전
void left_rotation(rbtree *t, node_t *x) {
  node_t *y = x -> right; // y는 현재 x의 오른쪽 자식 노드임
  x -> right = y -> left; // y의 왼쪽 서브 트리를 먼저 떼서 x의 오른쪽 자식에 붙여줌
  if (y -> left != t -> nil) { // 떼고도 만약 y의 왼쪽이 nil이 아니라면
    y -> left -> parent = x; // y의 왼쪽 부모를 x로 변경 : 더 떼
  }
  y -> parent = x -> parent; // x의 부모를 y로 변경 : 회전시킴!
  // x의 위치 잡아주는 조건문 루트, 왼, 오 : 어쨌든 원래 x 자리에 y가 들어감
  if (x -> parent == t -> nil) { //x의 부모가 nil이라면 = x가 루트라면 : 회전했으니 루트를 바꾸자
    t -> root = y; // 트리의 루트를 y로 변경
  } else if (x == x -> parent -> left) { // 회전했는데 x가 x의 부모의 왼쪽자식이라면
    x -> parent -> left = y; //x의 부모의 왼쪽을 y로 변경
  } else { //x가 x의 부모의 오른쪽 자식이라면
    x -> parent -> right = y; //x의 부모의 오른쪽을 y로 변경
  }
  y -> left = x; //y의 왼쪽을 x로 변경
  x -> parent = y; //x의 부모를 y로 변경
  return; //리턴 값 없음 = void 선언
}

//우회전 = 좌회전 반대로
void right_rotation(rbtree *t, node_t *x) {
  node_t *y = x->left; //y는 현재 x의 왼쪽 자식 노드
  x -> left = y -> right; //오른쪽 떼서 왼쪽에 붙임
  if(y -> right != t -> nil) {
    y -> right -> parent = x;
  }
  y -> parent = x -> parent; //회전
  //x 위치 잡아주기
  if (x -> parent == t -> nil) {
    t -> root = y;
  } else if (x == x -> parent -> right) { //x가 right의 자식
    x -> parent -> right = y;
  } else {
    x -> parent -> left = y;
  }
  y -> right = x;
  x -> parent = y;
  return;
}

// 색 변경 함수
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z -> parent -> color == RBTREE_RED) { //z의 부모가 red일때
  // Case # 1 현재 노드의 삼촌, 부모가 red, 할아범이 black : !회전 안 함!
    if (z -> parent == z -> parent -> parent -> left) { //z의 부모가 z의 조상의 왼쪽 자식이면
      node_t *uncle = z -> parent ->parent -> right; //z의 부모의 형제노드 = 삼촌
      if (uncle -> color == RBTREE_RED) { // 삼촌이 red라면
        z -> parent -> color = RBTREE_BLACK; // 부모 red > black
        uncle -> color = RBTREE_BLACK; // 삼촌 red > black
        z -> parent -> parent -> color = RBTREE_RED; // 할아범 black > red
        z = z -> parent -> parent; // 할아버지 가리켜야 다시 while문을 돈다!
      } else { // 삼촌이 black Case 2, 3
        if (z == z -> parent ->right) { //꺾새 모양
          z = z -> parent;
          left_rotation(t, z); // 좌회전
        } // Case 3이 됨 > 색 바꿔주고 우회전하면 끝남
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        right_rotation(t, z -> parent -> parent); // 조상 기준으로 우회전해야함
      }
    } else { //z의 부모가 z의 조상의 오른쪽이면 위의 과정을 반대로 하면 됨
      node_t *uncle = z -> parent -> parent -> left;
      if (uncle -> color == RBTREE_RED) {
        z -> parent -> color = RBTREE_BLACK;
        uncle -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      } else {
        if (z == z -> parent -> left) {
          z = z -> parent;
          right_rotation(t, z);
        }
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        left_rotation(t, z -> parent ->parent);
      }
    }
  } //While문 끝
  t -> root -> color = RBTREE_BLACK; // 규칙 2를 위해서 root는 항상 black
  return; //void형
}

// 데이터 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y = t -> nil; // y = 트리의 nil 노드
  node_t *x = t -> root; // x = 트리의 root
  
  while (x != t -> nil) { //x가 nil에 도달할때까지 반복
    y = x;
    if (key < x->key) { //만약 x의 key값보다 삽입할 key의 값이 작으면
      x = x -> left; //x를 x의 왼쪽으로 변경
    } else { //만약 x의 key값보다 삽입할 key값이 크거나 같으면
      x = x -> right;  // x를 x의 오른쪽으로 변경
    }
  }
  //x가 nil을 가리킴 > z를 삽입할 시기
  node_t *z = (node_t *)calloc(1, sizeof(node_t)); // z 노드 생성
  z -> key = key; // z의 key값 넣어줌

  z -> parent = y; // z의 부모는 y
  if (y == t -> nil) { //y가 트리의 nil일때, 비어있던 애(첫 노드)
    t -> root = z; // Case 1 (고려할 필요 없는 경우임 걍 들어감)
  } else if (z -> key < y -> key) { // y의 key값이 삽입할 key값보다 작을 때
    y -> left = z; // y의 왼쪽 자식으로 감
  } else {
    y -> right = z;
  }
  
  z -> left = t -> nil; // z 왼쪽 자식 nil
  z -> right = t -> nil; // z 오른쪽 자식 nil
  z -> color = RBTREE_RED; // 삽입은 늘 red
  // left_rotation(t,z);
  // if (z->parent->parent != NULL) {
  //   right_rotation(t,z);
  // }
  
  rbtree_insert_fixup(t, z);

  return t->root;
}

// 데이터 검색
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t -> root; //x는 root, root에서부터 검색 시작
  while (x != t -> nil && key != x -> key) { //x가 nil노드가 아니거나 찾는 key값이 x가 아닐때
    if (x -> key < key) { // 찾는 key값보다 key가 클때
      x = x -> right; //x의 오른쪽 자식으로 이동
    } else { //찾는 key값이 더 작을 때
      x = x -> left; //x의 왼쪽 자식으로 이동
    }
  }
  if (x == t -> nil) { // x가 nil 노드 = key값을 찾지 못했을때
    return NULL; // NULL 반환
  }
  return x;
  // return t->root;
}

// 트리의 최소값 = BST특성
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *x = t -> root; //x는 트리의 루트
  while (x -> left != t -> nil) { //x의 왼쪽 자식이 nil이 아닐때
    x = x -> left; // 왼쪽이 제일 작은 값임
  }
  return x;
}

// 트리의 최대값 = BST특성
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *x = t -> root; //x는 트리의 루트
  while (x -> right != t -> nil) { //x의 왼쪽 자식이 nil이 아닐때
    x = x -> right; // 오른쪽이 제일 큰 값임
  }
  return x;
}

// 삭제될 노드를 떼어내기 위해 대체노드를 붙이는 함수
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  // u는 삭제될 노드, v는 대체될 노드 u를 지우고 v를 u.p에 연결시킬거임
  // u가 있던 자리에 걍 v 집어넣는 식 = u에 이어진 노드가 없어 고립되어 자동으로 떨어짐
  // 삭제 연산할때 default로 실행됨
  if (u -> parent == t -> nil) { // u의 부모가 nil = u가 root라는 소리
    t -> root = v; // 트리의 root 자리에 v를 집어넣음
  } else if (u == u -> parent -> left) { // 삭제될 노드가 왼쪽 자식일때
    u -> parent -> left = v; // u는 버려지고, v가 붙음
  } else { // 삭제될 노드가 오른쪽 자식
    u -> parent -> right = v; //오른쪽 자식은 이제 v가 됨
  }
  v -> parent = u -> parent; // v의 부모는 u의 부모
  return; //void형
}

// 색 변경 함수
void rbtree_erase_fixup(rbtree *t, node_t *x){
  node_t *w;
  // fix가 시작되는 조건 : 삭제한 x의 자식이 루트가 아니고, 색깔이 블랙일때 반복
  while ((x != t -> root) && (x -> color == RBTREE_BLACK)) {
    // !!!!기준이 되는 노드가 왼쪽일 떄!!!! 오른쪽은 else문으로 방향 바꿔 처리
    //case 1 : 이중흑색노드의 형제가 red인 경우 - 형제를 black, 부모를 red로 칠하고 부모 기준 좌회전
    if (x == x -> parent -> left) {
      w = x -> parent -> right; // w는 x의 오른쪽 형제 노드임
      if (w -> color == RBTREE_RED) { // 그 오른쪽 형제가 red이면
        w -> color = RBTREE_BLACK; // 오른쪽 형제 노드의 색을 black으로 바꿈
        x -> parent -> color = RBTREE_RED; // x의 부모의 색을 red로 바꿈
        left_rotation(t, x->parent); // x의 부모를 기준으로 좌회전
        w = x -> parent -> right; // case 1 end
      } // case 1을 해결할 경우, case 2, 3, 4 발생 
      // case 2(1-A) : 이중흑색노드의 형제가 black이고, 형제의 양쪽 자식 모두 black인 경우
      // 형제만 red로 만들고, 이중흑색노드의 black 1개를 부모에게 전달
      if ((w -> left -> color == RBTREE_BLACK) && (w -> right -> color == RBTREE_BLACK)) {
        w -> color = RBTREE_RED;
        x = x -> parent; //case 2 end
      } else {
      // 자식 중 최소 1개는 red - case 2의 else
      // case 3(1-B) : 이중흑색노드의 형제가 black이고, 형제의 왼쪽 자식이 red, 오른쪽 자식이 black인 경우
      // 형제노드(w)를 red로, w의 왼쪽 자식을 black으로 칠한 후에 형제노드 기준으로 우회전
        if (w -> right -> color == RBTREE_BLACK) {
          w -> left -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          right_rotation(t, w);
          w = x -> parent -> right; // case 3 end
        }
        //case 4(1-C) : case 2 가 아니고(else), 이중흑색노드의 형제가 black이고 형제의 오른쪽 자식이 red인 경우
        // 부모 노드의 색을 형제에게 넘김, 부모와 형제의 오른쪽 자식을 black으로 칠한 후, 부모기준으로 좌회전
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> right -> color = RBTREE_BLACK;
        left_rotation(t, x -> parent);
        x = t -> root; // while 종료 조건
      } 
    } 
    // 기준이 되는 노드가 오른쪽일 때 - 방향만 반대로 227의 else문
    else {
      w = x -> parent -> left; // w는 x의 왼쪽 형제 노드
      if (w -> color == RBTREE_RED) { // case 1 
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        right_rotation(t, x-> parent);
        w = x -> parent -> left; //case 1 end
      } // case 2
      if ((w -> right -> color == RBTREE_BLACK) && (w -> left -> color == RBTREE_BLACK)) {
        w -> color = RBTREE_RED;
        x = x -> parent;
      } else { // case 3a
          if (w -> left-> color == RBTREE_BLACK) {
              w -> right -> color = RBTREE_BLACK;
              w -> color = RBTREE_RED;
              left_rotation(t, w);
              w = x -> parent -> left;
          } // case 4
          w -> color = x -> parent -> color;
          x -> parent -> color = RBTREE_BLACK;
          w -> left -> color = RBTREE_BLACK;
          right_rotation(t, x -> parent);
          x = t -> root;
      }
    }
  }
  x -> color = RBTREE_BLACK; // 트리의 루트를 블랙으로 변경
  return;
}

// 직후 원소 찾기, 오른쪽 서브트리의 min값 = 제일 오른쪽 값
node_t *rbtree_successor(rbtree *t, node_t *x) {
  node_t *y = x; // y는 직후 원소
  while (y -> left != t -> nil) { //y의 왼쪽 자식이 nil일때까지 반복
    y = y -> left; //y는 y의 왼쪽 자식이 됨
  }
  return y;
}

// 노드 삭제
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *x; // fix_up의 기준이 될 노드 ptr, successor의 값의 자리를 저장해두는 포인터 변수
  node_t *y = p; // 삭제될 노드
  color_t y_origin_color = y -> color; // 변경 전 y의 색 저장

  //case 1 : 왼쪽 노드가 nil일때
  if (p -> left == t -> nil) {
    x = p -> right; // x는 삭제할 노드의 오른쪽을 가리킴
    rbtree_transplant(t, p, p->right); //삭제할 노드의 부모와 삭제할 노드의 오른쪽 연결, x 고립
  } else if (p -> right == t -> nil) { //case 2 : 오른쪽 노드가 nil일때
    x = p -> left;
    rbtree_transplant(t, p, p -> left);
  } else { // case 3 : 양쪽 모두 노드가 있을 때
  // y는 직후 원소 = 오른쪽 서브트리의 최소 키
    y = rbtree_successor(t, p -> right);
    y_origin_color = y -> color;
    x = y -> right; // x는 y의 오른쪽 자식을 가리킴
    if (y -> parent == p) { // min값을 구해서 찾았는데, 삭제할 y가 부모고 p가 y의 왼쪽 자식일때, 즉 case1과 동일한 상황임
      x -> parent = y; 
      // x는 successor의 자리를 저장해두는 애라 y를 따라다님(가리킴) 
      // x가 nil일 경우는 nil의 부모는 설정되지 않기 떄문에 따로 지정해줘야함
    } else { // y의 부모가 삭제할 노드가 아닐때
      rbtree_transplant(t, y, y -> right); // y의 부모노드를 y의 오른쪽 자식과 연결
      y -> right = p -> right; // y의 오른쪽 자식은 삭제할 노드 p의 오른쪽 자식
      y -> right -> parent = y; // y의 오른쪽 자식의 부모는 y
    }
    rbtree_transplant(t, p, y); // 삭제할 노드의 부모와 y를 연결
    y -> left = p -> left; // y의 왼쪽 자식은 삭제할 p의 왼쪽 자식
    y -> left -> parent = y; // y의 왼쪽 자식의 부모는 y
    y -> color = p -> color; // y의 색은 삭제할 p의 색임
  }
  free(p); // 할당 해제
  p = NULL; // 해줘야 누수가 방지됨
  if (y_origin_color == RBTREE_BLACK) { // 이중 흑색 노드 발생
    rbtree_erase_fixup(t, x); //색 변경 함수 호출
  }
  return 0;
}

//트리 중위순회하는 함수, 재귀
int rbtree_inorder(node_t *nil, node_t *root, key_t *arr, const size_t n, int index) {
  if (root == nil) { //root가 nil이면
    return index; //index 반환
  }
  // 재귀 종료 조건
  if (index == n) { 
    return index;
  }
  index = rbtree_inorder(nil, root->left, arr, n, index); //root의 왼쪽 자식으로 재귀
  if (index < n) { //index가 n보다 작을때만 arr에 값을 추가함
    arr[index++] = root -> key; //arr[현재 index] = 현재 노드의 key 값
  }
  index = rbtree_inorder(nil, root->right, arr, n, index); //root의 오른쪽 자식으로 재귀
  return index;
}

// 트리를 이용한 오름차순 arr 구현
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t -> root == t -> nil) { //트리의 루트가 nil이면 반환
    return 0;
  }
  rbtree_inorder(t->nil, t -> root, arr, n, 0); //중위 순회
  return 0;
}