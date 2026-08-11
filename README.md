# Bus Ticket Reservation System

A C-based bus ticket reservation system built for a Data Structures lab final project. Unlike a conventional booking system where a seat is locked for an entire trip, this system tracks bookings **per route segment** — allowing a single physical seat to be shared by multiple passengers travelling different, non-overlapping parts of the same journey.

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

## Build & Run

Requires `gcc` (or any standard C compiler).

```bash
gcc -o bus_reservation src/bus_reservation.c
./bus_reservation
```

## Project Structure

```
bus-reservation-system/
├── src/
│   └── bus_reservation.c   # main source code
├── docs/
│   └── bus_reservation_report_short.docx   # lab report
└── README.md
```

## Report

See `docs/` for the lab report covering the problem statement, benefits, and data structure reasoning.
