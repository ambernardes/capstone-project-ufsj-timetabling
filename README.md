# Project Description

This repository contains an implementation of an optimization system for the university timetable problem, using metaheuristic techniques such as Reactive GRASP and Tabu Search. The main objective is to develop efficient solutions for class scheduling, considering multiple constraints such as teacher availability, student needs, and the institution's physical resources (classrooms and time slots).

## Research Problem

The Timetable Problem is a classic optimization challenge involving a large number of variables and constraints. In many educational institutions, this task is still done manually, but automation has become common, especially in large organizations. This project addresses a formulation based on a university curriculum, focusing on the following entities:

 - Days: Days of the week when classes are held (usually five or six).
 - Time Slots: Daily periods available for classes.
 - Periods: A combination of day and time slot.
 - Courses and Instructors: Each course has a certain number of weekly classes to be scheduled in different periods, with specific teachers and students assigned.
 - Room: Each room has a limited seating capacity.
 - Curriculum: A set of courses shared by a group of students.
The challenge is to allocate classes in a way that meets the availability constraints of teachers and students while maximizing the use of the institution's resources.

## Project Structure
The project is organized as follows:

 - src/: `C++` Source code for the implementation of metaheuristics and optimization algorithms.
 - instancia/: Test instances and data provided by ITC2007.
 - saidas/: Results and evaluations of the executions.

## Project Setup

This guide explains the steps to set up and run the C++ code for the timetable optimization project using the provided Makefile.

### Prerequisites

Before running the code, make sure the following tools are installed:

1. g++ Compiler: Required to compile the C++ code. You can install it on Ubuntu with:

```bash
sudo apt update
sudo apt install g++
```
2. Make: Required to run the Makefile. Install it on Ubuntu with:

```bash
sudo apt update
sudo apt install make
```
### Compiling the Code

Once compiled, you can run the program with:

```bash
make
```
The Makefile will:

 - Compile each .cpp file in the src directory into object files in the obj directory.
 - Link all object files to create the final executable named `timetable`.

### Compiling the Code

Once compiled, you can run the program with:

```bash
./timetable

```

### Cleaning Up

To remove the compiled files and the obj directory, you can run:

```bash
make clean
```

This command deletes the `timetable` executable and all files in the `obj` directory.

## How to Contribute
Feel free to open issues with suggestions for improvement or report any problems. Contributions via pull requests are also welcome!