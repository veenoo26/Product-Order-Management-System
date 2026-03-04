#include "auth.h"
#include "user.h"
#include "utils.h"

/* Seed a default admin if no users exist */
void seedAdmin(void) {
    User users[MAX_USERS];
    int  n = loadUsers(users);
    if (n == 0) {
        User a;
        memset(&a, 0, sizeof(User));
        a.id       = 1;
        a.role     = ROLE_ADMIN;
        a.isActive = 1;
        strcpy(a.username, "admin");
        strcpy(a.password, "admin123");
        strcpy(a.fullName, "System_Admin");
        strcpy(a.phone,    "9800000000");
        strcpy(a.email,    "admin@poms.com");
        users[0] = a;
        saveUsers(users, 1);
        printf("\n  [INFO] Default admin created.\n");
        printf("  Username : admin\n");
        printf("  Password : admin123\n");
        pressEnter();
    }
}

/* Login */
int doLogin(void) {
    char uname[NAME_LEN], pass[PASS_LEN];
    printHeader("LOGIN");
    printf("  Username : "); scanf("%49s", uname); clearBuffer();
    printf("  Password : "); scanf("%29s", pass);  clearBuffer();

    User users[MAX_USERS];
    int  n = loadUsers(users);

    for (int i = 0; i < n; i++) {
        if (strcmp(users[i].username, uname) == 0 &&
            strcmp(users[i].password, pass)  == 0 &&
            users[i].isActive == 1)
        {
            gCurrentUser = users[i];
            gIsLoggedIn  = 1;
            printf("\n  [SUCCESS] Welcome, %s! (%s)\n",
                   gCurrentUser.fullName, roleName(gCurrentUser.role));
            pressEnter();
            return 1;
        }
    }
    printf("\n  [FAILED] Invalid credentials!\n");
    pressEnter();
    return 0;
}

/* Logout */
void doLogout(void) {
    printf("\n  Goodbye, %s!\n", gCurrentUser.fullName);
    gIsLoggedIn = 0;
    memset(&gCurrentUser, 0, sizeof(User));
    pressEnter();
}

/* Change own password */
void changePassword(void) {
    char old[PASS_LEN], new1[PASS_LEN], new2[PASS_LEN];
    printHeader("CHANGE PASSWORD");

    printf("  Current Password : "); scanf("%29s", old);  clearBuffer();
    if (strcmp(old, gCurrentUser.password) != 0) {
        printf("  [ERROR] Wrong current password!\n");
        pressEnter();
        return;
    }
    printf("  New Password     : "); scanf("%29s", new1); clearBuffer();
    printf("  Confirm Password : "); scanf("%29s", new2); clearBuffer();

    if (strcmp(new1, new2) != 0) {
        printf("  [ERROR] Passwords do not match!\n");
        pressEnter();
        return;
    }

    User users[MAX_USERS];
    int  n = loadUsers(users);
    for (int i = 0; i < n; i++) {
        if (users[i].id == gCurrentUser.id) {
            strcpy(users[i].password, new1);
            strcpy(gCurrentUser.password, new1);
            break;
        }
    }
    saveUsers(users, n);
    printf("  [SUCCESS] Password updated!\n");
    pressEnter();
}

/* Self-registration for customers */
void registerCustomer(void) {
    User users[MAX_USERS];
    int  n = loadUsers(users);
    User u;
    memset(&u, 0, sizeof(User));
    u.id       = nextUserId();
    u.role     = ROLE_CUSTOMER;
    u.isActive = 1;

    printHeader("CUSTOMER REGISTRATION");
    printf("  Full Name (no spaces) : "); scanf("%49s", u.fullName); clearBuffer();
    printf("  Username              : "); scanf("%49s", u.username); clearBuffer();
    printf("  Password              : "); scanf("%29s", u.password); clearBuffer();
    printf("  Phone                 : "); scanf("%14s", u.phone);    clearBuffer();
    printf("  Email                 : "); scanf("%59s", u.email);    clearBuffer();

    /* Check duplicate username */
    for (int i = 0; i < n; i++) {
        if (strcmp(users[i].username, u.username) == 0) {
            printf("  [ERROR] Username already taken!\n");
            pressEnter();
            return;
        }
    }

    users[n++] = u;
    saveUsers(users, n);
    printf("  [SUCCESS] Registered! You may now login.\n");
    pressEnter();
}