*This project has been created as part of the 42 curriculum by aalnahas.*

# Codexion

## Description

Codexion is a C implementation of the classic Dining Philosophers problem, reframed
around coders sharing dongles. Coders sit in a circle; each shares a dongle with each
neighbor. To compile, a coder needs both their left and right dongle at the same time.
Between compiles, coders debug and refactor (no dongles needed). If a coder goes too
long without starting a new compile, they burn out and the simulation stops.

The goal of the project is to safely share limited resources (dongles) between many
concurrent threads (coders), without deadlock, without starvation, and with precise,
race-free timing and logging — using a custom-built priority queue (min-heap) to
arbitrate access, according to either a FIFO or EDF (Earliest Deadline First) policy.

## Instructions

Build:
```
make
```

Run:
```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
  time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

- All numeric arguments must be positive integers (milliseconds, except
  `number_of_coders` and `number_of_compiles_required`, which are counts).
- `scheduler` must be exactly `fifo` or `edf`.

Example:
```
./codexion 5 800 200 100 150 3 100 fifo
```

Other Makefile rules: `make clean`, `make fclean`, `make re`.

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions):** each coder needs two dongles at
  once, which is the classic setup for circular-wait deadlock. Codexion breaks this by
  always acquiring dongles in a fixed, consistent order (by memory address, not by
  "left/right"), for every coder. This means no two neighboring coders can ever each
  hold one dongle while waiting on the other, eliminating circular wait entirely.
- **Starvation prevention:** dongle access is arbitrated by a custom min-heap per
  dongle, ordered by arrival time (FIFO) or burnout deadline (EDF), with a
  deterministic tie-break. No coder can be indefinitely skipped in favor of another.
- **Cooldown handling:** each dongle records its last release time; a coder acquiring
  a dongle waits out any remaining cooldown before it is marked taken again.
- **Precise burnout detection:** a dedicated monitor thread polls every 5ms,
  comparing each coder's time since their last compile start against
  `time_to_burnout`, guaranteeing detection well within the required 10ms window.
- **Log serialization:** all output goes through a single mutex-protected logging
  function, so concurrent log lines from different threads are never interleaved.

## Thread synchronization mechanisms

- **`pthread_mutex_t`** protects every piece of shared state: each dongle's
  availability/cooldown/waiting-heap (its own mutex, so unrelated dongles never
  block each other), the shared print lock (serializes all logging), and implicitly
  the `stop` flag's use around dongle state changes. Every read or write of shared
  data happens only while holding the relevant mutex, preventing races such as two
  coders both believing they've acquired the same free dongle.
- **`pthread_cond_t`** lets a coder thread sleep while waiting for a dongle instead
  of busy-waiting. A coder pushes itself onto the dongle's heap and calls
  `pthread_cond_wait`, which atomically releases the mutex while asleep and
  re-acquires it on wake. On release, the holder calls `pthread_cond_broadcast`,
  waking all waiters, each of which re-checks (in a `while` loop, not `if`) whether
  it is genuinely free AND at the top of the heap before proceeding — this is what
  lets many coders share one condition variable safely and correctly.
- **Monitor–coder communication:** the shared `stop` flag, always read/written under
  a dongle's lock when relevant, is how the monitor signals every coder to shut
  down. Since some coders may be asleep on a condition variable, the monitor
  broadcasts on every dongle's `cond` after setting `stop`, guaranteeing every
  sleeping thread wakes up, re-checks `stop`, and exits cleanly instead of hanging.

## Resources

- 42 Subject: Dining Philosophers (original problem this project is based on)
- `man pthread_create`, `man pthread_mutex_lock`, `man pthread_cond_wait`
- Wikipedia: Dining philosophers problem, Coffman conditions, Earliest Deadline First
- **AI usage:** Claude (Anthropic) was used throughout as a learning tool, not a code
  generator. It explained concepts (threads, mutexes, condition variables, deadlock/
  starvation, binary heaps, EDF scheduling) before any code was written, and guided
  incremental implementation of every file, with the author writing/testing/running
  each piece and confirming understanding before moving on. It was also used to help
  identify and fix a real race condition found during stress testing (a cooldown
  check that briefly released a lock at the wrong point), and to identify norminette
  violations and restructure functions to comply with them.
