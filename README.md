# Echo Server

This is a simple echo server implemented in C that can handle multiple simultaneous clients.

## Compilation

To compile the echo server, use the provided Makefile:

```
make
```

This will create an executable named `echo_server`.

## Running the Server

To run the server, simply execute the compiled binary:

```
./echo_server
```

The server will start listening on port 8888.

## Testing the Server

You can test the server using telnet. Open a terminal and type:

```
telnet localhost 8888
```

Once connected, type any message and press Enter. The server should echo back the same message.

You can open multiple telnet sessions to test the server's ability to handle multiple clients simultaneously.

## Cleaning Up

To remove the compiled binary, run:

```
make clean
```

This will delete the `echo_server` executable.