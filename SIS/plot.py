#!/usr/bin/python3.8
import sys
import matplotlib.pyplot as plt
import math 
  
# arguments from user
print('Usage: ')
print('plot.py [po] [beta] [gamma] [k] [t_max]')
if len(sys.argv) != 6:
    print("Bad number of arguments!")
    exit()

po = float(sys.argv[1])
beta = float(sys.argv[2])
gamma = float(sys.argv[3])
k = float(sys.argv[4])
t_max = int(sys.argv[5])

lam = beta / gamma
lam_c = 1 / k

# time
t = list(range(0, t_max))
p = []

for i in range(len(t)):
    v = po * (beta * k - gamma) / (po * beta * k - (gamma - beta * k * (1 - po))*math.exp(-1*(beta * k - gamma) * i))
    p.append(v)
  

plt.plot(t, p)
line_x = [0, t_max]
m = 1 - gamma / (beta * k)
line_y = [m, m]
if m > 0:
    plt.plot(line_x, line_y, 'r--') 


plt.xlabel('Time')
plt.ylabel('p(t)')

plt.suptitle("$p_0$ = " + str(po) + " " + r"$\lambda$ = " + "{:.2f}".format(lam) + " " +  r"$\lambda_C$ = " + "{:.2f}".format(lam_c)) 
plt.title(r"$\beta$ = " +  "{:.2f}".format(beta) + " "  + r"$\gamma$ = " +  "{:.2f}".format(gamma) + " " + "k = " + str(k))
plt.savefig('p_' + str(po) + '_' + "{:.2f}".format(lam) + "_" + "{:.2f}".format(lam_c)+ '.png')
plt.show()