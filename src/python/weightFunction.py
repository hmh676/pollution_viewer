	# linear algebra package
import numpy as np
# graphical display package
import pyqtgraph as pg
import matplotlib.pyplot as plt
# Qt
from PySide import QtGui

# compue dist(a,b)
# a,b: two np arrays
def dist(a,b):
	d = np.linalg.norm(a-b)
	return d


# compute shepard weight in x
# x : np array (typically [x,y,z])
# knownPos : list of known points [x_known,y_known] and x_know : np array of size n
# i : index
def shepardSimpleWeights(i,x,knownPos):
	# w_i(x) = 1/(d(x,x_i)**mu_i) * 1 / sum(j=0,n)[1/(d(x,x_j)**mu_j)]
	n = len(knownPos)
	mu = np.zeros(n)
	mu.fill(2.0)

	# sum compuation
	s = 0
	for j in range(n):
		x_j = knownPos[j]
		mu_j = mu[j]
		s += 1.0/(dist(x,x_j)**mu_j)

	x_i = knownPos[i]
	mu_i = mu[i]
	# w_i(x)
	w = (1.0/(dist(x,x_i))**mu[i]) * (1.0/s)

	return w

# singedIdentiy(s) = 0 if s<0
# 			   = s if s>0
def singedIdentiy(s):
	if (s < 0) :
		return s
	else : 
		return 0

	return 0

# compute shepard quadratic's weights in x
# x : np array (typically [x,y,z])
# knownPos : list of known points [x_known,y_known] and x_know : np array of size n
# i : index
def shepardQuadraticWeights(i,x,knownPos):
	Rw = 32

	x_i = knownPos[i]

	w_top = singedIdentiy(Rw - dist(x,x_i))
	w_bottom = Rw*dist(x,x_i)

	w = (w_top/w_bottom)**2

	return w

# k index
# pos : numpy array [x,y]
# knownPos list of two arrays (x_known and y_known)
def phi(k,pos,knownPos):
	Rq = 17

	x_k = knownPos[k] 

	phi_top = singedIdentiy(Rq-dist(pos,x_k))
	phi_bottom = Rq*dist(pos,x_k)

	phi_value = (phi_top/phi_bottom)**2

	return phi_value



