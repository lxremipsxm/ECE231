%Siddharth Vasudevan | ECE231 
%Plots time differences from BBB led toggle script

t = readmatrix("time_diff_file.txt");

plot(t);
title("Time Difference for each Iteration of LED Toggle");
subtitle("ECE231 Lab 2, Siddharth Vasudevan")
grid on;
ylabel("Time Difference(ns)");
xlabel("Iteration");