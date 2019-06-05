
path='/home/jubo/PycharmProjects/gcode/laserWidth.gcode'

Speed = 1000
Power=0.1
x_step=0.05
x_max_step=0.3
y_width=10
x=0
y=0
i=0
iterations=30


file = open(path,'w')

file.write("G0 X" + str(x) + " Y" + str(y) + "F" + str(1000) + "\n")

while x_step<=x_max_step:
    file.write(";Zone with " + str(x_step) + "mm steps " + "\n")
    while i<iterations:
        x=x+x_step
        file.write("G1 X" + str(x) + " Y" + str(y) + "F" + str(Speed) + "S" + str(Power) + "\n")
        y=y+y_width
        file.write("G1 X" + str(x) + " Y" + str(y) + "\n")
        x=x+x_step
        file.write("G1 X" + str(x) + " Y" + str(y) + "\n")
        y=y-y_width
        file.write("G1 X" + str(x) + " Y" + str(y) + "\n")
        i=i+1
    x_step=x_step+0.05
    i=0
    x=x+3
    file.write(";small step before the next rectangular" + "\n")
    file.write("G0 X" + str(x) + "F" + str(1000) + "\n \n")


file.close()
