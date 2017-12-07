###Chapter_1 Welcome

Python 是一门优雅而健壮的编程语言，继承了传统编译语言的强大性和通用性，同时也借鉴了简单脚本和解释语言的易用性。

简洁的代码设计，高级的数据结构和模块化的组件，可以在提升项目规模的同时，确保灵活性和一致性，并缩短必要的调试时间。（Shell的代码重用率很低，又臭又长）

- 可以进行系统调用的解释性脚本语言。不是纯粹的解释性语言，源文件被解释器编译成字节码。
- 有高级的数据结构，在核心语言提供这些重要的”构建单元“， 可以减少以前“框架”开发需要的时间，并提高可读性。（C语言中，类似list与map的结构常常被重复实现，反复拷贝）
- 面向对象，而且融汇了函数语言等多种风格。
- 项目组织成多个文件和模块，一致的访问语法，灵活的扩展性。可以用C重写性能瓶颈，一致的模块接口及一致的代码和数据访问方法。
- “安全合理”的异常处理机制。提供“堆栈跟踪”。
- 高效的原型开发工具。标准库完备，第三方模块丰富。
- 内存管理器，内存管理由解释器负责。

调用python脚本时，会先载入python 解释器，然后运行脚本。

- python script.py
- **chmod u+x script.py; ./script.py**

脚本第一行使用shell魔术字符串， #! /usr/bin/python， 如果解释器路径不确定或可能变化，使用，**#! /usr/bin/env python**



### Chapter_2 Quick Start

学习一门语言的一般路线：

1. 基本输入输出

   - **raw_input**([prompt]):  string with no '\n'
   - **print**: keyword

2. 基本风格

   - 注释： #
   - doc string
   - 使用强制性缩进对齐来表示代码块 （而非{})

3. 运算符

   - 算数： +,-,*,/,%,**  (不支持++, --)
   - 比较： <, <=, >, >=, ==, !=
   - 逻辑： and, or, not

4. 变量与赋值

   - **动态强类型语言**
   - C标识符
   - 赋值(=)时，**确定类型，创建对象，赋值引用**
   - 支持增量赋值(+=)

5. 基本类型

   - 数字： int, long, bool, float, complex
   - 字符串： str （', '', ''')
   - 列表: list ([])
   - 元组: tuple (())
   - 字典: dict ({})

6. 条件和循环语句

   ```python
   #if
   if True:
       print "hello"
       
   #if-else
   if True:
       print "hello"
   else:
       print "bye"

   #if-elif-else
   if True:
       print "hello"
   elif True:
       print "haha"
   else:
       print "byte"
       
   #while
   counter = 5
   while counter > 0:
       print counter
       counter -= 1
       
   #for
   array = "abcd"
   for item in array:  #range(4)
       print item

   for id in range(len(array)):
       print id, array[id]

   for id,item in enumerate(array):
       print id,item
       
   #列表解析
   square = [x ** 2 for x in range(5)]
   even = [x for x in range(10) if x % 2 == 0]
   ```

7. 文件操作

8. 异常处理

9. 函数

10. 类

11. 模块

    ```python
    #! /usr/bin/env python

    import os

    def write_txt():
        '''
    Write each line from stdin to make a txt file.
        '''
        file_name = raw_input("Please input file name:").lstrip()
        if os.path.exists(file_name):
            print "file %s already exists!" % file_name
            return False

        fp = file(file_name, "w")

        lines = []
        while True:
            line = raw_input(">")
            if line == ".":
                break
            #print >> fp, line
            lines.append(line)

        fp.writelines(["%s%s" % (x, os.linesep) for x in lines])

        fp.close()

    def read_txt():
        '''
        Read strings from a txt file.
        '''
        file_name = raw_input("Please input file name: ").lstrip()
        #if not os.path.isfile(file_name):
        #    print "file %s is not exists!" % file_name
        #    quit()

        try:
            fp = file(file_name, "r")
        except IOError, e:
            print "file %s open error:\n %s" %  (file_name, e)
        else:
            #for i in fp.readlines():
            for i in fp:
                print i,
            fp.close()

    if __name__ == "__main__":
        #write_txt()
        #read_txt()

    ```

    ​

    ​

