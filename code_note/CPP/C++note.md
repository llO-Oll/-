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

## 泛型算法类型

### 只读算法

### 写容器元素的算法



```c++
vector<int> vec;//	空vector
//fill_n()接受一个单迭代器、一个计数值和一个值，将给定值赋予迭代器指向的元素开始的指定个元素
fill_n(vec.begin(),vec.size(),0);//将所有元素重置为0

```

非常容易犯的错误

```c++
vector<int> vec;//	空vector
//	灾难：修改vec中的10个(不存在)元素
fill_n(vec.begin(),10,0);
```

**back_inserter**

一种保证算法有足够元素空间来容纳输出数据的方法是使用**插入迭代器**。插入迭代器是一种向容器中添加元素的迭代器。通常情况，当我们通过一个迭代器向容器元素赋值时，值被赋予迭代器指向的元素。而当我们通过一个插入迭代器赋值时，一个与赋值号右侧值相等的元素被添加到容器中。

```c++
vector<int> vec;	//	空向量
auto it = back_inserter(vec);	//	通过它赋值会将元素添加到vec中
*it = 42;	//	vec中现在有一个元素，值为42
```

```c++
vector<int> vec;	//	空向量
//	正确：back_inserter创建一个插入选代器，可用来向vec添加元素
fill_n(back_inserter(vec),10,0);	//添加10个元素到vec
```

**拷贝算法**

拷贝(copy)算法是另一个向目的位置迭代器指向的输出序列中的元素写入数据的算法。此算法接受三个迭代器，前两个表示一个输入范围，第三个表示目的序列的起始位置。此算法将输入范围中的元素拷贝到目的序列中。传递给copy的目的序列至少要包含与输入序列一样多的元素，这一点很重要。

```c++
int a1[] = {0,1,2,3,4,5,6};
int a2[sizeof(a1)/sizeof(*a1)];	//	a2与a1大小一样	数组元素个数 = 数组大小 / 单个元素的大小
//	copy返回的是其目的位置迭代器递增后的值	ret指向拷贝到a2的尾元素之后的位置
auto ret = copy(begin(a1),end(a1),a2);	//	把a1的内容拷贝给a2

```

### 重排容器元素的算法

## 定制操作

### 向算法传递函数

#### 谓词

谓词是一个可调用表达式，其返回结果是一个能用作条件的值。

```c++
//	比较函数，用来按长度排序单词
bool isShorter(const string &s1, const string &s2)
{
	return s1.size() < s2.size();
}
//	按长度由短至长排序words
sort(words.begin(),words.end(),isShorter);
```

### lambda表达式

C++11的一大亮点就是引入了Lambda表达式。利用Lambda表达式，**可以方便的定义和创建匿名函数**。

lambda表达式有如下优点：

- 声明式编程风格：就地匿名定义目标函数或函数对象，不需要额外写一个命名函数或者函数对象。以更直接的方式去写程序，好的可读性和可维护性。
- 简洁：不需要额外再写一个函数或者函数对象，避免了代码膨胀和功能分散，让开发者更加集中精力在手边的问题，同时也获取了更高的生产率。
- 在需要的时间和地点实现功能闭包，使程序更灵活。



lambda 表达式定义了一个匿名函数，并且可以捕获一定范围内的变量。lambda 表达式的语法形式可简单归纳如下：

```
[capture list](parameter list) -> retrun type { function body }
```

其中， capture list（捕获列表）是一个 lambda所在函数中定义的局部变量的列表（通常为空）； `return type`、` parameter list`和 `function body`与任何普通函数一样，分别表示返回类型参数列表和函数体。但是，与普通函数不同， lambda必须使用尾置返回来指定返回类型。

可以忽略列表和返回类型，但必须永远包含捕获列表和函数体

```c++
auto f = []{ return 42; };
cout << f() << endl;	//	打印42
```

#### 使用 lambda 表达式捕获列表

lambda 表达式还可以通过捕获列表捕获一定范围内的变量：

- `[]` 不捕获任何变量。
- `[&]` 捕获外部作用域中所有变量，并作为引用在函数体中使用（按引用捕获）。
- `[=]` 捕获外部作用域中所有变量，并作为副本在函数体中使用（按值捕获）。
- `[=，&foo]` 按值捕获外部作用域中所有变量，并按引用捕获 foo 变量。
- `[bar]` 按值捕获 bar 变量，同时不捕获其他变量。
- `[this]` 捕获当前类中的 this [指针](http://c.biancheng.net/c/80/)，让lambda表达式拥有和当前类成员函数同样的访问权限。如果已经使用了&或者=，就默认添加此选项。捕获 this 的目的是可以在lambda中使用当前类的成员函数和成员变量。

【实例】lambda 表达式的基本用法。

```c++
class A
{
    public:
    int i_ = 0;
    void func(int x, int y)
    {
        auto x1 = []{ return i_; };        // error，没有捕获外部变量
        auto x2 = [=]{ return i_ + x + y; }; // OK，捕获所有外部变量
        auto x3 = [&]{ return i_ + x + y; }; // OK，捕获所有外部变量
        auto x4 = [this]{ return i_; };      // OK，捕获this指针
        auto x5 = [this]{ return i_ + x + y; };// error，没有捕获x、y
        auto x6 = [this, x, y]{ return i_ + x + y; };// OK，捕获this指针、x、y
        auto x7 = [this]{ return i_++; };// OK，捕获this指针，并修改成员的值
    }
};
int a = 0, b = 1;
auto f1 = []{ return a; };             // error，没有捕获外部变量
auto f2 = [&]{ return a++; };          // OK，捕获所有外部变量，并对a执行自加运算
auto f3 = [=]{ return a; };            // OK，捕获所有外部变量，并返回a
auto f4 = [=]{ return a++; };          // error，a是以复制方式捕获的，无法修改
auto f5 = [a]{ return a + b; };        // error，没有捕获变量b
auto f6 = [a, &b]{ return a + (b++); };// OK，捕获a和b的引用，并对b做自加运算
auto f7 = [=, &b]{ return a + (b++); };// OK，捕获所有外部变量和b的引用，并对b做自加运算
```

#### lambda 表达式的类型

lambda 表达式的类型在 C++11 中被称为“闭包类型（Closure Type）”。它是一个特殊的，匿名的非union的类类型。

因此，我们可以认为它是一个带有 operator() 的类，即仿函数。因此，我们可以使用 `std::function` 和 `std::bind` 来存储和操作 lambda 表达式：



```c++
std::function<int(int)>  f1 = [](int a){ return a; };
std::function<int(void)> f2 = std::bind([](int a){ return a; }, 123);


```

另外，对于没有捕获任何变量的 lambda 表达式，还可以被转换成一个普通的函数指针：

```c++
using func_t = int(*)(int);
func_t f = [](int a){ return a; };
f(123);
```

lambda 表达式可以说是就地定义仿函数闭包的“语法糖”。它的捕获列表捕获住的任何外部变量，最终均会变为闭包类型的成员变量。而一个使用了成员变量的类的 operator()，如果能直接被转换为普通的函数指针，那么 lambda 表达式本身的 this 指针就丢失掉了。而没有捕获任何外部变量的 lambda 表达式则不存在这个问题。

这里也可以很自然地解释为何按值捕获无法修改捕获的外部变量。因为按照 C++ 标准，lambda 表达式的 operator() 默认是 const 的。一个 const 成员函数是无法修改成员变量的值的。而 mutable 的作用，就在于取消 operator() 的 const。

需要注意的是，没有捕获变量的 lambda 表达式可以直接转换为函数指针，而捕获变量的 lambda 表达式则不能转换为函数指针。看看下面的代码：

```c++
typedef void(*Ptr)(int*);
Ptr p = [](int* p){delete p;};  // 正确，没有状态的lambda（没有捕获）的lambda表达式可以直接转换为函数指针
Ptr p1 = [&](int* p){delete p;};  // 错误，有状态的lambda不能直接转换为函数指针
```

就地定义匿名函数，不再需要定义函数对象，大大简化了标准库算法的调用。比如，在 C++11 之前，我们要调用 for_each 函数将 vector 中的偶数打印出来，如下所示。

【实例】lambda 表达式代替函数对象的示例。

```c++
class CountEven
{
    int& count_;
public:
    CountEven(int& count) : count_(count) {}
    void operator()(int val)
    {
        if (!(val & 1))       // val % 2 == 0
        {
            ++ count_;
        }
    }
};
std::vector<int> v = { 1, 2, 3, 4, 5, 6 };
int even_count = 0;
for_each(v.begin(), v.end(), CountEven(even_count));
std::cout << "The number of even is " << even_count << std::endl;
```

这样写既烦琐又容易出错。有了 lambda 表达式以后，我们可以使用真正的闭包概念来替换掉这里的仿函数，代码如下：

```c++
std::vector<int> v = { 1, 2, 3, 4, 5, 6 };
int even_count = 0;
for_each( v.begin(), v.end(), [&even_count](int val)
        {
            if (!(val & 1))  // val % 2 == 0
            {
                ++ even_count;
            }
        });
std::cout << "The number of even is " << even_count << std::endl;
```

lambda 表达式的价值在于，就地封装短小的功能闭包，可以极其方便地表达出我们希望执行的具体操作，并让上下文结合得更加紧密。



------

# 动态内存

我们的程序到目前为止只使用过静态内存或栈内存。**静态内存用来保存局部static对象、类static数据成员以及定义在任何函数之外的变量**。**栈内存用来保存定义在函数内的非static对象**。分配在静态或栈内存中的对象由编辑器自动创建和销毁。对于栈对象，仅在其定义的程序块运行时才存在；static对象在使用之前分配，在程序结束时销毁。

除了静态内存和栈内存，每个程序还拥有一个内存池。这部分内存被称为**自由空间**或**堆**。程序用堆来存储**动态分配**的对象。动态对象的生命期由程序来控制，当动态对象不再使用时，我们的代码必须显示地销毁它们。

## 动态内存与智能指针

在C++中，动态内存地管理是通过一对运算符来完成的`new`，在动态内存中为对象分配空间并返回一个指向该对象的指针，我们可以选择对对象进行初始化；`delete`，接受一个动态对象的指针，销毁该对象，并释放与之关联的内存。

动态内存的使用很容易出问题，因为确保在正确的时间释放内存是极其困难的。有时我们会忘记释放内存，在这种情况下就会产生内存泄漏：有时在尚有指针引用内存的情况下我们就释放了它，在这种情况下就会产生引用非法内存的指针。

为了更容易（同时也更安全）地使用动态内存，新的标准库提供了两种智能指针(smartpointer)类型来管理动态对象。智能指针的行为类似常规指针，重要的区别是它负责自动释放所指向的对象。新标准库提供的这两种智能指针的区别在于管理底层指针的方式：shared_ptr允许多个指针指向同一个对象：unique_ptr则“独占”所指向的对象。标准库还定义了一个名为weak_ptr的伴随类，它是一种弱引用，指向shared_ptr所管理的对象。这三种类型都定义在 memory头文件中。

### shared_ptr类

类似vector，智能指针也是模板。因此，当我们创建一个智能指针时，必须提供额外的信息——指针可以指向的类型。

```c++
shared_ptr<string> p1;		//	shared_ptr, 可以指向string
shared_ptr<list<int>> p2;	//	shared_ptr,可以指向int的list
```

默认初始化的智能指针中保存着一个空指针。

智能指针的使用方式与普通指针类似。解引用一个智能指针返回它指向的对象。如果在一个条件判断中使用智能指针，效果就是检测它是否为空：

```c++
//	如果p1不为空，检查它是否指向一个空string
if (p1 && p1->empty())
	*p1 = "hi";	//	如果p1指向一个空string，解引用p1, 将一个新值赋予string
```

shared_ptr和unique_ptr都支持的操作

| `shared_ptr<T> sp`  |      |
| ------------------- | ---- |
| `unique_ptr<T>  up` |      |
| `p`                 |      |
| `*p`                |      |
| `p->mem`            |      |
| `p.get`             |      |
| `swap(p,q)`         |      |
| `p.swap(q)`         |      |

shared_ptr独有的操作

| `make_shared<T>(args)` | 返回一个shared_ptr,指向一个动态分配的类型为T的对象。使用args初始化此对象。 |
| ---------------------- | ------------------------------------------------------------ |
| `shared_ptr<T>p(q)`    | p是shared_ptr q的拷贝；此操作会递增q中的计数器。             |
| `p = q`                | p和q都是 shared_ptr,所保存的指针必须能相互转换。此操作会递减p的引用计数，递增q的引用计数：若p的引用计数变为0,则将其管理的原内存释放 |
| `p.unique()`           | 若`p.use_count()`为1，返回true；否则返回false                |
| `p.use_count()`        | 返回与p共享对象的智能指针数量：可能很慢，主要用于调试        |

#### make_shared函数

最安全的分配和使用动态内存的方法是调用一个名为 make shared的标准库函数此函数在动态内存中分配一个对象并初始化它，返回指向此对象的shared_ptr。与智能指针一样，make_ shared也定义在头文件 memory中。

```c++
//	指向一个值为42的int的shared_ptr
shared_ptr<int> p3 = make_shared<int>(42);
//	p4指向一个值为"9999999999"的string
shared_ptr<string> p4 = make_shared<string>(10,'9');
//	p5指向一个值初始化的int,值为0
shared_ptr<int> p5 = make_shared<int>();
//	P6指向一个动态分配的空vector<string>
auto p6 = make_shared<vector<string>>();
```

#### shared_ptr的拷贝和赋值

当进行拷贝或赋值操作时，每个shared_ptr都会记录有多少个其他shared_ptr指向相同的对象：

```c++
auto p = make_shared<int>(42);	//	p指向的对象只有p一个引用者
auto q(p);	//	p和q指向相同对象，此对象有两个引用者
```

我们可以认为每个 shared ptr都有一个关联的计数器，通常称其为**引用计数**( referencecount)。无论何时我们拷贝一个 shared_ptr,计数器都会递增。例如，当用一个shared_ptr初始化另一个 shared_ptr,或将它作为参数传递给一个函数以及作为函数的返回值时，它所关联的计数器就会递增。

当我们给shared_ptr赋予一个新值或是shared_ptr被销毁(例如一个局部的 shared_ptr离开其作用域时，计数器就会递减。

一旦一个 shared_ptr的计数器变为0,它就会自动释放自己所管理的对象：

```c++
auto r =make_shared<int>(42);	//	r指向的int只有一个引用者
r = q;	//	给r赋值，令它指向另一个地址
		//	递增q指向的对象的引用计数
		//	递减r原来指向的对象的引用计数
		//	r原来指向的对象已没有引用者，会自动释放
```

#### shared_ptr自动销毁所管理的对象……

 

#### ……shared_ptr还会自动释放相关联的内存