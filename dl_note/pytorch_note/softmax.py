import torch
from torch import nn
from d2l import torch as d2l

batch_size =256
train_iter,test_iter = d2l.load_data_fashion_mnist(batch_size)

#   在线性层前定义了展平层（flatten），来调整网络输入的形状
#   在Sequential中添加一个带有10个输出的全连接层
net = nn.Sequential(nn.Flatten(),nn.Linear(784,10))

#   我们仍然以均值0和标准差0.01随机初始化权重。
def init_weights(m):
    if type(m) == nn.Linear:
        nn.init.normal_(m.weight,std=0.01)

net.apply(init_weights)

loss = nn.CrossEntropyLoss(reduction='none')


trainer = torch.optim.SGD(net.parameters(), lr=0.1)

num_epochs = 10
d2l.train_ch3(net, train_iter, test_iter, loss, num_epochs, trainer)