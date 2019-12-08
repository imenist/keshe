#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 100;
#define TRUE 1;
#define FALSE 0;
#define ERROR 0;

typedef int Status;


//�˿�����
typedef struct custom{
    int dealTime;    //��������ʱ��
    int money;       //����Ϊ������Ϊȡ��
    int ID;          //�ͻ����
    //struct custom *next;
}*qcustom,custom;

//�˿���ջ�ڵ�
typedef struct custNode{
   //struct custom data[MAX_SIZE];
   int data[50];
   int top;
}custNode;

//�¼�����
typedef struct event{
    int type;        //���ͣ�1�������¼���0�����뿪�¼�
    int time;        //�¼�����ʱ��
    int id;          //�˿����
}event;

//����ڵ�
typedef struct LNode{
    event data;             //������
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

custom *dealCustom = NULL;           //���ڴ���Ŀͻ�
custom *leaveCustom = NULL;          //�����뿪�Ŀͻ�
custom *tempCustom = NULL;           //��ʱ�Ŀͻ�
int total = 10000;                   //��ʼ�������ܽ��
int closeTime = 600;                 //����Ӫҵʱ��
int intervalTime = 15;                    //�����¼�֮����ʱ��
int dealTimeMax = 15;                     //�ͻ������ʱ��
int dealMoneyMax = 300;                    //�ͻ�����׶�
//custNode *cn = NULL;                  //�˿���ջ�ڵ�
LinkList eventList = NULL;           //�¼�����
Queue *handleQueue = NULL;           //�������1
Queue *waitQueue = NULL;             //�ȴ�����2
int nextCustomTime = 0;              //��һ���ͻ�����ʱ��
int customID = 1;                    //�ͻ����
int currentTime = 0;                 //��ǰʱ��
int handleTime;                      //���Դ���ҵ���ʱ��
//custom customArr[50];                //�洢�ͻ����ݵ�����

//��ʼ������
LinkList initList(){
     LinkList L = (LinkList*)malloc(sizeof(List));
     if(L == NULL){
        printf("Init failed");
        return NULL;
     }
     L->head = NULL;     //Ϊͷָ�뿪�ؿռ�
     L->tail = NULL;
     printf("Init success\n");
     return L;
}

//�������
Status insertList(event e,LinkList L){
     if(L == NULL){
        printf("����ʧ��");
        return FALSE;
     }
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
   if(q == NULL){
    printf("���г�ʼ��ʧ��");
    return NULL;
   }
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
     printf("%dɳ��\n",c.ID);
   }else{
     q->rear->next = p;
     q->rear = p;
     q->length++;
     printf("%d�쵰,��ǰ����%d\n",c.ID,q->length);
   }
}

//�����п�
Status emptyQueue(Queue *q){
   if(q->front == q->rear)
    return TRUE;
   return ERROR;
}


//���г��Ӳ���������
Status deQueue(Queue *q,custom *c){
    QNode *p;
    if(q == NULL)
        return ERROR;
    p = q->front;
    *c = p->data;
    q->front = p->next;
    if(q->rear == p){
        q->rear = NULL;
        q->front = NULL;
    }
    free(p);
    q->length--;
    return TRUE;
}


//���б���
Status traverseQueue(Queue *q){
    if(q == NULL){
        printf("�ն���");
        return ERROR;
    }
    QNode *p;
    custom *c;
    p = q->front;
    while(q->rear != p){
        *c = p->data;
        q->front = p->next;
        p = q->front;
        printf("%d�ͻ�����%dǮ\n",c->ID,c->money);
    }
    return TRUE;
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
   if(q->front == NULL)
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

/*
//�˿���ջ��ջ
Status custPush(custom *s){
   if(cn->top == MAX_SIZE){
    printf("���չ˿�����������������");
    return ERROR;
   }
   cn->data[cn->top] = s;
   cn->top++;
   return TRUE;
}
*/

//������ɿͻ���Ϣ
custom* random_custom(){
   custom *c;
   c = (qcustom)malloc(sizeof(custom));
   c->dealTime = (rand() % dealTimeMax) + 1;
   c->money = (rand() % (2*dealMoneyMax))- dealMoneyMax;
   c->ID = customID;
   customID++;
   return c;
}

//����ͻ�ʱ�����¼����������ؿͻ��Լ������һ���ͻ�����ʱ��
custom* arrive_event(){
      nextCustomTime = nextCustomTime + (rand() % intervalTime) + 1;
      printf("��ǰʱ�䣺 %d,",currentTime);
      custom *c;
      c = random_custom();
      event *e;                //�����¼��ڵ�
      e = (event*)malloc(sizeof(event));
      int customID = c->ID;
      e->id = customID;
      e->time = currentTime;
      e->type = 1;
      insertList(*e,eventList);
      printf("��ӭ��%dλ�ͻ�\n",customID);
      enQueue(handleQueue,*c);        //����ǰ����û����,�����
      return c;
}

//�ͻ��뿪ʱ��������
Status leave_custom(custom *c){
    int customID = c->ID;
    event *e = (event*)malloc(sizeof(event));
    e->id = customID;
    e->time = currentTime;
    e->type = 0;
    insertList(*e,eventList);
    printf("\t��%d�Ź˿�����\n",customID);
    return TRUE;
}

//����ͻ�
Status handle_custom(custom *c){
    //����ÿͻ��Ǵ�����ȡ����С�������ܽ��
    if(c->money > 0 || -(c->money) < total){
       int lastMoney;
       if(c->money > 0){
         lastMoney = total;
       }
       total += c->money;
       handleTime = currentTime + c->dealTime;
       leave_custom(c);
       wait_queue(lastMoney);
       //����Ǵ�����һ��ȴ�����
    }else{
       printf("��%d�Ե�,���Ǯ%d̫����\n",c->ID,c->money);
       //����ȴ�����
       enQueue(waitQueue,*c);
     }
   return TRUE;
}


//�����ŶӶ��У�����1��
Status handle_queue(){
   if(emptyQueue(handleQueue))
      return ERROR;
   //���ɴ���ʱ��ʱ,���Ӳ�����ÿͻ�
   if(handleTime == currentTime){
      deQueue(handleQueue,dealCustom);    //�����ӿͻ���Ϣ�洢��dealCustom��
      handle_custom(dealCustom);
   }
   return TRUE;
}



//����ȴ�����
Status wait_queue(int lastMoney){
    if(emptyQueue(waitQueue))
        return ERROR;
    int tempMoney = total;                //��Ϊ��������ӵ�еĽ����ý���lastMoney(�����տͻ�ǰ)��ʱ���������
    Queue *temp = initQuene();
    QNode *check = waitQueue->front;
    custom *tc = (custom*)malloc(sizeof(custom));
    while(waitQueue->front != NULL && tempMoney > lastMoney){
        //���ȴ������еĿͻ����ӣ�������뿪�������
        deQueue(waitQueue,tc);
        //���������ǲ��ܴ���ÿͻ�
        if(-(tc->money) > total){
            temp->front = check;
        }
        //����ܹ�����ÿͻ�
        else{
            tempMoney = tempMoney + tc->money;
        }
    }
}



int main(void){
    srand(time(NULL));     //�ṩ�����seed
    //cn->top = 0;     //��ʼ����ջ
    dealCustom = (custom*)malloc(sizeof(custom));
    leaveCustom = (custom*)malloc(sizeof(custom));
    handleQueue = initQuene();
    waitQueue = initQuene();
    eventList = initList();
    /*
    printf("\t\t\t********����ҵ��ģ��ϵͳ********\n");
    printf("��������������ʱ��:");
    scanf("%d",&intervalTime);
    printf("������ͻ������ʱ��:");
    scanf("%d",&dealTimeMax);
    printf("������ͻ�����׶�:");
    scanf("%d",&dealMoneyMax);
    */
    for(currentTime = 0;currentTime < closeTime;currentTime++){
        if(currentTime == nextCustomTime){
            tempCustom = (custom*)malloc(sizeof(custom));
            tempCustom = arrive_event();
        }
        if(currentTime == handleTime){
            handle_queue();
        }
    }
    /*
    dealCustom = arrive_event();
    leaveCustom = arrive_event();
    handle_custom(dealCustom);
    handle_custom(leaveCustom);
    */
    //linkTraverse(eventList);
}












