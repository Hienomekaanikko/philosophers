Philosophers is a project where we explore the topic of multithreading. The idea is, that we have a table surrounded by philosophers, and each has one fork. 
The program is ran like this: ./philo 2 800 200 200 5. 

2 = amount of philosophers
800 = time to die (ms)
200 = time to eat (ms)
200 = time to sleep (ms)
5 = amount of times that must be eaten (voluntary input)

Alright, sounds simple? Well, lets add some complexity. 

1. The philosopher must have 2 forks to eat (and the extra one must be from another philosopher on the left or right.)
2. If "time to die" runs out, a philo dies.
3. There cannot be data-races, meaning that only one thread can access same resources at once.
4. Philos cannot communicate with each other. They cannot know if one philo is about to die.

So how we tackle the main problems? So I decided to assign each philosopher the left fork as their own, and then 
to every odd number of philosophers, the right fork was a pointer to the fork of the guy on the right side. For 
even number of philosophers I assigned the right fork to be from the philo on the left. (crossing arms basically). The 
reason for that was to cause some inconsistency to prevent deadlocks, which could happen if suddenly theres a situation
where every philo would be trying to grab forks at same time. That would basically kill the simulation. Each forks type 
is pthread_mutex, which means that if we lock a mutex with pthread_mutex_lock(&philo[i].left_fork), that same fork cannot 
be used by another one until that is unlocked with pthread_mutex_unlock(&philo[i].left_fork). 

I also made a sort of waiting room, that makes sure that only n-1 philosophers are eating at once, preventing deadlocks.
Also every odd philosopher pick left work first and right fork first, and even philosophers do it in opposite way. If 
the total number of philosophers is odd, the "thinking time" will be at minimum 75% of (time to die - time to sleep - time to eat). 
If the total number of philosophers is even, the "thinking time" will be at minimum 15% of (time to die - time to sleep - time to eat). 
That was something i just tried out as there were unnecessary deaths taking place in certain cases. 15% was pretty much the only number
that worked for the even. 

How is the simulation stopped when one dies, or is successfully ended? There is a loophole, which is "monitoring thread". It's created to 
constantly loop through the existing philosphers and calculating if their time has ran out. If it has, then a "stop simulation" flag will be
triggered and every philo will exit their respective routines immediately. 

In the end of the program we clean the memory and kill the threads so that valgrind has nothing evil to say about my processes. 

To be fair, I didn't enjoy this too much right after the Minishell which was highly demanding project, but now after I passed it, it was 
a worthwhile knowledge to gain! Now I know how to utilize processes that use same bunch of resources at the same time without dataracing. 
I already knew about forks and child processes but those are different because they also make copies of the structs and everything so 
one child cannot really use data of another child until one child has finished it's run. 

Thanks for reading and on to the next one!

