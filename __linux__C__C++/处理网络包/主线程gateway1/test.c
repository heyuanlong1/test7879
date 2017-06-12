#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "v_rbtree.h"


typedef struct test_struct_s{
    v_rbtree_node_t					node;
	int								i;
    int 							j;
}test_struct_t;
typedef struct test_struct_time_s{
    v_rbtree_node_t					timer;
	int								i;
    int 							j;
}test_struct_time_t;


v_thread_volatile v_rbtree_t		 test_rbtree;
static v_rbtree_node_t				 v_sentinel;

v_thread_volatile v_rbtree_t  		v_cln_timer_rbtree;
static v_rbtree_node_t          	v_cln_timer_sentinel;

void insert();
test_struct_t* look1();
void del1(test_struct_t* u);

void insertTime();
void lookTime();


int main(int argc, char const *argv[])
{
	test_struct_t *u;
	v_rbtree_init(&test_rbtree, &v_sentinel,v_rbtree_insert_value);
	v_rbtree_init(&v_cln_timer_rbtree, &v_cln_timer_sentinel,v_rbtree_insert_timer_value);
	insert();
	u = look1();
	if (NULL != u){
		del1(u);
	}
	u = look1();



	//做时间堆
	insertTime();
	lookTime();

	return 0;
}

void insert()
{
	test_struct_t		*u1 = malloc(sizeof(test_struct_t));
	test_struct_t		*u2 = malloc(sizeof(test_struct_t));
	v_rbtree_node_t						*node;

	u1->i = 1;
	u1->j = 1;
	node = &u1->node;
	node->key.v = 1000;
	node->key.v2 = 1;
	v_rbtree_insert(&test_rbtree, node);

	u2->i = 2;
	u2->j = 2;
	node = &u2->node;
	node->key.v = 1000;
	node->key.v2 = 2;
	v_rbtree_insert(&test_rbtree, node);
}
test_struct_t* look1()
{
	v_rbtree_node_t						*node;
	test_struct_t						*u;
    v_rbtree_key_t                      key;

    key.v = 1000;
    key.v2 = 1;
	if (v_OK != v_rbtree_get(&test_rbtree, key, &v_sentinel, &node)) {
        printf("get u1 fail\n");
        return NULL;
    }
    u = container_of(node, test_struct_t, node);
	printf("%d\n", u->j);
	return u;
}

void del1(test_struct_t* u)
{
	v_rbtree_delete(&test_rbtree, &u->node);
}

void insertTime()
{
	test_struct_time_t		*u1 = malloc(sizeof(test_struct_time_t));
	test_struct_time_t		*u2 = malloc(sizeof(test_struct_time_t));
	test_struct_time_t		*u3 = malloc(sizeof(test_struct_time_t));
	test_struct_time_t		*u4 = malloc(sizeof(test_struct_time_t));
	test_struct_time_t		*u5 = malloc(sizeof(test_struct_time_t));

	u1->timer.key.v = 10;
	u1->timer.key.v2 = 3;
	u2->timer.key.v = 10;
	u2->timer.key.v2 = 1;
	u3->timer.key.v = 10;
	u3->timer.key.v2 = 2;
	u4->timer.key.v = 5;
	u4->timer.key.v2 = 0;
	u5->timer.key.v = 12;
	u5->timer.key.v2 = 0;

	v_rbtree_insert( &v_cln_timer_rbtree, &u1->timer );
	v_rbtree_insert( &v_cln_timer_rbtree, &u2->timer );
	v_rbtree_insert( &v_cln_timer_rbtree, &u3->timer );
	v_rbtree_insert( &v_cln_timer_rbtree, &u4->timer );
	v_rbtree_insert( &v_cln_timer_rbtree, &u5->timer );

}
void lookTime()
{
	v_rbtree_node_t      *node, *root, *sentinel;
	sentinel = v_cln_timer_rbtree.sentinel;
	for ( ;; ){
		root = v_cln_timer_rbtree.root;
		if (root == sentinel) {
			return;
		}
		node = v_rbtree_min(root, sentinel);
		printf("%d %d\n",node->key.v,node->key.v2 );
		v_rbtree_delete(&v_cln_timer_rbtree, node);
	}
	return;
}