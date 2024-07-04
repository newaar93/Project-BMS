#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // For _getch() to mask password input

#define MAX_BOOKS 100
#define MAX_USERS 50
#define MAX_BOOKS_PER_USER 5 // Define MAX_BOOKS_PER_USER as per your requirement
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "password"
#define BOOK_FILE "notes.dat"
#define TEXT_FILE "save/bookdata.txt"
#define USER_FILE "save/users.txt"

typedef struct {
    int id;
    char title[100];
    char author[100];
    int year;
    char isbn[20];
    char edition[50];
    int takenByUserId; // Added to track which user has borrowed the book (-1 if available)
} Book;

typedef struct {
    char username[50];
    char password[50];
    int borrowedBooks[MAX_BOOKS_PER_USER]; // Array to store IDs of borrowed books
    int numBorrowedBooks; // Number of books currently borrowed by the user
} User;

Book books[MAX_BOOKS];
int bookCount = 0;
User users[MAX_USERS];
int userCount = 0;

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
void adminProcess();
void loadBooksFromFile();
void saveBooksToFile();
void saveBooksToTextFile();
void loadUsersFromFile();
void saveUsersToFile();
void createUser();
void getPassword(char *password);
int findUserByUsername(const char *username);
void userProcess(const char *username);
void borrowBook(const char *username);
void returnBook(const char *username);
void viewUserBooks(const char *username);

int main() {
    loadBooksFromFile();
    loadUsersFromFile();

    while (1) {
        displayHomePage();
    }
    return 0;
}

void displayHomePage() {
    int roleChoice;
    printf("                Book Store  System\n");
    printf("||===============================================================||\n");
    printf("||                Home Page                                      ||\n");
    printf("||              Your role:                                       ||\n");
    printf("||         1.Admin                                               ||\n");
    printf("||         2.User                                                ||\n");
    printf("||         3.Exit                                                ||\n");
    printf("||         Enter your choice:                                    ||\n");
    printf("||===============================================================||\n");
    scanf("%d", &roleChoice);
    if (roleChoice == 1) {
        if (adminLogin() == 1) {
            adminProcess();
        } else {
            printf("Invalid username or password.\n");
        }
    } else if (roleChoice == 2) {
        char username[50];
        printf("Enter username: ");
        scanf("%s", username);
        int userIndex = findUserByUsername(username);
        if (userIndex != -1) {
            userProcess(username);
        } else {
            printf("User not found.\n");
        }
    } else if (roleChoice == 3) {
        saveBooksToFile();
        saveBooksToTextFile();
        saveUsersToFile();
        printf("Exiting...\n");
        exit(0);
    } else {
        printf("Invalid choice. Please try again.\n");
    }
}

void displayAdminMenu() {
    printf("\n");
    printf("!!================================================================!!\n");
    printf("||             Admin Menu                                        ||\n");
    printf("||             1- Add Book                                       ||\n");
    printf("||             2- View Books                                     ||\n");
    printf("||             3- Update Books                                   ||\n");
    printf("||             4- Delete Books                                   ||\n");
    printf("||             5- Create User                                    ||\n");
    printf("||             6- Logout                                         ||\n");
    printf("||      Choose a number according to your requirement            ||\n");
    printf("!!================================================================!!\n");
}

void displayUserMenu() {
    printf("\n");
    printf("!!================================================================!!\n");
    printf("||             User Menu                                          ||\n");
    printf("||             1- View Books                                      ||\n");
    printf("||             2- Borrow a Book                                   ||\n");
    printf("||             3- Return a Book                                   ||\n");
    printf("||             4- View Borrowed Books                             ||\n");
    printf("||             5- Logout                                          ||\n");
    printf("||      Choose a number according to your requirement             ||\n");
    printf("!!================================================================!!\n");
}

void addBook() {
    if (bookCount < MAX_BOOKS) {
        Book newBook;
        printf("Enter book ID: ");
        scanf("%d", &newBook.id);
        printf("Enter book title: ");
        getchar(); // to consume the newline character left by scanf
        fgets(newBook.title, sizeof(newBook.title), stdin);
        newBook.title[strcspn(newBook.title, "\n")] = 0; // Remove trailing newline
        printf("Enter book author: ");
        fgets(newBook.author, sizeof(newBook.author), stdin);
        newBook.author[strcspn(newBook.author, "\n")] = 0; // Remove trailing newline
        printf("Enter publication year: ");
        scanf("%d", &newBook.year);
        printf("Enter ISBN: ");
        getchar(); // to consume the newline character left by scanf
        fgets(newBook.isbn, sizeof(newBook.isbn), stdin);
        newBook.isbn[strcspn(newBook.isbn, "\n")] = 0; // Remove trailing newline
        printf("Enter edition: ");
        fgets(newBook.edition, sizeof(newBook.edition), stdin);
        newBook.edition[strcspn(newBook.edition, "\n")] = 0; // Remove trailing newline

        newBook.takenByUserId = -1; // Initially not borrowed by any user

        books[bookCount] = newBook;
        bookCount++;
        printf("Book added successfully.\n");
        saveBooksToFile();
        saveBooksToTextFile();
    } else {
        printf("Book storage is full. Cannot add more books.\n");
    }
}

void viewBooks() {
    if (bookCount == 0) {
        printf("No books available.\n");
    } else {
        printf("Book List:\n");
        for (int i = 0; i < bookCount; i++) {
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
        newTitle[strcspn(newTitle, "\n")] = 0; // Remove trailing newline
        if (strlen(newTitle) > 0) {
            strcpy(books[index].title, newTitle);
        }

        printf("Enter new author (leave blank to keep current): ");
        char newAuthor[100];
        fgets(newAuthor, sizeof(newAuthor), stdin);
        newAuthor[strcspn(newAuthor, "\n")] = 0; // Remove trailing newline
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
        newISBN[strcspn(newISBN, "\n")] = 0; // Remove trailing newline
        if (strlen(newISBN) > 0) {
            strcpy(books[index].isbn, newISBN);
        }

        printf("Enter new edition (leave blank to keep current): ");
        char newEdition[50];
        fgets(newEdition, sizeof(newEdition), stdin);
        newEdition[strcspn(newEdition, "\n")] = 0; // Remove trailing newline
        if (strlen(newEdition) > 0) {
            strcpy(books[index].edition, newEdition);
        }

        printf("Book updated successfully.\n");
        saveBooksToFile();
        saveBooksToTextFile();
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
        for (int i = index; i < bookCount - 1; i++) {
            books[i] = books[i + 1];
        }
        bookCount--;
        printf("Book deleted successfully.\n");
        saveBooksToFile();
        saveBooksToTextFile();
    } else {
        printf("Book not found.\n");
    }
}

int findBookById(int id) {
    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            return i;
        }
    }
    return -1;
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
                createUser();
                break;
            case 6:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}

void createUser() {
    if (userCount < MAX_USERS) {
        User newUser;
        printf("Enter new username: ");
        scanf("%s", newUser.username);
        printf("Enter new password: ");
        getPassword(newUser.password); // Masked input for password
        // Check if the username already exists
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, newUser.username) == 0) {
                printf("\nUsername already exists. Please choose a different username.\n");
                return;
            }
        }
        // Add the new user
        strcpy(users[userCount].username, newUser.username);
        strcpy(users[userCount].password, newUser.password);
        users[userCount].numBorrowedBooks = 0; // Initialize number of borrowed books
        userCount++;
        printf("\nUser account created successfully.\n");
        saveUsersToFile();
    } else {
        printf("\nUser storage is full. Cannot add more users.\n");
    }
}

void getPassword(char *password) {
    char ch;
    int index = 0;

    while (1) {
        ch = _getch();
        if (ch == 13) // Enter pressed
            break;
        if (ch == '\b') // Backspace pressed
        {
            if (index > 0) {
                index--;
                printf("\b \b");
            }
            continue;
        }
        password[index++] = ch;
        printf("*");
    }
    password[index] = '\0';
}

void loadBooksFromFile() {
    FILE *fp = fopen(BOOK_FILE, "rb");
    if (fp == NULL) {
        printf("No existing data file found. Starting with an empty book list.\n");
        return;
    }

    bookCount = 0;
    while (fread(&books[bookCount], sizeof(Book), 1, fp) == 1) {
        bookCount++;
    }
    fclose(fp);
}

void saveBooksToFile() {
    FILE *fp = fopen(BOOK_FILE, "wb");
    if (fp == NULL) {
        perror("Error opening file for writing");
        return;
    }

    fwrite(books, sizeof(Book), bookCount, fp);
    fclose(fp);
}

void saveBooksToTextFile() {
    FILE *fp = fopen(TEXT_FILE, "w");
    if (fp == NULL) {
        perror("Error opening text file for writing");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        fprintf(fp, "ID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nISBN: %s\nEdition: %s\n\n",
                books[i].id, books[i].title, books[i].author, books[i].year, books[i].isbn, books[i].edition);
    }

    fclose(fp);
}

void loadUsersFromFile() {
    FILE *fp = fopen(USER_FILE, "rb");
    if (fp == NULL) {
        printf("No existing user data file found. Starting with an empty user list.\n");
        return;
    }

    userCount = 0;
    while (fread(&users[userCount], sizeof(User), 1, fp) == 1) {
        userCount++;
    }
    fclose(fp);
}

void saveUsersToFile() {
    FILE *fp = fopen(USER_FILE, "wb");
    if (fp == NULL) {
        perror("Error opening user file for writing");
        return;
    }

    fwrite(users, sizeof(User), userCount, fp);
    fclose(fp);
}

int adminLogin() {
    char username[50];
    char password[50];
    char ch;
    int index = 0;

    printf("Enter admin username: ");
    scanf("%s", username);

    printf("Enter admin password: ");
    while (1) {
        ch = _getch();
        if (ch == 13) // Enter pressed
            break;
        if (ch == '\b') // Backspace pressed
        {
            if (index > 0) {
                index--;
                printf("\b \b");
            }
            continue;
        }
        password[index++] = ch;
        printf("*");
    }
    password[index] = '\0';

    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        return 1; // Return 1 on successful login
    } else {
        return 0; // Return 0 on failed login
    }
}

int findUserByUsername(const char *username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1; // User not found
}

void userProcess(const char *username) {
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
                borrowBook(username);
                break;
            case 3:
                returnBook(username);
                break;
            case 4:
                viewUserBooks(username);
                break;
            case 5:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}

void borrowBook(const char *username) {
    int id;
    printf("Enter book ID to borrow: ");
    scanf("%d", &id);

    int bookIndex = findBookById(id);
    if (bookIndex != -1) {
        if (books[bookIndex].takenByUserId == -1) {
            int userIndex = findUserByUsername(username);
            if (userIndex != -1) {
                if (users[userIndex].numBorrowedBooks < MAX_BOOKS_PER_USER) {
                    books[bookIndex].takenByUserId = userIndex;
                    users[userIndex].borrowedBooks[users[userIndex].numBorrowedBooks++] = id;
                    printf("Book borrowed successfully.\n");
                    saveBooksToFile();
                    saveUsersToFile();
                } else {
                    printf("You have reached the maximum limit of borrowed books (%d).\n", MAX_BOOKS_PER_USER);
                }
            } else {
                printf("User not found.\n");
            }
        } else {
            printf("Book already taken by another user.\n");
        }
    } else {
        printf("Book not found.\n");
    }
}

void returnBook(const char *username) {
    int id;
    printf("Enter book ID to return: ");
    scanf("%d", &id);

    int bookIndex = findBookById(id);
    if (bookIndex != -1) {
        int userIndex = books[bookIndex].takenByUserId;
        if (userIndex != -1 && strcmp(users[userIndex].username, username) == 0) {
            books[bookIndex].takenByUserId = -1;
            // Remove book ID from user's borrowed books list
            int found = 0;
            for (int i = 0; i < users[userIndex].numBorrowedBooks; i++) {
                if (users[userIndex].borrowedBooks[i] == id) {
                    for (int j = i; j < users[userIndex].numBorrowedBooks - 1; j++) {
                        users[userIndex].borrowedBooks[j] = users[userIndex].borrowedBooks[j + 1];
                    }
                    users[userIndex].numBorrowedBooks--;
                    found = 1;
                    break;
                }
            }
            if (found) {
                printf("Book returned successfully.\n");
                saveBooksToFile();
                saveUsersToFile();
            } else {
                printf("You have not borrowed this book.\n");
            }
        } else {
            printf("Book not borrowed by the current user.\n");
        }
    } else {
        printf("Book not found.\n");
    }
}

void viewUserBooks(const char *username) {
    int userIndex = findUserByUsername(username);
    if (userIndex != -1) {
        if (users[userIndex].numBorrowedBooks == 0) {
            printf("You have not borrowed any books.\n");
        } else {
            printf("Books borrowed by %s:\n", username);
            for (int i = 0; i < users[userIndex].numBorrowedBooks; i++) {
                int bookId = users[userIndex].borrowedBooks[i];
                int bookIndex = findBookById(bookId);
                if (bookIndex != -1) {
                    printf("ID: %d, Title: %s, Author: %s, Year: %d, ISBN: %s, Edition: %s\n",
                           books[bookIndex].id, books[bookIndex].title, books[bookIndex].author,
                           books[bookIndex].year, books[bookIndex].isbn, books[bookIndex].edition);
                }
            }
        }
    } else {
        printf("User not found.\n");
    }
}
