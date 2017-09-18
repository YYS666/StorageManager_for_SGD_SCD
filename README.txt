Storage Manager and Experiments are coded in StorageExperiments.cpp.
To run the code, c++ language and g++ compileer is needed, actually clang complier
is also usable.
I have attached in the stript complileSE.sh for compiling, the executable program 
is called se_program, feel free to adjust and recompile the code and overried 
the current program. The command to compile is:
g++ -std=c++0x StorageExperiments.cpp -o se_program
To run the program, the command is
./se_program $1 $2
where $1 is number of atrribbutes of each data instances and $2 is number of 
data instances.
Part of the results is recored in res1.txt, to get the output shown in res1.txt,
you can run the program multiple times or use a stript such as shown in
increasingIns.sh.
Similarly, to get the results with different matrix layout, just change the 
arguments of the program. Example of stript is shown in matrixLayout.sh.
As for the OS environment, it is verified that this program can operate on macOS
and ubuntu 14.04LTS, however, only in ubuntu 14.04LTS can ensure all data kept in
memory if dataset is not larger than memory. macOS will interfere the memory use.