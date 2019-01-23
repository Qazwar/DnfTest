//-------------------------------------------------------------------------
// �ļ�: cpuid.h
// ʱ��: 2005-12-10 09:42
// ����: ��Ϸ��ҵ�ܶ��ֵܷܶ��Ļ���
// ����: ��ö�Ӧ��CPU��Ϣ
// �汾:
// [v1.0]2005-12-10 09:42:��ʼ�汾
//                        ���ִ���ȡ��MSDN����<cpuid sample>
// [v1.1]2005-12-12 10:51:���ָ�SSE3 E3DNOW
// [v1.2]2005-12-19 19:02:���GetProcessorName���CPU������
// [v1.3]2005-12-20 16:01:����ܽ�׼ȷԤ��CPU�ٶȵĺ���TestCpuSpeed
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
    char v_name[_MAX_VNAME_LEN];        // Vendor��
    char model_name[_MAX_MNAME_LEN];    // �����������磺Intel Pentium-Pro
    int family;                         // �������壬�磺6 = Pentium-Pro architecture
    int model;                          // ��ģ��ţ��磺���family=6��1 = Pentium-Pro
    int stepping;                       // ���������ذ��
    int feature;                        // ������ָ�����GetProcessorInfoָ��ķ�����ͬ
    int os_support;                     // �Ƿ����ϵͳ֧��
    int checks;                         // ���ü���˵�ָ�
} PROCESSORINFO;

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------
// ����: BOOL GetProcessorName( OUT char *szProcessorName)
// ˵��: ���������ͼ��ѯ��ǰCPUֱ�ӻ����Ϣ
// ����: szProcessorName	���շ���ֵ���ִ�������48�ֽڳ�
// ����: �Ƿ��ѯ�ɹ�
//-------------------------------------------------------------------------
BOOL GetProcessorName( OUT char *szProcessorName );

//-------------------------------------------------------------------------
// ����: ULONGLONG TestCpuSpeed()
// ˵��: ����Cpu���ٶ�
// ����: ��
// ����: һ����CPU��ѭ����(MHz)
//-------------------------------------------------------------------------
ULONGLONG TestCpuSpeed();

//-------------------------------------------------------------------------
// ����: int GetProcessorInfo (PROCESSORINFO *pinfo)
// ˵��: ��ö�ӦCPU��˵��
// ����: pinfo: PROCESSORINFO��ָ�룬�������Ҫ����Ϊ��
// ����: CPU֧�ֵ�ָ�
//-------------------------------------------------------------------------
int GetProcessorInfo( OUT PROCESSORINFO * );

#ifdef __cplusplus
}
#endif

#endif//_INC_CPUID
