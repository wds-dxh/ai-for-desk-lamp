import torch
import torch.nn as nn
from torch.autograd import Variable

class CNN_easy(nn.Module):
    class HandModel(nn.Module):
        def __init__(self):
            super(HandModel, self).__init__()
            self.flatten = nn.Flatten()
            self.linear1 = nn.Linear(224 * 224 * 3, 40)  # Assuming RGB images (3 channels)
            self.linear2 = nn.Linear(40, 32)
            self.linear3 = nn.Linear(32, 27)
            self.linear4 = nn.Linear(27, 20)  # Additional linear layer
            self.linear5 = nn.Linear(20, 4)  # Output for 4 classes

        def forward(self, input):
            input = input.view(input.size(0), -1)  # Flatten the input
            out = self.linear1(input)
            out = self.linear2(out)
            out = self.linear3(out)
            out = self.linear4(out)  # Passing through the fourth linear layer
            out = self.linear5(out)  # Output layer for 4 classes
            return out


class HandModel(nn.Module):
    def __init__(self):
        super(HandModel, self).__init__()
        self.linear1 = nn.Linear(48, 40)#
        self.linear2 = nn.Linear(40, 32)
        self.linear3 = nn.Linear(32, 27)

    def forward(self, input):
        input = input.to(torch.float32)
        out = self.linear1(input)
        out = self.linear2(out)
        out = self.linear3(out)
        return out


class DynamicHandModel(nn.Module):
    def __init__(self):
        super(DynamicHandModel, self).__init__()
        self.lstm = nn.LSTM(30, 256, num_layers=2)
        self.linear1 = nn.Linear(256, 3)

    def forward(self, input, hidden=None):
        input = input.to(torch.float32)
        if hidden is None:
            h_0 = input.data.new(2, 1,256).fill_(0).float()
            c_0 = input.data.new(2, 1,256).fill_(0).float()
            h_0, c_0 = Variable(h_0), Variable(c_0)
        else:
            h_0, c_0 = hidden

        out, hidden = self.lstm(input, (h_0, c_0))

        out = self.linear1(out.view(1, -1))

        return out, hidden



if __name__ == "__main__":
    print(HandModel())
    print(DynamicHandModel())
