#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TOTAL_SEATS 1
#define NUM_STOPS 5
const char *stopNames[NUM_STOPS] = {"Dhaka", "Comilla", "Feni", "Chittagong", "Cox's Bazar"};

// Each seat holds a LINKED LIST of segment bookings
// A seat can carry multiple passengers, as long as their segments don't overlap.
struct Booking {
    int start;             
    int end;            
    char name[50];
    struct Booking *next;
};

struct Seat {
    int seatNo;
    struct Booking *bookings;  // Each seat contains a booking list (Linked List)
}seats[TOTAL_SEATS];

struct QNode {
    char name[50];
    int start;
    int end;
    struct QNode *next;
}*front = NULL, *rear = NULL;

void initializeSeats() {
    for (int i = 0; i < TOTAL_SEATS; i++) {
        seats[i].seatNo = i + 1;
        seats[i].bookings = NULL;
    }
}

void enqueueWait(char *name, int start, int end) {
    struct QNode *node = (struct QNode*) malloc(sizeof(struct QNode));
    strcpy(node->name, name);
    node->start = start;
    node->end = end;
    node->next = NULL;
    if(rear == NULL){
        front = rear = node;
    }else {
        rear->next = node;
        rear = node;
    }
}

int dequeueWait(char *name, int *start, int *end) {
    if (front == NULL){
         return 0;
    }
    struct QNode *node = front;
    strcpy(name, node->name);
    *start = node->start;
    *end = node->end;
    front = front->next;
    if (front == NULL) {
        rear = NULL;
    }
    free(node);
    return 1;
}

// Two segments [s1,e1) and [s2,e2) overlap if one starts before the other ends
int segmentsOverlap(int s1, int e1, int s2, int e2) {
    return (s1 < e2 && s2 < e1);
}

int isSeatFreeForSegment(int seatIndex, int start, int end) {
    struct Booking *b = seats[seatIndex].bookings;
    while (b != NULL) {
        if (segmentsOverlap(b->start, b->end, start, end)){
             return 0;
        }
        b = b->next;
    }
    return 1;   // no existing booking conflicts with this segment 
}

void addBooking(int seatIndex, int start, int end, char *name) {
    struct Booking *node = (struct Booking*) malloc(sizeof(struct Booking));
    node->start = start;
    node->end = end;
    strcpy(node->name, name);
    node->next = seats[seatIndex].bookings;   // insert at head
    seats[seatIndex].bookings = node;
}

void removeBookingByName(int seatIndex, char *name, int *foundStart, int *foundEnd) {
    struct Booking *cur = seats[seatIndex].bookings;
    struct Booking *prev = NULL;
    *foundStart = -1;
    *foundEnd = -1;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            *foundStart = cur->start;
            *foundEnd = cur->end;
            if (prev == NULL) seats[seatIndex].bookings = cur->next;
            else prev->next = cur->next;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void printStops() {
    printf("Available stops:\n");
    for (int i = 0; i < NUM_STOPS; i++) {
        printf("%d. %s\n", i, stopNames[i]);
    }
}

/* ---------- Menu operations ---------- */
void bookSeat() {
    char name[50];
    int start, end;
    printf("Enter passenger name: ");
    scanf(" %[^\n]", name);
    printStops();
    printf("Boarding stop number: ");
    scanf("%d", &start);
    printf("Destination stop number: ");
    scanf("%d", &end);
    if (start < 0 || end >= NUM_STOPS || start >= end) {
        printf("Invalid stop range.\n");
        return;
    }
    for (int i = 0; i < TOTAL_SEATS; i++) {
        if (isSeatFreeForSegment(i, start, end)) {
            addBooking(i, start, end, name);
            printf("Ticket booked for %s on Seat %d (%s -> %s).\n", name, seats[i].seatNo, stopNames[start], stopNames[end]);
            return;
        }
    }
    printf("No seat available for that segment. Adding %s to the waiting list.\n", name);
    enqueueWait(name, start, end);
}

void cancelSeat() {
    int seatNo, seatIndex;
    char name[50];
    printf("Enter seat number to cancel from: ");
    scanf("%d", &seatNo);
    if (seatNo < 1 || seatNo > TOTAL_SEATS) {
        printf("Invalid seat number.\n");
        return;
    }
    seatIndex = seatNo - 1;
    if (seats[seatIndex].bookings == NULL) {
        printf("Seat %d has no active bookings.\n", seatNo);
        return;
    }
    printf("Current bookings on Seat %d:\n", seatNo);
    struct Booking *b = seats[seatIndex].bookings;
    while (b != NULL) {
        printf(" - %s (%s -> %s)\n", b->name, stopNames[b->start], stopNames[b->end]);
        b = b->next;
    }
    printf("Enter passenger name to cancel: ");
    scanf(" %[^\n]", name);
    int freedStart, freedEnd;
    removeBookingByName(seatIndex, name, &freedStart, &freedEnd);
    if (freedStart == -1) {
        printf("No booking found for %s on Seat %d.\n", name, seatNo);
        return;
    }
    printf("Booking for %s (%s -> %s) on Seat %d cancelled.\n", name, stopNames[freedStart], stopNames[freedEnd], seatNo);
    struct QNode *ptr;
    while(ptr!=NULL){
        ptr=ptr->next;
        if(front != NULL && isSeatFreeForSegment(seatIndex, front->start, front->end)){
            char waitName[50];
            int ws, we;
            dequeueWait(waitName, &ws, &we);
            addBooking(seatIndex, ws, we, waitName);
            printf("Seat %d now booked for %s (%s -> %s) from waiting list.\n", seatNo, waitName, stopNames[ws], stopNames[we]);
            ptr=front;
        }
    }
}

void checkSeat() {
    int seatNo, seatIndex;
    printf("Enter seat number to check: ");
    scanf("%d", &seatNo);
    if (seatNo < 1 || seatNo > TOTAL_SEATS) {
        printf("Invalid seat number.\n");
        return;
    }
    seatIndex = seatNo - 1;
    if (seats[seatIndex].bookings == NULL) {
        printf("Seat %d is fully free for the whole route.\n", seatNo);
        return;
    }
    printf("Seat %d bookings:\n", seatNo);
    struct Booking *b = seats[seatIndex].bookings;
    while (b != NULL) {
        printf(" - %s: %s -> %s\n", b->name, stopNames[b->start], stopNames[b->end]);
        b = b->next;
    }
}

void waitingList() {
    printf("--- Waiting List ---\n");
    if (front == NULL) {
        printf("(empty)\n");
        return;
    }
    struct QNode *n = front;
    int i = 1;
    while (n != NULL) {
        printf("%d. %s (%s -> %s)\n", i, n->name, stopNames[n->start], stopNames[n->end]);
        n = n->next;
        i++;
    }
}

void display() {
    printf("Seat summary:\n");
    for (int i = 0; i < TOTAL_SEATS; i++) {
        int cnt = 0;
        struct Booking *b = seats[i].bookings;
        while (b != NULL){
            cnt++; 
            b = b->next; 
        }
        printf("Seat %d: %d booking(s)\n", seats[i].seatNo, cnt);
    }
    printf("\n");
}

int main() {
    initializeSeats();
    int choice = -1;
    while (choice != 6) {
        printf("==== Multi-Stop Bus Reservation System ====\n\n");
        display();
        printf("1. Book a Ticket\n");
        printf("2. Cancel a Ticket\n");
        printf("3. Check Seat Bookings\n");
        printf("4. View Waiting List\n");
        printf("5. View Route Stops\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: bookSeat(); break;
            case 2: cancelSeat(); break;
            case 3: checkSeat(); break;
            case 4: waitingList(); break;
            case 5: printStops(); break;
            case 6: printf("Exiting system. Thank you!\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
        printf("\n\n");
    }
    return 0;
}