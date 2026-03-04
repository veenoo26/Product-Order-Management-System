#ifndef PRODUCT_H
#define PRODUCT_H

#include "models.h"

/* CRUD */
void addProduct(void);
void listProducts(void);
void editProduct(void);
void removeProduct(void);
void searchProduct(void);

/* Display */
void printProductTable(Product *arr, int n);

/* File I/O */
int  loadProducts(Product *arr);
void saveProducts(Product *arr, int n);

/* Helpers */
int  nextProductId(void);
int  findProductById(int id, Product *out);

#endif /* PRODUCT_H */