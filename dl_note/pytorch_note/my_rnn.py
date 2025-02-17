import math
from tokenize import String
import torch
from torch import nn
from torch.nn import functional as F
from d2l import torch as d2l

batch_size, num_steps =32, 35   #num_steps每一次看多长的序列
train_iter, vocab = d2l.load_data_time_machine(batch_size,num_steps)


# 每次采样的小批量数据形状是二维张量:(批量大小（2），时间步数（5）)
X=torch.arange(10).reshape(2,5)
# 转置(batch_size, time, vacab)——>(time, batch_size, vacab)
# 这将使我们能够更方便地通过最外层的维度，一步一步地更新小批量数据的隐状态。
F.one_hot(X.T,28)

# 初始化模型参数
def get_params(vocab_size, num_hiddens, device):
    num_inputs = num_outputs = vocab_size

    # 从均值为0，标准差为0.01的正太分布采样随机数来初始化权重
    def normal(shape):
        return torch.randn(size=shape, device=device) * 0.01
    
    # 隐藏层大小
    W_xh = normal((num_inputs, num_hiddens))
    W_hh = normal((num_hiddens,num_hiddens))
    b_h = torch.zeros(num_hiddens,device=device)

    # 输出层大小
    W_hq = normal((num_hiddens,num_outputs))
    b_q = torch.zeros(num_outputs,device=device)

    # 附加梯度
    params = [W_xh, W_hh, b_h,W_hq, b_q]
    for param in params:
        param.requires_grad_(True)
    return params

# 初始化隐藏状态
def init_rnn_state(batch_size, num_hiddens, device):
    return (torch.zeros((batch_size, num_hiddens), device=device), )

def rnn(inputs, state,params):
    W_xh,W_hh,b_h,W_hq,b_q = params
    H,=state
    outputs = []

    for X in inputs:
        H = torch.tanh(torch.mm(X, W_xh) + torch.mm(H,W_hh) + b_h)
        Y = torch.mm(H,W_hq) + b_q
        outputs.append(Y)
    return torch.cat(outputs,dim=0), (H,)

class RNNModelScratch:

    def __init__(self, vocab_size, num_hiddens, device, 
    get_params, init_state,forward_fn):
        self.vocab_size, self.num_hiddens = vocab_size, num_hiddens
        self.params = get_params(vocab_size, num_hiddens, device)
        self.init_state, self.forward_fn = init_state, forward_fn

    def __call__(self,X,state):
        X = F.one_hot(X.T, self.vocab_size).type(torch.float32)
        return self.forward_fn(X, state, self.params)
    
    def begin_state(self, batch_size):
        return self.init_state(batch_size, self.num_hiddens,device)

def predict_ch8(prefix, num_preds, net, vocab, device):
    
    state = net.begin_state(batch_size=1,device =device)
    outputs = [vocab[prefix[0]]]
    get_input = lambda: torch.tensor([outputs[-1]],device=device).reshape((1, 1))
    for y in prefix[1:]:    # 预热期
        _, state = net(get_input(), state)
        outputs.append(vocab[y])
    for _ in range(num_preds):
        y, state = net(get_input(),state)
        outputs.append(int(y.argmax(dim=1).reshape(1)))
    return ''.join([vocab.idx_to_token[i] for i in outputs])

# 梯度裁剪
def grad_clipping(net, theta):

    if isinstance(net, nn.Module):
        params = [p for p in net.parameters() if p.requires_grad]
    else:
        params = net.params
    norm = torch.sqrt(sum(torch.sum((p.grad ** 2)) for p in params))
    if norm > theta:
        for param in params:
            param.grad[:] *=theta / norm


def train_epoch_ch8(net, train_iter, loss, updater, device, use_random_iter):

    state, timer = None, d2l.Timer()
    metric = d2l.Accumulator(2)
    for X, Y in train_iter:
        if state is None or use_random_iter:
            state = net.begin_state(batch_size=X.shape[0])
        else:
            if isinstance(net,nn.Module) and not isinstance(state, tuple):
                state.detach_()
            else:
                for s in state:
                    s.detach_()
        y = Y.T.reshape(-1)
        print(device)
        X,y = X.to(device),y.to(device)
        y_hat, state = net(X,state)
        l = cross_entropy(y_hat, y.long()).mean()
        if isinstance(updater,torch.optim.Optimizer):
            updater.zero_grad()
            l.backward()
            grad_clipping(net, 1)
            updater.step()
        else:
            l.backward()
            grad_clipping(net,1)

            updater(batch_size=1)
        metric.add(l * y.numel(), y.numel())
    return math.exp(metric[0] / metric[1]),metric[1] / timer.stop()

def cross_entropy(y_hat,y):
    return - torch.log(y_hat[range(len(y_hat)),y])   

num_epochs, lr = 500, 1

def updater(batch_size):
    return d2l.sgd(net.params, lr, batch_size)
num_hiddens = 512
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
print(torch.cuda.is_available())
net = RNNModelScratch(len(vocab), num_hiddens, device, get_params,
                      init_rnn_state, rnn)      

                       
train_epoch_ch8(net, train_iter, cross_entropy, lr, num_epochs,device)
