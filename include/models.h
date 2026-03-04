#ifndef MODELS_H
#define MODELS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================
   CONSTANTS
   ============================================================ */
#define MAX_USERS       50
#define MAX_PRODUCTS    100
#define MAX_ORDERS      200
#define MAX_BILLS       200
#define MAX_ITEMS       10

#define NAME_LEN        50
#define PASS_LEN        30
#define PHONE_LEN       15
#define EMAIL_LEN       60
#define DESC_LEN        100
#define DATE_LEN        25
#define LINE_W          70

/* ============================================================
   FILE PATHS
   ============================================================ */
#define USERS_FILE      "data/users.dat"
#define PRODUCTS_FILE   "data/products.dat"
#define ORDERS_FILE     "data/orders.dat"
#define BILLS_FILE      "data/bills.dat"

/* ============================================================
   ENUMS
   ============================================================ */
typedef enum {
    ROLE_ADMIN    = 1,
    ROLE_STAFF    = 2,
    ROLE_CUSTOMER = 3
} UserRole;

typedef enum {
    ORD_PENDING   = 1,
    ORD_CONFIRMED = 2,
    ORD_DELIVERED = 3,
    ORD_CANCELLED = 4
} OrderStatus;

typedef enum {
    PAY_UNPAID = 1,
    PAY_PAID   = 2
} PayStatus;

/* ============================================================
   STRUCTURES  (all schemas in one place)
   ============================================================ */

/* User Schema */
typedef struct {
    int      id;
    char     username[NAME_LEN];
    char     password[PASS_LEN];
    char     fullName[NAME_LEN];
    char     phone[PHONE_LEN];
    char     email[EMAIL_LEN];
    UserRole role;
    int      isActive;          /* 1=active  0=deleted */
} User;

/* Product Schema */
typedef struct {
    int   id;
    char  name[NAME_LEN];
    char  category[NAME_LEN];
    char  description[DESC_LEN];
    float price;
    int   stock;
    int   isActive;
} Product;

/* Order Item Schema (single line in an order) */
typedef struct {
    int   productId;
    char  productName[NAME_LEN];
    int   qty;
    float unitPrice;
    float amount;               /* qty * unitPrice */
} OrderItem;

/* Order Schema */
typedef struct {
    int         id;
    int         customerId;
    char        customerName[NAME_LEN];
    int         createdBy;      /* staff/admin who placed it */
    OrderItem   items[MAX_ITEMS];
    int         itemCount;
    float       totalAmount;
    OrderStatus status;
    char        orderDate[DATE_LEN];
} Order;

/* Bill Schema */
typedef struct {
    int       id;
    int       orderId;
    int       customerId;
    char      customerName[NAME_LEN];
    char      customerPhone[PHONE_LEN];
    OrderItem items[MAX_ITEMS];
    int       itemCount;
    float     subTotal;
    float     discount;
    float     tax;              /* 13% VAT */
    float     grandTotal;
    PayStatus payStatus;
    char      billDate[DATE_LEN];
} Bill;

/* ============================================================
   GLOBAL SESSION  (defined once in src/utils.c)
   ============================================================ */
extern User gCurrentUser;
extern int  gIsLoggedIn;

#endif /* MODELS_H */