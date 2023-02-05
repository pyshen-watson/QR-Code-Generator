# QR Code Generator

|⚠️WARNING|
|---|
|This public repository is just for my personal portfolio. The content is for the course: ***Introduction to Computer, Fall 2018, CSIE, NTU***. If you are taking this course right now, any form of referring or copying is ***not allowed***. Please finish your assignment by yourself.|

|Author|OS|Language|
|--|--|--|
|沈韋辰, 黃義峰|Windows 10|C|

## Introduction
This program can turn a  string (1~17 characters) into a QR code.

<img src="https://imgur.com/k9pJpwi.png" width=50% height=50%>


## How to run

### Method 1
Run the [executable file](generator.exe) directly.

### Method 2
1. Follow [this video](https://www.youtube.com/watch?v=2jpLG_XBVWI) to install [MinGW](https://sourceforge.net/projects/mingw/).
1. Set the environment variable correctly.
1. Use the command `mingw32-make` to compile the source code.
1. Execute `generator.exe`.

## Note
- This program can only run on Windows due to the dependency `windows.h`.
- Here is the [report](report.pdf)
- [MIT Licence](LICENCE) © [Pyshen](https://pyshen.net)