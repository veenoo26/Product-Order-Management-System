#ifndef BILL_H
#define BILL_H

#include "models.h"

/* Operations */
void generateBill(void);
void listAllBills(void);
void listMyBills(void);
void printBillById(void);
void showBill(Bill *b);

/* File I/O */
int  loadBills(Bill *arr);
void saveBills(Bill *arr, int n);

/* Helpers */
int  nextBillId(void);

#endif /* BILL_H */