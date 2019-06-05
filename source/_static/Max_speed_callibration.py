
path='/home/jubo/PycharmProjects/gcode/maxSpeedCal.gcode'

Speed = 100
Low_Speed = 100
High_Speed = 1500
Function = 'G'
Power=0.1
Speed_step = 100
x_width=10
y_width=10
x=0
y=0
i=0


file = open(path,'w')

Speed = Low_Speed

file.write(';Rectangular Zig zag for low speed test \n \n')
file.write("G0 X" + str(x) + " Y" + str(y) + "F" + str(1000) + "\n")
while i<10:
    file.write(";rectangular at" + str(Speed) + "mm/s" + "\n")
    x=x+x_width
    file.write("G1 X" + str(x) + " Y" + str(y) + "F" + str(Speed) + "S" + str(Power) + "\n")
    y=y+y_width
    file.write("G1 X" + str(x) + " Y" + str(y) + "\n")
    x=x+x_width
    file.write("G1 X" + str(x) + " Y" + str(y) + "\n")
    y=y-y_width
    file.write("G1 X" + str(x) + " Y" + str(y) + "\n")
    file.write(";small step before the next rectangular" + "\n")
    x=x+5
    file.write("G0 X" + str(x) + "F" + str(1000) + "\n \n")
    i=i+1
    Speed = Speed + Speed_step

i=0
y = y + 20
x = 0
file.write("G0 Y" + str(y) + "F" + str(1000) + "\n")

Speed = High_Speed

file.write(';Rectangular Zig zag for High speed test \n \n')
while i < 10:
    file.write(";rectangular at" + str(Speed) + "mm/s" + "\n")
    x = x + x_width
    file.write("G1 X" + str(x) + " Y" + str(y) + "F" + str(Speed) + "S" + str(Power) + "\n")
    y = y + y_width
    file.write("G1 X" + str(x) + " Y" + str(y) + "\n")
    x = x + x_width
    file.write("G1 X" + str(x) + " Y" + str(y) + "\n")
    y = y - y_width
    file.write("G1 X" + str(x) + " Y" + str(y) + "\n")
    file.write(";small step before the next rectangular" + "\n")
    x = x + 5
    file.write("G0 X" + str(x) + "F" + str(1000) + "\n \n")
    i=i+1
    Speed=Speed+Speed_step

file.close()
