# OVERVIEW

# GDB

**GDB does not run the program for you in a special way.**

It creates or attaches to a process and then uses Linux debugging mechanisms to:

- run the process
- stop the process
- read registers
- read memory
- set breakpoints
- monitor memory
- retrieve the stack
- debug threads/processes

To use GBD tocheck the program, firstly, we must compile the program using this command:

```bash
gcc -g -O0 program.c -o program
```

Why we need `-g` in command ?

If I remove `-g` from this command, the binary file primarily contains machine code, CPU does not know the function or variable names.

Using `-g` parameter allows us to see the **debug informations**.

What about `-O0`?

When compile, the compiler can optimize the variable. If we want to source code run correspond to the execution process, use `-O0`.

Example:

```c
int x = 10;
int y = 20;
int z = x + y;
```

`-g` keep the debug informations and `-O0` does not optimize `z` variable to 30.

Example simply program:

```c
/*
function that subtracts 'a' until it reaches to 0
*/
int subtract(int *a)
{
    while (*a != 0)
    {
        (*a)--;
    }
}

/*
function that compute the sum of `a` and `b`
*/
int add(int a, int b)
{
    int result = a + b;
    return result;
}

int main(int argc, char *argv[])
{
    int a = 10;
    int b = 100;

    /* subtract b until to 0*/
    subtract(&b);

    /* print out the result */
    printf("Result: %d\n", add(a, b));

    return 0;
}
```

I will use `gbd` to check this program.

First compile this program:

```bash
gdb -g -O0 test.c -o test
```

To run with `gbd`, use the command bellow:

```bash
gdb ./test
```

We will see: *(gdb)*. This is **gdb prompt.**

Use `run` or `r` to run the program normally then finished.

```text
Starting program: /home/hungubuntu/Vim_C_code/session_6/test 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
Result: 10
[Inferior 1 (process 40442) exited normally]
```

*Some gdb prompt is commonly useed:*

|Prompt|Effect|
|:---|:---|
|run (r)|run the program|
|break (p)|set a break point with function name|
|delete|delete all breakpoint|
|disable 2|temporarily disable breakpoint 2|
|enable 2|enable breakpoint 2|
|tbreak|set a temporary breakpoint|
|next (n)|perform a step at the source code level|
|step (s)|perform a step that can enter into the function|
|print (p)|see the value of variable or return function called in context of current function|
|info locals|see all local variables of current stack frame or those matching REGEXPs|
|backtrace (bt)|see the function that gdb current stopped|
|finish|jump to return function|
|quit (q)|exit from gdb|

## Condition breakpoint

Example we have a function:

```c
    for (int i = 0; i < 100; i++)
    {
        printf("i = %d\n", i);
    }
```

And we want to set a breakpoint at line `printf`, everytime the program run to this line, it will be stopped. Accoding to the example above, it will be stopped 100 times and we don't want to this behave. We want to the program stop when `i = 50`. How do it?

We use:

```text
break 44 if i == 50
```

Where `44` is the number of the line we want to break, `if i == 50` is the condition of breakpoint.

If the breakpoint already existed, we can add condition to it by using:

```text
    condition 2 i == 50
```

`2` is the number of the breakpoint that we want to add condition.

## Ignore breakpoint

Example we have a program call a function 3 times:

```c
int  foo()
{
}

int main()
{
    foo();
    foo();
    foo();

    return 0;
}
```

We set a breakpoint at `foo` function and it can be called 3 times. We want to skip the first two times and want to stop at third, we can use:

```text
ignore 1 2
```

Where `1` is the number of breakpoint, `2` is the number of times we want to skip.

## See source code

```text
list

or

list 41
```

where `41` is the number of the line we want to see source code.

Example:

```c
(gdb) list 41
36	    int result = a + b;
37	    return result;
38	}
39	
40	int main(int argc, char *argv[])
41	{
42	    for (int i = 0; i < 10; i++)
43	    {
44	        printf("i = %d\n", i);
45	    }
```

## Watchpoint
