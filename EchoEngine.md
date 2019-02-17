# EchoEngine



| Simultaneous Processes | Time Taken (average over 10 requests) |
| ------ | ------ |
| One | 12.4 microseconds|
| Two |31.2 microseconds|
| Three | 41 microseconds |
| Four |89.3 microseconds, spikes in the range of 2000 microseconds |
| Five | 220 microseconds, spikes in the range of 2000-3000 microseconds |

In the above case, I sent continuous requests through the clients. With simultaneous processes, I observed that some requests were answered almost immediately (8-12ms), most were in the average range (50-80ms), while there were spikes in the response time(>1000ms). The number of spikes increased with the number of simultaneous processes.

