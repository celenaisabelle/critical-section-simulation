# Critical Section Simulation

A C implementation of the critical section problem using POSIX threads and mutex synchronization.

This project was developed as an Operating Systems assignment at Florida International University. It demonstrates how multiple threads can safely access and modify shared data while preventing race conditions through mutual exclusion.

## Overview

The program simulates two threads operating on a shared bank account balance:

- **Thread A (Depositor)** adds funds to the shared balance.
- **Thread B (Withdrawer)** removes funds from the shared balance.
- A POSIX mutex protects the shared balance while either thread performs an update.

Both threads execute concurrently and synchronize access to the critical section.

## Critical Section

The shared resource in the program is:

```c
static int balance = 0;
```

Because both threads modify `balance`, access to it is protected using a POSIX mutex:

```c
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
```

Each thread follows the critical-section structure:

```text
Entry
  ↓
Critical Section
  ↓
Exit
  ↓
Remainder
```

Before modifying the balance, a thread acquires the mutex:

```c
pthread_mutex_lock(&mtx);
```

After completing the operation, the thread releases it:

```c
pthread_mutex_unlock(&mtx);
```

This ensures that only one thread modifies the shared balance at a time.

## Thread A — Depositor

The depositor performs deposit operations until reaching the defined deposit limit.

```c
#define DEPOSIT_LIMIT 2000000
```

Normally, one unit is added to the balance. The program also implements a bonus condition that can increase the deposit amount to 50.

The depositor tracks:

- Total deposit operations
- Number of bonuses received
- Shared account balance

## Thread B — Withdrawer

The withdrawer performs two million withdrawal operations:

```c
#define WITHDRAW_LIMIT 2000000
```

Each operation subtracts one unit from the shared balance while holding the mutex.

## Thread Management

The program creates both threads using `pthread_create()`:

```c
pthread_create(&ta, NULL, depositor, &stats);
pthread_create(&tb, NULL, withdrawer, NULL);
```

The parent thread then waits for both worker threads to complete using `pthread_join()` before displaying the final balance.

## Building the Project

Compile using GCC with the POSIX threads library:

```bash
gcc -o fname thread-solution.c -lpthread
```

## Running the Program

```bash
./fname
```

Because the depositor and withdrawer execute concurrently, their completion order and intermediate printed balance may vary between runs.

## Project Structure

```text
critical-section-simulation/
├── README.md
├── thread-solution.c
└── .gitignore
```

## Concepts Demonstrated

- Critical sections
- Mutual exclusion
- POSIX threads
- Mutex synchronization
- Shared memory between threads
- Concurrent execution
- Race-condition prevention
- Thread creation and synchronization
- `pthread_create()`
- `pthread_mutex_lock()`
- `pthread_mutex_unlock()`
- `pthread_join()`

## What I Learned

This assignment provided hands-on experience with concurrency and synchronization in C. It demonstrated why shared data must be protected when multiple threads can modify it concurrently and how mutexes can be used to control access to a critical section.

The project also provided experience creating and coordinating POSIX threads, working with shared state, and waiting for concurrent operations to complete before returning control to the parent thread.