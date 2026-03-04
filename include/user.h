#ifndef USER_H
#define USER_H

#include "models.h"

/* CRUD */
void addUser(void);
void listUsers(void);
void editUser(void);
void removeUser(void);

/* Display */
void printUserTable(User *arr, int n);

/* File I/O */
int  loadUsers(User *arr);
void saveUsers(User *arr, int n);

/* Helpers */
int  nextUserId(void);
int  findUserById(int id, User *out);

#endif /* USER_H */