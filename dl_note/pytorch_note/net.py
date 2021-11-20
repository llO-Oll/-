from typing import ForwardRef
import torch
import torch.nn as nn

import torch.nn.functional as F

class Net(nn.Module):
    def __init__(self):
        super().__init__()

        # 卷积层 ‘1’表示输入图片为单通道， ‘6’表示输出通道数，‘3’表示卷积核为3*3
        self.conv1 = nn.Conv2d(1,6,3)
        # 线性层， 输入1350个特征， 输出10个特征
        self.fc1 = nn.Linear(1350,10) #这里的1350是如何计算的呢？这就要看后面的forward函数

    #正向传播
    def forward(self,x):
        print(x.size())#[1, 1, 32, 32]
        # 卷积 -> 激活 -> 池化
        x=self.conv1(x)
        x=F.relu(x)
        print(x.size())#[1,6,30,30]
        x=F.max_pool2d(x,(2,2))#池化层，计算结果是15
        x=F.relu(x)
        print(x.size())

        x=x.view(x.size(0),-1)# x.seze()[0]指batchsize
        #卷积或者池化之后的tensor的维度为(batchsize，channels，x，y)，
        #其中x.size(0)指batchsize的值，最后通过x.view(x.size(0), -1)将tensor的结构转换为了(batchsize, channels*x*y)，即将（channels，x，y）拉直，然后就可以和fc层连接了

        print(x.size())
        x=self.fc1(x)
        return x

net =Net()
print(net)

#net.named_parameters可同时返回可学习的参数及名称。
for name,parameters in net.named_parameters():
    print(name,':',parameters.size())

input = torch.randn(1,1,32,32)#对应forward输入
out =net(input)
out.size()

net.zero_grad() #在反向传播前，先要将所有参数的梯度清零
out.backward(torch.ones(1,10)) # 反向传播的实现是PyTorch自动实现的，我们只要调用这个函数即可

y = torch.arange(0,10).view(1,10).float()
criterion = nn.MSELoss()
loss = criterion(out, y)
#loss是个scalar，我们可以直接用item获取到他的python类型的数值
print(loss.item())

