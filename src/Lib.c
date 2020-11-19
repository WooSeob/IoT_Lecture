#include <stdio.h>
#include <stdlib.h>
#include "Dot_graphic_library.h"
#include "Lib.h"

static Node* AllocNode() {
	return calloc(1, sizeof(Node));
}

//n이 가리키는 노드의 각 멤버에 값을 설정
static void SetNode(Node* n, const Drawable* x, const Node* next) {
	n->value = *x;
    n->next = next;
}

//초기화
void Initialize (RenderQueue* list) {
	list->head = NULL;
    list->cur = NULL;
}

//머리에 노드 삽입
void InsertFront (RenderQueue* list, const Drawable* x) {
	Node* ptr = list->head;
    list->head = list->cur = AllocNode();
    SetNode(list->head, x, ptr);
}

//머리 노드를 삭제
void RemoveFront (RenderQueue* list) {
	if(list->head != NULL) {
    	Node* ptr = list->head->next;
        free(list->head);
        list->head = list->cur = ptr;
    }
}

void MoveNext (RenderQueue* list){
    Node * n = list->cur;
    if(n != NULL){
        n = n->next;
        list->cur = n;
    }
}

//모든 노드를 삭제
void Clear (RenderQueue* list) {
	while(list->head != NULL)
    	RemoveFront(list);
    list->cur = NULL;
}