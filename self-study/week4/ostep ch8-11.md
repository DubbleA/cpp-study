# Scheduling: The Multi-Level Feedback Queue

The Multi-Level Feedback Queue (MLFQ) tries to optimzie `turnaround time` and minimize `response time`. 
It does this by using the past to predict the future. Such approaches are common in operating systems (and many other places in Computer Science, including hardware branch predictors and caching algorithms).

How to schedule without perfect knowledge? How can we design a scheduler that both minimizes response time for
interactive jobs while also minimizing turnaround time without a priori knowledge of job length?

### 8.1 MLFQ: Basic Rules:

The MLFQ has a number of distinct **queues**, each assigned a different **priority level**. At any given time, a job that is ready to be run is on a single queue (some jobs may share the `same` priority and share the same queue). A job with a higher priority (i.e. on a higher queue) will be chosen to run. If there are multiple jobs in the same queue, we will utilize round robin scheduling to pick. 

Two basic rules for MLFQ:
- **Rule 1**: If Priority(A) > Priority(B), A runs (B doesn't)
- **Rule 2**: If Priority(A) == Priority(B), A & B run in RR

The scheduler sets priorities based on `variance` of `observed behavior` instead of a fixed priority for each job. MLFQ will try to `learn` about processes as they run, using the `history` of a job to predict its `future` behavior. For example, if a job repeatededly reliquishes CPU control while waiting for input from the keyboard, MLFQ will keep its prioirity high (to reduce response time for an interactive job). 

### 8.2 How to Change Priority

One large part of how MLFQ functions is how job priority (and the queue that it is on) `changes` over time. To keep track of our workload we will track a jobs **allotment**. (the amount of time a job can spend at a given priority level before the scheduler reduces its priority.)
