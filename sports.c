#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>

// Structure for a product
struct Product {
    char title[100];
    float price;
    char description[200];
    struct Product* next;
};

// Structure for a category
struct Category {
    char name[50];
    struct Product* products;
};

// Function to add a product to a category
void addProduct(struct Category* category, const char* title, float price, const char* description) {
    struct Product* newProduct = (struct Product*)malloc(sizeof(struct Product));
    if (newProduct == NULL) {
        printf("Memory allocation failed. Product not added.\n");
        return;
    }

    strncpy(newProduct->title, title, sizeof(newProduct->title) - 1);
    newProduct->title[sizeof(newProduct->title) - 1] = '\0'; // Ensure null-termination
    newProduct->price = price;
    strncpy(newProduct->description, description, sizeof(newProduct->description) - 1);
    newProduct->description[sizeof(newProduct->description) - 1] = '\0'; // Ensure null-termination
    newProduct->next = category->products;
    category->products = newProduct;

    printf("Product added to %s category.\n", category->name);
}

// Function to remove a product from a category
void removeProduct(struct Category* category, const char* title) {
    struct Product* current = category->products;
    struct Product* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            if (previous == NULL) {
                category->products = current->next;
            } else {
                previous->next = current->next;
            }

            free(current);
            printf("Product '%s' removed from %s category.\n", title, category->name);
            return;
        }

        previous = current;
        current = current->next;
    }

    printf("Product '%s' not found in %s category.\n", title, category->name);
}

// Function to update a product's information
void updateProduct(struct Category* category, const char* title, float price, const char* description) {
    struct Product* current = category->products;

    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            current->price = price;
            strncpy(current->description, description, sizeof(current->description) - 1);
            current->description[sizeof(current->description) - 1] = '\0'; // Ensure null-termination
            printf("Product '%s' updated in %s category.\n", title, category->name);
            return;
        }

        current = current->next;
    }

    printf("Product '%s' not found in %s category.\n", title, category->name);
}

// Structure to store matching products
struct MatchingProduct {
    struct Product* product;
    struct MatchingProduct* next;
};

// Function to search for products by title or description substring
void searchProduct(const struct Category* category, const char* searchQuery) {
    struct Product* current = category->products;
    struct MatchingProduct* matchingProducts = NULL;
    struct MatchingProduct* lastMatchingProduct = NULL;

    while (current != NULL) {
        if (strstr(current->title, searchQuery) != NULL || strstr(current->description, searchQuery) != NULL) {
            struct MatchingProduct* matchingProduct = (struct MatchingProduct*)malloc(sizeof(struct MatchingProduct));
            if (matchingProduct == NULL) {
                printf("Memory allocation failed. Search results may be incomplete.\n");
                break;
            }
            matchingProduct->product = current;
            matchingProduct->next = NULL;

            if (matchingProducts == NULL) {
                matchingProducts = matchingProduct;
            } else {
                lastMatchingProduct->next = matchingProduct;
            }
            lastMatchingProduct = matchingProduct;
        }

        current = current->next;
    }

    if (matchingProducts == NULL) {
        printf("No products matching '%s' found in %s category.\n", searchQuery, category->name);
    } else {
        printf("Matching Products in %s category:\n", category->name);
        struct MatchingProduct* matchingProduct = matchingProducts;
        int resultCount = 0;

        while (matchingProduct != NULL) {
            printf("Result %d:\n", ++resultCount);
            printf("Title: %s\n", matchingProduct->product->title);
            printf("Price: $%.2f\n", matchingProduct->product->price);
            printf("Description: %s\n", matchingProduct->product->description);
            printf("\n");

            struct MatchingProduct* temp = matchingProduct;
            matchingProduct = matchingProduct->next;
            free(temp);
        }
    }
}




int main() {
    struct Category categories[100];
    int categoryCount = 0;

    while (1) {
        printf("\nSports Shop Inventory System\n");
        printf("1. Add Category\n");
        printf("2. Add Product to Category\n");
        printf("3. Remove Product from Category\n");
        printf("4. Update Product Information\n");
        printf("5. Search for Product\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        if (choice == 6) {
            break;
        }

        if (choice == 1) {
            if (categoryCount < 100) {
                printf("Enter category name: ");
                char categoryName[50];
                getchar(); 
                fgets(categoryName, sizeof(categoryName), stdin);
                categoryName[strcspn(categoryName, "\n")] = '\0'; 

                strncpy(categories[categoryCount].name, categoryName, sizeof(categories[categoryCount].name) - 1);
                categories[categoryCount].name[sizeof(categories[categoryCount].name) - 1] = '\0'; // Ensure null-termination
                categories[categoryCount].products = NULL; // Initialize the linked list
                categoryCount++;
                printf("Category added.\n");
            } else {
                printf("Maximum category limit reached. Cannot add more categories.\n");
            }
        } else if (choice >= 2 && choice <= 5) {
            char categoryName[50];
            printf("Enter category name: ");
            getchar(); 
            fgets(categoryName, sizeof(categoryName), stdin);
            categoryName[strcspn(categoryName, "\n")] = '\0'; 

            struct Category* selectedCategory = NULL;

            for (int i = 0; i < categoryCount; i++) {
                if (strcmp(categories[i].name, categoryName) == 0) {
                    selectedCategory = &categories[i];
                    break;
                }
            }

            if (selectedCategory == NULL) {
                printf("Category '%s' not found.\n", categoryName);
                continue;
            }

            if (choice == 2) {
                char title[100];
                float price;
                char description[200];

                printf("Enter product title: ");
                /* getchar();  */
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0'; 
                printf("%s\n",title);
                printf("Enter product price(dollars): ");
                scanf("%f", &price);
                printf("Enter product description: ");
                getchar(); 
                fgets(description, sizeof(description), stdin);
                description[strcspn(description, "\n")] = '\0'; 

                addProduct(selectedCategory, title, price, description);
            } else if (choice == 3) {
                char title[100];
                printf("Enter product title to remove: ");
                getchar(); 
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0'; 
                removeProduct(selectedCategory, title);
            } else if (choice == 4) {
                char title[100];
                float price;
                char description[200];

                printf("Enter product title to update: ");
                getchar(); 
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0'; 
                printf("Enter new product price(dollars): ");
                scanf("%f", &price);
                printf("Enter new product description: ");
                getchar(); 
                fgets(description, sizeof(description), stdin);
                description[strcspn(description, "\n")] = '\0'; 

                updateProduct(selectedCategory, title, price, description);
            } else if (choice == 5) {
                char title[100];
                printf("Enter product title to search: ");
                getchar(); 
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0'; 
                searchProduct(selectedCategory, title);
            }
        } else {
            printf("Invalid choice. Please enter a valid option.\n");
        }
    }

    // Free dynamically allocated memory for products
    for (int i = 0; i < categoryCount; i++) {
        struct Product* current = categories[i].products;
        while (current != NULL) {
            struct Product* next = current->next;
            free(current);
            current = next;
        }
    }

    return 0;
}

