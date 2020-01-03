#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define MAX_SIZE 100
#define TRUE 1;
#define FALSE 0;
#define ERROR 0;

typedef int Status;


//�˿�����
typedef struct custom{
    int arriveTime;  //�ͻ�����ʱ��
    int durtime;     //��������ʱ��
    int amount;       //����Ϊ������Ϊȡ��
    int ID;          //�ͻ����
    struct custom *next;
}*qcustom,custom;

//�ͻ��ڵ�
typedef struct customNode{
    int top;
    custom poor[MAX_SIZE];
}customNode;


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
int intervalTime;                    //���¼����ʱ��
int intervalMaxTime;                 //�����¼�֮�������ʱ��
int intervalMinTime;                 //�����¼�֮����С���ʱ��
int dealTime;                        //�ͻ�����ʱ��
int dealTimeMax;                     //�ͻ������ʱ��
int dealTimeMin;                     //�ͻ���С����ʱ��
int dealMoneyMax;                    //�ͻ�����׶�
LinkList eventList = NULL;           //�¼�����
Queue *handleQueue = NULL;           //�������1
Queue *waitQueue = NULL;             //�ȴ�����2
customNode cn;                       //�ͻ��ڵ�
int nextCustomTime = 0;              //��һ���ͻ�����ʱ��
int customID = 1;                    //�ͻ����
int currentTime = 0;                 //��ǰʱ��
int handleTime = 0;                  //���Դ���ҵ���ʱ��
custom customArr[50];                //�洢�ͻ����ݵ�����

//��ʼ������
LinkList initList(){
     LinkList L = (LinkList*)malloc(sizeof(List));
     if(L == NULL){
        printf("Init failed");
        return NULL;
     }
     L->head = NULL;     //Ϊͷָ�뿪�ؿռ�
     L->tail = NULL;
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
   }else{
     q->rear->next = p;
     q->rear = p;
     q->length++;
   }
}

//�����п�
Status emptyQueue(Queue *q){
   if(q->front == NULL){
    return TRUE;
   }
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
        printf("%d�ͻ�����%dǮ\n",c->ID,c->amount);
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
   if(p == NULL){
    printf("wait��");
    return ERROR;
   }
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


//��ʼ��ջ
Status initStack(customNode* cn){
   cn ->top = 0;
   return TRUE;
}

//��ջ
custom popStack(customNode* cn){
  if(cn->top != 0){
    custom* c;
    c = (custom*)malloc(sizeof(custom));
    *c = cn->poor[--(cn->top)];
    return *c;
  }
}

//�Զ���malloc
Status myMalloc(custom* c){
    int top;
    top = MAX_SIZE - cn.top;
    if(top > 0){
        printf("qwe");
        pushStack(&cn,c);
        printf("456");
        return TRUE;
    }else{
        printf("�޿ռ�ɷ���\n");
        return ERROR;
    }
}

custom myFree(customNode cn){
    custom c;
    c = popStack(&cn);
    printf("�黹�ɹ�\n");
    return c;
}

//��ջ
Status pushStack(customNode *cn,custom* c){
    if(cn->top == 0){
        cn->poor[cn->top] = *c;
        cn->top++;
    }else{
        custom *old;
        int oldTop = cn->top - 1;
        *old = cn->poor[oldTop];
        cn->poor[cn->top] = *c;
        cn->top++;
        old->next = c;
    }
    return TRUE;
}

//��ȡջ��Ԫ��
Status getStack(customNode cn){
  custom c;
  c = cn.poor[cn.top];
  printf("%d\n",c.ID);
  return TRUE;
}

//������ɿͻ���Ϣ
custom random_custom(){
   qcustom c;
   c = (custom*)malloc(sizeof(custom));
   c->arriveTime = currentTime;
   c->durtime = (rand() % dealTimeMax) + 1;
   c->amount = (rand() % (2*dealMoneyMax))- dealMoneyMax;
   c->ID = customID;
   customID++;
   return *c;
}

//����ͻ�ʱ�����¼����������ؿͻ��Լ������һ���ͻ�����ʱ��
custom* arrive_event(){
      nextCustomTime = nextCustomTime + (rand() % intervalTime) + intervalMinTime;
      //printf("��ǰʱ�䣺 %d,",currentTime);
      custom* c;
      custom c1;
      c1 = random_custom();
      c = &c1;
      myMalloc(c);
      event *e;                //�����¼��ڵ�
      e = (event*)malloc(sizeof(event));
      int customID = c->ID;
      e->id = customID;
      e->time = currentTime;
      e->type = 1;
      insertList(*e,eventList);
      //printf("��ӭ��%dλ�ͻ�,����%dԪҪ����\n",customID,c->amount);
      return c;
}

//�ͻ��뿪ʱ��������
Status leave_custom(custom *c){
    event *e = (event*)malloc(sizeof(event));
    e->id = c->ID;
    e->time = currentTime;
    e->type = 0;
    insertList(*e,eventList);
    //printf("\t��%d�Ź˿�����\n",customID);
    custom c1;
    c1 = popStack(&cn);
    printf("%d����\n",c1.ID);
    printf("%d",cn.top);
    return TRUE;
}

//����ͻ�
Status handle_custom(custom *c){
    //printf("��ǰʱ��:%d\t",currentTime);
    //����ÿͻ��Ǵ�����ȡ����С�������ܽ��
    if(c->amount > 0 || -(c->amount) < total){
       int lastMoney;
       if(c->amount > 0){
         lastMoney = total;
       }
       total += c->amount;
       handleTime = currentTime + c->durtime;
       printf("��һ������ʱ����%d",handleTime);
       printf("\t��ǰ����ͻ��ǵ�%dλ,����%dԪҪ����\t",c->ID,c->amount);
       leave_custom(c);
       printf("��л%d�ͻ�,����������%d�ܶ����\n",c->ID,total);
       if(c->amount > 0)
          wait_queue(lastMoney);
       //����Ǵ�����һ��ȴ�����
    }else{
       printf("��%d�Ե�,���Ǯ%d̫����\n",c->ID,c->amount);
       //����ȴ�����
       enQueue(waitQueue,*c);
     }
   return TRUE;
}

//����λ�ڶ���2��������Ҫ��Ŀͻ�
Status handle_waitCustom(custom *c){
    total += c->amount;
    leave_custom(c);
    printf("%d�ͻ��Ⱦ���,����������%d�ܶ����\n",c->ID,total);
    return TRUE;
}

//�����ŶӶ��У�����1��
Status handle_queue(){
   if(emptyQueue(handleQueue))
      return ERROR;
   //���ɴ���ʱ��ʱ,���Ӳ�����ÿͻ�
    if(deQueue(handleQueue,dealCustom))   //�����ӿͻ���Ϣ�洢��dealCustom��
       printf("�ɷ���ʱ�䵽,");
    handle_custom(dealCustom);
   return TRUE;
}



//����ȴ�����
Status wait_queue(int lastMoney){
    if(emptyQueue(waitQueue))
        return ERROR;
    int tempMoney = total;                //��Ϊ��������ӵ�еĽ����ý���lastMoney(�����տͻ�ǰ)��ʱ���������
    Queue *temp = initQuene();            //�½�һ�����У���������Ҫ��Ŀͻ��Ž��˶���
    QNode *check = waitQueue->front;
    custom *tc = (custom*)malloc(sizeof(custom));
    while(waitQueue->front != NULL && tempMoney > lastMoney){
        //���ȴ������еĿͻ����ӣ�������뿪�������
        deQueue(waitQueue,tc);
        //���������ǲ��ܴ���ÿͻ�
        if(-(tc->amount) > total){
            enQueue(temp,*tc);
        }
        //����ܹ�����ÿͻ�
        else{
            tempMoney = tempMoney + tc->amount;
            handle_waitCustom(tc);
        }
    }
    if(!emptyQueue(temp)){
        insertQueue(temp,waitQueue);  //��������Ҫ��Ķ��в嵽�ȴ����е�β��
    }
    return TRUE;
}



int main(void){
    initStack(&cn);             //��ʼ����̬ջ
    printf("%d\n",sizeof(custom));
    printf("%d",sizeof(cn));
    srand(time(NULL));          //�ṩ�����seed
    dealCustom = (custom*)malloc(sizeof(custom));
    leaveCustom = (custom*)malloc(sizeof(custom));
    handleQueue = initQuene();
    waitQueue = initQuene();
    eventList = initList();

    printf("\t\t\t\t\t********����ҵ��ģ��ϵͳ********\n");
    printf("���г�ʼ���:\n");
    scanf("%d",&total);
    printf("���йر�ʱ��:\n");
    scanf("%d",&closeTime);
    a:
    printf("�����������������ʱ��:");
    scanf("%d",&intervalMaxTime);
    printf("��������������С���ʱ��:");
    scanf("%d",&intervalMinTime);
    if(intervalMaxTime < intervalMinTime){
        printf("���ʱ�����Сʱ��С������������\n");
        goto a;
    }
    b:
    printf("������ͻ������ʱ��:");
    scanf("%d",&dealTimeMax);
    printf("������ͻ���С����ʱ��:");
    scanf("%d",&dealTimeMin);
    if(dealTimeMax < dealTimeMin){
        printf("���ʱ�����Сʱ��С������������\n");
        goto b;
    }
    printf("������ͻ�����׶�:");
    scanf("%d",&dealMoneyMax);

    //���ü��ʱ��ʹ�����Ϊ�������С�Ĳ���ڲ����ڴ�����������
    intervalTime = intervalMaxTime - intervalMinTime;
    dealTime = dealTimeMax - dealTimeMin;
    for(currentTime = 0;currentTime < closeTime;currentTime++){
        if(cn.top == 50)
            printf("�������������Ե�\n");
        else{
            if(currentTime >= nextCustomTime){
            tempCustom = (custom*)malloc(sizeof(custom));
            tempCustom = arrive_event();
            enQueue(handleQueue,*tempCustom);        //����ǰ����û����,�����
            }
        }
        if(currentTime >= handleTime){
            handle_queue();
        }
    }
    printf("\t\tӪҵ����������������\n");
    while(!emptyQueue(handleQueue)){
        deQueue(handleQueue,leaveCustom);
        leave_custom(leaveCustom);
    }
    while(!emptyQueue(waitQueue)){
        deQueue(waitQueue,leaveCustom);
        leave_custom(leaveCustom);
    }

    linkTraverse(eventList);
}












