#!/bin/bash

for i in $(ls | grep "mata" | grep -o ".*\.");
	do
		make $i"svg" $i"out";
	done;
