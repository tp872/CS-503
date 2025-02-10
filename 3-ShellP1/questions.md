1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**: For this use, fgets() is perfect since it limits the input size, so preventing buffer overflow and safely reads user input including spaces. It guarantees that the whole line is caught, returns NULL when input ends, and correctly manages end-of- file (EOF). This is a safe and quick option for reading commands in a shell program since string manipulation tools like strcspn() let one easily remove trailing newlines 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:   The utilization of malloc() for cmd_buff facilitates dynamic memory allocation, enabling the program to accommodate commands of varying lengths without incurring memory wastage or being constrained by a predetermined size. In contrast to a fixed-size array, malloc() offers the adaptability to modify the buffer size as required, thereby optimizing memory utilization for variable input dimensions.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Eliminating leading and trailing spaces guarantees the shell accurately recognizes the command and its parameters. Failure to trim excess spaces may lead to misinterpretation, resulting in errors or erroneous command execution.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**: Redirecting Standard Output to a File: Turning a command's output toward a file lets you save it for later use. This commands saves the output to file_list.txt and lists directory contents
     
    Challenge: This calls for opening the designated file for writing and making sure that, depending on the user's intention, current material is either rewritten or appended.

    Redirecting Standard Error to a File: Debugging and logging benefit from separate file containing capture of error messages. This command tries to list a nonexistent directory and points the error message toward error_log.txt.
    Challenge: Careful file stream management is necessary to properly handle file descriptors capturing only error messages without influencing standard output.

    Redirecting Both Standard Output and Standard Error to the Same File: Comprehensive logging benefits from combining outputs into one file. This command points standard error and output both to all_output.txt.
    Challenge: One must grasp the sequence of redirection. First the 2&1 syntax sends standard error to standard output; subsequently, standard output is sent to the file. Misreading might produce unexpected outcomes.
    
- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Purpose: 
   For a command, redirection adjusts the source or destination of input/output. Rather than the default streams (stdout or stdin), it entails forwarding the output of a command to a file or receiving input from a file.
    
    Piping lets one command's output "flow" straight to the input of another, so enabling the output of one command to directly influence another.

    How They Work:
    Redirection is the process of sending output to a file or receiving input from a file by utilizing symbols such as > (output redirection), < (input redirection), and 2> (error redirection).
    For example, the command "ls > file_list.txt" forwards the output of "ls" to a file.
    
    The pipe symbol | is employed in piping to transfer the output of one command as input to another. It does not directly interact with files.
    For example, the command ls | grep "txt" transfers the output of ls to grep for additional filtering.

    Handling Multiple Commands:
   Redirection typically alters the input or output behavior of a single command at a time.

    Piping facilitates the connection of numerous commands, enabling the creation of intricate chains of commands in which each command operates on the output of the previous one.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is crucial to distinguish between STDOUT (regular output) and STDERR (error messages) in order to facilitate flexible logging, facilitate debugging, and improve error handling. It enables the management of normal output and errors in a separate manner, such as redirecting them to separate files or processing them separately in scripts. Control and clarity are enhanced by this separation when managing shell outputs.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Our shell should be capable of capturing STDOUT and STDERR separately, but it should also be able to merge them if necessary using the 2>&1 syntax. This allows for unified output while still allowing for separate handling to facilitate more effective error tracking.