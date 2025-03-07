#include "matrix.h"
#define _GNU_SOURCE
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

// Helper function to initialize a perf event
int setup_perf_event(struct perf_event_attr *pe, int config, int group_fd) {
    memset(pe, 0, sizeof(struct perf_event_attr));
    pe->type = PERF_TYPE_HARDWARE;
    pe->size = sizeof(struct perf_event_attr);
    pe->config = config;
    pe->disabled = 1;
    pe->exclude_kernel = 1;
    pe->exclude_hv = 1;

    int fd = perf_event_open(pe, 0, -1, group_fd, 0);
    if (fd == -1) {
        fprintf(stderr, "Error opening perf event: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fd;
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

    struct perf_event_attr pe;
    
    // Open multiple events
    int cycles_fd = setup_perf_event(&pe, PERF_COUNT_HW_CPU_CYCLES, -1);
    int instructions_fd = setup_perf_event(&pe, PERF_COUNT_HW_INSTRUCTIONS, cycles_fd);
    int branches_fd = setup_perf_event(&pe, PERF_COUNT_HW_BRANCH_INSTRUCTIONS, cycles_fd);
    int branch_misses_fd = setup_perf_event(&pe, PERF_COUNT_HW_BRANCH_MISSES, cycles_fd);

    // Get initial system resource usage
    struct rusage usage_start, usage_end;
    getrusage(RUSAGE_SELF, &usage_start);

    // Start measuring time
    double start_time = get_time();
    
    // Enable counters
    ioctl(cycles_fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(cycles_fd, PERF_EVENT_IOC_ENABLE, 0);


    mtx.benchmark();

    // Disable counters
    ioctl(cycles_fd, PERF_EVENT_IOC_DISABLE, 0);

    // Stop measuring time
    double end_time = get_time();
    
    // Read values
    long long cycles, instructions, branches, branch_misses;
    read(cycles_fd, &cycles, sizeof(long long));
    read(instructions_fd, &instructions, sizeof(long long));
    read(branches_fd, &branches, sizeof(long long));
    read(branch_misses_fd, &branch_misses, sizeof(long long));

    // Get final system resource usage
    getrusage(RUSAGE_SELF, &usage_end);
    
    double elapsed_time = end_time - start_time;
    double user_time = (usage_end.ru_utime.tv_sec - usage_start.ru_utime.tv_sec) +
                       (usage_end.ru_utime.tv_usec - usage_start.ru_utime.tv_usec) / 1e6;
    double sys_time = (usage_end.ru_stime.tv_sec - usage_start.ru_stime.tv_sec) +
                      (usage_end.ru_stime.tv_usec - usage_start.ru_stime.tv_usec) / 1e6;

    // Print statistics
    printf("\nPerformance counter stats for './your_program':\n\n");
    printf("%16.2f msec task-clock                       #    %.3f CPUs utilized\n", elapsed_time * 1000, 1.0);
    printf("%16lld      cpu-cycles                      #    %.2f GHz\n", cycles, cycles / (elapsed_time * 1e9));
    printf("%16lld      instructions                     #    %.2f  insn per cycle\n", instructions, (double)instructions / cycles);
    printf("%16lld      branches                         #    %.2f M/sec\n", branches, branches / (elapsed_time * 1e6));
    printf("%16lld      branch-misses                    #    %.2f%% of all branches\n", branch_misses, 100.0 * branch_misses / branches);
    printf("\n%16.9f seconds time elapsed\n", elapsed_time);
    printf("%16.9f seconds user\n", user_time);
    printf("%16.9f seconds sys\n", sys_time);

    // Close FDs
    close(cycles_fd);
    close(instructions_fd);
    close(branches_fd);
    close(branch_misses_fd);
    
    mtx.verify();
    mtx.report();
    
    return 0;
}

