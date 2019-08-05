import multiprocessing
import os


def busy():
    n = 0
    while True:
        n += 1

if __name__ == '__main__':
    jobs = []
    n_processes = 15
    
    for i in range(n_processes):
        p = multiprocessing.Process(target=busy)
        jobs.append(p)      
        p.start()
        os.system("taskset -p -c %d %d" % ((i % os.cpu_count()), p.pid))

    for p in jobs:
        p.join()