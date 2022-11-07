'''
@Author     : 赵宇恒
@Time       : 2022/08/15 22:33
@Function   :实现离散小波变换，可用于一维、二维数据。
'''
import torch
from torch.autograd import Function

class DWTFuntion_1D(Function):
    def forward(ctx, input, matrix_Low, matrix_High):
        '''
        ctx         :
        input       :一维序列
        matrix_Low  :低频矩阵
        matrix_High :高频矩阵
        '''
        ctx.save_for_backward(matrix_Low, matrix_High)
        L = torch.matmul(input, matrix_Low.t())
        H = torch.matmul(input, matrix_High.t())
        return L,H
    
    def backward(ctx, grad_L, grad_H):
        matrix_L, matrix_H = ctx.saved_variables
        grad_input = torch.add(torch.matmul(grad_L,matrix_L),torch.matmul(grad_H, matrix_H))
        return grad_input, None, None

class IDWTFunction_1D(Function):
    def forward(ctx, input_L, input_H, matrix_L, matrix_H):
        ctx.save_for_backward(matrix_L, matrix_H)
        output = torch.add(torch.matmul(input_L, matrix_L),
                           torch.matmul(input_H, matrix_H))
        return output       
    
    def backward(ctx,grad_output):
        matrix_L, matrix_H = ctx.saved_variables
        grad_L = torch.matmul(grad_output, matrix_L.t())
        grad_H = torch.matmul(grad_output, matrix_H.t())
        return grad_L, grad_H, None, None        