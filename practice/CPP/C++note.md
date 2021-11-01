# 输入输出

`<<`输出运算符

接受两个运算对象，左侧运算对象必须是一个ostream对象，右侧是要打印的值。此运算法符将给定的值写到给定的ostream对象中，**输出运算符的结果就是其左侧运算对象（ostream对象）**。

```
std::cout << "Enter two number:" << std::endl;
```

上述代码等于`(std::cout << "Enter two number:") << std::endl `

`endl`操纵符

写入`endl`的效果是结束当前行，并将与设备关联的缓冲区（buffer）中的内容刷到设备中。缓冲刷新操作可以保证到目前为止程序所产生的所有输出都真正写入输出流中，而不是仅停留在内存中等待写入流。

`>>`输入运算符

接受一个istream对象作为其左侧运算对象。

# 基本类型

## 复合类型

### 引用

引用即别名，引用并非对象，他只是为一个已经存在的对象所起的另外一个名字。

```
int ival = 1024;
int &refVal = ival;
int &refVal12;

refVal = 2;
int ii = refVal;
int &refVal13=refVal;	//refVal13绑定到了那个refVal绑定的对象上，这里就是绑定到ival上。

```

```
//引用的定义
int i = 1024, i2 = 2048;
int &r = i, r2 =i2;		//r是一个引用，与i绑定在一起，r2是int
int i3 = 1024, &ri = i3;//ri是一个引用，与i3绑定在一起
int &r3 = i3, &r4 = i2;	//r3和r4都是引用

//错误的定义
int &refVal4 = 10;	//错误：引用类型的初始值必须是一个对象
doubel dval = 3.14;
int &refVal5 = dval;//错误：此处引用类型的初始值必须为是int型对象
```





