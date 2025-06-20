#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float arrival;        // 到达时间
    float burst;          // 运行时间
    float start;          // 开始时间
    float finish;         // 完成时间
    float tat;            // 周转时间
    float wtat;           // 带权周转时间
} Process;

// 比较函数用于排序（按到达时间升序）
int compare(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return (p1->arrival > p2->arrival) - (p1->arrival < p2->arrival);
}

int main() {
    int n;
    printf("请输入进程数量: ");
    scanf("%d", &n);

    Process *proc = (Process*)malloc(n * sizeof(Process));

    // 输入进程数据
    printf("请输入%d组到达时间和运行时间（用空格分隔）:\n", n);
    for (int i = 0; i < n; i++) {
	printf("输入进程 %d 的到达时间和运行时间（空格分隔）: ", i+1);
        scanf("%f %f", &proc[i].arrival, &proc[i].burst);
    }

    // 按到达时间排序（FCFS核心步骤）
    qsort(proc, n, sizeof(Process), compare);

    // 计算调度时间
    float current_time = 0;
    for (int i = 0; i < n; i++) {
        proc[i].start = (current_time > proc[i].arrival) ? current_time : proc[i].arrival;
        proc[i].finish = proc[i].start + proc[i].burst;
        proc[i].tat = proc[i].finish - proc[i].arrival;
        proc[i].wtat = proc[i].tat / proc[i].burst;
        current_time = proc[i].finish;
    }

    // 打印结果表头（带竖线分隔）
    printf("\n%-10s|%-10s|%-10s|%-10s|%-10s|%-15s\n", 
       "到达时间", "运行时间", "开始时间", 
       "完成时间", "周转时间", "带权周转时间");
       
    // 打印分隔线（增强表格效果）
    printf("-----------|-----------|-----------|-----------|-----------|-------------\n");

    // 打印每个进程的结果（带竖线对齐）
    float total_tat = 0, total_wtat = 0;
    for (int i = 0; i < n; i++) {
    	printf("%-10.2f|%-10.2f|%-10.2f|%-10.2f|%-10.2f|%-15.2f\n",
           proc[i].arrival,
           proc[i].burst,
           proc[i].start,
           proc[i].finish,
           proc[i].tat,
           proc[i].wtat);
	total_tat += proc[i].tat;
        total_wtat += proc[i].wtat;
    }
    printf("\n平均周转时间: %.2f\n", total_tat / n);
    printf("平均带权周转时间: %.2f\n", total_wtat / n);
    free(proc);
    return 0;
}
