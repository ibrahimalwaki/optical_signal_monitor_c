                        Optical Signal Monitor
Overview
This project simulates a real-time optical signal monitoring system written in pure C.
It models an embedded firmware pipeline:
Signal acquisition (simulated ADC)
Digital signal processing (low-pass filtering)
Metrics computation (RMS, peak detection)
Fault detection (dropout & noise spike)
Multi-threaded producer/consumer architecture
CSV logging
Unit testing
Clean shutdown with signal handling
The design mimics how high-performance optical or embedded systems process streaming data in real time.
Architecture
Producer Thread (Acquisition)
        ↓
Thread-Safe Block Queue (FIFO, mutex + condition variables)
        ↓
Consumer Thread (Processing + Detection + Logging)

Modules
acquisition.c
Generates sine wave signal with configurable noise and fault injection.
processing.c
Implements:
First-order IIR low-pass filter
RMS computation
Peak detection
fault.c
Detects:
Signal dropout (low RMS)
Noise spikes (high peak)
Includes cooldown logic to prevent alert spam.
queue.c
Thread-safe ring buffer using:
pthread_mutex
pthread_cond
Producer/consumer blocking behavior.
logger.c
Writes CSV logs:
t_ms,raw_rms,raw_peak,fil_rms,fil_peak,fault
tests/
Lightweight unit tests:
Metrics correctness
Fault detection logic
Queue FIFO behavior

Features
Real-time periodic signal generation
Multi-threaded processing
Stateful DSP filtering
Fault injection for testing robustness
Fault detection with configurable thresholds
Clean shutdown via SIGINT (Ctrl+C)
CSV logging for offline analysis
Unit tests in pure C (no external framework)

Build Instructions (macOS / Linux)
From project root:
rm -rf build
cmake -S . -B build
cmake --build build


Run
Main application
./build/optical_monitor

Stop with:
Ctrl + C

Expected shutdown message:
Shutdown complete.

Logs are written to:
logs/run.csv

Make sure the directory exists:
mkdir -p logs


Run Unit Tests
./build/optical_tests

Expected:
ALL TESTS PASSED


Example Output
raw: rms=0.7082 peak=1.0341 | filtered: rms=0.5513 peak=0.7902
ALERT: NOISE_SPIKE (raw rms=0.7321 peak=1.48)


Technical Highlights
C11
POSIX threads
Mutex + condition variables
Ring buffer design
IIR filtering
RMS signal power measurement
Deterministic block processing
Atomic shutdown flag
Defensive programming
Separation of concerns (modular design)
Testable components

Why This Project Matters
This project demonstrates:
Embedded-style real-time architecture
DSP fundamentals
Multi-threaded system design
Fault detection logic
Software lifecycle best practices
Designing for test and maintainability
It is directly aligned with roles involving:
Embedded software
Signal processing
Optical/telecom systems
Real-time systems
Low-level C development

Possible Extensions
Performance/latency measurements
Backpressure detection
Dynamic configuration via CLI
Network telemetry output
RTOS/QNX adaptation
Hardware ADC integration

