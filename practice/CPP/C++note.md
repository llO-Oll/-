# 输入输出

`<<`输出运算符

接受两个运算对象，左侧运算对象必须是一个ostream对象，右侧是要打印的值。此运算法符将给定的值写到给定的ostream对象中，**输出运算符的结果就是其左侧运算对象（ostream对象）**。

```c++
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

```c++
int ival = 1024;
int &refVal = ival;
int &refVal12;

refVal = 2;
int ii = refVal;
int &refVal13=refVal;	//refVal13绑定到了那个refVal绑定的对象上，这里就是绑定到ival上。

```

```c++
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



### 指针

#### 获取对象的地址

指针存放某个对象的地址，要想获取该地址，需要使用取地址符(操作符`&`)

```c++
int ival = 42;
int *p = &ival;	//p存放变量ival的地址，或者说p是指向变量的指针	
```

```c++
double dval;
double *pd = &dval;	//	正确：初始值是double型对象的地址
double *pd2 =pd;	//	正确：初始值是指向double对象的指针

int *pi = pd;		//	错误：指针pi的类型和pd的类型不匹配
```

#### 指针值

指针的值(地址)应属于下列4种状态：

1. 指向一个对象；
2. 指向紧邻对象所占空间的下一个位置；
3. 空指针，指针没有指向任何对象；
4. 无效指针，上述情况之外的其他值；

#### 利用指针访问对象

如果指针指向了一个对象，则允许使用解引用符(操作符*)来访问该对象:

```c++
int ival = 42;
int *p = &ival;	//	p存放者ival的地址，或者说p是指向变量ival的指针
cout << *p;		//	由符号*得到指针p所指向的对象，输出42
```

对指针解引用会得出所指的对象，因此如果给解引用的结果赋值，实际上也就是给指针所指的对象赋值；

```c++
*p = 0;		//	由符号*得到指针p所指向的对象，即可经由p为变量ival赋值
cout << *p;	//	由符号*得到指针p所指的对象，输出42
```

**`&`和`*`的多重含义**

像`&`和`*`既能做表达式里的运算符，也能作为声明的一部分出现：

```c++
int i = 42;
int &r = i;		//	&紧随类型名出现，是声明的一部分，r是一个引用
int *p;			//	*紧随类型名出现，是神明的一部分，p是一个指针
p = &i;			//	&出现在表达式中，取地址符
*p = i;			//	*出现在表达式中，解引用符
int &r2 = *p;	//	&是声明的一部分，*是一个解引用符
```

#### 空指针

生成空指针的方法：

```c++
int *p1 = nullptr;	//	等效于int *p1 = 0;
int *p2	= 0;		//	直接将p2初始化为字面常量0;
```

把`int`变量直接复制给指针是错误的操作，即使`int`变量的值恰好等于0也不行。

```c++
int zero = 0;
pi = zero;		//	错误：不能把int变量直接赋值给指针
```

#### 赋值和指针

```c++
int i = 42;
int *pi = 0;		//	pi被初始化，但没有指向任何对象
int *pi2 = &i;		//	pi2被初始化，存有i的地址
int *pi3;			//	如果pi3定义于块内，则pi3的值是无法确定的

pi3 = pi2;			//	pi3和pi2指向同一个对象i
pi2 = 0;			//	现在pi2不指向任何对象

pi = ival;			//	pi的值被改变，现在pi指向了ival
*pi = 0;			//	ival的值被改变，指针pi并没有改变
```

#### void* 指针

`void*`是一种特殊的指针类型，可用于存放任意对象的地址。

#### 指向指针的指针

```c++
int ival = 1024;
int *pi = &ival;	//	pi指向一个int的数
int **ppi = &pi;	//	ppi指向一个int的指针
```

#### 指向指针的引用

```c++
int i = 42;
int *p;			//	p是一个int型指针
int *&r = p;	//	r是一个对指针p的引用

r = &i;			//	r引用了一个指针，因此给r赋值&i
*r = 0;

```



### `const`限定符

定义一个变量，他的值不能被改变。默认状态下，const对象仅在文件内有效。

在多个文件间共享，在声明和定义之前添加`extern`关键字。



### 处理类型

#### 类型别名`typedef`



#### `auto`类型说明符

自动分析表达式所属的类型。

```c++
auto item = val1 + val2;	//item初始化为val1和val2相加的结果，item类型与他俩一致。
```

#### `decltype`类型指示符



# 字符串、向量和数组



## 命名空间的`using`声明

```c++
using namespce::name;
```

## 标准库类型String

```c++
#include <string>
using std::string;
```

### 定义和初始化`string`对象

```c++
string s1;
string s2 = s1;
string s3 = "hiya";
string s4(10,'c');		//s4的内容是cccccccccc
```

### string对象的操作

```c++
os<<s
is>>s
getline(is,s) 
s.empty()
s.size()
s[n]
s1+s2
s1=s2
s1==s2  
```

### 读写string对象

```c++
int main()
{
	string s;			//	空字符串
	cin >> s;			//	将string对象读入s，遇到空白停止
	cout << s << endl;	//	输出s
	return 0;
}
```

