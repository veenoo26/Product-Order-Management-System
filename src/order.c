#include "order.h"
#include "product.h"
#include "user.h"
#include "utils.h"

/* ============================================================
   FILE I/O
   ============================================================ */
int loadOrders(Order *arr) {
    FILE *fp = fopen(ORDERS_FILE, "rb");
    if (!fp) return 0;
    int n = 0;
    while (n < MAX_ORDERS && fread(&arr[n], sizeof(Order), 1, fp) == 1)
        n++;
    fclose(fp);
    return n;
}

void saveOrders(Order *arr, int n) {
    FILE *fp = fopen(ORDERS_FILE, "wb");
    if (!fp) { printf("  [ERROR] Cannot write orders file!\n"); return; }
    fwrite(arr, sizeof(Order), n, fp);
    fclose(fp);
}

int nextOrderId(void) {
    Order arr[MAX_ORDERS];
    int   n = loadOrders(arr), maxId = 0;
    for (int i = 0; i < n; i++)
        if (arr[i].id > maxId) maxId = arr[i].id;
    return maxId + 1;
}

int findOrderById(int id, Order *out) {
    Order arr[MAX_ORDERS];
    int   n = loadOrders(arr);
    for (int i = 0; i < n; i++) {
        if (arr[i].id == id) { *out = arr[i]; return 1; }
    }
    return 0;
}

/* ============================================================
   DISPLAY
   ============================================================ */
void printOrderTable(Order *arr, int n) {
    if (n == 0) { printf("  No orders found.\n"); return; }
    printLine('-', LINE_W);
    printf("  %-9s %-18s %-5s %12s %-12s %s\n",
           "Order-ID", "Customer", "Items", "Total(Rs)", "Status", "Date");
    printLine('-', LINE_W);
    for (int i = 0; i < n; i++) {
        printf("  ORD-%-5d %-18s %-5d %12.2f %-12s %s\n",
               arr[i].id, arr[i].customerName, arr[i].itemCount,
               arr[i].totalAmount, orderStatusName(arr[i].status),
               arr[i].orderDate);
    }
    printLine('-', LINE_W);
    printf("  Total: %d order(s)\n", n);
}

/* ============================================================
   OPERATIONS
   ============================================================ */
void placeOrder(void) {
    printHeader("PLACE ORDER");

    Order ord;
    memset(&ord, 0, sizeof(Order));
    ord.id        = nextOrderId();
    ord.status    = ORD_PENDING;
    ord.createdBy = gCurrentUser.id;
    getDate(ord.orderDate);

    /* Determine customer */
    if (gCurrentUser.role == ROLE_STAFF ||
        gCurrentUser.role == ROLE_ADMIN)
    {
        printf("  Customer ID : ");
        int cid; scanf("%d", &cid); clearBuffer();
        User cust;
        if (!findUserById(cid, &cust) || cust.role != ROLE_CUSTOMER) {
            printf("  [ERROR] Customer not found!\n");
            pressEnter();
            return;
        }
        ord.customerId = cust.id;
        strcpy(ord.customerName, cust.fullName);
    } else {
        ord.customerId = gCurrentUser.id;
        strcpy(ord.customerName, gCurrentUser.fullName);
    }

    /* Show available products */
    Product prods[MAX_PRODUCTS];
    int     np = loadProducts(prods);
    printf("\n  --- Available Products ---\n");
    printProductTable(prods, np);

    /* Add items loop */
    printf("\n  Add items (Enter product ID, 0 to finish):\n");
    while (ord.itemCount < MAX_ITEMS) {
        printf("\n  Product ID : ");
        int pid; scanf("%d", &pid); clearBuffer();
        if (pid == 0) break;

        Product prod;
        if (!findProductById(pid, &prod)) {
            printf("  [!] Product not found!\n");
            continue;
        }
        if (prod.stock == 0) {
            printf("  [!] Out of stock!\n");
            continue;
        }
        printf("  Qty (Available: %d) : ", prod.stock);
        int qty; scanf("%d", &qty); clearBuffer();
        if (qty <= 0 || qty > prod.stock) {
            printf("  [!] Invalid quantity!\n");
            continue;
        }

        /* Build item */
        OrderItem item;
        item.productId = prod.id;
        strcpy(item.productName, prod.name);
        item.qty       = qty;
        item.unitPrice = prod.price;
        item.amount    = (float)qty * prod.price;
        ord.items[ord.itemCount++] = item;
        ord.totalAmount += item.amount;

        /* Deduct stock immediately */
        Product all[MAX_PRODUCTS];
        int     total = loadProducts(all);
        for (int i = 0; i < total; i++)
            if (all[i].id == pid) { all[i].stock -= qty; break; }
        saveProducts(all, total);

        printf("  [+] %s x%d  =>  Rs. %.2f\n",
               prod.name, qty, item.amount);
    }

    if (ord.itemCount == 0) {
        printf("  [INFO] No items added. Order not saved.\n");
        pressEnter();
        return;
    }

    /* Save order */
    Order orders[MAX_ORDERS];
    int   total = loadOrders(orders);
    orders[total++] = ord;
    saveOrders(orders, total);

    printLine('-', LINE_W);
    printf("  [SUCCESS] Order Placed!\n");
    printf("  Order ID     : ORD-%04d\n",  ord.id);
    printf("  Customer     : %s\n",         ord.customerName);
    printf("  Total Items  : %d\n",         ord.itemCount);
    printf("  Total Amount : Rs. %.2f\n",   ord.totalAmount);
    printLine('-', LINE_W);
    pressEnter();
}

void listAllOrders(void) {
    Order orders[MAX_ORDERS];
    int   n = loadOrders(orders);
    printHeader("ALL ORDERS");
    printOrderTable(orders, n);
    printFooter();
    pressEnter();
}

void listMyOrders(void) {
    Order all[MAX_ORDERS];
    int   total = loadOrders(all);
    Order mine[MAX_ORDERS];
    int   cnt = 0;
    for (int i = 0; i < total; i++)
        if (all[i].customerId == gCurrentUser.id)
            mine[cnt++] = all[i];

    printHeader("MY ORDERS");
    printOrderTable(mine, cnt);
    printFooter();
    pressEnter();
}

void updateOrderStatus(void) {
    printHeader("UPDATE ORDER STATUS");
    printf("  Order ID : ");
    int id; scanf("%d", &id); clearBuffer();

    Order orders[MAX_ORDERS];
    int   n = loadOrders(orders), idx = -1;
    for (int i = 0; i < n; i++)
        if (orders[i].id == id) { idx = i; break; }

    if (idx == -1) {
        printf("  [ERROR] Order not found!\n");
        pressEnter();
        return;
    }

    printf("  Current Status : %s\n", orderStatusName(orders[idx].status));
    printf("  New Status [1=Pending 2=Confirmed 3=Delivered 4=Cancelled] : ");
    int s; scanf("%d", &s); clearBuffer();
    if (s < 1 || s > 4) {
        printf("  [ERROR] Invalid status!\n");
        pressEnter();
        return;
    }
    orders[idx].status = (OrderStatus)s;
    saveOrders(orders, n);
    printf("  [SUCCESS] Status updated to: %s\n",
           orderStatusName(orders[idx].status));
    pressEnter();
}