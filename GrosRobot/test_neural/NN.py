import numpy as np # helps with the math
import matplotlib.pyplot as plt # to plot error during training

# input data
xAll = np.array([[0, 0, 0],
                   [0, 0, 1],
                   [0, 1, 0],
                   [0, 1, 1],
                   [1, 0, 0],
                   [1, 0, 1],
                   [1, 1, 0],
                   [1, 1, 1]
                   ])
# output data

y = np.array([[1.0, 1.0],
              [0.5, 1.0],
              [0.0, 0.0],
              [0.5, 0.0], 
              [1.0, 0.5], 
              [1.0, 1.0],
              [0.0, 0.5],
              [0.0, 0.0]
              ]) 
#   { 1, 1},  
#   { 0.5, 1}, 
#   { 0, 0}, 
#   { 0.5, 0}, 
#   { 1, 0.5}, 
#   { 1, 1}, 
#   { 0, 0.5}, 
#   { 0, 0} 

# scale units
xAll = xAll/np.amax(xAll, axis=0) # scaling input data
 # scaling output data (max test score is 100)

# split data
X = np.split(xAll, [8])[0] # training data
xPredicted = np.split(xAll, [8])[0] # testing data

test=np.array([[0, 1, 1]])

class Neural_Network(object):
  def __init__(self):
  #parameters
    self.inputSize = 3
    self.outputSize = 2
    self.hiddenSize = 4

  #weights
    self.W1 = np.random.randn(self.inputSize, self.hiddenSize) # (3x2) weight matrix from input to hidden layer
    self.W2 = np.random.randn(self.hiddenSize, self.outputSize) # (3x1) weight matrix from hidden to output layer

  def forward(self, X):
    #forward propagation through our network
    self.z = np.dot(X, self.W1) # dot product of X (input) and first set of 3x2 weights
    # print "X \n" + str(X)
    # print "* self.W1 \n" + str(self.W1)
    # print "= z \n" + str(self.z)
    self.z2 = self.sigmoid(self.z) # activation function
    # print "z2 \n" + str(self.z2)
    self.z3 = np.dot(self.z2, self.W2) # dot product of hidden layer (z2) and second set of 3x1 weights
    # print "* W2 \n" + str(self.W2)
    # print "= z3 \n" + str(self.z3)
    o = self.sigmoid(self.z3) # final activation function
    # print "o \n" + str(o)
    return o

  def sigmoid(self, s):
    # activation function
    return 1/(1+np.exp(-s))

  def sigmoidPrime(self, s):
    #derivative of sigmoid
    return s * (1 - s)

  def backward(self, X, y, o):
    # backward propagate through the network
    self.o_error = y - o # error in output
    self.o_delta = self.o_error*self.sigmoidPrime(o) # applying derivative of sigmoid to error

    self.z2_error = self.o_delta.dot(self.W2.T) # z2 error: how much our hidden layer weights contributed to output error
    self.z2_delta = self.z2_error*self.sigmoidPrime(self.z2) # applying derivative of sigmoid to z2 error

    self.W1 += X.T.dot(self.z2_delta) # adjusting first set (input --> hidden) weights
    self.W2 += self.z2.T.dot(self.o_delta) # adjusting second set (hidden --> output) weights

  def train(self, X, y):
    o = self.forward(X)
    self.backward(X, y, o)

  def saveWeights(self):
    np.savetxt("w1.txt", self.W1, fmt="%s")
    np.savetxt("w2.txt", self.W2, fmt="%s")

  def predict(self):
    print "Predicted data based on trained weights: ";
    print "Input (scaled): \n" + str(xPredicted);
    print "Output: \n" + str(self.forward(xPredicted));

NN = Neural_Network()
i = 0
while (np.mean(np.square(y - NN.forward(X)))>0.00004): # trains the NN 1,000 times
  if (i%1000==0):
    print "# " + str(i) + "\n"
    print "Input (scaled): \n" + str(X)
    print "Actual Output: \n" + str(y)
    print "Predicted Output: \n" + str(NN.forward(X))
    print "Loss: \n" + str(np.mean(np.square(y - NN.forward(X)))) # mean sum squared loss
    print "\n"  
  NN.train(X, y)
  i+=1

NN.saveWeights()
NN.predict()

print "test 0,1,1 :\n"+str(NN.forward(test))