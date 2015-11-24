# Industruino RS485 Example Sketch
This is an example sketch I wrote in order to explore the RS485 capabilities of the Industruino IND.I/O kit.

##What the sketch DOES
The sketch does:
* Initialize Serial1 (the onboard RS485 tranceiver)
* Initialize the modbus master
* Write a value to a slave device (using 0x04 function code)
* Read out values from a slave (using 0x03 function code)
* Print out values to the display

For testing purpose, I used a IME MFD4421 (http://bit.ly/1MOjBJo) I bought for another project. Any other modbus slave may work.

##Libraries
TBC

##License
The MIT License (MIT)

Copyright (c) 2015 Gianluca Stella (gianluca.stella  -  gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
