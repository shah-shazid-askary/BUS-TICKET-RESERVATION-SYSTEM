#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for sleep function
#include <ctype.h> // for is digit function

#define MAX_SEATS 10
#define MAX_BUSES 5
#define SEAT_PRICE 100

const char *busSchedules[MAX_BUSES] = {"6AM", "8AM", "10AM", "1PM", "6PM"};
const char *routes[2] = {"Dhaka to Kushtia", "Kushtia to Dhaka"};

// Structure to represent a bus seat
typedef struct {
    int seatNumber;
    char customerName[50];
    int isOccupied;
} Seat;

// Structure to represent a bus
typedef struct {
    int busNumber;
    char schedule[10];
    char route[20];
    Seat seats[MAX_SEATS];
} Bus;

// Structure to represent a user
typedef struct {
    char phoneOrEmail[50];
    char password[20];
} User;

// Function prototypes
void displayMenu();
void checkBusSchedules();
void searchBusByRoute();
void addBus(Bus buses[]);
void selectSeat(Bus buses[]);
void cancelSelection(Bus buses[]);
void displaySeats(Bus buses[]);
void registerUser(User *user);
int loginUser(User *user);
void makePayment();
void downloadTicket(Bus buses[], User *user);
void clearConsole();
void saveUserData(User *user);
int loadUserData(User *user);

int main() {
    Bus buses[MAX_BUSES];
    User user;
    int choice;
    int loggedIn = 0;

    printf("===\\\\//=== Welcome to the SB Super Deluxe Online Bus Ticket Reservation System for Customers ===\\\\//===\n\n");

    // Initialize buses
    for (int i = 0; i < MAX_BUSES; i++) {
        buses[i].busNumber = i + 1;
        strcpy(buses[i].schedule, "");
        strcpy(buses[i].route, "");
        for (int j = 0; j < MAX_SEATS; j++) {
            buses[i].seats[j].seatNumber = j + 1;
            buses[i].seats[j].isOccupied = 0;
            strcpy(buses[i].seats[j].customerName, "Empty");
        }
    }

    // Load user data
    if (!loadUserData(&user)) {
        // Register user if no data found
        registerUser(&user);
        saveUserData(&user);
    }

    // User login
    while (!loggedIn) {
        loggedIn = loginUser(&user);
        if (!loggedIn) {
            printf("Invalid credentials, please try again.\n");
        }
    }

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                checkBusSchedules();
                break;
            case 2:
                searchBusByRoute();
                break;
            case 3:
                addBus(buses);
                break;
            case 4:
                selectSeat(buses);
                break;
            case 5:
                cancelSelection(buses);
                break;
            case 6:
                displaySeats(buses);
                break;
            case 7:
                makePayment();
                break;
            case 8:
                downloadTicket(buses, &user);
                break;
            case 9:
                printf("Exiting the program...\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 9.\n");
        }
    } while (choice != 9);

    clearConsole();
    return 0;
}

// Function to display menu options
void displayMenu() {
    printf("\n<>======<> Thank You for allowing us to serve You! <>======<>\n\n");
    printf("1. Check Bus Schedules\n");
    printf("2. Search Bus by Route\n");
    printf("3. Add Bus\n");
    printf("4. Select a Seat\n");
    printf("5. Cancel Selection\n");
    printf("6. Display Available Seats\n");
    printf("7. Make Payment\n");
    printf("8. Download Ticket\n");
    printf("9. Exit\n");
}

// Function to check bus schedules
void checkBusSchedules() {
    printf("Available Bus Schedules:\n");
    for (int i = 0; i < MAX_BUSES; i++) {
        printf("%d. %s\n", i + 1, busSchedules[i]);
    }
}

// Function to search bus by route
void searchBusByRoute() {
    char route[20];
    printf("Enter route (Dhaka to Kushtia or Kushtia to Dhaka): ");
    scanf(" %[^\n]s", route);

    printf("Available buses for route %s:\n", route);
    for (int i = 0; i < MAX_BUSES; i++) {
        if (strcmp(routes[0], route) == 0 || strcmp(routes[1], route) == 0) {
            printf("Bus %d: %s\n", i + 1, busSchedules[i]);
        }
    }
}

// Function to add a bus
void addBus(Bus buses[]) {
    int busNum;
    char schedule[10];
    char route[20];

    printf("Enter bus number to add (1-5): ");
    scanf("%d", &busNum);

    if (busNum < 1 || busNum > MAX_BUSES) {
        printf("Invalid bus number!\n");
        return;
    }

    printf("Enter bus schedule (6AM, 8AM, 10AM, 1PM, 6PM): ");
    scanf("%s", schedule);

    int validSchedule = 0;
    for (int i = 0; i < MAX_BUSES; i++) {
        if (strcmp(busSchedules[i], schedule) == 0) {
            validSchedule = 1;
            break;
        }
    }
    if (!validSchedule) {
        printf("Invalid schedule!\n");
        return;
    }

    printf("Enter bus route (Dhaka to Kushtia or Kushtia to Dhaka): ");
    scanf(" %[^\n]s", route);

    if (strcmp(routes[0], route) != 0 && strcmp(routes[1], route) != 0) {
        printf("Invalid route!\n");
        return;
    }

    buses[busNum - 1].busNumber = busNum;
    strcpy(buses[busNum - 1].schedule, schedule);
    strcpy(buses[busNum - 1].route, route);

    printf("Bus %d added successfully with schedule %s and route %s.\n", busNum, schedule, route);
}

// Function to select seat
void selectSeat(Bus buses[]) {
    int busNum, seatNum;

    printf("Enter bus number to select seat (1-5): ");
    scanf("%d", &busNum);

    if (busNum < 1 || busNum > MAX_BUSES) {
        printf("Invalid bus number!\n");
        return;
    }

    printf("Enter seat number to select (1-10): ");
    scanf("%d", &seatNum);

    if (seatNum < 1 || seatNum > MAX_SEATS) {
        printf("Invalid seat number!\n");
        return;
    }

    if (buses[busNum - 1].seats[seatNum - 1].isOccupied) {
        printf("Seat %d in bus %d is already occupied!\n", seatNum, busNum);
    } else {
        printf("Enter your name: ");
        scanf(" %[^\n]s", buses[busNum - 1].seats[seatNum - 1].customerName);
        buses[busNum - 1].seats[seatNum - 1].isOccupied = 1;
        printf("Seat %d in bus %d selected successfully for %s.\n", seatNum, busNum, buses[busNum - 1].seats[seatNum - 1].customerName);
    }
}

// Function to cancel a selection
void cancelSelection(Bus buses[]) {
    int busNum, seatNum;

    printf("Enter bus number to cancel selection (1-5): ");
    scanf("%d", &busNum);

    if (busNum < 1 || busNum > MAX_BUSES) {
        printf("Invalid bus number!\n");
        return;
    }

    printf("Enter seat number to cancel selection (1-10): ");
    scanf("%d", &seatNum);

    if (seatNum < 1 || seatNum > MAX_SEATS) {
        printf("Invalid seat number!\n");
        return;
    }

    if (!buses[busNum - 1].seats[seatNum - 1].isOccupied) {
        printf("Seat %d in bus %d is not selected.\n", seatNum, busNum);
    } else {
        buses[busNum - 1].seats[seatNum - 1].isOccupied = 0;
        strcpy(buses[busNum - 1].seats[seatNum - 1].customerName, "Empty");
        printf("Selection for seat no %d in bus no %d canceled successfully.\n", seatNum, busNum);
    }
}

// Function to display available seats
void displaySeats(Bus buses[]) {
    int busNum;

    printf("Enter bus number to display available seats (1-5): ");
    scanf("%d", &busNum);

    if (busNum < 1 || busNum > MAX_BUSES) {
        printf("Invalid bus number!\n");
        return;
    }

    printf("Available Seats in Bus %d:\n", busNum);
    printf("==========================\n");
    for (int i = 0; i < MAX_SEATS; i++) {
        if (!buses[busNum - 1].seats[i].isOccupied) {
            printf("Seat no %d: Available\n", buses[busNum - 1].seats[i].seatNumber);
        }
    }
}

// Function to register a user
void registerUser(User *user) {
    printf("Register:\n");
    printf("Enter phone number or email: ");
    scanf("%s", user->phoneOrEmail);
    printf("Enter password: ");
    scanf("%s", user->password);
    printf("Registration successful!\n");
}

// Function to login a user
int loginUser(User *user) {
    char phoneOrEmail[50];
    char password[20];

    printf("Login:\n");
    printf("Enter phone number or email: ");
    scanf("%s", phoneOrEmail);
    printf("Enter password: ");
    scanf("%s", password);

    if (strcmp(user->phoneOrEmail, phoneOrEmail) == 0 && strcmp(user->password, password) == 0) {
        printf("Login successful!\n");
        return 1;
    } else {
        return 0;
    }
}

// Function to make payment
void makePayment() {
    char phoneNumber[12];
    char pin[6];
    int seats;

    printf("Enter your Bkash phone number (11 digits): ");
    scanf("%s", phoneNumber);

    while (strlen(phoneNumber) != 11 || !isdigit(phoneNumber[0])) {
        printf("Invalid phone number. Please enter an 11-digit phone number: ");
        scanf("%s", phoneNumber);
    }

    printf("Enter your Bkash pin (5 digits): ");
    scanf("%s", pin);

    while (strlen(pin) != 5 || !isdigit(pin[0])) {
        printf("Invalid pin. Please enter a 5-digit pin: ");
        scanf("%s", pin);
    }

    printf("Enter number of seats selected: ");
    scanf("%d", &seats);

    int totalBill = seats * SEAT_PRICE;

    printf("Total Bill: %d Taka\n", totalBill);
    printf("Processing payment...\n");
    // Simulate payment processing delay
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");

    printf("<><> Congratulations! Your booking is successful! <><>\n");
}

// Function to download ticket
void downloadTicket(Bus buses[], User *user) {
    int busNum;
    int seatNum;
    int totalBill;

    printf("Enter bus number (1-5): ");
    scanf("%d", &busNum);

    if (busNum < 1 || busNum > MAX_BUSES) {
        printf("Invalid bus number!\n");
        return;
    }

    printf("Enter seat number (1-10): ");
    scanf("%d", &seatNum);

    if (seatNum < 1 || seatNum > MAX_SEATS) {
        printf("Invalid seat number!\n");
        return;
    }

    if (!buses[busNum - 1].seats[seatNum - 1].isOccupied) {
        printf("Seat no %d in bus no %d is not selected.\n", seatNum, busNum);
        return;
    }

    totalBill = SEAT_PRICE;

    printf("\n====== Ticket Details ======\n");
    printf("Customer Name: %s\n", buses[busNum - 1].seats[seatNum - 1].customerName);
    printf("Customer Phone Number or Gmail: %s\n", user->phoneOrEmail);
    printf("Bus Number: %d\n", busNum);
    printf("Selected Bus Time by Customer: %s\n", buses[busNum - 1].schedule);
    printf("Seat Number: %d\n", seatNum);
    printf("Bill: %d Taka\n", totalBill);
    printf("============================\n");
}

// Function to clear the console
void clearConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Function to save user data to a file
void saveUserData(User *user) {
    FILE *file = fopen("user_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing user data.\n");
        return;
    }
    fprintf(file, "%s\n%s\n", user->phoneOrEmail, user->password);
    fclose(file);
}

// Function to load user data from a file
int loadUserData(User *user) {
    FILE *file = fopen("user_data.txt", "r");
    if (file == NULL) {
        return 0; // No user data found
    }
    fscanf(file, "%s\n%s\n", user->phoneOrEmail, user->password);
    fclose(file);
    return 1;
}
