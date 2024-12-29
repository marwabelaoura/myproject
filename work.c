//part widad
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_ROLE 10
#define MAX_TITLE 100
#define MAX_AUTHOR 100
#define MAX_BOOKS 100

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    float price;
    int quantity;
} Book;

typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char role[MAX_ROLE]; // "Admin" or "Member"
} User;

const char *usersFile = "users.txt";
const char *booksFile = "books.txt";

// Function Prototypes
void registerUser();
int loginUser(User *loggedInUser);
void addBook();
void viewBooks();
void updateBook();
void deleteBook();
void borrowBook(const char *username);

// Helper Functions
int isUniqueUsername(const char *username);
int generateBookID();

// Main Function
int main() {
    User loggedInUser;
    int choice;

    printf("===== Library Management System =====\n");
    printf("1. Register\n2. Login\n3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            registerUser();
            break;
        case 2:
            if (loginUser(&loggedInUser)) {
                if (strcmp(loggedInUser.role, "Admin") == 0) {
                    do {
                        printf("\n===== Admin Menu =====\n");
                        printf("1. Add Book\n2. View Books\n3. Update Book\n4. Delete Book\n5. Logout\n");
                        printf("Enter your choice: ");
                        scanf("%d", &choice);

                        switch (choice) {
                            case 1: addBook(); break;
                            case 2: viewBooks(); break;
                            case 3: updateBook(); break;
                            case 4: deleteBook(); break;
                            case 5: printf("Logging out...\n"); break;
                            default: printf("Invalid choice.\n");
                        }
                    } while (choice != 5);
                } else {
                    do {
                        printf("\n===== Member Menu =====\n");
                        printf("1. View Books\n2. Borrow Book\n3. Logout\n");
                        printf("Enter your choice: ");
                        scanf("%d", &choice);

                        switch (choice) {
                            case 1: viewBooks(); break;
                            case 2: borrowBook(loggedInUser.username); break;
                            case 3: printf("Logging out...\n"); break;
                            default: printf("Invalid choice.\n");
                        }
                    } while (choice != 3);
                }
            }
            break;
        case 3:
            printf("Exiting system...\n");
            exit(0);
        default:
            printf("Invalid choice.\n");
    }

    return 0;
}

void registerUser() {
    User newUser;
    FILE *file = fopen(usersFile, "a");

    if (file == NULL) {
        printf("Error: Could not open users file.\n");
        return;
    }

    printf("Enter username: ");
    scanf("%s", newUser.username);

    if (!isUniqueUsername(newUser.username)) {
        printf("Error: Username already exists.\n");
        fclose(file);
        return;
    }

    printf("Enter password: ");
    scanf("%s", newUser.password);
    printf("Enter role (Admin/Member): ");
    scanf("%s", newUser.role);

    fprintf(file, "%s %s %s\n", newUser.username, newUser.password, newUser.role);
    fclose(file);

    printf("Registration successful.\n");
}

int loginUser(User *loggedInUser) {
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    FILE *file = fopen(usersFile, "r");

    if (file == NULL) {
        printf("Error: Could not open users file.\n");
        return 0;
    }

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    while (fscanf(file, "%s %s %s", loggedInUser->username, loggedInUser->password, loggedInUser->role) != EOF) {
        if (strcmp(loggedInUser->username, username) == 0 && strcmp(loggedInUser->password, password) == 0) {
            printf("Login successful.\n");
            fclose(file);
            return 1;
        }
    }

    printf("Error: Invalid username or password.\n");
    fclose(file);
    return 0;
}

int isUniqueUsername(const char *username) {
    FILE *file = fopen(usersFile, "r");
    User tempUser;

    if (file == NULL) return 1; // No file means no users yet

    while (fscanf(file, "%s %s %s", tempUser.username, tempUser.password, tempUser.role) != EOF) {
        if (strcmp(tempUser.username, username) == 0) {
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}
//Razans part 
void addBook() {
    FILE *file = fopen(booksFile, "a");

    if (file == NULL) {
        printf("Error: Could not open books file.\n");
        return;
    }

    Book newBook;
    newBook.id = generateBookID();

    printf("Enter book title: ");
    scanf(" %[^\n]", newBook.title);
    printf("Enter book author: ");
    scanf(" %[^\n]", newBook.author);
    printf("Enter book price: ");
    scanf("%f", &newBook.price);
    printf("Enter book quantity: ");
    scanf("%d", &newBook.quantity);

    fprintf(file, "%d,%s,%s,%.2f,%d\n", newBook.id, newBook.title, newBook.author, newBook.price, newBook.quantity);
    fclose(file);

    printf("Book added successfully with ID %d.\n", newBook.id);
}

void viewBooks() {
    FILE *file = fopen(booksFile, "r");
    Book book;

    if (file == NULL) {
        printf("Error: Could not open books file.\n");
        return;
    }

    printf("\n===== Available Books =====\n");
    while (fscanf(file, "%d,%[^,],%[^,],%f,%d", &book.id, book.title, book.author, &book.price, &book.quantity) != EOF) {
        printf("ID: %d | Title: %s | Author: %s | Price: %.2f | Quantity: %d\n",
               book.id, book.title, book.author, book.price, book.quantity);
    }

    fclose(file);
}

int generateBookID() {
    FILE *file = fopen(booksFile, "r");
    int id = 0;
    Book book;

    if (file != NULL) {
        while (fscanf(file, "%d,%[^,],%[^,],%f,%d", &book.id, book.title, book.author, &book.price, &book.quantity) != EOF) {
            if (book.id > id) id = book.id;
        }
        fclose(file);
    }

    return id + 1;
}

//marwa s part 

void updateBook() {
    int bookID, found = 0;
    Book book;
    FILE *file = fopen(booksFile, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Error: Could not open books file.\n");
        return;
    }

    printf("Enter the ID of the book to update: ");
    scanf("%d", &bookID);

    while (fscanf(file, "%d,%[^,],%[^,],%f,%d", &book.id, book.title, book.author, &book.price, &book.quantity) != EOF) {
        if (book.id == bookID) {
            found = 1;

            printf("Enter new title (current: %s): ", book.title);
            scanf(" %[^\n]", book.title);

            printf("Enter new author (current: %s): ", book.author);
            scanf(" %[^\n]", book.author);

            printf("Enter new price (current: %.2f): ", book.price);
            scanf("%f", &book.price);

            printf("Enter new quantity (current: %d): ", book.quantity);
            scanf("%d", &book.quantity);
        }

        fprintf(temp, "%d,%s,%s,%.2f,%d\n", book.id, book.title, book.author, book.price, book.quantity);
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(booksFile);
        rename("temp.txt", booksFile);
        printf("Book updated successfully.\n");
    } else {
        remove("temp.txt");
        printf("Error: Book with ID %d not found.\n", bookID);
    }
}

void deleteBook() {
    int bookID, found = 0;
    Book book;
    FILE *file = fopen(booksFile, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Error: Could not open books file.\n");
        return;
    }

    printf("Enter the ID of the book to delete: ");
    scanf("%d", &bookID);

    while (fscanf(file, "%d,%[^,],%[^,],%f,%d", &book.id, book.title, book.author, &book.price, &book.quantity) != EOF) {
        if (book.id == bookID) {
            found = 1;
        } else {
            fprintf(temp, "%d,%s,%s,%.2f,%d\n", book.id, book.title, book.author, book.price, book.quantity);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(booksFile);
        rename("temp.txt", booksFile);
        printf("Book deleted successfully.\n");
    } else {
        remove("temp.txt");
        printf("Error: Book with ID %d not found.\n", bookID);
    }
}

void borrowBook(const char *username) {
    int id, quantity, found = 0;
    const int MAX_BORROW_QUANTITY = 4; // Limite maximale par emprunt
    Book book;
    FILE *file = fopen(booksFile, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Error: Could not open books file.\n");
        return;
    }

    printf("Enter the ID of the book to borrow: ");
    scanf("%d", &id);

    while (fscanf(file, "%d,%[^,],%[^,],%f,%d", &book.id, book.title, book.author, &book.price, &book.quantity) != EOF) {
        if (book.id == id) {
            found = 1;

            printf("Enter the quantity to borrow (max %d, available: %d): ", MAX_BORROW_QUANTITY, book.quantity);
            scanf("%d", &quantity);

            if (quantity > 0 && quantity <= MAX_BORROW_QUANTITY && quantity <= book.quantity) {
                book.quantity -= quantity;
                printf("You have successfully borrowed %d copies of '%s'.\n", quantity, book.title);
            } else if (quantity > MAX_BORROW_QUANTITY) {
                printf("Error: You can only borrow up to %d copies at a time.\n", MAX_BORROW_QUANTITY);
            } else if (quantity > book.quantity) {
                printf("Error: Insufficient stock for '%s'. Only %d available.\n", book.title, book.quantity);
            } else {
                printf("Error: Invalid quantity.\n");
            }
        }

        fprintf(temp, "%d,%s,%s,%.2f,%d\n", book.id, book.title, book.author, book.price, book.quantity);
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(booksFile);
        rename("temp.txt", booksFile);
    } else {
        remove("temp.txt");
        printf("Error: Book with ID %d not found.\n", id);
    }
}

// fin code 
