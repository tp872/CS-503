1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_answer here_ By looking for an EOF character (`RDSH_EOF_CHAR`), a fixed-length header, or a delimiter like `"END_OF_OUTPUT"`, the client can determine when a command's output is complete.  It must loop over `recv()` until all expected data is received because data can arrive in chunks.  The best way to ensure full command output with your `dsh` shell is to continue reading until `RDSH_EOF_CHAR` appears.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_answer here_ Message boundaries must be defined by a shell using delimiters, a fixed-length header, or an EOF marker because TCP does not maintain them.  Commands may split, merge, or block in the absence of this.  Reliable execution is ensured by well-defined boundaries.

3. Describe the general differences between stateful and stateless protocols.

_answer here_ A stateless protocol, like HTTP, handles each request separately, whereas a stateful protocol, like TCP or FTP, remembers previous exchanges.  Stateless is easier to use and scales better than stateful, which requires more resources but is seamless.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_answer here_ Because it doesn't require connection setup or retransmissions like TCP, UDP is employed because it is quick, lightweight, and has minimal latency.  It's perfect for real-time applications like VoIP, DNS, online gaming, and video streaming where speed is more important than dependability.  These apps don't require the overhead of TCP to deal with sporadic packet loss. 

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_answer here_ The socket API is an abstraction for network communication offered by the operating system.  Applications can use socket(), bind(), listen(), connect(), send(), and recv() to establish, send, receive, and manage network connections.  This eliminates the requirement for low-level networking knowledge and permits smooth communication over protocols like TCP and UDP.