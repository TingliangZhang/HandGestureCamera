
import matplotlib.pyplot as plt
import math
import numpy as np
def plotlines(impedance):
    # fig, ax= plt.subplots()
    fig =plt.figure()
    # print(len(impedance))
    dotX = np.cos(np.linspace(0,2*(1 - 1/8)*math.pi,8))
    dotY = np.sin(np.linspace(0,2*(1 - 1/8)*math.pi,8))
    Ipdnc = np.array(impedance)[:,2]
    color_weight = (Ipdnc - min(Ipdnc)*np.ones(len(impedance))) / (max(Ipdnc)- min(Ipdnc))
    # print(color_weight)
    for i in range(0,len(impedance)):
        n0 = int(impedance[i][0])
        n1 = int(impedance[i][1])
        x_pair = [dotX[n0], dotX[n1]]
        
        y_pair = [dotY[n0], dotY[n1]]
        # print(x_pair,y_pair)
        plt.plot(x_pair, y_pair,color=[0.9*color_weight[i],0.2,1-color_weight[i]], linewidth = 2)
    for i in range(0,8):
        plt.text(1.1*dotX[i],1.1*dotY[i],str(i),fontsize = 10)
    plt.axis('equal')
    plt.show()