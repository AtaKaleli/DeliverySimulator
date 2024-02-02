#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

#define SIZE 100
#define TRUE 1
#define FALSE 0

#include "queue.h"

void parse(int *numberOfCouriers,int *numberOfCustomers,int *maximumPurchaseTime,int *maximumDeliveryTime,int argc, char* argv[])
{
    *numberOfCouriers=atoi(argv[1]);
    *numberOfCustomers=atoi(argv[2]);
    *maximumPurchaseTime=atoi(argv[3]);
    *maximumDeliveryTime=atoi(argv[4]);

}

int main(int argc, char* argv[])
{

srand(time(NULL));

List customerList;
List copyList;
customerList=createList();
copyList=createList();

Queue sellerQueue;
sellerQueue=createQueue();




int numberOfCustomers,numberOfCouriers,maximumPurchaseTime,maximumDeliveryTime,i;//I am taking inputs as I could not parse the input

parse(&numberOfCouriers,&numberOfCustomers,&maximumPurchaseTime,&maximumDeliveryTime,argc,argv);





//creates a temporary array to hold values of couriers
int *courier;
courier=(int*)malloc(numberOfCouriers*sizeof(int));
if(courier==NULL)
    printf("Out of memory!\n");

courier=initializeSimulator(sellerQueue,numberOfCouriers);//this holds my courier array, 1=available---0=not available

//creates a list of customers
createCustomerList(customerList,numberOfCustomers,numberOfCouriers,maximumPurchaseTime,maximumDeliveryTime);//I am creating the list of customers

//these are my statistic variables

int startTime,endTime,serveTime,waitingTime=0,totalWaitingTime=0;
int time=0,servedCustomer=0,maxWaitingTime=0;
int expressCount=0,standartCount=0,freeCount=0,A_Count=0,B_Count=0;

float averageWaitingTime;


copyCustomerList(customerList,copyList);//this is copy of my customer list, I used it for not to lose the data of the original list





int *courierCounter;//courierCounter holds the count of the couriers which served to a customer, if courier 1 is served a customer, I made courier1++;
courierCounter=(int*)malloc(numberOfCouriers*sizeof(int));

for(i=0;i<numberOfCouriers;i++)//I initialized all courierCounts to 0 at the beginning, as they did not start to serve
{
    courierCounter[i]=0;
}

struct Node *tmpHead, *headHolder;//here I am creating temporary pointers, headHolder for holding the head->next, and tmpHead for moving one by one in my customer list
tmpHead=customerList->head->next;
headHolder=customerList->head->next;


int avaiableCourier=numberOfCouriers;//this is the number of my available couriers. At the beginning, it equals to number of couriers

while(servedCustomer != numberOfCustomers)//this loop runs until all customers are served
{


if(isEmptyQueue(sellerQueue) && courier[numberOfCouriers-1] != 0 && time==tmpHead->purchaseTime)//here, if all couriers are available, no need to put them in queue,just serve
    {

        for(i=0;i<numberOfCouriers;i++)// here, I am randomly chose the courier and assign it as customerId number.
        {
            if(courier[i]==1)//after that, I make the number of avaiable courier--
            {
                courier[i]=0;
                tmpHead->CourierID=i+1;
                courierCounter[i]++;
                avaiableCourier--;
                break;
            }
        }

        tmpHead->deliveryStartTime=time;


        if(tmpHead->deliveryType=='E')
            expressCount++;
        else if(tmpHead->deliveryType=='S')
            standartCount++;
        else if(tmpHead->deliveryType=='F')
            freeCount++;


        if(tmpHead->amountOfPurchase=='A')
            A_Count++;
        else if(tmpHead->amountOfPurchase=='B')
            B_Count++;




        if(tmpHead->next!=NULL)
            tmpHead=tmpHead->next;

        deleteList(copyList);//I am deleting the served customer from my copy list to not lose any data from original list

    }


    else if(avaiableCourier==0 && time==tmpHead->purchaseTime)//if no courier is available, put the customer(priority enqueue) into the queue
    {
        newCustomer(copyList,sellerQueue);//adding the customer to the queue
        deleteList(copyList);

    }

    else if(!isEmptyQueue(sellerQueue) && avaiableCourier>0)//if there is an customer in queue, take it and start serve(dequeue), when time comes!
    {
        tmpHead->deliveryStartTime=time;
        waitingTime=tmpHead->deliveryStartTime-tmpHead->purchaseTime;
        totalWaitingTime=totalWaitingTime+waitingTime;
        if(maxWaitingTime<waitingTime)
            maxWaitingTime=waitingTime;

        for(i=0;i<numberOfCouriers;i++)
        {
            if(courier[i]==1)
            {
                courier[i]=0;
                tmpHead->CourierID=i+1;
                courierCounter[i]++;
                avaiableCourier--;
                break;
            }
        }

        if(tmpHead->deliveryType=='E')
            expressCount++;
        else if(tmpHead->deliveryType=='S')
            standartCount++;
        else if(tmpHead->deliveryType=='F')
            freeCount++;


        if(tmpHead->amountOfPurchase=='A')
            A_Count++;
        else if(tmpHead->amountOfPurchase=='B')
            B_Count++;




        if(tmpHead->next!=NULL)
            tmpHead=tmpHead->next;

       serveCustomer(sellerQueue);


    }



      if(headHolder->deliveryStartTime+headHolder->deliveryTime==time && headHolder->deliveryStartTime!=0)//if customer is served, make the courier to available,who served to customer
    {


        servedCustomer++;



        //I made available the courier which is done with serving
        int j=headHolder->CourierID-1;//If this is the first courier, the courierID should be 1, and I assigned courierID-1 to j and made the courier[j]=0
        courier[j]=1;
        avaiableCourier++;

        if(headHolder->next!=NULL)
            headHolder=headHolder->next;


        time--;



        printf("\n");
    }




time++;


}

int completionTime=time;
averageWaitingTime=(float)waitingTime/numberOfCustomers;




reportStatictics(numberOfCouriers,numberOfCustomers,expressCount,standartCount,freeCount,courierCounter,completionTime,averageWaitingTime,maxWaitingTime,A_Count,B_Count);

return 0;
}

void MakeEmptyList(List l)
{
    l->head=(struct Node*)malloc(sizeof(struct Node));
    if(l->head==NULL)
        printf("out of memory!\n");

    l->head->next=NULL;
    l->tail=l->head;
    l->size=0;
}
List createList()
{

    List l;
    l=(List)malloc(sizeof(List));
    if(l==NULL)
        printf("Out of memory!\n");

    MakeEmptyList(l);

    return l;
}
int isEmptyList(List l)
{

    if(l->size==0)
        return TRUE;
    else
        return FALSE;

}
void createCustomerList(List l,int numberOfCustomers,int numberOfCouriers,int maximumPurchaseTime,int maximumDeliveryTime)
{

int i;

int *sortedPurchaseTime;//this temporary array holds my sorted random starting times
sortedPurchaseTime=(int*)malloc(numberOfCustomers*sizeof(int));
sortedPurchaseTime=randomPurchaseTime(numberOfCustomers,maximumPurchaseTime);





for(i=0;i<numberOfCustomers;i++)
{
    struct Node *newNode;
    newNode=(struct Node*)malloc(sizeof(struct Node));
    if(newNode==NULL)
        printf("Out of memory!\n");



    newNode->CourierID=0;
    newNode->deliveryStartTime=0;
    newNode->purchaseTime=sortedPurchaseTime[l->size];
    newNode->deliveryTime=randomDeliveryTime(maximumDeliveryTime);
    newNode->amountOfPurchase=randomAmountOfPurchase();
    newNode->deliveryType=randomCustomerType();
    newNode->next=NULL;

    l->tail->next=newNode;
    l->tail=newNode;
    l->size++;
}

}
int *randomPurchaseTime(int numberOfCustomers,int maximumPurchaseTime)
{

int i,j;
   int *sort;
   sort=(int*)malloc(numberOfCustomers*sizeof(int));
   if(sort==NULL)
    printf("Out of memory!");

   for(i=0;i<numberOfCustomers;i++)
   {
       int random=rand()%(maximumPurchaseTime)+1;
       sort[i]=random;
   }


    for(i=1;i<numberOfCustomers;i++) //Insertion sort to make the purchase times in increasing order
   {
      int tmp = sort[i];

      for(j=i; j>0 && tmp< sort[j-1]; j--)
         sort[j] = sort[j-1];
      sort[j] = tmp;
    }

return sort;//I return my array which is sorted in increasing order

}
int randomDeliveryTime(int maximumDeliveryTime)
{
    int randomDelivery;
    randomDelivery=rand()%(maximumDeliveryTime)+1;
    return randomDelivery;
}
char randomAmountOfPurchase()
{
    int randomAmount;
    randomAmount=rand()%1000+1;

    if(randomAmount>=500)
        return 'A';
    else
        return 'B';
}
char randomCustomerType()
{
    int randomType;
    randomType=rand()%4;

    if(randomType==1)
        return 'F';
    else if(randomType==2)
        return 'S';
    else
        return 'E';
}

void copyCustomerList(List l,List copy)
{

int i;
struct Node *tmp;
tmp=l->head->next;

while(tmp != NULL)
{
    struct Node *newNode;
    newNode=(struct Node*)malloc(sizeof(struct Node));
    if(newNode==NULL)
        printf("Out of memory!\n");



    newNode->CourierID=tmp->CourierID;
    newNode->deliveryStartTime=tmp->deliveryStartTime;
    newNode->purchaseTime=tmp->purchaseTime;
    newNode->deliveryTime=tmp->deliveryTime;
    newNode->amountOfPurchase=tmp->amountOfPurchase;
    newNode->deliveryType=tmp->deliveryType;
    newNode->next=NULL;

    copy->tail->next=newNode;
    copy->tail=newNode;
    copy->size++;
    tmp=tmp->next;
}



}


void displayList(List l)
{

    struct Node *p;
    p=l->head->next;

    while(p!=NULL)
    {
        printf("->%c   ->%d  ->%d  ->%d  ->%d  ->%c  \n",p->deliveryType,p->purchaseTime,p->deliveryTime,p->deliveryStartTime,p->CourierID,p->amountOfPurchase);
        p=p->next;

    }

}
Queue createQueue()
{
    Queue q;

    q=(struct QueueRecord*)malloc(sizeof(struct QueueRecord));
    if(q==NULL)
        printf("Out of memory!\n");
    //makeEmptyQueue(q);

    return q;
}
int isEmptyQueue(Queue q)
{
    if(q->size==0)
        return TRUE;
    else
        return FALSE;
}
int *initializeSimulator(Queue q, int numberOfCouriers)
{

    q->front=(struct Node*)malloc(sizeof(struct Node));

    if(q->front==NULL)
        printf("out of memory!\n");

    q->front->next=NULL;
    q->rear=q->front;
    q->size=0;


    int *courierArr;
    courierArr=(int*)malloc(numberOfCouriers*sizeof(int));
    if(courierArr==NULL)
        printf("Out of memory!");

    int i;

    for(i=0;i<numberOfCouriers;i++)
        courierArr[i]=1;

    return courierArr;

}

void deleteList(List copy)
{
      if(!isEmptyList(copy))//here, I execute the function if the list is not empty//
    {
        struct Node *tmp;//the purpose of tmp pointer is that it holds the address ,which is the previous of the node,which we want to delete//

        tmp=copy->head;



            struct Node *remove;//remove is pointing to deleted node,I made connections with tmp and remove->next//
            remove=tmp->next;
            tmp->next=tmp->next->next;
            free(remove);//after making connections, I free the memory of remove//



    copy->size--;//I decrement the size as I deleted a team//

    }
    else
    {
        printf("List is empty!");
    }
}

void newCustomer(List l,Queue q)
{

    struct Node *temp;
    temp=l->head->next;

int i;


    struct Node *newNode;
    newNode=(struct Node*)malloc(sizeof(struct Node));
    if(newNode==NULL)
        printf("Out of memory!\n");

    newNode->CourierID=temp->CourierID;
    newNode->deliveryStartTime=temp->deliveryStartTime;
    newNode->purchaseTime=temp->purchaseTime;
    newNode->deliveryTime=temp->deliveryTime;
    newNode->amountOfPurchase=temp->amountOfPurchase;
    newNode->deliveryType=temp->deliveryType;
    newNode->next=NULL;



    int customerPriority;

    if(newNode->deliveryType=='E')
        customerPriority=3;
    else if(newNode->deliveryType=='S')
        customerPriority=2;
    else if(newNode->deliveryType=='F')
        customerPriority=1;

    newNode->prty=customerPriority;


if(isEmptyQueue(q))
{
    q->rear->next=newNode;
    q->rear=newNode;
    q->size++;

}

else
{


    if(newNode->prty > q->front->next->prty)
    {
        newNode->next=q->front->next;
        q->front->next=newNode;
    }
    else
    {
        struct Node *prevTmp;
        struct Node *tmp;

        prevTmp=q->front->next;
        tmp=q->front->next->next;

        while(tmp != NULL && tmp->prty >= newNode->prty )
        {
            prevTmp=tmp;
            tmp=tmp->next;
        }
        if(tmp == NULL)
        {
            prevTmp->next=newNode;
            q->rear=newNode;
        }
        else
        {
            prevTmp->next=newNode;
            newNode->next=tmp;
        }
    }


q->size++;

}








}
void serveCustomer(Queue q)
{
    if(isEmptyQueue(q))
        printf("queue is emptY!\n");
    else
    {

        struct Node *removeNode;
        int delVal;

        removeNode=q->front->next;
        q->front->next=removeNode->next;
        free(removeNode);
        q->size--;

        if(q->size==0)
            q->rear=q->front;

    }
}
void displayQueue(Queue q)
{
    struct Node *p;
    p=q->front->next;

    while(p != NULL)
    {
        printf("->%c  ->%d  ->%d  ->%d  ->%d  ->%c\n",p->deliveryType,p->purchaseTime,p->deliveryTime,p->deliveryStartTime,p->CourierID,p->amountOfPurchase);
        p=p->next;
    }
}


void reportStatictics(int numberOfCouriers,int numberOfCustomers,int Etype,int Stype,int Ftype,int *courierCounter,int CompletionTime,float AverageTimeSpent,int maxWaitingTime,int Apurchase,int Bpurchase)
{
int i;
printf("****************Delivery Statistics*****************\n\n");
printf("The number of couriers: %d\n",numberOfCouriers);
printf("The number of customers: %d\n",numberOfCustomers);
printf("Number of customers for each delivery type:\n");
printf("Express: %d\nStandart: %d\nFree: %d\n",Etype,Stype,Ftype);
printf("Number of customers for each courier:\n");
for(i=0;i<numberOfCouriers;i++)
{
    printf("Courier %d: %d\n",i+1,courierCounter[i]);
}
printf("Completion time: %d\n",CompletionTime);
printf("Average time spent in the queue: %.2f\n",AverageTimeSpent);
printf("Maximum waiting time is %d\n",maxWaitingTime);

if(Apurchase>Bpurchase)
    printf("Popular purchase: A\n");
else if(Bpurchase>Apurchase)
    printf("Popular purchase: B\n");
else
    printf("No popular purchase! Draw!\n");



}



