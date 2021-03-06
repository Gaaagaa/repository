# C++11 —— 获取 tuple 参数列表中指定数据类型的索引位置

## 1. 问题背景

&emsp;&emsp;在 C++11 的标准中，我们可以通过 `std::get< Index >(tuple)` （以常量整数值为索引号）操作 tuple 中的参数，而到了 C++14 之后的标准，新增了 `std::get< Type >(tuple)` （以数据类型为索引）的方式操作 tuple 中的参数。那么，若只是在 C++11 标准中，是否有办法使用 **以数据类型为索引** 的方式操作 tuple 中的参数呢？

## 2. 解决办法

&emsp;&emsp;解决上面所提到的问题，其本质上，就是要解决 **如何获取 tuple 参数列表中指定数据类型的索引位置** 的问题。凭借这几天我积累到经验（可参看我前几篇关于 tuple 的文章），设计了以下代码中的几个模板类，有效的解决了该问题，请先详细参看如下代码：

```
////////////////////////////////////////////////////////////////////////////////

/**
 * @struct X_type_index
 * @brief  类型索引模板：从变参列表中查找指定类型的索引位置（以 < _Fy, _Indx > 结对进行判断）。
 * 
 * @param[in ] _Fy    : 待查找类型。
 * @param[in ] _Indx  : 待查找类型在变参列表中的第几个（以 0 为起始索引）。
 * @param[in ] _Ty... : 变参列表。
 */
template< typename _Fy, size_t _Indx, typename... _Ty >
struct X_type_index;

/**
 * @struct X_type_index_iter
 * @brief  协助 X_type_index 进行 变参列表 递归遍历操作。
 * 
 * @param[in ] _Vy    : 模板特化的参数；
 *                      为 true  时，转向 X_type_index_jter 继续进行递归遍历；
 *                      为 false 时，转回 X_type_index 继续进行递归遍历。
 * @param[in ] _Fy    : 待查找类型。
 * @param[in ] _Indx  : 待查找类型在变参列表中的第几个（以 0 为起始索引）。
 * @param[in ] _Ty... : 变参列表。
 */
template< bool _Vy, typename _Fy, size_t _Indx, typename... _Ty >
struct X_type_index_iter;

/**
 * @struct X_type_index_jter
 * @brief  协助 X_type_index_iter 进行 变参列表 递归遍历操作。
 * 
 * @param[in ] _Indx  : 待查找类型在变参列表中的第几个（以 0 为起始索引）；
 *                      为 0 时，终止递归下降，否则转回 X_type_index 继续进行递归遍历。
 * @param[in ] _Fy    : 待查找类型。
 * @param[in ] _Ty... : 变参列表。
 */
template< size_t _Indx, typename _Fy, typename... _Ty >
struct X_type_index_jter;

/**
 * @brief 终止 X_type_index_jter 递归下降。
 */
template< typename _Fy, typename... _Ty >
struct X_type_index_jter< 0, _Fy, _Ty... >
{
	enum { value = 0 };
};

/**
 * @brief 转回 X_type_index 继续进行递归下降遍历。
 */
template< size_t _Indx, typename _Fy, typename... _Ty >
struct X_type_index_jter
{
	enum { value = 1 + X_type_index< _Fy, _Indx - 1, _Ty... >::value };
};

/**
 * @brief 转向 X_type_index_jter 继续进行递归遍历。
 */
template< typename _Fy, size_t _Indx, typename... _Ty >
struct X_type_index_iter< true, _Fy, _Indx, _Ty... >
{
	enum { value = 0 + X_type_index_jter< _Indx, _Fy, _Ty...>::value };
};

/**
 * @brief 转回 X_type_index 继续进行递归遍历。
 */
template< typename _Fy, size_t _Indx, typename... _Ty >
struct X_type_index_iter< false, _Fy, _Indx, _Ty... >
{
	enum { value = 1 + X_type_index< _Fy, _Indx, _Ty... >::value };
};

/**
 * @brief X_type_index 递归遍历入口。
 */
template< typename _Fy, size_t _Indx, typename _Hy, typename... _Ty >
struct X_type_index< _Fy, _Indx, _Hy, _Ty... >
{
	enum { value = X_type_index_iter< std::is_same< _Fy, _Hy >::value, _Fy, _Indx, _Ty... >::value };
};

/**
 * @brief X_type_index 递归遍历终结位置。
 */
template< typename _Fy, size_t _Indx >
struct X_type_index< _Fy, _Indx >
{
	enum { value = 0x1FFFFFFF };
};

/**
 * @brief 从 tuple 的变参列表中查找指定类型的索引位置（以 < _Fy, _Indx > 结对进行操作）。
 * 
 * @param [in ] _Fy   : 待查找类型。
 * @param [in ] _Indx : 待查找类型在变参列表中的第几个（以 0 为起始索引）。
 * @param [in ] _Ty   : tuple 的参数列表。
 * 
 * @return size_t
 *         - 返回索引位置;
 *         - 若返回值大于等于 X_type_index< _Fy, _Indx >::value[0x1FFFFFFF] 时，表示未找到查找的类型。
 */
template< typename _Fy, size_t _Indx, typename... _Ty >
constexpr size_t X_tuple_type_index(const std::tuple< _Ty... > &)
{
    return X_type_index< _Fy, _Indx, _Ty... >::value;
}

////////////////////////////////////////////////////////////////////////////////
```
&emsp;&emsp;以上代码中，使用 ```std::is_same< _Fy, _Hy >::value``` 判断数据类型是否一致，然后特化 X_type_index_iter<> 和 X_type_index_jter<> 这两个模板类进行递归推导操作，过程中，通过逐步累积索引值到 value 中，最后得到我们需要类型索引值。这过程，如下面的流程图所示：

![type_index](https://img2018.cnblogs.com/blog/1494489/201812/1494489-20181222022628739-1548264838.jpg)


## 3. 使用示例

&emsp;&emsp;下面给出完成的测试程序代码，亦可作为使用示例的参看代码：

```
#include <type_traits>
#include <tuple>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

/**
 * @struct X_type_index
 * @brief  类型索引模板：从变参列表中查找指定类型的索引位置（以 < _Fy, _Indx > 结对进行判断）。
 * 
 * @param[in ] _Fy    : 待查找类型。
 * @param[in ] _Indx  : 待查找类型在变参列表中的第几个（以 0 为起始索引）。
 * @param[in ] _Ty... : 变参列表。
 */
template< typename _Fy, size_t _Indx, typename... _Ty >
struct X_type_index;

/**
 * @struct X_type_index_iter
 * @brief  协助 X_type_index 进行 变参列表 递归遍历操作。
 * 
 * @param[in ] _Vy    : 模板特化的参数；
 *                      为 true  时，转向 X_type_index_jter 继续进行递归遍历；
 *                      为 false 时，转回 X_type_index 继续进行递归遍历。
 * @param[in ] _Fy    : 待查找类型。
 * @param[in ] _Indx  : 待查找类型在变参列表中的第几个（以 0 为起始索引）。
 * @param[in ] _Ty... : 变参列表。
 */
template< bool _Vy, typename _Fy, size_t _Indx, typename... _Ty >
struct X_type_index_iter;

/**
 * @struct X_type_index_jter
 * @brief  协助 X_type_index_iter 进行 变参列表 递归遍历操作。
 * 
 * @param[in ] _Indx  : 待查找类型在变参列表中的第几个（以 0 为起始索引）；
 *                      为 0 时，终止递归下降，否则转回 X_type_index 继续进行递归遍历。
 * @param[in ] _Fy    : 待查找类型。
 * @param[in ] _Ty... : 变参列表。
 */
template< size_t _Indx, typename _Fy, typename... _Ty >
struct X_type_index_jter;

/**
 * @brief 终止 X_type_index_jter 递归下降。
 */
template< typename _Fy, typename... _Ty >
struct X_type_index_jter< 0, _Fy, _Ty... >
{
	enum { value = 0 };
};

/**
 * @brief 转回 X_type_index 继续进行递归下降遍历。
 */
template< size_t _Indx, typename _Fy, typename... _Ty >
struct X_type_index_jter
{
	enum { value = 1 + X_type_index< _Fy, _Indx - 1, _Ty... >::value };
};

/**
 * @brief 转向 X_type_index_jter 继续进行递归遍历。
 */
template< typename _Fy, size_t _Indx, typename... _Ty >
struct X_type_index_iter< true, _Fy, _Indx, _Ty... >
{
	enum { value = 0 + X_type_index_jter< _Indx, _Fy, _Ty...>::value };
};

/**
 * @brief 转回 X_type_index 继续进行递归遍历。
 */
template< typename _Fy, size_t _Indx, typename... _Ty >
struct X_type_index_iter< false, _Fy, _Indx, _Ty... >
{
	enum { value = 1 + X_type_index< _Fy, _Indx, _Ty... >::value };
};

/**
 * @brief X_type_index 递归遍历入口。
 */
template< typename _Fy, size_t _Indx, typename _Hy, typename... _Ty >
struct X_type_index< _Fy, _Indx, _Hy, _Ty... >
{
	enum { value = X_type_index_iter< std::is_same< _Fy, _Hy >::value, _Fy, _Indx, _Ty... >::value };
};

/**
 * @brief X_type_index 递归遍历终结位置。
 */
template< typename _Fy, size_t _Indx >
struct X_type_index< _Fy, _Indx >
{
	enum { value = 0x1FFFFFFF };
};

/**
 * @brief 从 tuple 的变参列表中查找指定类型的索引位置（以 < _Fy, _Indx > 结对进行操作）。
 * 
 * @param [in ] _Fy   : 待查找类型。
 * @param [in ] _Indx : 待查找类型在变参列表中的第几个（以 0 为起始索引）。
 * @param [in ] _Ty   : tuple 的参数列表。
 * 
 * @return size_t
 *         - 返回索引位置;
 *         - 若返回值大于等于 X_type_index< _Fy, _Indx >::value[0x1FFFFFFF] 时，表示未找到查找的类型。
 */
template< typename _Fy, size_t _Indx, typename... _Ty >
constexpr size_t X_tuple_type_index(const std::tuple< _Ty... > &)
{
    return X_type_index< _Fy, _Indx, _Ty... >::value;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{
    using _Tuple = std::tuple< int, int, int, char, float, double >;

    //======================================
    // 获取索引号

    std::cout << "< int   , 0 > : " << X_type_index< int , 0 >::value << std::endl;
    std::cout << "< int   , 0 > : " << X_type_index< int , 0, int >::value << std::endl;
    std::cout << "< char  , 0 > : " << X_type_index< char, 0, int, int >::value << std::endl;
    std::cout << "< int   , 1 > : " << X_type_index< int , 1, int, int, char >::value << std::endl;
    std::cout << "< char  , 0 > : " << X_type_index< char, 0, int, int, char, char >::value << std::endl;
    std::cout << "< char  , 1 > : " << X_type_index< char, 1, int, int, char, char, char >::value << std::endl;

    std::cout << "tuple type = std::tuple< int, int, int, char, float, double >" << std::endl;
    std::cout << "< int   , 1 > : " << X_tuple_type_index< int   , 1 >(_Tuple{}) << std::endl;
    std::cout << "< int & , 1 > : " << X_tuple_type_index< int & , 1 >(_Tuple{}) << std::endl;
    std::cout << "< char  , 1 > : " << X_tuple_type_index< char  , 1 >(_Tuple{}) << std::endl;
    std::cout << "< double, 1 > : " << X_tuple_type_index< double, 1 >(_Tuple{}) << std::endl;
    std::cout << "< float , 1 > : " << X_tuple_type_index< float , 1 >(_Tuple{}) << std::endl;
    std::cout << "< void *, 1 > : " << X_tuple_type_index< void *, 1 >(_Tuple{}) << std::endl;

    //======================================
    // 按 数据类型方式索引 tuple 的参数

    _Tuple xtuple{ 100, 200, 300, 'A', 1.234F, 1.234 };

    std::cout << " xtpule< 0 > = " << std::get< 0 >(xtuple) << std::endl;
    std::cout << " xtpule< 1 > = " << std::get< 1 >(xtuple) << std::endl;
    std::cout << " xtpule< 2 > = " << std::get< 2 >(xtuple) << std::endl;
    std::cout << " xtpule< 3 > = " << std::get< 3 >(xtuple) << std::endl;
    std::cout << " xtpule< 4 > = " << std::get< 4 >(xtuple) << std::endl;
    std::cout << " xtpule< 5 > = " << std::get< 5 >(xtuple) << std::endl;

    std::get< X_tuple_type_index< int   , 0 >(xtuple) >(xtuple) = 101;
    std::get< X_tuple_type_index< int   , 1 >(xtuple) >(xtuple) = 201;
    std::get< X_tuple_type_index< int   , 2 >(xtuple) >(xtuple) = 301;
    std::get< X_tuple_type_index< char  , 0 >(xtuple) >(xtuple) = 'B';
    std::get< X_tuple_type_index< float , 0 >(xtuple) >(xtuple) = 1.245F;
    std::get< X_tuple_type_index< double, 0 >(xtuple) >(xtuple) = 1.236;

    std::cout << " xtpule< int   , 0 >[0] = " << std::get< 0 >(xtuple) << std::endl;
    std::cout << " xtpule< int   , 1 >[1] = " << std::get< 1 >(xtuple) << std::endl;
    std::cout << " xtpule< int   , 2 >[2] = " << std::get< 2 >(xtuple) << std::endl;
    std::cout << " xtpule< char  , 0 >[3] = " << std::get< 3 >(xtuple) << std::endl;
    std::cout << " xtpule< float , 0 >[4] = " << std::get< 4 >(xtuple) << std::endl;
    std::cout << " xtpule< double, 0 >[5] = " << std::get< 5 >(xtuple) << std::endl;

    //======================================

    return 0;
}
```
