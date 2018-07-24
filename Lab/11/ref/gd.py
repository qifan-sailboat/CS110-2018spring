import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import time
import sys
import pdb
from functools import reduce 
def gd(epsilon, theta, learning_rate, inpx, inpy):
    '''
    input:
        epsilon:        float
        theta:          np.array[2,1]
        learning_rate:  float
        inpx:           np.array[n,2]( [1,x] on each row)
        inpy:           np.array[n,1]

    output:
        cur_theta:      np.array[2,1]
        it_nm:          int
    '''
    def cost(theta, inpx, inpy):
        return np.sum( (np.dot(inpx,theta) - inpy)**2 ) / 2 / inpx.shape[0]
    dif_err = epsilon * 1e2

    cur_theta = theta
    cur_J = cost(cur_theta, inpx, inpy)
    it_nm = 0
    while(dif_err > epsilon):
        bf_grad = np.dot(np.dot(inpx.T, inpx),cur_theta) - np.dot(inpx.T,inpy) #[2,1]
        cur_theta -= learning_rate * bf_grad / inpx.shape[0]
        it_nm += 1

        old_J = cur_J
        cur_J = cost(cur_theta, inpx, inpy)
        dif_err = abs(cur_J - old_J)

        sys.stdout.write("\rIterator: %d : dif_err: %f" %(it_nm, dif_err))
    sys.stdout.write("\n")
    return cur_theta, it_nm

def mul(a,b):
        m = len(a)
        n = len(b[0])
        l = np.zeros([m,n])
        for i in range(m):
            for j in range(n):
                tmp = list(map(lambda x, y: x*y, a[i], b.T[j]))
                l[i][j] = reduce(lambda x, y: x+y, tmp)
                # print(reduce(lambda x, y: x+y, tmp))
        return l

def mapreduce_gd(epsilon, theta, learning_rate, inpx, inpy):
    '''
    input:
        epsilon:        float
        theta:          np.array[2,1]
        learning_rate:  float
        inpx:           np.array[n,2]( [1,x] on each row)
        inpy:           np.array[n,1]

    output:
        cur_theta:      np.array[2,1]
        it_nm:          int
    '''
    def cost(theta, inpx, inpy):
        return np.sum( (np.dot(inpx,theta) - inpy)**2 ) / 2 / inpx.shape[0]
    dif_err = epsilon * 1e2

    cur_theta = theta
    cur_J = cost(cur_theta, inpx, inpy)
    it_nm = 0
    while(dif_err > epsilon):
        #TODO
        matrix = mul(inpx.T, inpx)
        bf_grad = mul(matrix,cur_theta) 
        b = mul(inpx.T,inpy)
        for i in range(len(bf_grad)):
            bf_grad[i] = list(map(lambda x, y: x - y, bf_grad[i], b[i]))

        # bf_grad = 0;       
        cur_theta -= learning_rate * bf_grad / inpx.shape[0]
        it_nm += 1

        old_J = cur_J
        cur_J = cost(cur_theta, inpx, inpy)
        dif_err = abs(cur_J - old_J)

        sys.stdout.write("\rIterator: %d : dif_err: %f" %(it_nm, dif_err))
    sys.stdout.write("\n")
    return cur_theta, it_nm


if __name__ == "__main__":

    epsilon = 10e-9

    theta = np.array([[1.],[1.]])

    learning_rate = 10e-4

    df = pd.read_csv('GPA.txt', sep='\t|\s+', header=0,engine='python')
    inpy = df.values[:,1].reshape((1,-1)).astype(float)
    inpx = np.ones((2,inpy.shape[1]))
    inpx[1,:] = df.values[:,0].astype(float)
    inpx = inpx.T
    inpy = inpy.T

    #gd
    st = time.time()
    opted_theta1, it_nm1 = gd(epsilon, theta, learning_rate, inpx, inpy)
    ed = time.time() - st
    print("GD Takes: %f s"%ed)
    
    mapreduce_gd
    theta = np.array([[1.],[1.]])
    st = time.time()
    opted_theta2, it_nm1 = mapreduce_gd(epsilon, theta, learning_rate, inpx, inpy)
    ed = time.time() - st
    print("mapreduce_GD Takes: %f s"%ed)
 

    # plot 
    plt.plot(inpx[:,1].tolist(),inpy[:,0].tolist(),'bo')
    rangex = [np.min(inpx[:,1]), np.max(inpx[:,1])]
    rangey1 = np.dot(np.array([[1,1],rangex]).T, opted_theta1).tolist()
    rangey2 = np.dot(np.array([[1,1],rangex]).T, opted_theta2).tolist()
    plt.plot(rangex, rangey1,label='GD')
    plt.plot(rangex, rangey2,label='Mapreduce_GD')
    plt.legend()
    plt.xlabel("x / High School GPA")
    plt.ylabel("y / University GPA")
    plt.show()