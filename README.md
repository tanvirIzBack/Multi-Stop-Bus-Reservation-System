# Bus Ticket Reservation System

A C-based bus ticket reservation system built for a Data Structures lab final project. Unlike a conventional booking system where a seat is locked for an entire trip, this system tracks bookings **per route segment**, allowing a single physical seat to be shared by multiple passengers travelling different, non-overlapping parts of the same journey.

## Features

- Book a ticket for a specific boarding stop → destination stop
- Automatic seat assignment (the system finds any seat with a free matching segment)
- A seat can be booked by multiple passengers if their segments don't overlap
- Waiting list when no seat can serve the requested segment
- Automatic reassignment from the waiting list when a seat frees up
- Cancel a specific passenger's booking on a seat
- Check all current bookings on a given seat
- View the full waiting list
- View the list of route stops

## Data Structures Used

| Structure | Where | Why |
|---|---|---|
| Array | `seats[]` | Fixed number of seats -> O(1) direct access by seat number |
| Linked list | Per-seat `Booking` list | Each seat can hold a variable number of segment bookings |
| Linked list (queue) | Waiting list (`front`/`rear`) | Strict first-come, first-served ordering |
| Interval overlap check | `isSeatFreeForSegment()` | Decides whether a seat can be shared between two segments |

## Diagram

<img width="1536" height="1024" alt="ChatGPT Image Aug 11, 2026, 11_29_46 PM" src="https://github.com/user-attachments/assets/d528405a-e9f5-430c-98b8-3d37fe7711b1" />

## Build & Run

Requires `gcc` (or any standard C compiler).

```bash
gcc -o bus_reservation src/bus_reservation.c
./bus_reservation
```

## Project Structure

```
MAIN MENU
    │
    ├── 1. bookSeat()
    │       ├── printStops()
    │       ├── isSeatFreeForSegment()
    │       │       └── segmentsOverlap()
    │       ├── addBooking()
    │       └── enqueueWait()
    │
    ├── 2. cancelSeat()
    │       ├── removeBookingByName()
    │       ├── isSeatFreeForSegment()
    │       │       └── segmentsOverlap()
    │       ├── dequeueWait()
    │       └── addBooking()
    │
    ├── 3. checkSeat()
    │       └── [reads Booking linked list]
    │
    ├── 4. waitingList()
    │       └── [reads Waiting Queue]
    │
    ├── 5. printStops()
    │       └── [reads stopNames[]]
    │
    └── 6. Exit
            └── Program terminates
```

## Report

See `docs/` for the lab report covering the problem statement, benefits, and data structure reasoning.
