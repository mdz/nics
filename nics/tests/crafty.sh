#!/bin/sh

max=2
current=0

while true; do
  if [ $current -ge $max ]; then
    break;
  fi
  ./crafty.expect crafty$current &
  next=`expr $current + 1`
  ./crafty.expect crafty$next crafty$current &
  current=`expr $next + 1`
done
