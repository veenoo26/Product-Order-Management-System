#include "utils.h"

/* ============================================================
   GLOBAL VARIABLE DEFINITIONS  (extern declared in models.h)
   ============================================================ */
User gCurrentUser;
int  gIsLoggedIn = 0;

/* ============================================================
   SCREEN
   ============================================================ */
void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLine(char ch, int n) {
    for (int i = 0; i < n; i++) putchar(ch);
    putchar('\n');
}

void printHeader(const char *title) {
    clearScreen();
    printLine('=', LINE_W);
    printf("        PRODUCT ORDER MANAGEMENT SYSTEM\n");
    printf("             ABC Enterprieses\n");
    printLine('=', LINE_W);
    if (gIsLoggedIn) {
        printf("  User : %-25s | Role : %s\n",
               gCurrentUser.fullName, roleName(gCurrentUser.role));
        printLine('-', LINE_W);
    }
    printf("  >> %s\n", title);
    printLine('-', LINE_W);
}

void printFooter(void) {
    printLine('=', LINE_W);
}

/* ============================================================
   INPUT
   ============================================================ */
void clearBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pressEnter(void) {
    printf("\n  Press ENTER to continue...");
    clearBuffer();
}

/* ============================================================
   DATE
   ============================================================ */
void getDate(char *buf) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, DATE_LEN, "%Y-%m-%d %H:%M:%S", tm_info);
}

/* ============================================================
   DATA DIRECTORY
   ============================================================ */
void initDataDir(void) {
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif
}

/* ============================================================
   ENUM -> STRING CONVERTERS
   ============================================================ */
const char *roleName(UserRole r) {
    switch (r) {
        case ROLE_ADMIN:    return "Admin";
        case ROLE_STAFF:    return "Staff";
        case ROLE_CUSTOMER: return "Customer";
        default:            return "Unknown";
    }
}

const char *orderStatusName(OrderStatus s) {
    switch (s) {
        case ORD_PENDING:   return "Pending";
        case ORD_CONFIRMED: return "Confirmed";
        case ORD_DELIVERED: return "Delivered";
        case ORD_CANCELLED: return "Cancelled";
        default:            return "Unknown";
    }
}

const char *payStatusName(PayStatus p) {
    switch (p) {
        case PAY_UNPAID: return "Unpaid";
        case PAY_PAID:   return "Paid";
        default:         return "Unknown";
    }
}