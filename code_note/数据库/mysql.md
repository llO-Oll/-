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

```mysql
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

```mysql
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

```sql
SELECT 字段列表 FROM 表名列表 WHERE 条件列表 
GROUP BY 分组字段列表 HAVING 分组后条件列表
ORDER BY 排序字段列表
LIMIT 分页参数
```


