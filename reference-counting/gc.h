//
// Created by jiangxin on 2020/3/15.
//

#ifndef GC_IMPL_REFERENCE_COUNTING_GC_H
#define GC_IMPL_REFERENCE_COUNTING_GC_H

#endif //GC_IMPL_REFERENCE_COUNTING_GC_H

/**
 * 1字节的byte类型，用来做标识位
 */
typedef unsigned char byte;



/**
 * 类描述
 */
typedef struct class_descriptor{
    char *name;//类名称
    int size;//类大小，即对应sizeof(struct)
    int num_fields;//属性数量
    int *field_offsets;//类中的属性偏移，即所有属性在struct中的偏移量（字节）
} class_descriptor;

/**
 * 基本对象类型
 * 所有对象都继承于Object
 * C中没有继承的概念，不过可以通过定义相同属性来实现，所有“继承”Object的struct，都需要将class/marked属性定义在开头
 */
typedef struct _object object;
struct _object {
    class_descriptor *class;//对象对应的类型
    int ref_cnt;//对象被引用的次数，"人气"
};

/**
 * free-list的单元节点
 * 为了实现简单，在此算法中不考虑“碎片化”的问题
 * 将单元的大小设置的足够大，并且不允许分配超过此单元大小的内存
 * 每个单元只存放一个Object
 */
typedef struct _node node;
struct _node{
    node *next;
    byte used;//是否使用
    int size;
    object *data;//单元中的数据
};

#define NODE_SIZE 128//free-list单元大小(B)

#define MAX_HEAP_SIZE MAX_ROOTS*4*NODE_SIZE//50MB

#define MAX_ROOTS 100

const static byte TRUE = 1;
const static byte FALSE = 0;

/**
 * GC ROOT 的当前下标，即记录到了第几个元素
 */
extern node *next_free;
extern node *head;
/**
 * malloc的堆（起始）地址
 */
/**
 * 下一个空闲的内存地址（heap中的相对位置）
 */


/**
 * 堆总大小
 */
//extern int heap_size;
/*===============================GC interface=================================*/

/**
 * 初始化GC
 * @param size
 */
extern void gc_init(int size);

/**
 * GC结束，彻底清理堆
 */
extern void gc_done();

/**
 * 在GC堆上分配指定类型的内存
 * 使用顺序分配(sequential allocation)内存的方式
 * @param class 需要分配的类型
 * @return 分配的对象指针
 */
extern object *gc_alloc(class_descriptor *class);

/**
 * DUMP GC状态
 * @return
 */
extern char *gc_get_state();

/**
 * 获取GC ROOTS数量
 * @return ROOTS数量
 */
extern int gc_num_roots();

/**
 * 修改引用
 * @param ptr 原指针
 * @param obj 新对象指针
 */
extern void gc_update_ptr(object** ptr, void *obj);

/**
 * 将对象添加到GC ROOTS，这里因为没有tracing的需求，所以直接更新计数器，不再维护roots
 */
extern void gc_add_root(void *p);

/**
 * 将对象从GC ROOTS移除，模拟变量出栈
 * @param p
 */
extern void gc_remove_root(void *p);
/**
 * 暂存GC ROOTS下标
 */
#define gc_save_rp          int __rp = _rp;

/**
 * 恢复GC ROOTS下标
 */
#define gc_restore_roots    _rp = __rp;
