#include "models.h"
#include "utils.h"
#include "auth.h"
#include "user.h"
#include "product.h"
#include "order.h"
#include "bill.h"

/* Forward declarations */
void adminMenu(void);
void staffMenu(void);
void customerMenu(void);

/* ============================================================
   MAIN
   ============================================================ */
int main(void) {
    initDataDir();
    seedAdmin();

    int running = 1;
    while (running) {
        printHeader("WELCOME  —  MAIN MENU");
        printf("  1. Login\n");
        printf("  2. Register as Customer\n");
        printf("  0. Exit\n");
        printFooter();
        printf("  Choice : ");
        int ch; scanf("%d", &ch); clearBuffer();

        switch (ch) {
            case 1:
                if (doLogin()) {
                    switch (gCurrentUser.role) {
                        case ROLE_ADMIN:    adminMenu();    break;
                        case ROLE_STAFF:    staffMenu();    break;
                        case ROLE_CUSTOMER: customerMenu(); break;
                    }
                }
                break;
            case 2:
                registerCustomer();
                break;
            case 0:
                running = 0;
                break;
            default:
                printf("  [ERROR] Invalid choice!\n");
                pressEnter();
        }
    }

    printHeader("GOODBYE");
    printf("  Thank you for using POMS!\n");
    printFooter();
    return 0;
}

/* ============================================================
   ADMIN MENU
   ============================================================ */
void adminMenu(void) {
    int running = 1;
    while (running && gIsLoggedIn) {
        printHeader("ADMIN DASHBOARD");
        printf("  ----- USER MANAGEMENT -----\n");
        printf("   1.  Add User\n");
        printf("   2.  View All Users\n");
        printf("   3.  Edit User\n");
        printf("   4.  Deactivate User\n");
        printf("  ----- PRODUCT MANAGEMENT -----\n");
        printf("   5.  Add Product\n");
        printf("   6.  View Products\n");
        printf("   7.  Edit Product\n");
        printf("   8.  Remove Product\n");
        printf("   9.  Search Product\n");
        printf("  ----- ORDER MANAGEMENT -----\n");
        printf("  10.  Place Order\n");
        printf("  11.  View All Orders\n");
        printf("  12.  Update Order Status\n");
        printf("  ----- BILL MANAGEMENT -----\n");
        printf("  13.  Generate Bill\n");
        printf("  14.  View All Bills\n");
        printf("  15.  Print Bill\n");
        printf("  ----- ACCOUNT -----\n");
        printf("  16.  Change Password\n");
        printf("   0.  Logout\n");
        printFooter();
        printf("  Choice : ");
        int ch; scanf("%d", &ch); clearBuffer();

        switch (ch) {
            case  1: addUser();          break;
            case  2: listUsers();        break;
            case  3: editUser();         break;
            case  4: removeUser();       break;
            case  5: addProduct();       break;
            case  6: listProducts();     break;
            case  7: editProduct();      break;
            case  8: removeProduct();    break;
            case  9: searchProduct();    break;
            case 10: placeOrder();       break;
            case 11: listAllOrders();    break;
            case 12: updateOrderStatus();break;
            case 13: generateBill();     break;
            case 14: listAllBills();     break;
            case 15: printBillById();    break;
            case 16: changePassword();   break;
            case  0: doLogout(); running = 0; break;
            default:
                printf("  [ERROR] Invalid choice!\n");
                pressEnter();
        }
    }
}

/* ============================================================
   STAFF MENU
   ============================================================ */
void staffMenu(void) {
    int running = 1;
    while (running && gIsLoggedIn) {
        printHeader("STAFF DASHBOARD");
        printf("  ----- PRODUCT -----\n");
        printf("  1.  Add Product\n");
        printf("  2.  View Products\n");
        printf("  3.  Edit Product\n");
        printf("  4.  Search Product\n");
        printf("  ----- ORDER -----\n");
        printf("  5.  Place Order (for Customer)\n");
        printf("  6.  View All Orders\n");
        printf("  7.  Update Order Status\n");
        printf("  ----- BILL -----\n");
        printf("  8.  Generate Bill\n");
        printf("  9.  View All Bills\n");
        printf("  10. Print Bill\n");
        printf("  ----- ACCOUNT -----\n");
        printf("  11. Change Password\n");
        printf("   0. Logout\n");
        printFooter();
        printf("  Choice : ");
        int ch; scanf("%d", &ch); clearBuffer();

        switch (ch) {
            case  1: addProduct();        break;
            case  2: listProducts();      break;
            case  3: editProduct();       break;
            case  4: searchProduct();     break;
            case  5: placeOrder();        break;
            case  6: listAllOrders();     break;
            case  7: updateOrderStatus(); break;
            case  8: generateBill();      break;
            case  9: listAllBills();      break;
            case 10: printBillById();     break;
            case 11: changePassword();    break;
            case  0: doLogout(); running = 0; break;
            default:
                printf("  [ERROR] Invalid choice!\n");
                pressEnter();
        }
    }
}

/* ============================================================
   CUSTOMER MENU
   ============================================================ */
void customerMenu(void) {
    int running = 1;
    while (running && gIsLoggedIn) {
        printHeader("CUSTOMER DASHBOARD");
        printf("  1. View Products\n");
        printf("  2. Search Product\n");
        printf("  3. Place Order\n");
        printf("  4. View My Orders\n");
        printf("  5. View My Bills\n");
        printf("  6. Print My Bill\n");
        printf("  7. Change Password\n");
        printf("  0. Logout\n");
        printFooter();
        printf("  Choice : ");
        int ch; scanf("%d", &ch); clearBuffer();

        switch (ch) {
            case 1: listProducts();   break;
            case 2: searchProduct();  break;
            case 3: placeOrder();     break;
            case 4: listMyOrders();   break;
            case 5: listMyBills();    break;
            case 6: printBillById();  break;
            case 7: changePassword(); break;
            case 0: doLogout(); running = 0; break;
            default:
                printf("  [ERROR] Invalid choice!\n");
                pressEnter();
        }
    }
}