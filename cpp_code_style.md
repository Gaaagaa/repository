# <center>我的C++代码风格</center>

## 1 前言

&emsp;&emsp;在多人团队开发中，使用统一的代码规范，项目开发前期利于团队协作，中期利于工作审核，后期利于降低维护成本，总而言之，代码规范很重要。这话虽然简单，但一直未能引起重视，直到我看到下图的那个新闻后，我就知道有很多码农还活着真不简单啊。我们姑且不去深究这个新闻的真实性，但至少我们也该意识到代码规范的重要性了吧。

<center>

![avatar]()

</center>

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
2. 调出的 "VA Snippet Editor"对话框 如下图所示：

<center>

![avatar](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAA7QAAAJhCAYAAABxQ/yGAAAgAElEQVR4nOzdD3RUVZ4v+i+C2ALmD6RFJKkQghToTcw0QSNUJ6N4TYQ1XMm8Ngwselob0stl7rrQc6Gl19Xr0nmNo2sGZl14rg6OdjcPJqFvQz/mgcEnOmEiBAndEVqgIDGkEkTpQFIBVP4Ib+/zp3Kq6lTVqf9V8P2srk7VPv92HaCsb3777DOsqKjkBoiIiIiIiIjSzG3J7gARERERERFRJBhoiYiIiIiIKC0x0BIREREREVFaYqAlIiIiIiKitMRAS0RERERERGmJgZaIiIiIiIjS0ohkd4CIiIiIiIgSwY66hnoszB1q2bemAqt2Jq9H0Qq7Qnv9xnX03NaNb3ApHv0hIiIiIiKieJj3jFeYlWat3oy6aYk5/LPP/ggtLc3Kz3CWBRNWoB0x7AaKs4dh+B2XMDfvOv77tOF4ZtJteGQsMHJYWMdNHTkPYVHdQjyck+yOhGnqE6hb9BCUbqfreyAiIiIiooSSFVmHQz5q0dArW3LheNyekGO//fav8Nvf/m8RWp/BD37wf3ja5XPZJpfJdcJhacjxcBFW/8Z2G6rzbsOY22/DM0dHoSRrGGZnXMcB9wWcun4BPx03Gr0XRqHxy+G4eiN0up1a9RyewPtY33TSqz3n4YVYWPgZGrZ8jD6lZRweXvQ0Zp73X9fffaiqexxTtFcd772JphNW3mEq0N7nWGNbJ95b/x7S5i0QEREREVHq2vkCVnleONHlgsyzcHU5E9aFf/7n/6X8/G//7b962uRzGWb1ZeEIGWhHDb+Omff048DlS+joHoGckbfj0rff4v881Yuey5dx123DUT8pD8WifeaIb1AyEnil904MXh8edL8nPuvEE08UYCpOGgLbOBQWZqOjTQ+zQk4hCtGP81N81/WlhlmIELv+hLqvhxc9gaknQgTCvo+xZf3HoU5DDKiBNbsteMiOKIR7vQdrxyEiIiIiolvYvNewepZ8sh/NCb6G1jfURhpmpZBDjn9eeB0P3TUCY4YNw3vnB/B/nzmL/xi4gI6vv8HE229HxZhR+H/6+vH2l304OHgRE/ENVo5zYxhuBN/xiS50iKg6eaqhTYbXsZ347ISxaTLQ+f+hrcNnXV852Rh7/hDaPNuew4EtrG4SERERERF5EWG2ZfUj4kkvGpa+gGTMCXXhwgXT5+EKWqGdMeYa/tN3ruHctyMwcswY/M97xuN/fn4GThFm7xABN08E2pfH5eD6tW+Vx7dXv8W1b64i/9vrKLv9O9h/dVSQvZ/EZx2P44nJ94lwqw4lluF1bMchn4qtyLPvncMJEXSfKH0IOScM1VsjEarPj30cpVM/NqlMyurtDPQfPI+ZMwuVlvMHt2LLgXPq9acLs9CmDO3Vq5vvA0+YDV0OtVw/1tAy9Thjh9qeeA51pYcMQ6qt8t6vuvND6k/Pe+jCZNPj+G5rHMrs857OR9I3IiIiIiJKD3bULdHD7GKsP574HhivmZXk8wsXLnpehyNooH1YBNpvr1xD1jDgeyNux7ffXMF4DEPtd+/GaLH8/x28gKtfX1YD7RURaMXjxrUbuPHtDTx4/RL2I1igFTm27RBKF+pDifXhxobrZKfOwEx8hgaZrvq60PHEDBTmiLBlmrZOoum9AtTJIPeE2bWn2ZiZfQjrRbsaAP8zHu5swAGTPU15okBsL0Kq0ocnxP68hy4HXq4F54Y3sV7poxoWq/rFuuvPWxoKPEXpv/aiQ79u2Hc4tXYN8ljfrU+aHEfddqwI1utlgNf7LA+yPsB7IiIiIiKim1wPupIQZuUsxjLAvv32O54JoGSFVg4/vuuuMbGdFGo0ruHalavqixs35P+w5I5RGC5C7o1r1/FfcDuuDH4jwux1XL+qPm58e11Zb9Swy0Dwy2hFSO1E5/mnlaHEJ86rw43bDGFv6uRCnO/cqlUL1YpuaeE4HOg7Z74/ESpl4FMmlqoTga/DOJFUPw7qYVk57uSA3ep4zxCGTxzCwVKtjydCLEcBpojgjIXPiSA+5Hz2uBAnwnhsk8A7tUCtnBralWuQ/QKtCX3bA4ZzFuo9ERERERHRTcqJ9QsrsB52TJO360lgqDULs5L+XC4zvrYiaKD94hvg2xFXceO6yLPXryuVVxlkr1wbCrDXr8rhxtfVyux1NfSKtXF25OjQgRbn0NnZj4WT70NOtu9w4/sweQowFk+jzpgOMQNTDwQPX30HGrD+gFqZrJp6MrGTIwUcsms91CZGP/rPJ7sPRERERESUWHbUNdQr96OVt/BZlcBAK4NqoLAabFkwQSeF+vDCSFz96hquXbqCqxeu4Ir7Mi73i8f5b8RP8RCvr166imvffItvZchVAq0aavcPz7DUgb7Oz3B+ygw8UYihCqokK4vKtZ5vYr3n8b7/RFI6OYy46j6fxshC25TJQ/vJefg/Y6bPRFUBl8uJrsbOQKmhf1OrnkCwuaws8dvvODxcWhjWtk88bAjU2lDuTl4oS0REREREaSxohbb325HYPXgnHrsx4LlG9vq3aiU2mNbbM9E5YrS1HijDf2XAOoT3DAFLDjeW15B6F1f9J5Ia2s/HeK9/IerktaYaOXz3QAShrQMFhv34X48bePlJNDVkY9HCoetglSHEyjOtGh1iUiiva2jlMOmGBvEejNcHa+0HOwHTTOt/nKb1QFWdsdIt+8yJn4iIiIiIbj36kOObw7CiopLg6fTGDdRc68P3vzkHbTxxUPtFmP3X70zA9WHDYtXHBAp1D1fe45WIiIiIiChVBK3QKkQwbbz9uzg4bDTmXu6D/fpXGOYbbMU6J2+7E00jc+C83WJlloiIiIiIiCgKoQOt5rMRo7B+hA13XL+Ge69dxlhchazBnhe7OD3iO7h8m+VdEREREREREUUt9JBjIiIiIiIiohQUdJZjIiIiIiIiolTFQEtERERERERpiYGWiIiIiIiI0hIDLREREREREaUlBloiIiIiIiJKSwy0RERERERElJYYaImIiIiIiCgtMdASERERERFRWmKgJSIiIiIiorTEQEtERERERERpiYGWiIiIiIiI0hIDLREREREREaUlBloiIiIiIiJKSxEF2rnTLsW6H0RERERERERhGWF1xYrJX6NnYAQ+O3875olAu+v4aEweexV5WdfQ/NmdUXXizjvV7b/++mvvzo0YoSy7cOFCVPt/9NG/DLr8ww//Par933vvBNjtduW50+nE55+fiWp/REREREREFJrlQDv4zW14YPxlJdDuFGFWkq973bdH3YnvfOc7uOee8Th27LhXe25uLr755puoA208DR8+HPn5+Z5Q7HDMxp//3IerV6+GvS8Z3q9cuYJvv/3WdPnYsWNxxx134MyZ2ATmUEFfF23gJyIiIiIiigfLgfaPn9+hPKRdWqD9t2NjYtIJGf7Gjx+Pzz7rwuXLl5U2GRTz8nJx9OixqPff3z+g/Gxvb/dqtxrogpk8ebISyHW33347pk6dik8//TSs/eTk5KCo6D8pAf6TTw7jq6++8lqel5ennKM//vGPUfeZiIiIiIjoZmAp0MprZuUwY1mZ1cNsLMmhxtev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)

</center>

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
