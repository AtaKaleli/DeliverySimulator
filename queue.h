#ifndef QUEUE_H
#define QUEUE_H

/*
=========================================================================================================================

                                    COMMENTS ABOUT FUNCTIONS

-Node=It contains deliveryType,PurchaseTime,DeliveryTime,DeliveryStartTime,CourierID and amountOfPurchase of a customer

-createList=It creates a list for customers and makes it empty at the beginning by calling makeEmptyList function

-isEmptyList=It check whether my list is empty or not.

-createCustomerList=This function should randomly create customers based on the input
(the number of customers, the number of couriers, the maximum purchase time, the maximum delivery time)

-randomPurchaseTime=This function returns an array with sorted random purchase times for customers

-randomDeliveryTime=This function randomly creates delivery times for customers

-randomAmountOfPurchase=This function generates random purchase number between 0-1000. If the number is >=500,
the category  will be A, else, the category will be B.

-randomCustomerType=This function generates random customer types, which will be E,S or F

-createQueue=It creates a queue for seller and makes it empty by calling makeEmptyQueue function

-isEmptyQueue=It check whether my queue is empty or not.

-initializeSimulator=This function should create an empty queue, and also an integer
array to keep the availability of the couriers, and returns to that integer array.

-newCustomer= This function takes a customer from the customer list based on the purchase
time and add him/her to the queue.

-serveCustomer= This function takes a customer from the queue to be served by a courier

-reportStatistics= This function reports the statistics




=========================================================================================================================
*/

/**
Hocam my code sometimes works, I did not understand when it did not work. But in my opinion, when any two customer has same purchase time, my program goes crazy!
I also could not parse the input, so I Took them in main by asking the user
**/


struct Node
{
    char deliveryType;
    int purchaseTime;
    int deliveryTime;
    int deliveryStartTime;
    int CourierID;
    char amountOfPurchase;
    int prty;
    struct Node *next;

};

struct ListRecord
{
    struct Node *head;
    struct Node *tail;
    int size;

};

struct QueueRecord
{
    struct Node *front;
    struct Node *rear;
    int size;
};


typedef struct ListRecord *List;
typedef struct QueueRecord *Queue;


List createList();
void MakeEmptyList(List);
int isEmptyList(List);
void copyCustomerList(List,List);
void createCustomerList(List,int,int,int,int);
int *randomPurchaseTime(int,int);
int randomDeliveryTime(int);
char randomAmountOfPurchase();
char randomCustomerType();
void displayList(List);
void deleteList(List);

Queue createQueue();
int isEmptyQueue(Queue);
void displayQueue(Queue);

int *initializeSimulator(Queue,int);
void newCustomer(List,Queue);
void serveCustomer(Queue);
void reportStatictics(int,int,int,int,int,int *,int,float,int,int,int);


#endif // QUEUE_H

