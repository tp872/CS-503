1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  The child process can be replaced with a different program using execvp, and a new process can be created using fork. The benefits of employing a fork include concurrent execution, process control, and the ability to keep the parent process unaffected.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  Fork() returns -1 in the event that it fails, signifying an error brought on by memory or resource constraints. Check this return value, and if it's negative, use perror() to print an error and exit(1) to end the process gracefully. You are in the child process if the return value is zero, and the parent process if it is positive. When handled correctly, system resource problems don't affect the program's stability.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  By looking through the folders specified in the PATH environment variable, execvp() locates the command. In the absence of a path, the command looks for a matching executable in each directory in PATH. It returns an error if there isn't a match.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The parent process can avoid the child becoming a zombie process, collect the child's exit status, and wait for the child to finish by calling wait(). In the absence of wait(), the child's resources wouldn't be cleaned up, and the parent might proceed without knowing the child's completion state.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  The child process's exit status—whether it ended successfully or with an error—is retrieved by WEXITSTATUS(). The parent should be aware of the child's termination outcome so that they can react appropriately.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  When handling quoted parameters, build_cmd_buff() treats spaces inside quotes as if they were a single argument. In order to ensure appropriate command parsing, this is required to prevent mistakenly dividing parameters that contain spaces.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I modified the parsing mechanism to appropriately handle special characters and quoted arguments, which weren't covered in the prior assignment. Keeping efficiency while handling edge circumstances like escape characters and nested quotes proved difficult.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are used in Linux systems to alert processes to events such as interruptions or terminations. Signals are asynchronous and lightweight in contrast to other IPC techniques, emphasizing control actions above data interchange.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL: This signal stops a process right away without giving it time to adjust. It is employed when a process must be abruptly terminated and the signal cannot be ignored or caught. Usually, it's employed to terminate blocked or unresponsive processes.

    SIGTERM: A process that receives this signal is asked to end gracefully so that it can clear its resources before doing so. It is helpful for orderly shutdowns, like when shutting a server or application, because it may be detected and managed by the process.


    SIGINT: SIGINT pauses a process and requests that it end when the user presses Ctrl+C in the terminal. In an interactive session, it's frequently used to end an active command or process in the terminal.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  Unlike SIGINT, SIGSTOP causes a process to pause and is not catchable, blockable, or ignorable. This guarantees that the process is consistently halted until it receives SIGCONT to continue.
