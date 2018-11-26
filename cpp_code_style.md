# <center>我的C++代码风格</center>

## 1 前言

&emsp;&emsp;在多人团队开发中，使用统一的代码规范，项目开发前期利于团队协作，中期利于工作审核，后期利于降低维护成本，总而言之，代码规范很重要。这话虽然简单，但一直未能引起重视，直到我看到下图的那个新闻后，我就知道有很多码农还活着真不简单啊。我们姑且不去深究这个新闻的真实性，但至少我们也该意识到代码规范的重要性了吧。

![0001.png](https://img2018.cnblogs.com/blog/1494489/201811/1494489-20181126165800614-127950833.png)

&emsp;&emsp;关于C++的代码风格，网上有很多，比较权威的有 **谷歌** 和 **华为** 这两种，我个人推荐使用 **华为** 的，这也是我喜欢的代码风格。之所以选择 **华为** 的，而不是 **谷歌** 的，主要还是因为它们形成的背景年代不一样。**谷歌的代码风格**，很多都受老一辈程序员（ Unix 时代的程序员）的影响，他们早期工作的显示设备，都是低分辨率的屏幕，代码倾向于压缩排版，以期待显示更多内容。而到了现在，我们工作的绝大多数显示设备都是高分辨率的屏幕，压缩排版的代码布局，看起来就显得过于紧凑和密集，这会导致我们阅读上的疲劳。相比之下，**华为的代码风格**，在代码排布上，很多地方经常会使用空格、空行进行分隔，这样显得比较宽松，阅读起来降低了疲劳感。

&emsp;&emsp;在这里，我不打算重复去谈代码编写中很细节的代码风格问题（比如 命名、对齐、缩进 等），而着重于谈谈我平时在 C++类编写时的代码排版方式，以及如何编写符合 Doxygen 文档生成规范的代码注释，并提一提我在用 IDE（QtCreator、VisualStudio） 编码时使用的一些技巧来改善代码的布局。


## 2 C++类的布局基本结构

&emsp;&emsp;通常的C++类编写，常使用头文件（\*.h、\*.hh 等）写类的声明，而用源文件（\*.cpp、\*.cc、\*.cxx 等）写类的实现，当然，对于模板类一般不会这样将声明与实现分开两部分文件编写，它要么是声明与实现合在一起放到头文件中，要么是头文件后用一个（或者多个）内嵌文件（如 \*.inl）去编写类的实现流程。此处，我们不提模板类的代码布局方式（直接参考常规类的代码布局方式），而着重谈常规类的代码布局。

#### 2.1 类声明文件（头文件）
先给出类声明文件的整体区段排序的摘要，如下所示：

  1. 文件说明区段：版权说明、版本号、生成日期、作者、内容、功能、修改日志等注释内容。
  2. 类声明的开始。
  3. 类中，共同属性内容区段（可选内容，主要填写一些 类的友元关系、隶属库的某些宏声明 等等）。
  4. 类中，构造函数 与 析构函数 区段。
  5. 类中，通用数据类型的声明 区段。
  6. 类中，类属性（静态属性）的相关函数与数据成员 区段。
  7. 类中，重载区段：重载父类的接口（虚函数）。
  8. 类中，扩展区段：继承自该类的子类可重载的接口（虚函数）。
  9. 类中，可对外调用的接口区段（public访问属性的成员函数）。
  10. 类中，只对内调用的接口区段（protected访问属性的成员函数）。
  11. 类中，类对象的相关数据成员 区段。
  12. 类声明的结束。

以上区段的排序，在实际项目中，可视具体情况做调整，也可增加其他区段。在此，我给出如下的样例代码（内容中，以 `//=>` 开头的内容，均为说明性的内容，非代码中要编写的内容）：

```
//=> 文件说明区段：版权说明、版本号、生成日期、作者、内容、功能、修改日志等注释内容。
/**
 * @file    FileName.h
 * <pre>
 * Copyright (c) 2018, Gaaagaa All rights reserved.
 * 
 * 文件名称：FileName.h
 * 创建日期：2018年11月25日
 * 文件标识：
 * 文件摘要：
 * 
 * 当前版本：1.0.0.0
 * 作    者：
 * 完成日期：2018年11月25日
 * 版本摘要：
 * 
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 * 版本摘要：
 * </pre>
 */

//=> 文件头的守卫宏定义（防止 文件头重复包含时 产生重复定义）
#ifndef __HEADFILENAME_H__
#ifndel __HEADFILENAME_H__

//=> 包含类设计所依赖的外部头文件
#include "dependent.h"
#include <dependent.h>

//=> 以 80 个正斜杠字符进行大区段分割，有承前启后的作用，这里标识一个大区段（类）内容的开始
////////////////////////////////////////////////////////////////////////////////
// class name  //=> 填写要设计的类名，表示这个大区段的内容主题

//=> 此处可增加一些类设计所依赖的其他数据类型、接口函数等的前置声明

//=> 类的简要说明（用于 Doxygen 文档生成操作）
/**
 * @class ClassName
 * @brief 类的简要描述。
 */
class ClassName : public SuperClass
{
    //=> 类的共同属性内容区段（可选内容，主要填写一些 类的友元关系、隶属库的某些宏声明 等等），举例如下：
    friend class FriendClass;     //=> 友元关系
    Q_OBJECT                      //=> QT 库的宏声明（或特性的实现）
    DECLARE_DYNCREATE(ClassName)  //=> 库的宏声明（或特性的实现）

    //=> 构造函数 与 析构函数 区段
    // constructor/destructor
public:
    ClassName(void);
    virtual ~ClassName(void);

    //=> 通用数据类型的声明
    // common data types
public:
    /**
     * @enum  emConstValue
     * @brief 类相关枚举常量值。
     */
    typedef enum emConstValue
    {
        ECV_CLASS_ID = 0x00000000,
    } emConstValue;

    typedef std::vector<Type> VecType;

    //=> 类属性（静态属性）的相关函数与数据成员
    // common invoking
public:
    //=> 关于 函数/方法 的注释方式，后面会提到
    /**********************************************************/
    /**
     * @brief 申请对象流水号。
     */
    static int make_object_seqno(void);

protected:
    //=> （类静态的 或 类对象的）成员变量的注释，符合 Doxygen 文档生成的规范两种注释
    static int ms_object_seqno_builder;    ///< 用于对象流水号申请操作的生成器  //=> 单行注释

    /** 所创类对象的计数器 */ //=> 与成员变量不同行的注释，可多行编写
    static int ms_object_counter;

    //=> 重载区段：重载父类的接口（虚函数）
    // overrides
protected:
    //=> 函数/方法 的注释方式
    /**********************************************************/ //=> 以此线进行分隔，表示函数的小区段
    /**
     * @brief 重载父类的接口。  //=> 接口的功能 简要说明
     * 
     * @param [in    ] param_1: 入参形式的参数注释。     //=> 几种功能类型的参数注释（可选段）
     * @param [out   ] param_2: 回参形式的参数注释。
     * @param [in,out] param_3: 可作入参和回参形式的参数注释。
     * 
     * @return int            //=> 返回值的注释（可选段）
     *         - 成功，返回 0；
     *         - 失败，返回 错误码。
     */
    virtual int super_method(int param_1, int & param_2, int & param_3) override;   //=> override 为 C++11 后的关键字，代码中可选

    //=> 扩展区段：继承自该类的子类可重载的接口（虚函数）
    // extensible interfaces
protected:
    //=> 函数/方法 的注释方式
    /**********************************************************/ //=> 以此线进行分隔，表示函数的小区段
    /**
     * @brief 继承自该类的子类可重载的接口。  //=> 接口的功能 简要说明
     * 
     * @param [in    ] param_1: 入参形式的参数注释。     //=> 几种功能类型的参数注释（可选段）
     * @param [out   ] param_2: 回参形式的参数注释。
     * @param [in,out] param_3: 可作入参和回参形式的参数注释。
     * 
     * @return int            //=> 返回值的注释（可选段）
     *         - 成功，返回 0；
     *         - 失败，返回 错误码。
     */
    virtual int extensible_method(int param_1, int & param_2, int & param_3);

    //=> 可对外调用的接口区段（public访问属性的成员函数）
    // public interfaces
public:
    //=> 精简的 函数/方法 的注释方式
    /**********************************************************/
    /**
     * @brief 获取对象标识 ID。
     */
    int get_object_id(void);

    //=> 只对内调用的接口区段（protected访问属性的成员函数）
    // inner invoking methods
protected:
    //=> 精简的 函数/方法 的注释方式
    /**********************************************************/
    /**
     * @brief 返回计算得到对象标识 ID。
     */
    int calc_object_id(void);

    //=> 类对象的相关数据成员 区段
    // data members
protected:
    int m_object_id;   ///< 对象标识 ID
};

//=> 以 80 个正斜杠字符进行大区段分割，有承前启后的作用，这里标识一个大区段（类）内容的结束
////////////////////////////////////////////////////////////////////////////////

#endif // __HEADFILENAME_H__


```

#### 2.2 类的实现文件（源文件）
对于类的文件，要描述的区段没有声明文件中的多，主要有如下区段：

  1. 文件说明区段：版权说明、版本号、生成日期、作者、内容、功能、修改日志等注释内容。
  2. 类实现的区段：类属性（静态属性）的相关函数与数据成员。
  3. 类实现的区段：构造函数 与 析构函数 区段。
  4. 类实现的区段：重载区段：重载父类的接口（虚函数）。
  5. 类实现的区段：扩展区段：继承自该类的子类可重载的接口（虚函数）。
  6. 类实现的区段：可对外调用的接口区段（public访问属性的成员函数）。
  7. 类实现的区段：只对内调用的接口区段（protected访问属性的成员函数）。

以上区段的排序，在实际项目中，可视具体情况做调整，也可增加其他区段。在此，也给出如下的样例代码（内容中，以 `//=>` 开头的内容，均为说明性的内容，非代码中要编写的内容）：

```
//=> 文件说明区段：版权说明、版本号、生成日期、作者、内容、功能、修改日志等注释内容。
/**
 * @file    FileName.cpp
 * <pre>
 * Copyright (c) 2018, Gaaagaa All rights reserved.
 * 
 * 文件名称：FileName.cpp
 * 创建日期：2018年11月25日
 * 文件标识：
 * 文件摘要：
 * 
 * 当前版本：1.0.0.0
 * 作    者：
 * 完成日期：2018年11月25日
 * 版本摘要：
 * 
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 * 版本摘要：
 * </pre>
 */

//=> 包含类设计所依赖的外部头文件
#include "dependent.h"
#include <dependent.h>

//=> 以 80 个正斜杠字符进行大区段分割
////////////////////////////////////////////////////////////////////////////////
// class name  //=> 大区段的内容主题（可多行注释）

//=> 以 70个字符（“=”）进行分割，表示次级区段
//====================================================================

// 
// ClassName : common invoking ( static methods and data )  //=> 类属性（静态属性）的相关函数与数据成员 区段
// 

//=> 优先声明静态成员（或初始化）
int ClassName::ms_object_seqno_builder = 0;  ///< 用于对象流水号申请操作的生成器
int ClassName::ms_object_counter       = 0;  ///< 所创类对象的计数器


//=> 再编写 静态函数的实现流程
/**********************************************************/
/**
 * @brief 申请对象流水号。
 */
int ClassName::make_object_seqno(void)
{
    return ++ms_object_seqno_builder;
}

//====================================================================

// 
// ClassName : constructor/destructor   //=> 构造函数 与 析构函数 区段
// 

ClassName::ClassName(void)
{
    m_object_id = calc_object_id();

    ClassName::ms_object_counter += 1;
}

ClassName::~ClassName(void)
{
    ClassName::ms_object_counter -= 1;
}

//====================================================================

// 
// ClassName : overrides   //=> 重载区段：重载父类的接口（虚函数）
// 

/**********************************************************/
/**
 * @brief 重载父类的接口。
 * 
 * @param [in    ] param_1: 入参形式的参数注释。
 * @param [out   ] param_2: 回参形式的参数注释。
 * @param [in,out] param_3: 可作入参和回参形式的参数注释。
 * 
 * @return int
 *         - 成功，返回 0；
 *         - 失败，返回 错误码。
 */
int ClassName::super_method(int param_1, int & param_2, int & param_3) override
{
    //=> 以 40 个字符（“=”）的短线分割，明确某个代码块具备一定的逻辑相关性，便于阅读与理解
    //======================================
    // 代码块说明

    param_2 = param_1 + get_object_id();
    param_3 = param_3 + get_object_id();

    //======================================

    return 0;
}

//====================================================================

// 
// ClassName : extensible interfaces   //=> 扩展区段：继承自该类的子类可重载的接口（虚函数）
// 

/**********************************************************/
/**
 * @brief 继承自该类的子类可重载的接口。
 * 
 * @param [in    ] param_1: 入参形式的参数注释。
 * @param [out   ] param_2: 回参形式的参数注释。
 * @param [in,out] param_3: 可作入参和回参形式的参数注释。
 * 
 * @return int
 *         - 成功，返回 0；
 *         - 失败，返回 错误码。
 */
int ClassName::extensible_method(int param_1, int & param_2, int & param_3)
{
    //======================================
    // 代码块说明

    param_2 = param_1 + get_object_id();
    param_3 = param_3 + get_object_id();

    //======================================

    return 0;
}

//====================================================================

// 
// ClassName : public interfaces   //=> 可对外调用的接口区段（public访问属性的成员函数）
// 

/**********************************************************/
/**
 * @brief 获取对象标识 ID。
 */
int ClassName::get_object_id(void)
{
    return m_object_id;
}

//====================================================================

// 
// ClassName : inner invoking   //=> 只对内调用的接口区段（protected访问属性的成员函数）
// 

/**********************************************************/
/**
 * @brief 返回计算得到对象标识 ID。
 */
int ClassName::calc_object_id(void)
{
    return ClassName::make_object_seqno();
}

```
在上面代码中， `ClassName::super_method()` 内部使用了短线分割进行代码逻辑块标注，这种方式，在较长代码流程中很便于代码的阅读和理解，我个人的很多代码中会经常使用这种方式进行代码块的注释。


## 3. 其他数据类型的风格

&emsp;&emsp;C++除了类这种数据类型外，主要的还有结构体、枚举、C函数这些，这部分基本上属于原来 C 部分的范畴，只要代码中的注释规范符合 Doxygen 的规范即可，其他的不做过多的说明，只给出简单的参考样例：
```
/**
 * @struct StructName
 * @brief  结构体的描述信息说明。
 */
typedef struct StructName
{
    int  v1;  ///< v1 字段说明
    int  v2;  ///< v2 字段说明
    int  v3;  ///< v3 字段说明
} StructName;

/**
 * @enum  emContext
 * @brief 枚举常量值。
 */
typedef enum emContext
{
    ECV_CONTEXT_VALUE_V1  = 0x0001,   ///< 枚举值说明
    ECV_CONTEXT_VALUE_V2  = 0x0002,   ///< 枚举值说明
    ECV_CONTEXT_VALUE_V3  = 0x0003,   ///< 枚举值说明
    ECV_CONTEXT_VALUE_V4  = 0x0004,   ///< 枚举值说明
} emContext;

/**********************************************************/
/**
 * @brief 字符串忽略大小写的比对操作。
 *
 * @param [in ] xszt_lcmp : 比较操作的左值字符串。
 * @param [in ] xszt_rcmp : 比较操作的右值字符串。
 *
 * @return x_int32_t
 *         - xszt_lcmp <  xszt_rcmp，返回 <= -1；
 *         - xszt_lcmp == xszt_rcmp，返回 ==  0；
 *         - xszt_lcmp >  xszt_rcmp，返回 >=  1；
 */
x_int32_t vx_stricmp(x_cstring_t xszt_lcmp, x_cstring_t xszt_rcmp)
{
    x_int32_t xit_lvalue = 0;
    x_int32_t xit_rvalue = 0;

    const x_char_t * xct_lptr = xszt_lcmp;
    const x_char_t * xct_rptr = xszt_rcmp;

    if (xszt_lcmp == xszt_rcmp)
        return 0;
    if (X_NULL == xszt_lcmp)
        return -1;
    if (X_NULL == xszt_rcmp)
        return 1;

    do
    {
        if (((xit_lvalue = (*(xct_lptr++))) >= 'A') && (xit_lvalue <= 'Z'))
        {
            xit_lvalue -= ('A' - 'a');
        }

        if (((xit_rvalue = (*(xct_rptr++))) >= 'A') && (xit_rvalue <= 'Z'))
        {
            xit_rvalue -= ('A' - 'a');
        }

    } while (xit_lvalue && (xit_lvalue == xit_rvalue));

    return (xit_lvalue - xit_rvalue);
}
```
## 4. IDE中的代码编写技巧

&emsp;&emsp;就我个人使用过的 IDE（针对 C++ 开发的），主要有 **Visual C++** 和 **QtCreator** 这两款，这里，我也就针对这两款 IDE 谈一谈我的一些使用技巧，怎么对自己编写的代码进行注释和排版。

#### 4.1 Visual C++

&emsp;&emsp;首先，我不得不说，VC++自身的代码智能提示真的很弱鸡，所以我安装完 VS IDE 后，都会安装 VisualAssitX 这个插件。按照上面提到我的代码风格，利用这个插件的代码片段快速插入功能，可极大提高代码编写的效率。

&emsp;&emsp;VisualAssitX的代码片段快速插入功能，按如下步骤，我们可以设置自定义的代码片段以及对应的快捷键：

1. 依次操作："VAssistX" => "Visual Assist Options..." => "Visual Assist Options"对话框 => 左"Suggestions" => 右侧"Edit VA Snippets"按钮 => "VA Snippet Editor"对话框；
2. 调出的 "VA Snippet Editor"对话框 如下图所示：<br>
![0002.png](https://img2018.cnblogs.com/blog/1494489/201811/1494489-20181126165951379-1149823690.png)
3. 在 `Language:` 的下拉选择框中，设置为 `C++` ，`Type:` 设置为 `All by Shortcut`；
4. 点击工具栏上的第一个按钮，即可新增“**代码片段**”，在右侧可编辑“**代码片段**”的“**Shortcut**”（快捷输入的短字符串）和代码片段“**内容**”；在这里，也可对原有的“**代码片段**”进行修改操作，在完成所有的“**代码片段**”设置后，记得点击“**OK**”按钮保存所有更改的内容。
5. 在**步骤3**中，`Type`也可以选择其他的选项，修改一些默认的代码片段内容，以适配我们实际需要；比如，我自己就修改了 `Refactoring` 下的 `Document Method` 的内容，满足自己代码中的函数注解格式。

在这里列举几个我常用的代码片段（主要用在“**注释**”和“**代码区段分割**”上）：
- 文件头部的说明：快捷键为 `//-`，代码片段内容如下
```
/**
 * @file    $FILE_BASE$.$FILE_EXT$
 * <pre>
 * Copyright (c) $YEAR$, Gaaagaa All rights reserved.
 * 
 * 文件名称：$FILE_BASE$.$FILE_EXT$
 * 创建日期：$YEAR$年$MONTH_02$月$DAY_02$日
 * 文件标识：
 * 文件摘要：$end$
 * 
 * 当前版本：1.0.0.0
 * 作    者：
 * 完成日期：$YEAR$年$MONTH_02$月$DAY_02$日
 * 版本摘要：
 * 
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 * 版本摘要：
 * </pre>
 */
```
- 大区段分割线（80 个字符）：快捷键为 `///` ，代码片段如下：
```
////////////////////////////////////////////////////////////////////////////////
```
- 次级区段分割线（70 个字符）：快捷键为 `//=` ，代码片段如下：
```
//====================================================================
```
- 函数的简短注释：快捷键为 `//;` ，代码片段如下：
```
/**********************************************************/
/**
 * @brief $end$
 */
```
- 逻辑相关（函数内 或 结构内定义）的代码块分割线（40 个字符）：快捷键为 `//=s` ，代码片段如下：
```
//======================================
```
- 在上面的“**步骤5**”中，我们提到修改 `Document Method` 的内容如下：
```
/**********************************************************/
/**
 * @brief $end$
 * 
 * @param [in ] $MethodArgName$ : 
 * 
 * @return $SymbolType$
 *         
 */
```
  在编辑代码时，我们将光标定位到（放到）函数名中，然后通过如下菜单步骤添加函数的注释：
```
"VAssistX" => "Code Generation and Refactoring" => "Document Method"
```
  另外，我们可以通过添加**全局的组合快捷键：`Ctrl+Shift+D`** 来快速调用该菜单命令，设置步骤如下：
```
1. 菜单 “Tools” => “Options...” => “Options” 对话框 => “Environment” => “Keyboard”；
2. 在 “Show commands containing:” 输入：VAssistX.RefactorDocumentMethod ，然后在列表中选中该命令项；
3. “Use new shortcut in:” ：Global
4. “Press shortcut keys:” ：Ctrl+Shift+D
5. 点击 “Assign” 按钮，然后再点击 “OK” 按钮保存设置后退出，这就完成了整个命令的快捷键设置流程。
```
以上提到的代码片段，也只是我个人常用的一部分，其他的，视个人习惯与项目情况，自行配制。VisualAssistX设置的代码片段最终会保存到磁盘中的文件：`C:\Users\{用户名}\AppData\Roaming\VisualAssist\Autotext\cpp.tpl` ，可以备份该文件用于其他情况的用途（如团队中使用统一的代码片段输入方式）。我个人的可在此下载[cpp.tpl](https://raw.githubusercontent.com/Gaaagaa/repository/master/cpp.tpl)。

#### 4.2 QtCreator

&emsp;&emsp;QtCreator没有VisualAssistX那样强大的插件，但也提供了代码片段快速插入的功能，可通过如下步骤进行配置：
1. 菜单“Tools” => “Options...” => “Options”对话框 => “Text Editor” => “Snippets” 选项卡；
2. 此时可添加相应的代码片段，其中“Trigger” 为代码编写过程中触发的字符串（相当于快捷键）;
3. 完成添加（或修改）操作后，记得保存。

&emsp;&emsp;至于要配置成什么样的代码片段功能，我这里就不再赘述，但就我个人的使用经验来讲，这方面，QtCreator当前版本（4.7.1版）做得还不够成熟，期待在以后的升级版本中，能有更大改进。

#### 4.3 Doxygen 的代码文档生成
&emsp;&emsp;这部分的内容，以后我会在其他文章中再来详细写吧。


## 5. 参考资料
- Doxygen官方手册：[http://www.stack.nl/~dimitri/doxygen/manual/index.html](http://www.stack.nl/~dimitri/doxygen/manual/index.html)
- 华为编程规范和范例：[https://github.com/Gaaagaa/repository/raw/master/华为编程规范和范例.doc](https://github.com/Gaaagaa/repository/raw/master/华为编程规范和范例.doc)
