## Info
Uses a Tcp Client/Server to trade ICMP packets from a self-made Icmp class.

## Usage
Build the files with Cmake.

## Server
In the build folder, execute the commmand:
```sh
./server <port>
```
to open a server listening to tcp connections with icmp packets as content.

## Client
In the build folder, execute the command:
```sh
./client <hostname> <port>
```
to start a client requesting connetion to hostname at the given port, this client sends ICMP  echo packets and expects echo replies.

## Example usage
![example usage](https://i.imgur.com/l19Z8zW.png)

## Note
As of now the program is not accepting the alias localhost, so use 127.0.0.1
