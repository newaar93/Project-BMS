//book management system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum number of books
#define MAX_BOOKS 100
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "password"

// Define the structure for a book
typedef struct {
    int id;
    char title[100];
    char author[100];
    int year;
    char isbn[20];
    char edition[50];
} Book;

// Declare the book array and count
Book books[MAX_BOOKS];
int bookCount = 0;

// Function prototypes
void displayHomePage();
void displayAdminMenu();
void displayUserMenu();
void addBook();
void viewBooks();
void updateBook();
void deleteBook();
int findBookById(int id);
int adminLogin();
void userViewBooks();
void adminProcess();

int main() {
    while (1) {
        displayHomePage();
    }
    return 0;
}

void displayHomePage() {
    int roleChoice;
    printf("                     Book Store  System\n");
	printf("??==============================================================??\n");
	printf("||                      Home Page                               ||\n");
    printf("||                                                              ||\n");
	printf("||                  Your role:                                  ||\n");
    printf("||                                                              ||\n");
    printf("||                  1.Admin                                     ||\n");
    printf("||                  2.User                                      ||\n");
	printf("||                  3.Exit                                      ||\n");
	printf("||                                                              ||\n");
	printf("||                  Enter your choice:                          ||\n");
    printf("||                                                              ||\n");
	printf("??==============================================================??\n\n");
    scanf("%d", &roleChoice);
    if (roleChoice == 1) {
        if (adminLogin() == 1) {
            adminProcess();
        } else {
            printf("Invalid username or password.\n");
        }
    } else if (roleChoice == 2) {
        userViewBooks();
    } else if (roleChoice == 3) {
        printf("Exiting...\n");
        exit(0);
    } else {
        printf("Invalid choice. Please try again.\n");
    }
}

void displayAdminMenu() {
	printf("!!==================================================================!!\n");
	printf("||                      Admin Menu                                  ||\n");
    printf("||                                                                  ||\n");
	printf("||                      1- Add Book                                 ||\n");
	printf("||                      2- View Books                               ||\n");
    printf("||                      3- Update Books                             ||\n");
    printf("||                      4- Delete Books                             ||\n");
	printf("||                      5- Logout                                   ||\n");
    printf("||                                                                  ||\n");
	printf("||              Choose a number according to your requirement:      ||\n");
	printf("!!==================================================================!!\n");	
	}

void displayUserMenu() {
    printf("\nUser Menu\n");
    printf("1. View Books\n");
    printf("2. Logout\n");
}

int adminLogin() {
    char username[50];
    char password[50];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        return 1; // Login successful
    } else {
        return 0; // Login failed
    }
}

void adminProcess() {
    int choice;
    do {
        displayAdminMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                viewBooks();
                break;
            case 3:
                updateBook();
                break;
            case 4:
                deleteBook();
                break;
            case 5:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}

void addBook() {
    if (bookCount < MAX_BOOKS) {
        Book newBook;
        printf("Enter book ID: ");
        scanf("%d", &newBook.id);
        printf("Enter book title: ");
        getchar(); // to consume the newline character left by scanf
        fgets(newBook.title, sizeof(newBook.title),  stdin);
        newBook.title[strcspn(newBook.title, "\n")] = 0; 
        printf("Enter book author: ");
        fgets(newBook.author, sizeof(newBook.author), stdin);
        newBook.author[strcspn(newBook.author, "\n")] = 0; 
        printf("Enter publication year: ");
        scanf("%d", &newBook.year);
        printf("Enter ISBN: ");
        getchar(); // to consume the newline character left by scanf
        fgets(newBook.isbn, sizeof(newBook.isbn), stdin);
        newBook.isbn[strcspn(newBook.isbn, "\n")] = 0; 
        printf("Enter edition: ");
        fgets(newBook.edition, sizeof(newBook.edition), stdin);
        newBook.edition[strcspn(newBook.edition, "\n")] = 0; 

        books[bookCount] = newBook;
        bookCount++;
        printf("Book added successfully.\n");
    } else {
        printf("Book storage is full. Cannot add more books.\n");
    }
}

void viewBooks() {
    if (bookCount == 0) {
        printf("No books available.\n");
    } else {
        printf("Book List:\n");
        int i = 0;
        for ( i=0;i < bookCount; i++) {
            printf("ID: %d, Title: %s, Author: %s, Year: %d, ISBN: %s, Edition: %s\n",
                   books[i].id, books[i].title, books[i].author, books[i].year, books[i].isbn, books[i].edition);
        }
    }
}

void updateBook() {
    int id;
    printf("Enter book ID to update: ");
    scanf("%d", &id);

    int index = findBookById(id);
    if (index != -1) {
        printf("Enter new title (leave blank to keep current): ");
        getchar(); // to consume the newline character left by scanf
        char newTitle[100];
        fgets(newTitle, sizeof(newTitle), stdin);
        newTitle[strcspn(newTitle, "\n")] = 0; 
        if (strlen(newTitle) > 0) {
            strcpy(books[index].title, newTitle);
        }

        printf("Enter new author (leave blank to keep current): ");
        char newAuthor[100];
        fgets(newAuthor, sizeof(newAuthor), stdin);
        newAuthor[strcspn(newAuthor, "\n")] = 0; 
        if (strlen(newAuthor) > 0) {
            strcpy(books[index].author, newAuthor);
        }

        printf("Enter new publication year (0 to keep current): ");
        int newYear;
        scanf("%d", &newYear);
        if (newYear != 0) {
            books[index].year = newYear;
        }

        printf("Enter new ISBN (leave blank to keep current): ");
        getchar(); // to consume the newline character left by scanf
        char newISBN[20];
        fgets(newISBN, sizeof(newISBN), stdin);
        newISBN[strcspn(newISBN, "\n")] = 0; 
        if (strlen(newISBN) > 0) {
            strcpy(books[index].isbn, newISBN);
        }

        printf("Enter new edition (leave blank to keep current): ");
        char newEdition[50];
        fgets(newEdition, sizeof(newEdition), stdin);
        newEdition[strcspn(newEdition, "\n")] = 0; 
        if (strlen(newEdition) > 0) {
            strcpy(books[index].edition, newEdition);
        }

        printf("Book updated successfully.\n");
    } else {
        printf("Book not found.\n");
    }
}

void deleteBook() {
    int id;
    printf("Enter book ID to delete: ");
    scanf("%d", &id);

    int index = findBookById(id);
    if (index != -1) {
    	int i = index;
        for ( i=index;i < bookCount - 1; i++) 
		{
            books[i] = books[i + 1];
        }
        bookCount--;
        printf("Book deleted successfully.\n");
    } else {
        printf("Book not found.\n");
    }
}

int findBookById(int id) {
	int i = 0; 
    for (i=0;i < bookCount; i++) {
        if (books[i].id == id) {
            return i;
        }
    }
    return -1;
}

void userViewBooks() {
    int choice;
    do {
        displayUserMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                viewBooks();
                break;
            case 2:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}

