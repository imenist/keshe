#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define MAX_SIZE 100
#define TRUE 1;
#define FALSE 0;
#define ERROR 0;

typedef int Status;


//顾客数据
typedef struct custom{
    int arriveTime;  //客户到达时间
    int durtime;     //交易所需时间
    int amount;       //正数为存款，负数为取款
    int ID;          //客户序号
    struct custom *next;
}*qcustom,custom;

//客户节点
typedef struct customNode{
    int top;
    custom poor[MAX_SIZE];
}customNode;


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
int intervalTime;                    //两事件间隔时间
int intervalMaxTime;                 //两个事件之间最大间隔时间
int intervalMinTime;                 //两个事件之间最小间隔时间
int dealTime;                        //客户交易时间
int dealTimeMax;                     //客户最大交易时间
int dealTimeMin;                     //客户最小交易时间
int dealMoneyMax;                    //客户最大交易额
LinkList eventList = NULL;           //事件链表
Queue *handleQueue = NULL;           //处理队列1
Queue *waitQueue = NULL;             //等待队列2
customNode cn;                       //客户节点
int nextCustomTime = 0;              //下一个客户到达时间
int customID = 1;                    //客户序号
int currentTime = 0;                 //当前时间
int handleTime = 0;                  //可以处理业务的时间
custom customArr[50];                //存储客户数据的数组

//初始化链表
LinkList initList(){
     LinkList L = (LinkList*)malloc(sizeof(List));
     if(L == NULL){
        printf("Init failed");
        return NULL;
     }
     L->head = NULL;     //为头指针开拓空间
     L->tail = NULL;
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
   }
}

//队列判空
Status emptyQueue(Queue *q){
   if(q->front == NULL){
    return TRUE;
   }
   return ERROR;
}


//队列出队并返回数据
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
        p = q->front;
        printf("%d客户，有%d钱\n",c->ID,c->amount);
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
   if(p == NULL){
    printf("wait空");
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


//初始化栈
Status initStack(customNode* cn){
   cn ->top = 0;
   return TRUE;
}

//出栈
custom popStack(customNode* cn){
  if(cn->top != 0){
    custom* c;
    c = (custom*)malloc(sizeof(custom));
    *c = cn->poor[--(cn->top)];
    return *c;
  }
}

//自定义malloc
Status myMalloc(custom* c){
    int top;
    top = MAX_SIZE - cn.top;
    if(top > 0){
        printf("qwe");
        pushStack(&cn,c);
        printf("456");
        return TRUE;
    }else{
        printf("无空间可分配\n");
        return ERROR;
    }
}

custom myFree(customNode cn){
    custom c;
    c = popStack(&cn);
    printf("归还成功\n");
    return c;
}

//入栈
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

//获取栈顶元素
Status getStack(customNode cn){
  custom c;
  c = cn.poor[cn.top];
  printf("%d\n",c.ID);
  return TRUE;
}

//随机生成客户信息
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

//到达客户时插入事件链表，并返回客户以及获得下一个客户到达时间
custom* arrive_event(){
      nextCustomTime = nextCustomTime + (rand() % intervalTime) + intervalMinTime;
      //printf("当前时间： %d,",currentTime);
      custom* c;
      custom c1;
      c1 = random_custom();
      c = &c1;
      myMalloc(c);
      event *e;                //创建事件节点
      e = (event*)malloc(sizeof(event));
      int customID = c->ID;
      e->id = customID;
      e->time = currentTime;
      e->type = 1;
      insertList(*e,eventList);
      //printf("欢迎第%d位客户,他有%d元要处理\n",customID,c->amount);
      return c;
}

//客户离开时插入链表
Status leave_custom(custom *c){
    event *e = (event*)malloc(sizeof(event));
    e->id = c->ID;
    e->time = currentTime;
    e->type = 0;
    insertList(*e,eventList);
    //printf("\t请%d号顾客慢走\n",customID);
    custom c1;
    c1 = popStack(&cn);
    printf("%d慢走\n",c1.ID);
    printf("%d",cn.top);
    return TRUE;
}

//处理客户
Status handle_custom(custom *c){
    //printf("当前时间:%d\t",currentTime);
    //如果该客户是存款或者取款金额小于银行总金额
    if(c->amount > 0 || -(c->amount) < total){
       int lastMoney;
       if(c->amount > 0){
         lastMoney = total;
       }
       total += c->amount;
       handleTime = currentTime + c->durtime;
       printf("下一个服务时间是%d",handleTime);
       printf("\t当前服务客户是第%d位,他有%d元要处理\t",c->ID,c->amount);
       leave_custom(c);
       printf("感谢%d客户,我们现在有%d总额度了\n",c->ID,total);
       if(c->amount > 0)
          wait_queue(lastMoney);
       //如果是存款，则检查一遍等待队列
    }else{
       printf("请%d稍等,你的钱%d太多了\n",c->ID,c->amount);
       //放入等待队列
       enQueue(waitQueue,*c);
     }
   return TRUE;
}

//处理位于队列2的且满足要求的客户
Status handle_waitCustom(custom *c){
    total += c->amount;
    leave_custom(c);
    printf("%d客户等久了,我们现在有%d总额度了\n",c->ID,total);
    return TRUE;
}

//处理排队队列（队列1）
Status handle_queue(){
   if(emptyQueue(handleQueue))
      return ERROR;
   //到可处理时间时,出队并服务该客户
    if(deQueue(handleQueue,dealCustom))   //将出队客户信息存储到dealCustom中
       printf("可服务时间到,");
    handle_custom(dealCustom);
   return TRUE;
}



//处理等待队列
Status wait_queue(int lastMoney){
    if(emptyQueue(waitQueue))
        return ERROR;
    int tempMoney = total;                //作为现在银行拥有的金额，当该金额比lastMoney(即接收客户前)少时，跳出检查
    Queue *temp = initQuene();            //新建一条队列，将不符合要求的客户放进此队列
    QNode *check = waitQueue->front;
    custom *tc = (custom*)malloc(sizeof(custom));
    while(waitQueue->front != NULL && tempMoney > lastMoney){
        //将等待队列中的客户出队，看情况离开或者入队
        deQueue(waitQueue,tc);
        //银行总余额还是不能处理该客户
        if(-(tc->amount) > total){
            enQueue(temp,*tc);
        }
        //如果能够满足该客户
        else{
            tempMoney = tempMoney + tc->amount;
            handle_waitCustom(tc);
        }
    }
    if(!emptyQueue(temp)){
        insertQueue(temp,waitQueue);  //将不符合要求的队列插到等待队列的尾部
    }
    return TRUE;
}



int main(void){
    initStack(&cn);             //初始化静态栈
    printf("%d\n",sizeof(custom));
    printf("%d",sizeof(cn));
    srand(time(NULL));          //提供随机数seed
    dealCustom = (custom*)malloc(sizeof(custom));
    leaveCustom = (custom*)malloc(sizeof(custom));
    handleQueue = initQuene();
    waitQueue = initQuene();
    eventList = initList();

    printf("\t\t\t\t\t********银行业务模拟系统********\n");
    printf("银行初始款额:\n");
    scanf("%d",&total);
    printf("银行关闭时间:\n");
    scanf("%d",&closeTime);
    a:
    printf("请输入两事情最大间隔时间:");
    scanf("%d",&intervalMaxTime);
    printf("请输入两事情最小间隔时间:");
    scanf("%d",&intervalMinTime);
    if(intervalMaxTime < intervalMinTime){
        printf("最大时间比最小时间小，请重新输入\n");
        goto a;
    }
    b:
    printf("请输入客户最大交易时间:");
    scanf("%d",&dealTimeMax);
    printf("请输入客户最小交易时间:");
    scanf("%d",&dealTimeMin);
    if(dealTimeMax < dealTimeMin){
        printf("最大时间比最小时间小，请重新输入\n");
        goto b;
    }
    printf("请输入客户最大交易额:");
    scanf("%d",&dealMoneyMax);

    //设置间隔时间和处理金额为最大与最小的差，便于产生在此区间的随机数
    intervalTime = intervalMaxTime - intervalMinTime;
    dealTime = dealTimeMax - dealTimeMin;
    for(currentTime = 0;currentTime < closeTime;currentTime++){
        if(cn.top == 50)
            printf("客人已满，请稍等\n");
        else{
            if(currentTime >= nextCustomTime){
            tempCustom = (custom*)malloc(sizeof(custom));
            tempCustom = arrive_event();
            enQueue(handleQueue,*tempCustom);        //不管前面有没有人,先入队
            }
        }
        if(currentTime >= handleTime){
            handle_queue();
        }
    }
    printf("\t\t营业结束，请明天再来\n");
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












