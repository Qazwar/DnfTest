//-------------------------------------------------------------------------
// 文件: cpuid.h
// 时间: 2005-12-10 09:42
// 作者: 游戏行业很多兄弟奋斗的积累
// 名称: 获得对应的CPU信息
// 版本:
// [v1.0]2005-12-10 09:42:初始版本
//                        部分代码取自MSDN例子<cpuid sample>
// [v1.1]2005-12-12 10:51:添加指令集SSE3 E3DNOW
// [v1.2]2005-12-19 19:02:添加GetProcessorName获得CPU的名称
// [v1.3]2005-12-20 16:01:添加能较准确预测CPU速度的函数TestCpuSpeed
//-------------------------------------------------------------------------
#ifndef _INC_CPUID
#define _INC_CPUID

#define _CPU_FEATURE_MMX	0x0001
#define _CPU_FEATURE_SSE	0x0002
#define _CPU_FEATURE_SSE2	0x0004
#define _CPU_FEATURE_3DNOW	0x0008
#define _CPU_FEATURE_SSE3	0x0010
#define _CPU_FEATURE_E3DNOW	0x0020

#define _MAX_VNAME_LEN  16
#define _MAX_MNAME_LEN  48

#ifndef BOOL
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#endif

typedef struct PROCESSORINFO {
    char v_name[_MAX_VNAME_LEN];        // Vendor名
    char model_name[_MAX_MNAME_LEN];    // 处理器名，如：Intel Pentium-Pro
    int family;                         // 处理器族，如：6 = Pentium-Pro architecture
    int model;                          // 子模块号，如：如果family=6，1 = Pentium-Pro
    int stepping;                       // 处理器的重版号
    int feature;                        // 处理器指令集，与GetProcessorInfo指令的返回相同
    int os_support;                     // 是否操作系统支持
    int checks;                         // 设置检查了的指令集
} PROCESSORINFO;

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------
// 函数: BOOL GetProcessorName( OUT char *szProcessorName)
// 说明: 这个函数试图查询当前CPU直接获得信息
// 参数: szProcessorName	接收返回值的字串，至少48字节长
// 返回: 是否查询成功
//-------------------------------------------------------------------------
BOOL GetProcessorName( OUT char *szProcessorName );

//-------------------------------------------------------------------------
// 函数: ULONGLONG TestCpuSpeed()
// 说明: 测试Cpu的速度
// 参数: 无
// 返回: 一秒钟CPU的循环数(MHz)
//-------------------------------------------------------------------------
ULONGLONG TestCpuSpeed();

//-------------------------------------------------------------------------
// 函数: int GetProcessorInfo (PROCESSORINFO *pinfo)
// 说明: 获得对应CPU的说明
// 参数: pinfo: PROCESSORINFO的指针，如果不需要可以为空
// 返回: CPU支持的指令集
//-------------------------------------------------------------------------
int GetProcessorInfo( OUT PROCESSORINFO * );

#ifdef __cplusplus
}
#endif

#endif//_INC_CPUID
