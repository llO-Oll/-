# 多线程

多线程类似于同时执行多个不同程序，多线程运行有如下优点：

> - 使用线程可以把占据长时间的程序中的任务放到后台去处理。
> - 用户界面可以更加吸引人，这样比如用户点击了一个按钮去触发某些事件的处理，可以弹出一个进度条来显示处理的进度
> - 程序的运行速度可能加快
> - 在一些等待的任务实现上如用户输入、文件读写和网络收发数据等，线程就比较有用了。在这种情况下我们可以释放一些珍贵的资源如内存占用等等。

# with

文件管理器

# zip() 函数

**zip()** 函数用于将可迭代的对象作为参数，将对象中对应的元素打包成一个个元组，然后返回由这些元组组成的列表。

```python
a=[1,2,3]
b=[4,5,6]
c=[4,5,6,7,8]
zipped = zip(a,b)
zipped
# output:<zip object at 0x103abc288>
list(zipped)
# output:[(1,4),(2,5),(3,6)]
list(zip(a,c))
# output:[(1,4),(2,5),(3,6)]	#与最短列表对齐
```

# `if __name__ == '__main__':`

当我们把模块A中的代码在模块B中进行import A时，只要B模块代码运行到该import语句，模块A的代码会被执行。

```python3
# 模块A

a = 100

print('你好，我是模块A……')

print(a)
```

```python3
# 模块B

from package01 import A

b = 200

print('你好，我是模块B……')

print(b)
```

```python3
你好，我是模块A……
100
你好，我是模块B……
200
```

```python3
# 模块A

a = 100

print('你好，我是模块A……')

if __name__=='__main__':

    print(a)
```

```python3
你好，我是模块A……
你好，我是模块B……
200
```

# assert 

```
assert expression
```

等价于

```
if not expression:
    raise AssertionError
```

后面可以加参数

```
assert expression [, arguments]
```

```
if not expression:
    raise AssertionError(arguments)
```