#ifndef __v_RBTREE_H__
#define __v_RBTREE_H__



#define v_thread_volatile   volatile
#ifdef WIN32
#define v_inline __inline
#else
#define v_inline inline
#endif

#define v_OK                                   0
#define v_ERROR                                -1



typedef struct v_rbtree_s          v_rbtree_t;
typedef struct v_rbtree_node_s     v_rbtree_node_t;


typedef struct v_rbtree_key_s      v_rbtree_key_t;
typedef int                        v_rbtree_key_int_t;
typedef  int                       v_int32_t;

struct v_rbtree_key_s
{
    unsigned long long     v;
    unsigned long long     v2;
};

struct v_rbtree_node_s
{
    v_rbtree_key_t        key;
    v_rbtree_node_t	  *left;
    v_rbtree_node_t      *right;
    v_rbtree_node_t      *parent;
    unsigned char          color;
    unsigned char          data;
};

typedef void (*v_rbtree_insert_pt) (v_rbtree_node_t *root,
    v_rbtree_node_t *node, v_rbtree_node_t *sentinel);

typedef int (*v_rbtree_compare_pt)(v_rbtree_key_t *p1, v_rbtree_key_t *p2);

#define v_RBTREE_TYPE_DEFAULT_NUMBER                   0
#define v_RBTREE_TYPE_STR                              1

struct v_rbtree_s {
    v_rbtree_node_t     *root;
    v_rbtree_node_t     *sentinel;
    v_rbtree_insert_pt   insert;
    v_rbtree_compare_pt  compare;
    v_int32_t            rbtree_type;
};

#define v_rbtree_init(tree, s, i)                                           \
    v_rbtree_sentinel_init(s);                                              \
    (tree)->root = s;                                                         \
    (tree)->sentinel = s;                                                     \
    (tree)->compare = 0;                                                      \
    (tree)->insert = i                                                  

#define v_rbtree_init_with_compare(tree,s,i,c)                                \
    v_rbtree_sentinel_init(s);                                                \
    (tree)->root = s;                                                          \
    (tree)->sentinel = s;                                                      \
    (tree)->compare = c;                                                       \
    (tree)->insert = i                                                  

void v_rbtree_insert(v_thread_volatile v_rbtree_t *tree,
    v_rbtree_node_t *node);
void v_rbtree_delete(v_thread_volatile v_rbtree_t *tree,
    v_rbtree_node_t *node);
int v_rbtree_get(v_thread_volatile v_rbtree_t *tree, v_rbtree_key_t  key,
	v_rbtree_node_t *sentinel, v_rbtree_node_t **node);



//v_rbtree_insert_value 和 v_rbtree_insert_timer_value  里面的实现是一样的。
void v_rbtree_insert_value(v_rbtree_node_t *root, v_rbtree_node_t *node,
    v_rbtree_node_t *sentinel);
void v_rbtree_insert_timer_value(v_rbtree_node_t *root,
    v_rbtree_node_t *node, v_rbtree_node_t *sentinel);


#define v_rbt_red(node)               ((node)->color = 1)
#define v_rbt_black(node)             ((node)->color = 0)
#define v_rbt_is_red(node)            ((node)->color)
#define v_rbt_is_black(node)          (!v_rbt_is_red(node))
#define v_rbt_copy_color(n1, n2)      (n1->color = n2->color)

#define v_rbtree_sentinel_init(node)  v_rbt_black(node)

static v_inline v_rbtree_node_t*
v_rbtree_min(v_rbtree_node_t *node, v_rbtree_node_t *sentinel)
{
    while (node->left != sentinel) {
        node = node->left;
    }

    return node;
}

static v_inline v_rbtree_node_t*
v_rbtree_max(v_rbtree_node_t *node, v_rbtree_node_t *sentinel)
{
	while (node->right != sentinel) {
		node = node->right;
	}

	return node;
}


#define offsetof(type, member) (size_t)&(((type*)0)->member)
#define container_of(ptr, type, member) ({                      \
         const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})



#endif //
