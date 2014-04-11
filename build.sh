#!/bin/bash
g++ -std=c++11 md5.cpp wordlist_cracker.cpp -o wordlist_cracker
./wordlist_cracker --help
