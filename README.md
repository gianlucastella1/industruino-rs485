# Industruino RS485 Example Sketch
This is an example sketch I wrote in order to explore the RS485 capabilities of the Industruino IND.I/O kit.

##What the sketch DOES
The sketch does:
1. Initialize Serial1 (the onboard RS485 tranceiver)
2. Initialize the modbus master
3. Write a value to a slave device (using 0x04 function code)
4. Read out values from a slave (using 0x03 function code)
5. Print out values to the display

For testing purpose, I used a IME MFD4421 (http://bit.ly/1MOjBJo) I bought for another project. Any other modbus slave may work.

##Libraries
TBC

