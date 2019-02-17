# DatabaseEngine



| Simultaneous Processes | Time Taken (average over 5 requests) |
| ------ | ------ |
| One | 211.4 microseconds|
| Two |240.8 microseconds|
| Three | 248.75 microseconds |
| Four |271 ms |
| Five | 266.1 ms |

In the above case, the numebr of processes is very less, hence there is not a wide variation that is observed. In all the above processes, the initial values of time taken were very large (1000-1500ms) which stabilised to values closer to the average. Also, similar to the echoEngine case, I observed spikes in values. 

