#include "user.h"
#include "utils.h"

/* ============================================================
   FILE I/O
   ============================================================ */
int loadUsers(User *arr) {
    FILE *fp = fopen(USERS_FILE, "rb");
    if (!fp) return 0;
    int n = 0;
    while (n < MAX_USERS && fread(&arr[n], sizeof(User), 1, fp) == 1)
        n++;
    fclose(fp);
    return n;
}

void saveUsers(User *arr, int n) {
    FILE *fp = fopen(USERS_FILE, "wb");
    if (!fp) { printf("  [ERROR] Cannot write users file!\n"); return; }
    fwrite(arr, sizeof(User), n, fp);
    fclose(fp);
}

int nextUserId(void) {
    User arr[MAX_USERS];
    int  n = loadUsers(arr), maxId = 0;
    for (int i = 0; i < n; i++)
        if (arr[i].id > maxId) maxId = arr[i].id;
    return maxId + 1;
}

int findUserById(int id, User *out) {
    User arr[MAX_USERS];
    int  n = loadUsers(arr);
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
void printUserTable(User *arr, int n) {
    int shown = 0;
    printLine('-', LINE_W);
    printf("  %-4s %-15s %-20s %-12s %-10s %-8s\n",
           "ID", "Username", "Full Name", "Phone", "Role", "Status");
    printLine('-', LINE_W);
    for (int i = 0; i < n; i++) {
        printf("  %-4d %-15s %-20s %-12s %-10s %-8s\n",
               arr[i].id,
               arr[i].username,
               arr[i].fullName,
               arr[i].phone,
               roleName(arr[i].role),
               arr[i].isActive ? "Active" : "Inactive");
        shown++;
    }
    printLine('-', LINE_W);
    printf("  Total: %d record(s)\n", shown);
}

/* ============================================================
   CRUD
   ============================================================ */
void addUser(void) {
    User users[MAX_USERS];
    int  n = loadUsers(users);
    User u;
    memset(&u, 0, sizeof(User));
    u.id       = nextUserId();
    u.isActive = 1;

    printHeader("ADD USER");
    printf("  Full Name (no spaces) : "); scanf("%49s", u.fullName); clearBuffer();
    printf("  Username              : "); scanf("%49s", u.username); clearBuffer();
    printf("  Password              : "); scanf("%29s", u.password); clearBuffer();
    printf("  Phone                 : "); scanf("%14s", u.phone);    clearBuffer();
    printf("  Email                 : "); scanf("%59s", u.email);    clearBuffer();
    printf("  Role [1=Admin 2=Staff 3=Customer] : ");
    int r; scanf("%d", &r); clearBuffer();
    u.role = (r >= 1 && r <= 3) ? (UserRole)r : ROLE_CUSTOMER;

    /* Duplicate check */
    for (int i = 0; i < n; i++) {
        if (strcmp(users[i].username, u.username) == 0) {
            printf("  [ERROR] Username already exists!\n");
            pressEnter();
            return;
        }
    }

    users[n++] = u;
    saveUsers(users, n);
    printf("  [SUCCESS] User added! (ID = %d)\n", u.id);
    pressEnter();
}

void listUsers(void) {
    User users[MAX_USERS];
    int  n = loadUsers(users);
    printHeader("ALL USERS");
    printUserTable(users, n);
    printFooter();
    pressEnter();
}

void editUser(void) {
    printHeader("EDIT USER");
    printf("  User ID : ");
    int id; scanf("%d", &id); clearBuffer();

    User users[MAX_USERS];
    int  n = loadUsers(users), idx = -1;
    for (int i = 0; i < n; i++)
        if (users[i].id == id) { idx = i; break; }

    if (idx == -1) {
        printf("  [ERROR] User not found!\n");
        pressEnter();
        return;
    }

    User *u = &users[idx];
    printf("  Current Name  : %s\n", u->fullName);
    printf("  New Full Name : "); scanf("%49s", u->fullName); clearBuffer();
    printf("  New Phone     : "); scanf("%14s", u->phone);    clearBuffer();
    printf("  New Email     : "); scanf("%59s", u->email);    clearBuffer();
    printf("  Status [1=Active  0=Inactive] : ");
    scanf("%d", &u->isActive); clearBuffer();

    saveUsers(users, n);
    printf("  [SUCCESS] User updated!\n");
    pressEnter();
}

void removeUser(void) {
    printHeader("DEACTIVATE USER");
    printf("  User ID : ");
    int id; scanf("%d", &id); clearBuffer();

    if (id == gCurrentUser.id) {
        printf("  [ERROR] Cannot deactivate yourself!\n");
        pressEnter();
        return;
    }

    User users[MAX_USERS];
    int  n = loadUsers(users);
    for (int i = 0; i < n; i++) {
        if (users[i].id == id) {
            users[i].isActive = 0;
            saveUsers(users, n);
            printf("  [SUCCESS] User deactivated!\n");
            pressEnter();
            return;
        }
    }
    printf("  [ERROR] User not found!\n");
    pressEnter();
}