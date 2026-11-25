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

- All numeric arguments must be non-negative integers not larger than `INT_MAX`. Every argument must be at least 1, except `dongle_cooldown`, which may be 0.
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

Each coder needs two dongles simultaneously, which could create a circular wait.

Codexion removes hold-and-wait: a coder never holds one dongle while waiting for the other. It claims both dongles in a single step, under one shared mutex (`dlock`), and only when both are free, both cooldowns have passed, and no higher-priority waiter sharing a dongle could start right now.


### Starvation prevention

Dongle access is managed using a custom min-heap for each dongle.

Requests are ordered according to the selected scheduling policy:

- **FIFO:** Requests are served in arrival order (the key is the time of the request).
- **EDF:** Requests are ordered by their burnout deadlines (the key is the time of the last compile start plus the burnout time).

The scheduling mechanism is designed to provide fair access and prevent starvation under feasible simulation parameters.

### Cooldown handling

Each dongle records its last release time.

A coder starts only when both of its dongles have been released for at least `dongle_cooldown` ms. While waiting, it sleeps with `pthread_cond_timedwait` until the cooldown expires, or until it is woken by a release or by the stop signal.


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

- All dongle state (availability, cooldown information, waiting heaps) through one shared mutex, `dlock`.
- Each coder's compile counter and last compile time.
- The logging output.
- Shared simulation state (the `stop` flag).

This prevents race conditions, such as two coders acquiring the same dongle simultaneously.

### pthread_cond_t

One condition variable (`dcond`), paired with `dlock`, lets coder threads wait for dongles without busy-waiting.

A coder pushes a request into the waiting heap of each of its two dongles, then waits on `dcond` (with a timeout while a cooldown is pending). Whenever dongles are released, `pthread_cond_broadcast` wakes the waiting coders. Each one re-checks its conditions inside a `while` loop, which also handles spurious wakeups.


### Thread communication and event signaling

The monitor thread communicates simulation termination through the shared `stop` flag.

When burnout is detected, the monitor updates the simulation state and broadcasts on the shared condition variable (`dcond`).

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

AI also helped diagnose a convoy problem under large cooldowns (coders holding one dongle while waiting for the other), which led to the single-lock design where a coder claims both dongles at once. The author applied, tested and verified this change.
