# 输入输出

`<<`输出运算符

接受两个运算对象，左侧运算对象必须是一个`ostream`对象，右侧是要打印的值。此运算法符将给定的值写到给定的`ostream`对象中，**输出运算符的结果就是其左侧运算对象（`ostream`对象）**。

```c++
std::cout << "Enter two number:" << std::endl;
```

上述代码等于`(std::cout << "Enter two number:") << std::endl `

`endl`操纵符

写入`endl`的效果是结束当前行，并将与设备关联的缓冲区（buffer）中的内容刷到设备中。缓冲刷新操作可以保证到目前为止程序所产生的所有输出都真正写入输出流中，而不是仅停留在内存中等待写入流。

`>>`输入运算符

接受一个`istream`对象作为其左侧运算对象。

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

#### const的引用

```c++
const int ci = 1024;
const int &r1 = ci;		//	正确：引用及其对应的对象都是常量
r1 = 42;				//	错误：r1是对常量的引用，常量
int &r2 = ci;			//	错误：试图让一个非常量引用指向一个常量对象
```

**允许一个常量引用绑定非常量的对象、字面值，甚至是个一般表达式：**

```c++
int i = 42;				
const int &r1 = i;		//允许将const int&绑定到一个普通int对象上
const int &r2 = 42;		//正确：r2是一个常量引用
const int &r3 = r1 * 2;	//正确：r3是一个常量引用
int &r4 = r1 * 2;		//错误：r4是一个普通的非常量引用
```

**常量引用如果引用一个并非const的对象，不能通过常量引用改变绑定对象的值。**

```c++
int i = 42;
int &r1 = i;		//	引用ri绑定对象i
const int &r2 = i;	//	r2也绑定对象i,但是不允许通过r2修改i的值
r1 = 0;				//	r1也并非常量，i的值修改为0
r2 = 0;				//	错误：r2是一个常量引用
```

#### 指针和const

与引用一样，也可以令指针指向常量或非常量。类似于常量引用，**指向常量的指针**不能用于改变其所指对象的值。**要想存放常量对象的地址，只能使用指向常量的指针**：

```c++
const double pi = 3.14;		//	pi是个常量，它的值不能改变
double *ptr = &pi;			//	错误：ptr是一个普通指针
const double *cptr = &pi;	//	正确：cptr可以指向一个双精度常量
*cptr = 42;					//	错误：不能给*cptr赋值
```

#### const指针

**常量指针**必须初始化，而且一旦初始化完成，则它的值（也就是存放指针中的那个地址）就不能再改变。把`*`放在`const`关键字之前用以说明指针是一个常量，不变的是指针本身的值而非指向的那个值：

```c++
int errNumb = 0;
int *const curErr = &errNumb;	//	curErr将一直指向errNumb
const double pi = 3.14159;
const double *const pip = &pi;	//	pip是一个指向常量对象的常量指针
```

要想弄清楚这些声明的含义最行之有效的办法是从右向左阅读。此例中，离`curErr`最近的符号是`const`,意味着`curErr`本身是一个常量对象，对象的类型由声明符的其余部分确定。**声明符中的下一个符号是`*`,意思是`curErr`是一个常量指针。**

`pip`是一个指向常量的常量指针，不论是pip所指的对象值还是pip自己存储的那个地址都不能改变。相反的，`curErr`指向的是一个一般非常量整数，可以用`curErr`去修改`errNumb`的值：

```c++
*pip = 2.72;				//	错误：pip是一个指向常量的指针

if (*curErr){
	*curErr = 0;			//	正确：把curErr所指的对象的值重置
}

```

#### 顶层const

指针本身是一个对象，它又可以指向另外一个对象。因此，**指针本身是不是常量以及指针所指的是不是一个常量就是两个相互独立的问题**。

**顶层`const`**(top-level const)表示指针本身是个常量，**底层`const`**(low- level const)表示指针所指的对象是一个常量。

更一般的，顶层`const`可以表示任意的对象是常量，这一点对任何数据类型都适用，如算术类型、类、指针等。底层`const`则与指针和引用等复合类型的基本类型部分有关。
比较特殊的是，指针类型既可以是顶层`const`也可以是底层`const`,这一点和其他类型相比区别明显：

```c++
int i = 0;
int *const p1 = &i;		//	不能改变p1的值，这是一个顶层const
const int ci = 42;		//	不能改变ci的值，这是一个顶层const
const int *p2 = &ci;	//	允许改变p2的值，这是一个底层const
const int *const p3 = p2;	//	靠右的const是顶层const，靠左的是底层
const int &r = ci;		//	用于声明引用的const都是底层const
```



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

## 标准库类型vector

`vector`表示对象的集合，其中所有对象的类型都相同。也被称作容器。

### 定义和初始化`vetor`对象

```c++
vector<T> v1;		//v1是一个空vector，他潜在的元素是T类型
vector<T> v2(v1);	//v2中包含有v1所有元素的副本
vector<T> V2 = V1;	//等价于v2(v1),v2中包含有v1所有元素的副本
vector<T> v3(n,val);//v3包含了n个重复的元素val


```

### vector的操作

```c++
v.push_back(t)		//向v的尾端添加一个值为t的元素
v.empty()			//如果v不含有任何元素，返回真；否则返回假
v.size()			//返回v中元素的个数
```

## 迭代器

我们知道，尽管不同容器的内部结构各异，但它们本质上都是用来存储大量数据的，换句话说，都是一串能存储多个数据的存储单元。因此，诸如数据的排序、查找、求和等需要对数据进行遍历的操作方法应该是类似的。

既然类似，完全可以利用泛型技术，将它们设计成适用所有容器的通用算法，从而将容器和算法分离开。但实现此目的需要有一个类似中介的装置，它除了要具有对容器进行遍历读写数据的能力之外，还要能对外隐藏容器的内部差异，从而以统一的界面向算法传送数据。

这是泛型思维发展的必然结果，于是迭代器就产生了。简单来讲，迭代器和C++的指针非常类似，它可以是需要的任意类型，通过迭代器可以指向容器中的某个元素，如果需要，还可以对该元素进行读/写操作。

### 迭代器类别

常用的迭代器按功能强弱分为**输入迭代器**、**输出迭代器**、**前向迭代器**、**双向迭代器**、**随机访问迭代器** 5 种。本节主要介绍后面的这 3 种迭代器。

1. 前向迭代器（forward iterator）
   假设 p 是一个前向迭代器，则 p 支持 ++p，p++，*p 操作，还可以被复制或赋值，可以用 == 和 != 运算符进行比较。此外，两个正向迭代器可以互相赋值。

2. 双向迭代器（bidirectional iterator）
   双向迭代器具有正向迭代器的全部功能，除此之外，假设 p 是一个双向迭代器，则还可以进行 --p 或者 p-- 操作（即一次向后移动一个位置）。

3. 随机访问迭代器（random access iterator）
   随机访问迭代器具有双向迭代器的全部功能。除此之外，假设 p 是一个随机访问迭代器，i是一个整型变量或常量，则 p 还支持以下操作：

   - p+=i：使得 p 往后移动i个元素。

   - p-= i：使得 p 往前移动i个元素。

   - p+ i：返回 p 后面第i个元素的迭代器。

   - p- i：返回 p 前面第i个元素的迭代器。

   - p[i]：返回 p 后面第i个元素的引用。

     

| 容器                               | 对应的迭代器类型 |
| :--------------------------------- | :--------------- |
| array                              | 随机访问迭代器   |
| vector                             | 随机访问迭代器   |
| deque                              | 随机访问迭代器   |
| list                               | 双向迭代器       |
| set / multiset                     | 双向迭代器       |
| map / multimap                     | 双向迭代器       |
| forward_list                       | 前向迭代器       |
| unordered_map / unordered_multimap | 前向迭代器       |
| unordered_set / unordered_multiset | 前向迭代器       |
| stack                              | 不支持迭代器     |
| queue                              | 不支持迭代器     |



### 迭代器定义

| 迭代器定义方式 | 具体格式                                     |
| -------------- | -------------------------------------------- |
| 正向迭代器     | `容器类名::iterator 迭代器名;`               |
| 常量正向迭代器 | `容器类名::const_iterator 迭代器名;`         |
| 反向迭代器     | `容器类名::reverse_iterator 迭代器名;`       |
| 常量反向迭代器 | `容器类名::const_reverse_iterator 迭代器名;` |

通过定义以上几种迭代器，就可以读取它指向的元素，`*迭代器名`就表示迭代器指向的元素。其中，常量迭代器和非常量迭代器的分别在于，通过非常量迭代器还能修改其指向的元素。另外，反向迭代器和正向迭代器的区别在于：

- 对正向迭代器进行 ++ 操作时，迭代器会指向容器中的后一个元素；
- 而对反向迭代器进行 ++ 操作时，迭代器会指向容器中的前一个元素。

```c++
//遍历 vector 容器。
#include <iostream>
//需要引入 vector 头文件
#include <vector>
using namespace std;
int main()
{
    vector<int> v{1,2,3,4,5,6,7,8,9,10}; //v被初始化成有10个元素
    cout << "1:" << endl;
    //size返回元素个数
    for (int i = 0; i < v.size(); ++i)
        cout << v[i] <<" "; //像普通数组一样使用vector容器
    //创建一个正向迭代器，当然，vector也支持其他 3 种定义迭代器的方式
    
       cout << endl << "2:" << endl;
       vector<int>::iterator i;
    //用 != 比较两个迭代器
    for (i = v.begin(); i != v.end(); ++i)
        cout << *i << " ";
    
       cout << endl << "3:" << endl;
    for (i = v.begin(); i < v.end(); ++i) //用 < 比较两个迭代器
        cout << *i << " ";
   
       cout << endl << "4:" << endl;
    i = v.begin();
    while (i < v.end()) { //间隔一个输出
        cout << *i << " ";
        i += 2; // 随机访问迭代器支持 "+= 整数"  的操作
    }
   system("pause");
   return 0;
}

```

```c++
第一种遍历方法：
1 2 3 4 5 6 7 8 9 10
第二种遍历方法：
1 2 3 4 5 6 7 8 9 10
第三种遍历方法：
1 2 3 4 5 6 7 8 9 10
第四种遍历方法：
1 3 5 7 9
```

list 容器的迭代器是双向迭代器。假设 v 和 i 的定义如下：

```c++
//创建一个 v list容器
list<int> v;
//创建一个常量正向迭代器，同样，list也支持其他三种定义迭代器的方式。
list<int>::const_iterator i;
```

则以下代码是合法的：

```c++
for(i = v.begin(); i != v.end(); ++i)
    cout << *i;
```

以下代码则不合法，因为双向迭代器不支持用“<”进行比较：

```c++
for(i = v.begin(); i < v.end(); ++i)
    cout << *i;
```

以下代码也不合法，因为双向迭代器不支持用下标随机访问元素：

```c++
for(int i=0; i<v.size(); ++i)
    cout << v[i];
```

 ## 数组



# 表达式





# 函数

## 参数传递

形参初始化的机理与变量初始化一样。

如果形参是引用类型，它将绑定到对应的实参上；否则，将实参的值拷贝后赋给形参。

### 引用传递

当形参是引用类型时，引用实参是它绑定的对象的别名；也就是说，引用形参是它对应的实参的别名。

### 值传递

当实参的值被拷贝给形参时，形参和实参是两个相互独立的对象。

### const形参和实参



### 数组形参

数组的两个特殊性质对我们定义和使用作用在数组上的函数有影响，这两个性质分别是：**不允许拷贝数组，使用数组时通常会将其转换成指针**。

当我们为函数传递一个数组时，实际上**传递的是指向数组首元素的指针**。

#### 使用标记制定数组长度

```c++
// C风格字符串存储在字符数组中，并且在最后一个字符后面跟着一个空字符。
//函数在处理C风格字符串时遇到空字符停止：
void print(const char *cp)
{
	if (cp)					//	若cp不是一个空指针
		while(*cp)			//	只要指针所指的字符不是空字符
			count << *cp++;	//	输出当前字符并将指针向前移动一个位置
}
```

*这种方法对于像int这样所有取值都是合法值得数据不适用。*



#### 使用标准库规范

```c++
void print(const int *beg,	const int *end)
{
    //	输出beg到end之间(不含end)的所有元素
    while(beg!=end)
        cout << *beg++ << endl;//	输出当前元素并将指针向前移动一个位置
}

int j[2] = {0,1};
//	j转换成指向它首元素的指针
//	第二个实参是指向j的尾后元素的指针
print(begin(j),end(j));		//begin和end函数
```



### main：处理命令行选项

有时我们需要给main传递实参，一种常见的情况是用户通过设置一组选项来确定函数所要执行的操作。例如，假定main函数位于可执行文件prog之内，我们可以向程序传递下面的选项：

```
prog -d -o ofile data0
```

这些命令行选项通过两个(可选的)形参传递给main函数：

```c++
int main(int argc, char *argv[]){...}
```

第二个形参`argv`指向char*，它的元素是指向C风格字符串的指针；第一个形参`argc`表示数组中字符串的数量。因为第二个形参是数组，所以main函数也可以定义成：

```c++
int main(int argc, char **argv){...}
```

当实参传给main函数之后，`argv`的第一个元素指向程序的名字或者一个空字符串，接下来的元素依次传递命令行提供实参。最后一个指针之后的元素保证为0。

以上面提供的命令行为例。



```c++
// argc等于5
argv[0] = "prog";
argv[1] = "-d";
argv[2] = "-o";
argv[3] = "ofile";
argv[4] = "data0";
argv[5] = 0;
```



------

# 类

类的基本思想是**数据抽象**(data abstraction)和**封装**(encapsulation)。数据抽象是一种依赖于**接口**(interface)和**实现**(implementation)分离的编程技术。类的接口包括用户所能执行的操作；类的实现则包括累的数据成员、负责接口实现的函数体以及定义类所需的各种私有函数。

## 定义类

```c++
struct Sales_data{
    //	新成员：关于Sales_data对象的操作
    std::string isbn() const { return bookNo; }
    Sales_data& combine (const Sales_data&);
    double avg_price() const;
    //	数据成员
	std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};
//	Sales_data的非成员接口函数
Sales_data add(const Sales_data&, const Sales_data&);
std::ostream &print(std::ostream&,const Sales_data&);
std::istream &read(std::istream&, Sales_data&);
```

### 定义成员函数

对于Sales_data类，`isbn()`函数定义在了类内，而`combine`和`avg_price`定义在了类外。

我们首先介绍`isbn`函数，它的参数列表为空，返回值是一个string对象

```c++
std::string isbn() const { return bookNo; }
```

和其他函数一样，成员函数体也是一个块。在此例中，块只有一条`return`语句，用于返回`Sales_data`对象的`bookNo`数据成员。关于`isbn`函数一件有意思的事情是：它是如何获得`bookNo`成员所依赖的对象的呢？

### 引入this

让我们再一次观察对`isbn`成员函数的调用：

```
total.isbn()
```

在这里，我们使用了点运算符来访问total对象的`isbn`成员，然后调用它。当我们调用成员函数时，实际上是在替某个对象调用它。当`isbn`使用`bookNo`时，它隐式地使用this指向的成员，就像我们书写了`this->bookNo`一样。`this`形参是隐式定义的。

### 引入const成员函数

`isbn`函数的另一个关键之处是紧随参数列表之后的`const`关键字，这里，`const`的作用是修改隐式`this`指针的类型。

**默认情况下，this的类型是指向类中非常量的常量指针。**例如在`Sales_data`成员函数中，`this`的类型是`Sales_data *const`。尽管`this`是隐式的，但它仍然需要遵循初始化规则，我们需要把this声明成`const Sales_data *const`。

然而，`this`是隐式的并且不会出现在参数列表中，所以在哪儿将`this`声明成指向常量的指针就成为我们必须面对的问题。C++语言的做法是允许把 `const`关键字放在成员函数的参数列表之后，此时，紧跟在参数列表后面的 `const`表示`this`是一个指向常量的指针。像这样使用`const`的成员函数被称作**常量成员函数**。

因为this是指向常量的指针，所以常量成员函数不能改变调用它的对象的内容。在上例中，`isbn`可以读取调用它的对象的数据成员，但是不能写入新值。

### 类作用域和成员函数

类本身就是一个作用域。类的成员函数的定义嵌套在类的作用域之内。

### 在类的外部定义成员函数

```c++
double Sales_data::avg_price() const {
	if (units_sold)
		return revenue/units_sold;
	else
		return 0;
}
```

函数名`Sales_data::avg_price`使用作用域运算符说明`avg_price`函数被声明在类`Sales_data`的作用域。

### 定义一个返回this对象的函数

函数`combine`的设计初衷类似于复合赋值运算符`+=`，调用该函数的对象代表运算符左侧的运算对象，右侧运算对象则通过显式的实参被传入函数：

```c++
Sales_data& Sales_data::combine(const Sales_data &rhs){
    units_sold += rhs.units_sold;	//把rhs的成员加到this对象的成员上
    revenue += rhs.revenue;
    return *this;					// 返回调用该函数的对象
}
```

当我们的交易处理程序调用如下的函数时，

```
total.combine(trans);
```

`total`的地址被绑定到隐式的`this`参数上，而`rhs`绑定到了trans上。因此，当combine执行下面的语句时，

```c++
units_sold += rhs.units_sold;	//	把rhs的成员添加到this对象的成员中
```

效果等同于求`total. units_sold`和`trans. unit_sold`的和，然后把结果保存到
`total. units_sold`中。

```c++
return *this;	//	返回调用该函数的对象
```



### 构造函数

```c++
struct Sales_data{
    //新增的构造函数
    Sales_data() = default;
    Sales_data(const std::string &s):bookNo(s) { }
    Sales_data(const std::string &s,unsigned n, double p):
                bookNo(s), units_sold(n), revenue(p*n) {}
    Sales_data(std::istream &);
    //之前已有的其他成员
    std::string isbn() const {	return bookNo;}
	Sales_data& combine(const Sales_data&);
    double avg_price() const;
    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};
```

`=default`的含义

`Sales_data()=default;`是默认构造函数。

### 拷贝、赋值和析构

除了定义类的对象如何初始化之外、类还需要控制拷贝、赋值和销毁对象时发生的行为。

## 访问控制与封装

### 访问说明符

`public`说明符后面的成员在整个程序内可被访问。

`private`说明符后的成员可以被类的成员函数访问。



------

# IO类





------

# 顺序容器

| 顺序容器类型 |                                          |
| ------------ | ---------------------------------------- |
| vector       | 可变大小数组                             |
| deque        | 双端队列                                 |
| list         | 双向链表                                 |
| forward_list | 单向链表                                 |
| array        | 固定大小数组                             |
| string       | 与vector相似的容器，但专门用于保存字符。 |

## 容器概览

### 容器定义和初始化

每个容器类型都定义了一个默认构造函数。除array之外，其他容器的默认构造函数都会创建一个指定类型的空容器，且都可以接受指定容器大小和元素初始值的参数。

```c++
//	默认构造函数，C如果是array，则c中元素按默认方式初始化；否则为空
C c;		

//	c1初始化为c2的拷贝。c1和c2必须是相同类型
C c1(c2)	
C C1=C2
 
//	c初始化为初始化列表中元素的拷贝。列表中元素的类型必须与C的元素类型相容。对于array类型，列表中元素数目必须等于或小于array的大小，任何遗漏的元素都进行值初始化。
C c{a,b,c…}		
C c={a,b,c…}

//	c初始化为迭代器b和e指定范围中的元素的拷贝。范围中元素的类型必须与C的元素类型相容(array不适用)
C c(b,e)

//	只有顺序容器(不包括array)的构造函数才能接受大小参数
C seq(n)	
    
C seq(n,t)	//seq包含n个初始化为值t的元素
    
```

### 容器赋值操作

```
c1=c2
```

## 顺序容器操作





------

# # 泛型算法

标准库容器定义的操作集合惊人得小。标准库并未给每个容器添加大量功能，而是提供了一组算法，这些算法中的大多数都独立于任何特定的容器。这些算法是通用的(generic,或称泛型的)：它们可用于不同类型的容器和不同类型的元素。

## 概述

大多数算法都定义在头文件`algorithm`中。

```c++
int val = 42;	//	我们将查找的值
auto result = find(vec.cbegin(),vec.cend(),val);
cout << "The value" << val
    << (result == vec.cend() ? "is not present" : " is present") << endl;
```



------

# 动态内存

