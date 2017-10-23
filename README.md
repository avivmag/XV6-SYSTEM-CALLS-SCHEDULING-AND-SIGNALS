# XV6-System-Calls-Scheduling-And-Signals

This repository extends [mit-pdos xv6 repository](https://github.com/mit-pdos/xv6-public).
It adds three improvements to the original implementation of xv6:
1.	Exit implicit call.
2.	Scheduling policies - Sanity tests are also included.
3.	Signals framework.

The development of this OS improvements was done as part of an assignment in "Operating system" course at Ben-Gurion University in the summer of 2016.

A detailed description of the implementation can be found in the assignment description attached.

## Exit implicit call

First improvement that is added is the missing support for implicit call to exit for a graceful termination of programs. It also store the exit status of the terminated process in the proc structure. When program terminates without system call to exit, the operating system adds a system call at the end of the code without informing the code developer.

## Scheduling policy

Second improvement that is added is that the scheduler now supplies three types of modified sub-policies. All of this sub-policies are based on lottery scheduler.</br>
Every process gets a number of tickets. The number of tickets is based on the current active sub-policy. The types of the sub-policies are:
*	Uniform time distribution:
	That means the tickets are distributed uniformly.
*	Priority scheduling:
	Every process receives amount of tickets based on its priority number. For example, process with priority of 5 receives 5 tickets.
*	Dynamic tickets allocation:
	This sub-policy approach is considered to be smarter than previous two.
	It tries to balance the amount of running time between each process.
	A newely created process will receive 20 tickets.
	Each time the process performs a system call, 10 tickets are added to the process's tickets amount - this way for a process which yields for kernel service via system call (asked for functions available only on kernel side), we will want him to receive some additional quanta.</br>
	Each time the process ends the quanta without performing a blocking system call, the amount of the tickets owned be the process will be reduced by 1 (to the minimum of 1) - processes which ends the quanta without system calls, means he ran full sprint toward its goal without stopping, and so, to balance this thread with the other ones we've adopted this policy.
	
A way to measure the performance of the above sub-policies is also included,
it measures the followings for every process:
*	process creation time.
*	process termination time.
*	the time the process spent on the SLEEPING state.
*	the time the process spent on the READY state.
*	the time the process spent on the RUNNING state.

You can run the sanity test added for better understanding of this policies.

## Signals framework

This improvement allows the processes minimal communication between them.
Signaling a process is only possible if a it has registered for receiving the same kind of the signal sent.

## Getting Started
### Prerequisites

1. Kubuntu - this program was tested only on kubuntu, but it probably can be ran on any other known nasm and gcc compatible operating systems.
	https://kubuntu.org/getkubuntu/</br>
2. QEMU 
	via ```sudo apt-get install qemu-system``` on ubuntu based os (kubuntu included).
3. GNU make
	https://www.gnu.org/software/make/
4. gcc compiler
	via ```sudo apt-get install gcc-4.8``` on ubuntu based os (kubuntu included).

### Simulating the process

1. open terminal and navigate to the program directory
2. type `make qemu` and press enter, the operating system should now boot on.
3. when shell is available, you can try the sanity test with `sanity` command, signal test with `sigtest` command, change scheduler policy via `policy <policy_number>` command when policy number can be 1, 2 or 3 for the policies mentioned above respectively (for better view of the available user commands use `ls` command). 
4. enjoy :).

## Built With

* [GNU make](https://www.gnu.org/software/make/) - A framework used for simple code compilation.
* [gcc](https://gcc.gnu.org/)
* [QEMU](https://www.qemu.org/)

## Useful links

* The original source of the assignment: https://www.cs.bgu.ac.il/~os163/Assignments/Assignment_1.
* https://en.wikipedia.org/wiki/Operating_system
* https://en.wikipedia.org/wiki/Xv6
* https://pdos.csail.mit.edu/6.828/2014/xv6/book-rev8.pdf
* https://en.wikipedia.org/wiki/Lottery_scheduling
* https://en.wikipedia.org/wiki/Signal_(IPC)
* https://en.wikipedia.org/wiki/Exit_(system_call)
