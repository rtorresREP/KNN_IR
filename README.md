# KNN_IR
This is an implementation of KNN over k2-tree using Incremental Radius strategy.

#TODO: change distance stored as double by float. This assures maximum additional memory used as 12*K instead of 16*K, being K the amount of neighbors searched.

You can compile modifying the Makefile. We highly recommend using testC.cpp as a template to run experiments. We include a few toy examples with the file extension *.kt.
