#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 100;
#define TRUE 1;
#define FALSE 0;
#define ERROR 0;

typedef int Status;


//顾客数据
typedef struct custom{
    int dealTime;    //交易所需时间
    int money;       //正数为存款，负数为取款
    int ID;          //客户序号
    //struct custom *next;
}*qcustom,custom;

//顾客链栈节点
typedef struct custNode{
   //struct custom data[MAX_SIZE];
   int data[50];
   int top;
}custNode;

//事件类型
typedef struct event{
    int type;        //类型，1代表到达事件，0代表离开事件
    int time;        //事件发生时间
    int id;          //顾客序号
}event;

//链表节点
typedef struct LNode{
    event data;             //数据域
    struct LNode *next;     //下一节点指针域
}LNode,*Link;

//链表结构
typedef struct{
   Link head;         //头节点
   Link tail;         //尾节点
}*LinkList,List;

//队列节点
typedef struct QNode{
    custom data;            //数据域
    struct QNode *next;      //下一节点指针
}*QNodeLink,QNode;

//队列结构
typedef struct Queue{
    QNodeLink front;    //队列头指针
    QNodeLink rear;     //队列尾指针
    int length;         //队列长度
}Queue;

custom *dealCustom = NULL;           //正在处理的客户
custom *leaveCustom = NULL;          //马上离开的客户
custom *tempCustom = NULL;           //临时的客户
int total = 10000;                   //初始化银行总金额
int closeTime = 600;                 //银行营业时间
int intervalTime = 15;                    //两个事件之间间隔时间
int dealTimeMax = 15;                     //客户最大交易时间
int dealMoneyMax = 100;                    //客户最大交易额
//custNode *cn = NULL;                  //顾客链栈节点
LinkList eventList = NULL;           //事件链表
Queue *handleQueue = NULL;           //处理队列1
Queue *waitQueue = NULL;             //等待队列2
int nextCustomTime = 0;              //下一个客户到达时间
int customID = 1;                    //客户序号
int currentTime = 0;                 //当前时间
int handleTime;                      //可以处理业务的时间
//custom customArr[50];                //存储客户数据的数组

//初始化链表
LinkList initList(){
     LinkList L = (LinkList*)malloc(sizeof(List));
     if(L == NULL){
        printf("Init failed");
        return NULL;
     }
     L->head = NULL;     //为头指针开拓空间
     L->tail = NULL;
     printf("Init success\n");
     return L;
}

//链表插入
Status insertList(event e,LinkList L){
     if(L == NULL){
        printf("插入失败");
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

//链表遍历
Status linkTraverse(LinkList L){
   if(L == NULL)
    return FALSE;
   Link p = L->head;
   printf("\n遍历结果:\n");
   printf("客户编号         事件发生时间         事件类型");
   printf("\n");
   while(p!=NULL){
    printf("\n    %d     \t%d",p->data.id,p->data.time);
    if(p->data.type == 1){
        printf("                     到达");
        printf("\n");
    }else{
        printf("                     离开");
        printf("\n");
    }
    p = p->next;
   }
   return TRUE;
}


//链表清空
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



//初始化队列
Queue* initQuene(){
   Queue *q = (Queue*)malloc(sizeof(Queue));
   if(q == NULL){
    printf("队列初始化失败");
    return NULL;
   }
   q->front = NULL;
   q->rear = NULL;
   q->length = 0;
   return q;
}

//队列插入
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
     printf("%d混蛋",c.ID);
   }
}


//队列出队并返回数据
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


//队列遍历
Status traverseQueue(Queue *q){
    if(q == NULL){
        printf("空队列");
        return ERROR;
    }
    QNode *p;
    custom *c;
    p = q->front;
    while(q->rear != p){
        *c = p->data;
        q->front = p->next;
        printf("123");
        p = q->front;
        printf("%d客户，有%d钱",c->ID,c->money);
    }
    return TRUE;
}

//队列销毁
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


//返回队头元素
Status popQueue(Queue *q,custom *c){
   if(q->front == NULL)
     return FALSE;
   *c = q->front->data;
   return TRUE;
}

//队列插入,将p插入到q前面
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
//顾客链栈入栈
Status custPush(custom *s){
   if(cn->top == MAX_SIZE){
    printf("接收顾客已满，请明天再来");
    return ERROR;
   }
   cn->data[cn->top] = s;
   cn->top++;
   return TRUE;
}
*/

//随机生成客户信息
custom* random_custom(){
   custom *c;
   c = (qcustom)malloc(sizeof(custom));
   c->dealTime = (rand() % dealTimeMax) + 1;
   c->money = (rand() % 2*dealMoneyMax) - dealMoneyMax;
   c->ID = customID;
   customID++;
   return c;
}

//到达客户时插入事件链表，并返回客户以及获得下一个客户到达时间
custom* arrive_event(){
   if(currentTime == nextCustomTime){
      nextCustomTime = nextCustomTime + (rand() % intervalTime) + 1;
      printf("当前时间： %d,",currentTime);
      custom *c;
      c = random_custom();
      event *e;                //创建事件节点
      e = (event*)malloc(sizeof(event));
      int customID = c->ID;
      e->id = customID;
      e->time = currentTime;
      e->type = 1;
      insertList(*e,eventList);
      printf("欢迎第%d位客户,他有%d这么多钱\n",customID,c->money);
      printf("我们银行还有%d块钱\n",total);
      return c;
   }else{
      custom *c;
      c = (qcustom)malloc(sizeof(custom));
      c->ID = -1;
      return c;
   }
}

//客户离开时插入链表
Status leave_custom(custom *c){
    int customID = c->ID;
    event *e = (event*)malloc(sizeof(event));
    e->id = customID;
    e->time = currentTime;
    e->type = 0;
    insertList(*e,eventList);
    printf("请%d位顾客慢走\n",customID);
    return TRUE;
}

//处理客户
Status handle_custom(custom *c){
   if(currentTime >= handleTime){
     //如果该客户是存款或者取款金额小于银行总金额
     if(c->money > 0 || -(c->money) < total){
        total += c->money;
        handleTime = currentTime + c->dealTime;
        leave_custom(c);
     }else{
         printf("请%d稍等,你的钱%d太多了\n",c->ID,c->money);
         enQueue(waitQueue,*c);
     }
   }else{
     enQueue(handleQueue,*c);
   }
   return TRUE;
}

//处理等待队列
Status wait_queue(){

}



int main(void){
    //cn->top = 0;     //初始化链栈
    dealCustom = (custom*)malloc(sizeof(custom));
    leaveCustom = (custom*)malloc(sizeof(custom));
    handleQueue = initQuene();
    waitQueue = initQuene();
    eventList = initList();
    /*
    printf("\t\t\t********银行业务模拟系统********\n");
    printf("请输入两事情间隔时间:");
    scanf("%d",&intervalTime);
    printf("请输入客户最大交易时间:");
    scanf("%d",&dealTimeMax);
    printf("请输入客户最大交易额:");
    scanf("%d",&dealMoneyMax);
    */
    for(currentTime = 0;currentTime < closeTime;currentTime++){
        tempCustom = (custom*)malloc(sizeof(custom));
        tempCustom = arrive_event();
        if(tempCustom->ID == -1)
            free(tempCustom);
        else{
            handle_custom(tempCustom);
        }
    }
    /*
    dealCustom = arrive_event();
    leaveCustom = arrive_event();
    handle_custom(dealCustom);
    handle_custom(leaveCustom);
    */
    //linkTraverse(eventList);
    int i = 50;
    i = traverseQueue(waitQueue);
    printf("%d",i);
}












