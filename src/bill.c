#include "bill.h"
#include "order.h"
#include "user.h"
#include "utils.h"

/* ============================================================
   FILE I/O
   ============================================================ */
int loadBills(Bill *arr) {
    FILE *fp = fopen(BILLS_FILE, "rb");
    if (!fp) return 0;
    int n = 0;
    while (n < MAX_BILLS && fread(&arr[n], sizeof(Bill), 1, fp) == 1)
        n++;
    fclose(fp);
    return n;
}

void saveBills(Bill *arr, int n) {
    FILE *fp = fopen(BILLS_FILE, "wb");
    if (!fp) { printf("  [ERROR] Cannot write bills file!\n"); return; }
    fwrite(arr, sizeof(Bill), n, fp);
    fclose(fp);
}

int nextBillId(void) {
    Bill arr[MAX_BILLS];
    int  n = loadBills(arr), maxId = 0;
    for (int i = 0; i < n; i++)
        if (arr[i].id > maxId) maxId = arr[i].id;
    return maxId + 1;
}

/* ============================================================
   BILL PRINTER
   ============================================================ */
void showBill(Bill *b) {
    printf("\n");
    printLine('=', LINE_W);
    printf("          PRODUCT ORDER MANAGEMENT SYSTEM\n");
    printf("               IOE Purbanchal Campus\n");
    printLine('=', LINE_W);
    printf("  Bill No  : BILL-%04d\n",       b->id);
    printf("  Date     : %s\n",              b->billDate);
    printf("  Order No : ORD-%04d\n",        b->orderId);
    printf("  Customer : %-20s  Phone: %s\n",
           b->customerName, b->customerPhone);
    printLine('-', LINE_W);
    printf("  %-3s %-22s %5s %11s %11s\n",
           "SN", "Product Name", "Qty", "Unit(Rs)", "Total(Rs)");
    printLine('-', LINE_W);
    for (int i = 0; i < b->itemCount; i++) {
        printf("  %-3d %-22s %5d %11.2f %11.2f\n",
               i + 1,
               b->items[i].productName,
               b->items[i].qty,
               b->items[i].unitPrice,
               b->items[i].amount);
    }
    printLine('-', LINE_W);
    printf("  %-42s Rs. %7.2f\n", "Sub Total  :", b->subTotal);
    printf("  %-42s Rs. %7.2f\n", "Discount   :", b->discount);
    printf("  %-42s Rs. %7.2f\n", "VAT (13%)  :", b->tax);
    printLine('-', LINE_W);
    printf("  %-42s Rs. %7.2f\n", "GRAND TOTAL:", b->grandTotal);
    printLine('-', LINE_W);
    printf("  Payment : %s\n", payStatusName(b->payStatus));
    printLine('=', LINE_W);
    printf("           Thank you for your purchase!\n");
    printLine('=', LINE_W);
}

/* ============================================================
   OPERATIONS
   ============================================================ */
void generateBill(void) {
    printHeader("GENERATE BILL");
    printf("  Order ID : ");
    int oid; scanf("%d", &oid); clearBuffer();

    Order ord;
    if (!findOrderById(oid, &ord)) {
        printf("  [ERROR] Order not found!\n");
        pressEnter();
        return;
    }
    if (ord.status == ORD_CANCELLED) {
        printf("  [ERROR] Cannot bill a cancelled order!\n");
        pressEnter();
        return;
    }

    /* Prevent duplicate bill */
    Bill bills[MAX_BILLS];
    int  nb = loadBills(bills);
    for (int i = 0; i < nb; i++) {
        if (bills[i].orderId == oid) {
            printf("  [INFO] Bill already exists: BILL-%04d\n", bills[i].id);
            pressEnter();
            return;
        }
    }

    /* Build bill from order */
    Bill b;
    memset(&b, 0, sizeof(Bill));
    b.id         = nextBillId();
    b.orderId    = ord.id;
    b.customerId = ord.customerId;
    strcpy(b.customerName, ord.customerName);
    b.itemCount  = ord.itemCount;
    for (int i = 0; i < ord.itemCount; i++)
        b.items[i] = ord.items[i];
    getDate(b.billDate);
    b.subTotal = ord.totalAmount;

    /* Get customer phone */
    User cust;
    if (findUserById(ord.customerId, &cust))
        strcpy(b.customerPhone, cust.phone);
    else
        strcpy(b.customerPhone, "N/A");

    /* Discount */
    printf("  Discount Amount (Rs) [0 for none] : ");
    scanf("%f", &b.discount); clearBuffer();
    if (b.discount < 0 || b.discount > b.subTotal) b.discount = 0;

    /* Tax = 13% on discounted amount */
    b.tax       = (b.subTotal - b.discount) * 0.13f;
    b.grandTotal = (b.subTotal - b.discount) + b.tax;

    /* Payment status */
    printf("  Payment [1=Unpaid  2=Paid] : ");
    int ps; scanf("%d", &ps); clearBuffer();
    b.payStatus = (ps == 2) ? PAY_PAID : PAY_UNPAID;

    /* Save bill */
    bills[nb++] = b;
    saveBills(bills, nb);

    /* Update order to Confirmed */
    Order allOrds[MAX_ORDERS];
    int   no = loadOrders(allOrds);
    for (int i = 0; i < no; i++)
        if (allOrds[i].id == oid) { allOrds[i].status = ORD_CONFIRMED; break; }
    saveOrders(allOrds, no);

    showBill(&b);
    printf("  [SUCCESS] Bill generated: BILL-%04d\n", b.id);
    pressEnter();
}

void listAllBills(void) {
    Bill bills[MAX_BILLS];
    int  n = loadBills(bills);
    printHeader("ALL BILLS");
    if (n == 0) {
        printf("  No bills found.\n");
    } else {
        printLine('-', LINE_W);
        printf("  %-9s %-8s %-18s %11s %-10s %s\n",
               "Bill-ID", "Ord-ID", "Customer", "Total(Rs)", "Payment", "Date");
        printLine('-', LINE_W);
        for (int i = 0; i < n; i++) {
            printf("  BILL-%-4d ORD-%-4d %-18s %11.2f %-10s %s\n",
                   bills[i].id, bills[i].orderId,
                   bills[i].customerName, bills[i].grandTotal,
                   payStatusName(bills[i].payStatus),
                   bills[i].billDate);
        }
        printLine('-', LINE_W);
        printf("  Total: %d bill(s)\n", n);
    }
    printFooter();
    pressEnter();
}

void listMyBills(void) {
    Bill all[MAX_BILLS];
    int  total = loadBills(all);
    printHeader("MY BILLS");
    printLine('-', LINE_W);
    printf("  %-9s %-8s %11s %-10s %s\n",
           "Bill-ID", "Ord-ID", "Total(Rs)", "Payment", "Date");
    printLine('-', LINE_W);
    int found = 0;
    for (int i = 0; i < total; i++) {
        if (all[i].customerId == gCurrentUser.id) {
            printf("  BILL-%-4d ORD-%-4d %11.2f %-10s %s\n",
                   all[i].id, all[i].orderId,
                   all[i].grandTotal,
                   payStatusName(all[i].payStatus),
                   all[i].billDate);
            found++;
        }
    }
    if (!found) printf("  No bills found for your account.\n");
    printLine('-', LINE_W);
    printFooter();
    pressEnter();
}

void printBillById(void) {
    printHeader("PRINT BILL");
    printf("  Bill ID : ");
    int bid; scanf("%d", &bid); clearBuffer();

    Bill bills[MAX_BILLS];
    int  n = loadBills(bills);
    for (int i = 0; i < n; i++) {
        if (bills[i].id == bid) {
            showBill(&bills[i]);
            pressEnter();
            return;
        }
    }
    printf("  [ERROR] Bill not found!\n");
    pressEnter();
}