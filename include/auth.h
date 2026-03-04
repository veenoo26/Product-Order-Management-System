#ifndef AUTH_H
#define AUTH_H

#include "models.h"

int  doLogin(void);
void doLogout(void);
void changePassword(void);
void registerCustomer(void);
void seedAdmin(void);

#endif /* AUTH_H */