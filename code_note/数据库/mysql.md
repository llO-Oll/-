![image-20220415115956025](mysql.assets/image-20220415115956025.png)

## DDL-数据库操作

```mysql
# 查询所有数据库
show databases;

# 查询当前数据库
SELECT DATABASE();

# 创建
CREATE DATABASE [IF NOT EXISTS] 数据库名 [DEFAULT CHARSET 字符集] [COLLATE 排序规则];

# 删除
DROP DATABASE [IF EXISTS] 数据库名;

# 使用
USE 数据库名;
```

## DDL-表操作

### 查询

```mysql
# 查询当前数据库所有表
SHOW TABLES;

# 查询表结构
DESC 表名;

# 查询指定表的建表语句
SHOW CREATE TABLE 表名;
```

### 创建

```mysql
CREATE TABLE 表名(
    字段1 字段1类型[COMMENT 字段1注释],
    ……
    字段n 字段n类型[COMMENT 字段n注释]
)[COMMENT 标注释];

# 示例
create tables ta_user(
    id int comment '编号',
    name varchar(50) comment '姓名',
    age int comment '年龄',
    gender varchar(1) comment '性别'
) comment '用户表';
```

### 数据类型

![image-20220415121206991](mysql.assets/image-20220415121206991.png)

![image-20220415121759980](mysql.assets/image-20220415121759980.png)

![image-20220415122116698](mysql.assets/image-20220415122116698.png)

### 修改

```sql
# 添加字段
ALTER TABLE 表名 ADD 字段名 类型(长度) [COMMENT 注释] [约束];

# 修改数据类型
ALTER TABLE 表名 MODIFY 字段名 新数据类型(长度);

# 修改字段名和字段类型
ALTER TABLE 表名 CHANGE 旧字段名 新字段名 类型(长度) [COMMENT 注释] [约束]; 

# 修改表名
ALTER TABLE 表名 RENAME TO 新表名;

# 删除字段
ALTER TABLE 表名 DROP 字段名;

# 删除表
DROP TABLE[IF EXISTS] 表名;

# 删除并重新创建该表
TRUNCATE TABLE 表名;
```

## DML-数据操作

### DML-添加数据

```sql
# 指定字段添加数据
INSERT INTO 表名(字段1，字段2，……) VALUES(值1,值2,……);

# 给全部字段添加数据
INSERT INTO 表名 VALUES(值1,值2,……);

# 批量添加数据
INSERT INTO 表名(字段1，字段2，……) VALUES(值1,值2,……),(值1,值2,……),(值1,值2,……);
```

### DML-修改数据

```
UPDATE 表名 SET 字段名1 = 值1, 字段名2 = 值2,……[WHERE 条件];
```

### DML-删除数据

```mysql
DELETE FROM 表名 [WHERE 条件]
```

## DQL

数据查询语言

```sql
SELECT 字段列表 FROM 表名列表 WHERE 条件列表 
GROUP BY 分组字段列表 HAVING 分组后条件列表
ORDER BY 排序字段列表
LIMIT 分页参数
```

### 条件查询WHERE

![](assets/2023-02-11-10-05-49-image.png)

### 分组查询GROUP BY

```sql
-- 根据性别分组，统计男性员工和女性员工数量
select gender，count(*) from emp group by gender;
/*
gender count
男     7
女     9
*/


-- 查询年龄小于45的员工，并根据工作地址分组，获取员工数量大于等于3的工作地址
select workaddress count(*) from emp where age < 45 group by workaddress having count(*) > 3
```

![](assets/2023-02-11-10-30-56-image.png)

### 排序查询ORDER BY

<img title="" src="assets/2023-02-11-10-33-39-image.png" alt="" data-align="center">

### 分页查询

![](assets/2023-02-11-10-58-54-image.png)

### DQL执行顺序

![](assets/2023-02-11-11-01-01-image.png)

## DCL

数据控制语言

![](assets/2023-02-11-11-12-56-image.png)

![](assets/2023-02-11-14-01-05-image.png)

<img title="" src="assets/2023-02-11-14-02-18-image.png" alt="" data-align="center">

## 函数



## 约束

作用于**表中字段**上的规则，用于限制存储在表中的数据。

作用：保证数据库中数据的正确、有效性和完整性。

<img src="assets/2023-02-11-14-38-40-image.png" title="" alt="" data-align="left">



示例

| 字段     | 字段含义   | 字段类型        | 约束条件          | 约束关键字                      |
| ------ |:------:| ----------- | ------------- | -------------------------- |
| id     | ID唯一标识 | int         | 主键，并且自动增长     | PRIMARY KEY,AUTO_INCREMENT |
| name   | 姓名     | varchar(10) | 不为空，并且唯一      | NOT NULL,UNIQUE            |
| age    | 年龄     | int         | 大于0，并且小于等于120 | CHECK                      |
| status | 状态     | char(1)     | 默认为1          | DEFAULT                    |
| gender | 性别     | char(1)     | 无             |                            |



```sql
create table user(
    id int primary key auto_increment comment '主键',
    name varchar(10) not null unique comment '姓名',
    age int check(age > 0 && age <= 120) comment '年龄',
    status char(1) default '1' comment '状态',
    gender char(1) comment '性别'
)comment '用户表';


insert into user(name,age,status,gender) 
values('tom1',19,'1','男'),
('tom2',25,'0','男');
```



### 外键约束

让两张表的数据之间建立连接，从而保证数据的一致性和完整性。


