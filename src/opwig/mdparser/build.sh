#!/bin/bash

flags="--parsefun-source=mdparser-parse.cxx"
flags="$flags --baseclass-header=mdparserbase.h"
flags="$flags --class-header=mdparser.h"
flags="$flags --implementation-header=mdparser.ih"

if [ $1 == "--cleanall" ]
then
  rm mdparser-parse.cxx mdparserbase.h mdparser.h mdparser.ih
elif [ $1 == "--generate" ]
then
  bisonc++ $flags mdparser.gr
fi

