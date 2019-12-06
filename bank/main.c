#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define TRUE 1;
#define FALSE 0;
#define ERROR 0;

typedef int Status;


//�˿�����
typedef struct custom{
    int dealTime;    //��������ʱ��
    int money;       //����Ϊ������Ϊȡ��
}*qcustom,custom;

//�¼�����
typedef struct event{
    int type;        //���ͣ�1�������¼���0�����뿪�¼�
    int time;        //�¼�����ʱ��
    int id;          //�˿����
}event;

//����ڵ�
typedef struct LNode{
    event data;      //������
    struct LNode *next;     //��һ�ڵ�ָ����
}LNode,*Link;

//����ṹ
typedef struct{
   Link head;         //ͷ�ڵ�
   Link tail;         //β�ڵ�
}*LinkList,List;

//���нڵ�
typedef struct QNode{
    custom data;            //������
    struct QNode *next;      //��һ�ڵ�ָ��
}*QNodeLink,QNode;

//���нṹ
typedef struct Queue{
    QNodeLink front;    //����ͷָ��
    QNodeLink rear;     //����βָ��
    int length;         //���г���
}Queue;

int total = 10000;           //��ʼ�������ܽ��
int closeTime = 600;         //����Ӫҵʱ��
int intervalTime;            //�����¼�֮����ʱ��
int dealTimeMax;             //�ͻ������ʱ��
int dealMoneyMax;            //�ͻ�����׶�
int currentTime = 0;         //��ǰʱ��
LinkList eq;                 //�¼�
Queue *handleQueue = NULL;           //�������1
Queue *waitQueue = NULL;             //�ȴ�����2


//��ʼ������
LinkList initList(){
     LinkList L = (LinkList*)malloc(sizeof(List));
     if(L == NULL)
        return NULL;
     L->head = NULL;     //Ϊͷָ�뿪�ؿռ�
     L->tail = NULL;
     printf("Init success\n");
     return L;
}

//�������
Status insertList(event e,LinkList L){
     if(L == NULL)
        return FALSE;
     if(L->head == NULL){
        L->head = (Link)malloc(sizeof(LNode));
        L->head->data = e;
        L->tail = L-> head;
        return TRUE
     }
      else{
        Link p = (Link)malloc(sizeof(LNode));
        p->data = e;
        p->next = NULL;
        L->tail->next = p;
        L->tail = p;
        return TRUE;
      }
}

//�������
Status linkTraverse(LinkList L){
   if(L == NULL)
    return FALSE;
   Link p = L->head;
   printf("\n�������:\n");
   printf("�ͻ����         �¼�����ʱ��         �¼�����");
   printf("\n");
   while(p!=NULL){
    printf("\n    %d     \t%d",p->data.id,p->data.time);
    if(p->data.type == 1){
        printf("                     ����");
        printf("\n");
    }else{
        printf("                     �뿪");
        printf("\n");
    }
    p = p->next;
   }
   return TRUE;
}


//�������
Status cleanLink(LinkList L){
   if(L == NULL)
    return ERROR;
   Link p = L->head;
   Link k = L->head->next;
   while(p != L->tail){
      free(p);
      p = k;
      if(k != L->tail)
        k = k->next;
   }
   free(p);
   L->head = NULL;
   L->tail = NULL;
   return TRUE;
}



//��ʼ������
Queue* initQuene(){
   Queue *q = (Queue*)malloc(sizeof(Queue));
   if(q == NULL)
     return NULL;
   q->front = NULL;
   q->rear = NULL;
   q->length = 0;
   return q;
}

//���в���
Status enQueue(Queue *q,custom c){
   QNode *p;
   p = (QNodeLink)malloc(sizeof(QNode));
   if(p == NULL)
    return ERROR;
   p->data = c;
   p->next = NULL;
   if(q->front == NULL){
     q->front = p;
     q->rear = p;
     q->length = 1;
   }else{
     q->rear->next = p;
     q->rear = p;
     q->length++;
   }
}


//���г��Ӳ���������
Status deQueue(Queue *q,custom *c){
    QNode *p;
    if(q == NULL)
        return ERROR;
    p = q->front;
    *c = p->data;
    q->front = p->next;
    if(q->rear == p)
        q->rear = NULL;
    free(p);
    q->length = 0;
    return TRUE;
}


//�����п�
Status emptyQueue(Queue q){
    if(q.rear == q.front)
        return TRUE;
    return FALSE;
}


//��������
Status destroyQueue(Queue *q){
    if(q == NULL)
        return ERROR;
    QNode* k ;
    QNode* p = q->front;
    while(q != q->rear){
        k = p->next;
        free(p);
        p = k;
    }
    free(q->rear);
    return TRUE;
}


//���ض�ͷԪ��
Status popQueue(Queue *q,custom *c){
   if(emptyQueue(*q))
     return FALSE;
   *c = q->front->data;
   return TRUE;
}

//���в���,��p���뵽qǰ��
Status insertQueue(Queue *q,Queue *p){
   if(p == NULL)
    return ERROR;
   if(q == NULL || q->rear == NULL || q->front == NULL){
      q->front = p->front;
      q->rear = p->rear;
   }else{
      p->rear->next = q->front;
      q->front = p->front;
   }
   return TRUE;
}

//������ɿͻ���Ϣ
custom random_custom(){
   custom *c;
   c = (qcustom)malloc(sizeof(custom));
   if(c == NULL)
    return ERROR;
   c->dealTime = (rand() % dealTimeMax) + 1;
   c->money = (rand() % 2*dealMoneyMax) - dealMoneyMax;
   return c;
}

//���������һ���ͻ�����ʱ��
Status random_arriveTime(){

}


int main(void){
    printf("\t\t\t********����ҵ��ģ��ϵͳ********\n");
    printf("��������������ʱ��:");
    scanf("%d",&intervalTime);
    printf("������ͻ������ʱ��:");
    scanf("%d",&dealTimeMax);
    printf("������ͻ�����׶�:");
    scanf("%d",&dealMoneyMax);
}












