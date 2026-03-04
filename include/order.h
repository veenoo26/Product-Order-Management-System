#ifndef ORDER_H
#define ORDER_H

#include "models.h"

/* Operations */
void placeOrder(void);
void listAllOrders(void);
void listMyOrders(void);
void updateOrderStatus(void);

/* Display */
void printOrderTable(Order *arr, int n);

/* File I/O */
int  loadOrders(Order *arr);
void saveOrders(Order *arr, int n);

/* Helpers */
int  nextOrderId(void);
int  findOrderById(int id, Order *out);

#endif /* ORDER_H */