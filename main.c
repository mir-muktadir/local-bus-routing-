#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STOPS 25
#define MAX_BUSES 10

/* ===================== STRUCTURES ===================== */

struct Bus {
    char name[30];
    char stops[MAX_STOPS][25];
    int stopCount;
};

struct User {
    char username[20];
    char password[20];
};

struct Admin {
    char username[20];
    char password[20];
};

struct Seat {
    int seatNumber;
    int isBooked;
};

/* ===================== UTILITY FUNCTIONS ===================== */

void toLower(char str[]) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

int findStop(char stops[][25], int count, char stop[]) {
    for (int i = 0; i < count; i++) {
        if (strcmp(stops[i], stop) == 0)
            return i;
    }
    return -1;
}

/* ===================== LOGIN SYSTEM ===================== */

void registerUser() {
    struct User u;
    FILE *fp = fopen("users.txt", "a");

    if (!fp) {
        printf("File error!\n");
        return;
    }

    printf("\n--- User Registration ---\n");
    printf("Username: ");
    scanf("%s", u.username);
    printf("Password: ");
    scanf("%s", u.password);

    fprintf(fp, "%s %s\n", u.username, u.password);
    fclose(fp);

    printf("Registration successful!\n");
}

int loginUser() {
    struct User u, f;
    FILE *fp = fopen("users.txt", "r");

    if (!fp) {
        printf("No users found!\n");
        return 0;
    }

    printf("\n--- User Login ---\n");
    printf("Username: ");
    scanf("%s", u.username);
    printf("Password: ");
    scanf("%s", u.password);

    while (fscanf(fp, "%s %s", f.username, f.password) != EOF) {
        if (!strcmp(u.username, f.username) &&
            !strcmp(u.password, f.password)) {
            fclose(fp);
            printf("Login successful!\n");
            return 1;
        }
    }

    fclose(fp);
    printf("Invalid credentials!\n");
    return 0;
}

/* ===================== FILE HANDLING ===================== */

void saveTicket(char bus[], char from[], char to[], int fare) {
    FILE *fp = fopen("tickets.txt", "a");
    if (!fp) return;

    fprintf(fp,
        "Bus: %s\nFrom: %s\nTo: %s\nFare: %d taka\n-------------------\n",
        bus, from, to, fare);

    fclose(fp);
}

void viewTickets() {
    FILE *fp = fopen("tickets.txt", "r");
    char ch;

    if (!fp) {
        printf("No ticket history found.\n");
        return;
    }

    printf("\n===== TICKET HISTORY =====\n");
    while ((ch = fgetc(fp)) != EOF)
        putchar(ch);

    fclose(fp);
}

/* ===================== DISPLAY ===================== */

void showBusList(struct Bus buses[], int count) {
    printf("\n===== AVAILABLE BUSES =====\n");
    for (int i = 0; i < count; i++)
        printf("%d. %s (%d stops)\n", i + 1, buses[i].name, buses[i].stopCount);
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

/* ===================== MAIN ===================== */

int main() {

    struct Bus buses[3] = {
        {"Raida",
        {"abdullahpur","housebuilding","airport","kuril","natunbazar","badda",
         "rampura","malibagh","mouchak","mogbazar","kakrail","shantinagar","motijheel"}, 13},

        {"Turag",
        {"tongi","abdullahpur","airport","kuril","natunbazar","badda",
         "rampura","malibagh","khilgaon","basabo","mugdapara","maniknagar","sayedabad","jatrabari"}, 14},

        {"Balaka",
        {"sadarghat","gulistan","gpo","paltan","kakrail","shantinagar","malibagh",
         "rampura","badda","natunbazar","kuril","airport","abdullahpur","tongi"}, 14}
    };

    int choice, loggedIn = 0;

    while (!loggedIn) {
        printf("\n===== BUS ROUTE SYSTEM =====\n");
        printf("1. Register\n2. Login\n3. Exit\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) registerUser();
        else if (choice == 2) loggedIn = loginUser();
        else return 0;
    }

    while (loggedIn) {
        printf("\n===== USER MENU =====\n");
        printf("1. Search Route\n2. View Tickets\n3. Bus List\n4. Logout\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char from[25], to[25];
            int isStudent;

            printf("From: "); scanf("%s", from);
            printf("To: "); scanf("%s", to);
            printf("Student? (1/0): "); scanf("%d", &isStudent);

            toLower(from);
            toLower(to);

            for (int i = 0; i < 3; i++) {
                int f = findStop(buses[i].stops, buses[i].stopCount, from);
                int t = findStop(buses[i].stops, buses[i].stopCount, to);

                if (f != -1 && t != -1 && f < t) {
                    int fare = (t - f) * 10;
                    if (isStudent) fare /= 2;

                    printf("\nBus: %s\nFare: %d taka\n", buses[i].name, fare);
                    saveTicket(buses[i].name, from, to, fare);
                }
            }
            pauseScreen();
        }
        else if (choice == 2) {
            viewTickets();
            pauseScreen();
        }
        else if (choice == 3) {
            showBusList(buses, 3);
            pauseScreen();
        }
        else if (choice == 4) {
            printf("Logged out.\n");
            break;
        }
    }

    return 0;
}
