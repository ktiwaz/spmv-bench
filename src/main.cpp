#include "matrix.h"
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
    return syscall(SYS_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

// Function to get time in seconds
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main(int argc, char **argv) {
    // Initialize the Matrix object and format it
    Matrix mtx(argc, argv);
    mtx.format();

    // Initialize base perf_event_attr for events
    struct perf_event_attr base_pe = {
        .size = sizeof(struct perf_event_attr),
        .disabled = 1,
        .exclude_kernel = 0,
        .exclude_hv = 1
    };

    // List of events to configure
    unsigned long long event_configs[] = {
        PERF_COUNT_HW_CPU_CYCLES,
        PERF_COUNT_HW_INSTRUCTIONS,
        PERF_COUNT_HW_CACHE_REFERENCES,
        PERF_COUNT_HW_CACHE_MISSES,
        PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
        PERF_COUNT_HW_BRANCH_MISSES,
        PERF_COUNT_SW_PAGE_FAULTS,
        (PERF_COUNT_HW_CACHE_DTLB) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16),
        (PERF_COUNT_HW_CACHE_DTLB) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16),
        (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16),
        (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16),
    };

    // File descriptors for each event
    int event_fds[13];

    // Configure and open file descriptors for each event
    for (int i = 0; i < 11; ++i) {
        struct perf_event_attr pe = base_pe;
        pe.config = event_configs[i];

        if (i < 6) {
            pe.type = PERF_TYPE_HARDWARE;
        }
        else if(i == 6) {
            pe.type = PERF_TYPE_SOFTWARE;
        } else if(i > 6) {
            pe.type = PERF_TYPE_HW_CACHE;
        }

        // Open the file descriptor for the event
        event_fds[i] = perf_event_open(&pe, 0, -1, -1, 0);

        // Check for errors
        if (event_fds[i] == -1) {
            perror("perf_event_open");
            return 1;
        }
    }

    // Reset and enable the counters
    for (int i = 0; i < 13; ++i) {
        ioctl(event_fds[i], PERF_EVENT_IOC_RESET, 0);
        ioctl(event_fds[i], PERF_EVENT_IOC_ENABLE, 0);
    }

    // Run the benchmark
    mtx.benchmark();

    // Disable the counters after benchmarking
    for (int i = 0; i < 13; ++i) {
        ioctl(event_fds[i], PERF_EVENT_IOC_DISABLE, 0);
    }

    // Read the results for each event and print them
    long long values[13];
    for (int i = 0; i < 13; ++i) {
        if (read(event_fds[i], &values[i], sizeof(long long)) < 0) {
            perror("read");
            return 1;
        }
    }

    // Print the results
    printf("CPU Cycles:            %lld\n", values[0]);
    printf("Instructions:          %lld\n", values[1]);
    printf("Cache References:      %lld\n", values[2]);
    printf("Cache Misses:          %lld (%.2f%% miss rate)\n", values[3], values[2] ? (100.0 * values[3] / values[2]) : 0);
    printf("Branch Instructions:   %lld\n", values[4]);
    printf("Branch Misses:         %lld (%.2f%% miss rate)\n", values[5], values[4] ? (100.0 * values[5] / values[4]) : 0);
    printf("DTLB Loads:            %lld\n", values[7]);
    printf("DTLB Misses:           %lld (%.2f%% miss rate)\n", values[8], values[7] ? (100.0 * values[8] / values[7]) : 0);
    printf("Page Faults:           %lld (%.2f%% fault rate)\n", values[6], values[1] ? (100.0 * values[6] / values[1]) : 0);
    printf("L1D Cache Loads:       %lld\n", values[9]);
    printf("L1D Cache Misses:      %lld (%.2f%% miss rate)\n", values[10], values[9] ? (100.0 * values[10] / values[9]) : 0);

    // Close the file descriptors
    for (int i = 0; i < 13; ++i) {
        close(event_fds[i]);
    }

    mtx.verify();
    mtx.report();

    return 0;
}

