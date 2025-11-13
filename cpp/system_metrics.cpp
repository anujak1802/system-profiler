#include <iostream>
#include <fstream>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <unistd.h>

double getCPUUsage() {
    host_cpu_load_info_data_t cpuinfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count);
    unsigned long long totalTicks = 0;
    for (int i = 0; i < CPU_STATE_MAX; i++) totalTicks += cpuinfo.cpu_ticks[i];
    return 100.0 * (1.0 - ((double)cpuinfo.cpu_ticks[CPU_STATE_IDLE]) / totalTicks);
}

double getMemoryUsage() {
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    vm_statistics64_data_t vmstat;
    host_statistics64(mach_host_self(), HOST_VM_INFO, (host_info64_t)&vmstat, &count);
    double used = (vmstat.active_count + vmstat.wire_count) * sysconf(_SC_PAGE_SIZE);
    double total = (vmstat.active_count + vmstat.inactive_count + vmstat.wire_count + vmstat.free_count) * sysconf(_SC_PAGE_SIZE);
    return (used / total) * 100.0;
}

int main() {
    std::ofstream log("metrics.log", std::ios::app);
    for (int i = 0; i < 5; ++i) {   // <-- run only 5 iterations
        double cpu = getCPUUsage();
        double mem = getMemoryUsage();
        log << cpu << "," << mem << std::endl;
        sleep(2);
    }
    return 0;  // exits cleanly
}
