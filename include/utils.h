#ifndef UTILS_H
#define UTILS_H

#include "models.h"

void        clearScreen(void);
void        printLine(char ch, int n);
void        printHeader(const char *title);
void        printFooter(void);
void        clearBuffer(void);
void        pressEnter(void);
void        getDate(char *buf);
void        initDataDir(void);
const char *roleName(UserRole r);
const char *orderStatusName(OrderStatus s);
const char *payStatusName(PayStatus p);

#endif /* UTILS_H */