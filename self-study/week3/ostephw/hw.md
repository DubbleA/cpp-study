### Ch 4
[github] (https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/cpu-intro/README.md)

1. Run `python3 process-run.py -l 5:100,5:100` What should the CPU utilization be (e.g., the percent of time the CPU is in use?) 

the process we specified is "5:100" which means it should consist of 5 instructions, and the chances that each instruction is a CPU instruction are 100%.

That means CPU utilization will be **100%** since the CPU will always be running something, we know this because there are no I/O breaks. 

2. Now run `python3 process-run.py -l 4:100,1:0` These flags specify one process with 4 instructions (all to use the CPU), and one that simply issues an I/O and waits for it to be done. How long does it take to complete both processes? 

Default I/O length is 5 ticks, so we will need 4 ticks to run process 0, 1 tick to run process 1, 5 ticks to run I/O, and a final tick to unblock the cpu and mark I/O as done for a total of 11 ticks

3. Now run `python3 process-run.py -l 1:0,4:100` What happens now? Does switching the order matter? Why?

Now that we are starting with the I/O it will take 1 tick to run process 0, 5 ticks to do the I/O and mark process 0 as blocked. While process 0 is blocked, we can run process 1 `4:100` in 4 ticks and it will finish before i/o finishes. then we have a final tick to mark I/O done. So in total we have `1 + 5 + 1 = 7`.

4. With the flag set to SWITCH ON END, the system will NOT switch to another process while one is doing I/O, instead waiting until the process is completely finished. What happens when you run the following two processes (`python3 process-run.py -l 1:0,4:100 -c -S SWITCH_ON_END`), one doing I/O and the other doing CPU work?

Since it will not overlap it will be 7 ticks for the I/O process + 4 for the CPU process = 11 ticks. 

5. Now run `python3 process-run.py -l 1:0,4:100 -c -S SWITCH_ON_IO` where we do switch to another process whenever one is waiting for I/O. 

This will be 7 ticks same as q3. We start our io, 5 ticks of I/O (while 4 ticks of CPU process runs), final tick of io done. 

6. One other important behavior is what to do when an I/O completes. With -I IO RUN LATER, when an I/O completes, the process that issued it is not necessarily run right away; rather, whatever was running at the time keeps running. What happens when you run this combination of processes? (`python3 process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -c -p -I IO_RUN_LATER`) Are system resources being effectively utilized?

so we have 3 instructions that all use io each, and 3 instructions that have cpu io each. To be efficient we could run all processes in 21 ticks. start io -> 5 ticks io (cpu process 1) + done io, 3. However since we run a process that keeps running, we will have 15 ticks of cpu process running before the first i/o is done. giving us a total of 31 ticks. 

7. run `python3 process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -c -p -I IO_RUN_IMMEDIATE`

this gives us our 21 tick run. Running a process that just completed an I/O again could be a good idea because we could run another process during the I/O block. also if a process is likely to use an i/o once its likely to use it again. 

8. Now run with some randomly generated processes using flags -s 1 -l 3:50,3:50 or -s 2 -l 3:50,3:50 or -s 3 -l 3:50, 3:50. See if you can predict how the trace will turn out. What happens when you use the flag -I IO RUN IMMEDIATE versus that flag -I IO RUN LATER? What happens when you use the flag -S SWITCH ON IO versus -S SWITCH ON END?

SWITCH_ON_IO always results in faster run times than SWITCH_ON_END, because we spend less time waiting on I/O to finish as we can run another process in the bg

### CH 5

1. Run `python3 fork.py -s 10` and see which actions are taken. Can you
predict what the process tree looks like at each step? Use the -c
flag to check your answers. Try some different random seeds (-s)
or add more actions (-a) to get the hang of it.

2. One control the simulator gives you is the fork percentage, controlled by the -f flag. The higher it is, the more likely the next
action is a fork; the lower it is, the more likely the action is an
exit. Run the simulator with a large number of actions (e.g., -a 100) and vary the fork percentage from 0.1 to 0.9. What do you
think the resulting final process trees will look like as the percentage changes? Check your answer with -c.
`python3 fork.py -a 100 -f 0.5`

3. now run `python3 fork.py -t` and guess which actions were taken


ngl atp i just reviewed the hws from this guy:
https://github.com/asnr/ostep/blob/master/virtualisation/6_cpu_mechanisms/README.md
