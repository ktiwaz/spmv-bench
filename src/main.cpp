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
    Matrix mtx(argc, argv);
    mtx.format();
    

    struct perf_event_attr pe_cycles, pe_instructions, pe_cache_refs, pe_cache_misses, pe_branches, pe_branch_misses;
    memset(&pe_cycles, 0, sizeof(struct perf_event_attr));
    memset(&pe_instructions, 0, sizeof(struct perf_event_attr));
    memset(&pe_cache_refs, 0, sizeof(struct perf_event_attr));
    memset(&pe_cache_misses, 0, sizeof(struct perf_event_attr));
    memset(&pe_branches, 0, sizeof(struct perf_event_attr));
    memset(&pe_branch_misses, 0, sizeof(struct perf_event_attr));

    // Common settings
    struct perf_event_attr base_pe = {
        .type = PERF_TYPE_HARDWARE,
        .size = sizeof(struct perf_event_attr),
        .disabled = 1,
        .exclude_kernel = 0,
        .exclude_hv = 1
    };

    // Configure events
    pe_cycles = base_pe;
    pe_cycles.config = PERF_COUNT_HW_CPU_CYCLES;

    pe_instructions = base_pe;
    pe_instructions.config = PERF_COUNT_HW_INSTRUCTIONS;

    pe_cache_refs = base_pe;
    pe_cache_refs.config = PERF_COUNT_HW_CACHE_REFERENCES;

    pe_cache_misses = base_pe;
    pe_cache_misses.config = PERF_COUNT_HW_CACHE_MISSES;

    pe_branches = base_pe;
    pe_branches.config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;

    pe_branch_misses = base_pe;
    pe_branch_misses.config = PERF_COUNT_HW_BRANCH_MISSES;

    // Open event file descriptors
    int fd_cycles = perf_event_open(&pe_cycles, 0, -1, -1, 0);
    int fd_instructions = perf_event_open(&pe_instructions, 0, -1, -1, 0);
    int fd_cache_refs = perf_event_open(&pe_cache_refs, 0, -1, -1, 0);
    int fd_cache_misses = perf_event_open(&pe_cache_misses, 0, -1, -1, 0);
    int fd_branches = perf_event_open(&pe_branches, 0, -1, -1, 0);
    int fd_branch_misses = perf_event_open(&pe_branch_misses, 0, -1, -1, 0);

    if (fd_cycles == -1 || fd_instructions == -1 || fd_cache_refs == -1 || fd_cache_misses == -1 || 
        fd_branches == -1 || fd_branch_misses == -1) {
        perror("perf_event_open");
        return 1;
    }

    // Reset and enable counters
    ioctl(fd_cycles, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd_instructions, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd_cache_refs, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd_cache_misses, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd_branches, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd_branch_misses, PERF_EVENT_IOC_RESET, 0);

    ioctl(fd_cycles, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(fd_instructions, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(fd_cache_refs, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(fd_cache_misses, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(fd_branches, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(fd_branch_misses, PERF_EVENT_IOC_ENABLE, 0);

    mtx.benchmark();

    // Disable counters
    ioctl(fd_cycles, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(fd_instructions, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(fd_cache_refs, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(fd_cache_misses, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(fd_branches, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(fd_branch_misses, PERF_EVENT_IOC_DISABLE, 0);

    // Read and print results
    long long cycles, instructions, cache_refs, cache_misses, branches, branch_misses;
    read(fd_cycles, &cycles, sizeof(long long));
    read(fd_instructions, &instructions, sizeof(long long));
    read(fd_cache_refs, &cache_refs, sizeof(long long));
    read(fd_cache_misses, &cache_misses, sizeof(long long));
    read(fd_branches, &branches, sizeof(long long));
    read(fd_branch_misses, &branch_misses, sizeof(long long));

    printf("CPU Cycles:        %lld\n", cycles);
    printf("Instructions:      %lld\n", instructions);
    printf("Cache References:  %lld\n", cache_refs);
    printf("Cache Misses:      %lld (%.2f%% miss rate)\n", cache_misses,
           cache_refs ? (100.0 * cache_misses / cache_refs) : 0);
    printf("Branch Instructions: %lld\n", branches);
    printf("Branch Misses:       %lld (%.2f%% miss rate)\n", branch_misses,
           branches ? (100.0 * branch_misses / branches) : 0);

    // Close file descriptors
    close(fd_cycles);
    close(fd_instructions);
    close(fd_cache_refs);
    close(fd_cache_misses);
    close(fd_branches);
    close(fd_branch_misses);

    mtx.verify();
    mtx.report();
    return 0;
}

