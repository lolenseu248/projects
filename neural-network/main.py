import numpy as np

# Sigmoid activation function
def sigmoid(x):
    return 1 / (1 + np.exp(-x))

# Derivative of sigmoid activation function
def dsigmoid(y):
    return y * (1 - y)

class NeuralNetwork:
    def __init__(self, inputs, hidden, outputs):
        self.inputs = inputs
        self.hidden = hidden
        self.outputs = outputs

        # Initialize weights and biases randomly
        self.weights1 = np.random.rand(inputs, hidden)
        self.weights2 = np.random.rand(hidden, outputs)

        self.bias1 = np.zeros((1, hidden))
        self.bias2 = np.zeros((1, outputs))
        
        self.hidden = self.hidden.reshape(self.hidden.shape[0], 1)

    def forward_pass(self, inputs):
        hidden_layer = sigmoid(np.dot(inputs, self.weights1) + self.bias1.T)
        hidden_layer = hidden_layer.reshape(hidden_layer.shape[0], 1) # Add this line

        output_layer = sigmoid(np.dot(hidden_layer, self.weights2) + self.bias2.T)

        return hidden_layer, output_layer


# Example usage:
nn = NeuralNetwork(2, 4, 3) # Two inputs, four in the hidden layer, three outputs
input_data = np.array([[1.0, 2.0]])

hidden_output, output = nn.forward_pass(input_data)
print(hidden_output)
print(output)

# Now let's train this network (this is highly simplified):
# Define loss function and compute gradients
def cross_entropy(y_pred, y_true):
    return -np.sum(y_true * np.log(y_pred))

# Train loop:
for epoch in range(1000):
    # Pass data through the network...
    hidden_output, output = nn.forward_pass(input_data)

    # Compute error (cross-entropy loss)
    error = cross_entropy(output, input_data.T)  # Here, input_data is "true" outputs and .T is transpose

    # Now perform weight updates based on gradients
    doutput = -input_data.T * output / error

    # Perform weight updates:
    nn.weights2 += np.dot(hidden_output.T, doutput)