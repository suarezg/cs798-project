import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys


algorithms = ['AVL', 'Linked List', 'RedBlack']
threads = [1, 2, 4, 8, 16]
workloads = [(50,50), (0,0)]
keyRange = [10000]
csv_files = ['results_AVL.csv', 'results_LL.csv', 'results_RB.csv']


for workload in workloads:
    print(f'Workload: {workload}')
    insert = workload[0]
    delete = workload[1]

    avg_throughputs = dict()
    for algorithm,csv_file in zip(algorithms,csv_files):
        
        dataframe = pd.read_csv(csv_file)
        print(f'Processing {csv_file}...')
        avg_throughputs[algorithm] = []

        for range in keyRange:
            for threadCount in threads:
                filtered_df = dataframe.sort_values(by=['totalThreads'])
                filtered_df = filtered_df.loc[filtered_df['keyRangeSize'] == range]
                filtered_df = filtered_df.loc[filtered_df['totalThreads'] == threadCount]
                filtered_df = filtered_df.loc[filtered_df['insertPercent'] == insert]
                filtered_df = filtered_df.loc[filtered_df['deletePercent'] == delete]
                avg_tp = filtered_df['throughput'].mean()
                print(f'[RANGE] {range} [THREAD] {threadCount} [Insert] {insert} [Delete] {delete} [Trials] {len(filtered_df)} [Avg. Throughput] {avg_tp}')
                avg_throughputs[algorithm].append(avg_tp)   
                print(filtered_df)

            plt.plot(threads, avg_throughputs[algorithm], marker='x')

    plt.legend([f'{alg}' for alg in algorithms], loc='upper left')
    plt.title(f'Throughput vs Threads \n(KeyRange: {str(range)}, Workload I:{insert}%, D:{delete}%' + ')')
    plt.xlabel('Number of threads')
    plt.ylabel('Throughput')
    plt.grid()
    plt.savefig(f'Workload_I_{insert}_D_{delete}_TP.png')
    plt.show()

        

