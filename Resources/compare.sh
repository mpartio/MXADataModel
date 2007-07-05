#! /bin/bash

h5dump /tmp/AttributeTest-After.h5 > /tmp/AttributeTest_After.txt
h5dump /tmp/AttributeTest-Before.h5 > /tmp/AttributeTest_Before.txt

opendiff /tmp/AttributeTest_After.txt /tmp/AttributeTest_Before.txt
