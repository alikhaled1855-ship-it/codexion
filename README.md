*This project has been created as part of the 42 curriculum by alsaleh.*

# Codexion

## Description

Codexion is a C implementation of the classic Dining Philosophers problem, reimagined around coders sharing USB dongles.

Coders sit in a circle, and each coder shares a dongle with each neighbor. To compile, a coder needs both their left and right dongles at the same time.

Between compiles, coders debug and refactor without needing dongles. If a coder goes too long without starting a new compile, they burn out and the simulation stops.

The goal of this project is to manage shared resources safely between multiple concurrent threads using POSIX threads, mutexes, condition variables, and a custom-built priority queue (min-heap).

The program supports two scheduling policies: FIFO (First In, First Out) and EDF (Earliest Deadline First).

It aims to prevent deadlocks and starvation while handling cooldown periods, precise timing, and thread-safe logging.

## Instructions

### Compilation

To compile the project, run:

```bash
make
```

### Execution

Run the program with the following arguments:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory.

- All numeric arguments must be positive integers (greater than 0 and not larger than `INT_MAX`).
- All time values are expressed in milliseconds.
- `number_of_coders` and `number_of_compiles_required` represent counts.
- `scheduler` must be exactly `fifo` or `edf`.
- If an argument is invalid, the program prints `Error` and exits with code 1.

### Example

```bash
./codexion 5 2000 200 100 100 3 50 fifo
```

All 5 coders finish their 3 compiles and the program stops normally.

With a burnout time that is too short, for example `./codexion 5 800 200 100 150 3 100 fifo`, a coder burns out and the simulation stops. This is the expected behavior.

### Makefile rules

```bash
make
make clean
make fclean
make re
```

## Blocking cases handled

### Deadlock prevention (Coffman's conditions)

Each coder needs two dongles simultaneously. This creates the possibility of circular-wait deadlock.

Codexion prevents this by acquiring dongles in a fixed order based on their memory addresses, rather than their left/right positions.

Using the same acquisition order prevents circular wait between coders.

### Starvation prevention

Dongle access is managed using a custom min-heap for each dongle.

Requests are ordered according to the selected scheduling policy:

- **FIFO:** Requests are served in arrival order (the key is the time of the request).
- **EDF:** Requests are ordered by their burnout deadlines (the key is the time of the last compile start plus the burnout time).

The scheduling mechanism is designed to provide fair access and prevent starvation under feasible simulation parameters.

### Cooldown handling

Each dongle records its last release time.

After a dongle is released, it remains unavailable until its cooldown period has elapsed.

A coder must wait until the cooldown expires before acquiring that dongle.

### Precise burnout detection

A dedicated monitor thread checks whether any coder has exceeded their allowed time without starting a new compile.

The monitor checks every 5 milliseconds and stops the simulation when burnout is detected.

This polling interval helps the program meet the required 10ms burnout logging window.

### Log serialization

All output is handled by a single mutex-protected logging function.

The print mutex ensures that messages from different threads do not get mixed together.

The timestamp is read after the print mutex is locked, so the timestamps in the output never go backwards.

Once the simulation has stopped, no more messages are printed.

## Thread synchronization mechanisms

### pthread_mutex_t

Mutexes protect shared resources and prevent multiple threads from modifying the same data simultaneously.

Codexion uses mutexes to protect:

- Each dongle's availability, cooldown information, and waiting heap.
- Each coder's compile counter and last compile time.
- The logging output.
- Shared simulation state (the `stop` flag).

This prevents race conditions, such as two coders acquiring the same dongle simultaneously.

### pthread_cond_t

Condition variables allow coder threads to wait for dongles without continuously checking their availability.

When a coder requests a dongle that is not free, or when other coders are already waiting for it, the coder joins the dongle's waiting heap and waits using `pthread_cond_wait`. If the dongle is free and nobody is waiting, the coder takes it directly.

When a dongle is released, `pthread_cond_broadcast` wakes the waiting threads.

Each awakened thread checks again whether the dongle is available and whether its request has priority in the heap before proceeding.

The checks are performed inside a `while` loop to handle spurious wakeups and ensure correct synchronization.

### Thread communication and event signaling

The monitor thread communicates simulation termination through the shared `stop` flag.

When burnout is detected, the monitor updates the simulation state and broadcasts on the dongles' condition variables.

This wakes sleeping coder threads so they can check the termination state and exit cleanly.

Condition-variable signaling is used to coordinate waiting threads and notify them about changes in dongle availability or simulation termination.

## Resources

- 42 Codexion Subject.
- `man pthread_create`
- `man pthread_mutex_lock`
- `man pthread_mutex_unlock`
- `man pthread_cond_wait`
- `man pthread_cond_broadcast`
- Wikipedia: Dining Philosophers Problem.
- Wikipedia: Coffman Conditions.
- Wikipedia: Earliest Deadline First (EDF).

### AI usage

Claude (Anthropic) was used throughout the project as a learning tool.

It helped explain important concepts before implementation, including:

- Threads and mutexes.
- Condition variables.
- Deadlocks and starvation.
- Binary heaps and priority queues.
- EDF scheduling.

AI also helped guide the incremental implementation of the project. The author wrote, tested, and ran each part while making sure to understand the implementation before moving forward.

During stress testing, AI helped identify and fix a race condition related to cooldown handling, where a lock was briefly released at the wrong point.

It was also used to identify Norminette violations and help restructure functions to comply with the 42 Norm.

In a final review, AI helped test the finished program (leak checks, data race checks, and stress tests with many different inputs). This review found two bugs, and AI provided the code for both fixes, which the author applied:

- Log timestamps could be printed out of order, because the time was read before the print mutex was locked.
- Argument validation accepted `0` and numbers too large for an `int`.
