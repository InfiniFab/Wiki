
path='/home/jubo/PycharmProjects/gcode/laserModulation.gcode'

Speed = 1000
Power=0
x_step=0.2 #laser width
Power_steps=100
y_width=10
x=0
y=0
i=0


file = open(path,'w')

file.write("G0 X" + str(x) + " Y" + str(y) + "F" + str(1000) + "\n")

while i<Power_steps:
    x=x+x_step
    y=y+y_width
    file.write("G1 X" + str(x) + " Y" + str(y) + "F" + str(Speed) + "S" + str(Power) + "\n")
    x=x+x_step
    y=y-y_width
    file.write("G1 X" + str(x) + " Y" + str(y) + "\n")
    Power=Power+1/Power_steps
    i=i+1

file.close()
