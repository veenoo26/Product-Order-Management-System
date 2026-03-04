#include "product.h"
#include "utils.h"

/* ============================================================
   FILE I/O
   ============================================================ */
int loadProducts(Product *arr) {
    FILE *fp = fopen(PRODUCTS_FILE, "rb");
    if (!fp) return 0;
    int n = 0;
    while (n < MAX_PRODUCTS && fread(&arr[n], sizeof(Product), 1, fp) == 1)
        n++;
    fclose(fp);
    return n;
}

void saveProducts(Product *arr, int n) {
    FILE *fp = fopen(PRODUCTS_FILE, "wb");
    if (!fp) { printf("  [ERROR] Cannot write products file!\n"); return; }
    fwrite(arr, sizeof(Product), n, fp);
    fclose(fp);
}

int nextProductId(void) {
    Product arr[MAX_PRODUCTS];
    int     n = loadProducts(arr), maxId = 0;
    for (int i = 0; i < n; i++)
        if (arr[i].id > maxId) maxId = arr[i].id;
    return maxId + 1;
}

int findProductById(int id, Product *out) {
    Product arr[MAX_PRODUCTS];
    int     n = loadProducts(arr);
    for (int i = 0; i < n; i++) {
        if (arr[i].id == id && arr[i].isActive) {
            *out = arr[i];
            return 1;
        }
    }
    return 0;
}

/* ============================================================
   DISPLAY
   ============================================================ */
void printProductTable(Product *arr, int n) {
    int shown = 0;
    printLine('-', LINE_W);
    printf("  %-4s %-18s %-13s %10s %6s  %-12s\n",
           "ID", "Name", "Category", "Price(Rs)", "Stock", "Description");
    printLine('-', LINE_W);
    for (int i = 0; i < n; i++) {
        if (!arr[i].isActive) continue;
        printf("  %-4d %-18s %-13s %10.2f %6d  %-12s\n",
               arr[i].id, arr[i].name, arr[i].category,
               arr[i].price, arr[i].stock, arr[i].description);
        shown++;
    }
    if (shown == 0) printf("  No products available.\n");
    printLine('-', LINE_W);
    printf("  Total: %d product(s)\n", shown);
}

/* ============================================================
   CRUD
   ============================================================ */
void addProduct(void) {
    Product prods[MAX_PRODUCTS];
    int     n = loadProducts(prods);
    Product p;
    memset(&p, 0, sizeof(Product));
    p.id       = nextProductId();
    p.isActive = 1;

    printHeader("ADD PRODUCT");
    printf("  Name        : "); scanf("%49s", p.name);        clearBuffer();
    printf("  Category    : "); scanf("%49s", p.category);    clearBuffer();
    printf("  Description : "); scanf("%99s", p.description); clearBuffer();
    printf("  Price (Rs)  : "); scanf("%f",   &p.price);      clearBuffer();
    printf("  Stock       : "); scanf("%d",   &p.stock);      clearBuffer();

    prods[n++] = p;
    saveProducts(prods, n);
    printf("  [SUCCESS] Product added! (ID = %d)\n", p.id);
    pressEnter();
}

void listProducts(void) {
    Product prods[MAX_PRODUCTS];
    int     n = loadProducts(prods);
    printHeader("PRODUCT LIST");
    printProductTable(prods, n);
    printFooter();
    pressEnter();
}

void editProduct(void) {
    printHeader("EDIT PRODUCT");
    printf("  Product ID : ");
    int id; scanf("%d", &id); clearBuffer();

    Product prods[MAX_PRODUCTS];
    int     n = loadProducts(prods), idx = -1;
    for (int i = 0; i < n; i++)
        if (prods[i].id == id && prods[i].isActive) { idx = i; break; }

    if (idx == -1) {
        printf("  [ERROR] Product not found!\n");
        pressEnter();
        return;
    }

    Product *p = &prods[idx];
    printf("  Current -> Name: %s | Price: %.2f | Stock: %d\n",
           p->name, p->price, p->stock);
    printf("  New Name     : "); scanf("%49s", p->name);        clearBuffer();
    printf("  New Category : "); scanf("%49s", p->category);    clearBuffer();
    printf("  New Desc     : "); scanf("%99s", p->description); clearBuffer();
    printf("  New Price    : "); scanf("%f",   &p->price);      clearBuffer();
    printf("  New Stock    : "); scanf("%d",   &p->stock);      clearBuffer();

    saveProducts(prods, n);
    printf("  [SUCCESS] Product updated!\n");
    pressEnter();
}

void removeProduct(void) {
    printHeader("REMOVE PRODUCT");
    printf("  Product ID : ");
    int id; scanf("%d", &id); clearBuffer();

    Product prods[MAX_PRODUCTS];
    int     n = loadProducts(prods);
    for (int i = 0; i < n; i++) {
        if (prods[i].id == id) {
            prods[i].isActive = 0;
            saveProducts(prods, n);
            printf("  [SUCCESS] Product removed!\n");
            pressEnter();
            return;
        }
    }
    printf("  [ERROR] Product not found!\n");
    pressEnter();
}

void searchProduct(void) {
    printHeader("SEARCH PRODUCT");
    printf("  Keyword : ");
    char kw[NAME_LEN];
    scanf("%49s", kw); clearBuffer();

    Product prods[MAX_PRODUCTS], res[MAX_PRODUCTS];
    int     n = loadProducts(prods), cnt = 0;
    for (int i = 0; i < n; i++) {
        if (prods[i].isActive &&
            (strstr(prods[i].name, kw)     != NULL ||
             strstr(prods[i].category, kw) != NULL))
            res[cnt++] = prods[i];
    }

    printf("\n  Results for '%s':\n", kw);
    printProductTable(res, cnt);
    printFooter();
    pressEnter();
}