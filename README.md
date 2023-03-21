# matrix-parallel-02-daguanno-rajasekaran
matrix-parallel-01-daguanno-rajasekaran created by GitHub Classroom

Distributed parallel computing
Explain the architecture of your solution. For the SIMD solution, the answer matrix is initialized to 0, and then the matrix multiplication is run. This is faster because the cpu architecture is used to compute the operations more efficiently. For the OMP solution, the matrix is split into lines for each core on a machine to compute. This is faster because it can make all of the computations concurrently on the same machine. For the MPI solution, rows of the matrix were sent out to other machines and computed there before returning to the main machine where they are all added up. This is faster because there can be many machines on a network that contribute to the computations of lines. For the OMP+MPI solution, the lines of the matrices were sent to different machines, and then OMP was used on those machines to make the computations even faster.

Explain the variations of algorithms you implemented. Several parallel computing algorithms were implemented for matrix multiplication, including SIMD, OMP, MPI, and OMP+MPI. These algorithms were designed to improve the efficiency of matrix multiplication. An unoptimized version of the matrix multiplication was also used in the project.
Teamwork
List all team members in the README.md explain the contribution of each person. 

Dario: Responsible for contributing to the writing portion of this project including the explanation of SIMD, OMP and MPI, and write up for the full project. Also was responsible for the OMP algorithm along with reading matrices from files for the main function.


Raj:


Harsh:


Did you lock the master branch as explained in GitHW2 Lab 2 - Git? How did you proceed to review each other's work? Yes, the master branch was locked as explained in GitHW2. For this project, our github repository had a dev branch, along with the master branch. After a team member would finish some code in their own branch, it would be pushed to the dev branch. This would work as a preliminary branch so that we could make sure the master branch would always be working properly. Before code from one of our branches could be merged to the dev branch, the other team members would need to review the code and allow it to merge manually, as explained in GitHW2.

Full project life cycle
Have you used a project board? How did you use it, or if you did not use a project board how did you plan and manage your project and team work? Yes, we did use the project board. Being that this project included many tasks, with 3 team members, the project board worked great to stay organized. The project board was used as intended, with each task not yet done tagged with a “todo”, and all completed tasks marked with “done”.

Is the usual cycle: write code, compile code,  run code, test code was the same when doing remote development on Wolfgand cluster. Did you need to adapt your way of working or use different tools? While coding for this project did take on the usual cycle: write code, run code, test code, this cycle was done very differently from past projects. Adaptation was necessary. When working with the Woldgand cluster, writing, compiling and testing was different then working on your local computer with an IDE. For some code in the project, code was written in linux with Vim. Vim is very bare bones, as compared to a very high level IDE such as CLion. 

What kind of testing did you plan to use for this project? Did you consider measuring speed, memory consumption and validity of results for matrix multiplication. Did you consider that the code provided by the professor could have flaws? For this project, there was the common testing done used to detect compiler and logical errors. There was also code to measure the speed at which each algorithm ran. The function clock_gettime() was an important function for this project. These measurements were saved in our graphs using the Gnuplot software.

Did you need to write code or use tools to generate random matrix of specific sizes? Did you put this in your plan? Code was used in order to generate random matrices. In the file mat.c, there is the function gen_matrix() which is used to create matrices. The function takes in an m and n integer in order to create a matrix of that size. It adds random doubles to the matrix then returns the matrix.

Did you put in your plan the work needed to generate tables or graphs? Did you automate this work? Work was planned and completed in order to generate graphs for the performance of the different strategies. In order to automate the generation of graphs, Gnuplot was used. A script was written for Gnuplot in order to graph the information from the different types of parallel processing that were used. 

What proportion of the tasks (and time) in your plan is  about writing variations on the matrix multiplication algorithm and what proportion is about testing and reporting activities? Looking at the project as a whole, the proportion of the tasks and time seemed to be about even. While there were more tasks focused on implementing the different algorithms, it was important to compare the time it took for each algorithm to run to completion. 
