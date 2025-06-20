#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int pid;         // 进程编号
    float arrive;    // 到达时间
    float run;       // 运行时间
    float start;     // 开始时间
    float finish;    // 完成时间
    float tat;       // 周转时间
    float wtat;      // 带权周转时间
} Process;

// 输入函数与FCFS保持一致
void input_processes(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        printf("输入进程 %d 的到达时间和运行时间（空格分隔）: ", i+1);
        scanf("%f %f", &proc[i].arrive, &proc[i].run);
        proc[i].pid = i + 1;
    }
}

// SJF调度核心逻辑
void sjf_schedule(Process proc[], int n) {
    bool *completed = (bool*)calloc(n, sizeof(bool));
    float current_time = 0;
    int count = 0;

    while (count < n) {
        int next = -1;
        float min_burst = __FLT_MAX__;
        float earliest_arrive = __FLT_MAX__;

        // 阶段一：寻找当前可执行的最短作业
        for (int i = 0; i < n; i++) {
            if (!completed[i] && proc[i].arrive <= current_time) {
                if (proc[i].run < min_burst) {
                    min_burst = proc[i].run;
                    next = i;
                }
            }
        }

        // 阶段二：处理CPU空闲情况
        if (next == -1) {
            for (int i = 0; i < n; i++) {
                if (!completed[i] && proc[i].arrive < earliest_arrive) {
                    earliest_arrive = proc[i].arrive;
                    next = i;
                }
            }
            current_time = earliest_arrive;
        }

        // 执行选中的进程
        proc[next].start = (current_time > proc[next].arrive) ? current_time : proc[next].arrive;
        proc[next].finish = proc[next].start + proc[next].run;
        proc[next].tat = proc[next].finish - proc[next].arrive;
        proc[next].wtat = proc[next].tat / proc[next].run;
        
        current_time = proc[next].finish;
        completed[next] = true;
        count++;
    }
    free(completed);
}

// 输出函数与FCFS完全一致
void print_results(Process proc[], int n) {
    printf("\n进程ID | 到达时间 | 运行时间 | 开始时间 | 完成时间 | 周转时间 | 带权周转时间\n");
    printf("-------+----------+----------+----------+----------+----------+------------\n");
    
    float total_tat = 0, total_wtat = 0;
    for (int i = 0; i < n; i++) {
        printf("%6d | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %10.2f\n",
               proc[i].pid,
               proc[i].arrive,
               proc[i].run,
               proc[i].start,
               proc[i].finish,
               proc[i].tat,
               proc[i].wtat);
        
        total_tat += proc[i].tat;
        total_wtat += proc[i].wtat;
    }
    
    printf("\n平均周转时间: %.2f\n", total_tat / n);
    printf("平均带权周转时间: %.2f\n", total_wtat / n);
}

int main() {
    int n;
    printf("输入进程数量: ");
    scanf("%d", &n);
    
    Process *proc = (Process*)malloc(n * sizeof(Process));
    
    input_processes(proc, n);    // 统一输入
    sjf_schedule(proc, n);       // SJF调度
    print_results(proc, n);      // 统一输出
    
    free(proc);
    return 0;
}
