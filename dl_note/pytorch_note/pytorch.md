## `nn.Sequential`

`nn.Sequential`一个有序的容器，神经网络模块将按照在传入构造器的顺序依次被添加到计算图中执行，同时以神经网络模块为元素的有序字典也可以作为传入参数。

example:

        model = nn.Sequential(
                  nn.Conv2d(1,20,5),
                  nn.ReLU(),
                  nn.Conv2d(20,64,5),
                  nn.ReLU()
                )
    
        # Example of using Sequential with OrderedDict
        model = nn.Sequential(OrderedDict([
                  ('conv1', nn.Conv2d(1,20,5)),
                  ('relu1', nn.ReLU()),
                  ('conv2', nn.Conv2d(20,64,5)),
                  ('relu2', nn.ReLU())
                ]))

在初始化函数`__init__`中，首先是if条件判断，如果传入的参数为1个，并且类型为`OrderedDict`，通过字典索引的方式将子模块添加到`self._modules`中，否则，通过for循环遍历参数，将所有的子模块添加到`self._modules`中。

```python
def __init__(self, *args):
    super(Sequential,self).__init__()
    if len(args) == 1 and isinstance(args[0], OrderedDict):
        for key,module in args[0].items():
            self.add_module(key,module)
    else:
        for idx,module in enumerate(args):
            self.add_module(str(idx),module)
```

接下来在看一下`forward`函数的实现：
因为每一个module都继承于nn.Module,都会实现`__call__`与`forward`函数，所以forward函数中通过for循环依次调用添加到`self._modules`中的子模块，最后输出经过所有神经网络层的结果

```python
def forward(self, input):
    for module in self._modules.values():
        input = module(input)
    return input
```

下面是简单的三层网络结构的例子：

```python
# hyper parameters
in_dim=1
n_hidden_1=1
n_hidden_2=1
out_dim=1

class Net(nn.Module):
    def __init__(self, in_dim, n_hidden_1, n_hidden_2, out_dim):
        super().__init__()

          self.layer = nn.Sequential(
            nn.Linear(in_dim, n_hidden_1), 
            nn.ReLU(True)，
            nn.Linear(n_hidden_1, n_hidden_2)，
            nn.ReLU(True)，
            # 最后一层不需要添加激活函数
            nn.Linear(n_hidden_2, out_dim)
             )

      def forward(self, x):
          x = self.layer(x)
          return x
```

### `nn.Flatten()`

把相邻维度展平成一个维度

`torch.nn.Flatten(*start_dim=1*, *end_dim=- 1*)`

```
input = torch.randn(32, 1, 5, 5)
>>> # With default parameters
>>> m = nn.Flatten()
>>> output = m(input)
>>> output.size()
torch.Size([32, 25])
>>> # With non-default parameters
>>> m = nn.Flatten(0, 2)
>>> output = m(input)
>>> output.size()
torch.Size([160, 5])
```

### `Tensor.requires_grad`&`Tensor.requires_grad_`

`requires_grad_()`函数会改变`Tensor`的`requires_grad`属性并返回`Tensor`，修改`requires_grad`的操作是原位操作(in place)。

```
>>> a = torch.tensor([1.0, 2.0])
>>> a.data
tensor([1., 2.])
>>> a.grad
>>> a.requires_grad
False
>>> a.requires_grad_()
tensor([1., 2.], requires_grad=True)
>>> c = a.pow(2).sum()
>>> c.backward()
>>> a.grad
tensor([2., 4.])
>>> b = a.detach()
>>> b.grad
>>> b.requires_grad
False
>>> b
tensor([1., 2.])
>>> b[0] = 6
>>> b
tensor([6., 2.])
>>> a
tensor([6., 2.], requires_grad=True)
```

### 矩阵乘法

`@`和`*`代表矩阵的两种相乘方式：`@`表示常规的数学上定义的矩阵相乘；`*`表示两个矩阵对应位置处的两个元素相乘。

`@`等同于`tdorch.mm(a, b)`

`*`等同于`torch.mul()`

## torch.stack()

按一个新的维度拼接两个tensor

# torch.unsqueeze()

对数据维度进行扩充，增加维度
