stl_alloc.h �����˿ռ�������
stl_construct.h ���������ڶ������������Ļ�������
stl_uninitialized.h �������ڴ����Ļ�������
stl_algobase.h �����˸��ֻ����㷨
stl_config.h ��SGI STL �Ļ�����̬�ļ�
stl_iterator.h ������iterator�ĸ�������
auto_ptr��Դ����ͷ�ļ�<memory>��


vertorά��һ�����򿪿ڵ��������Կռ䣬�ṩ��Random Access Iterators��
list�ǻ���˫���������ṩ����Bidirectional iterators��
dequeά��һ��˫�򿪿ڵ��������Կռ䣨��һ�ζεĶ��������ռ乹�ɣ����ṩ��Random Access Iterators��

priority_queueĬ�Ͼ������ѣ����Լ򵥸������С�ѡ�







iterator_traits������ȡ�����������ԣ��ж��ǵ���˫���random����������stl_iterator.h��

__type_traits������ȡ�ͱ������ ,�ж�����Ƕ���ͻ����Զ������͡���type_traits.h��
�������Ƕ���ͣ��ڴ洦���ֱ��memcpy֮��ģ�����ʹ��constructor��destructor�����磨stl_construct.h��




# if defined(__STL_NEED_BOOL)
    typedef int bool;
#   define true 1
#   define false 0
# endif
//ԭ��#�ǿ��Կտ����ģ���������������
#ifdef �� #if defined ���������ڣ����߿�����ɸ��ӵ�Ԥ��������������

#if defined (AAA) && defined (BBB)
xxxxxxxxx
#endif

#if defined (AAA) || VERSION > 12
xxxxxxxxx
#endif

��#ifdef �Ͳ�����������÷���Ҳ����˵������Ҫ�жϵ������Ƿ���ʱ
#ifdef �� #if defined Ч����һ���ģ����ǵ���Ҫ�жϸ��ӵ�����ʱ��ֻ���� #if


��ͨ����Ҳ��������������
inline void neilian(){}

