#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "v_rbtree.h"


typedef struct test_struct_s{
    vs_rbtree_node_t					node;
	int								i;
    int 							j;
}test_struct_t;
typedef struct test_struct_time_s{
    vs_rbtree_node_t					timer;
	int								i;
    int 							j;
}test_struct_time_t;


vs_thread_volatile vs_rbtree_t		 test_rbtree;
static vs_rbtree_node_t				 vs_sentinel;

vs_thread_volatile vs_rbtree_t  		vs_cln_timer_rbtree;
static vs_rbtree_node_t          	vs_cln_timer_sentinel;

void insert();
test_struct_t* look1();
void del1(test_struct_t* u);

void insertTime();
void lookTime();


int main(int argc, char const *argv[])
{
	test_struct_t *u;
	int i = 0;
	vs_rbtree_init(&test_rbtree, &vs_sentinel,vs_rbtree_insert_value);
	vs_rbtree_init(&vs_cln_timer_rbtree, &vs_cln_timer_sentinel,vs_rbtree_insert_timer_value);
	insert();
	u = look1();
	if (NULL != u){
		del1(u);
	}
	u = look1();


printf("%s\n", "-----------------------------");
	//鍋氭椂闂村爢
	insertTime();
	lookTime();

	return 0;
}

void insert()
{
	test_struct_t		*u1 = malloc(sizeof(test_struct_t));
	test_struct_t		*u2 = malloc(sizeof(test_struct_t));
	vs_rbtree_node_t						*node;

	u1->i = 1;
	u1->j = 1;
	node = &u1->node;
	node->key.v = 1000;
	node->key.v2 = 1;
	vs_rbtree_insert(&test_rbtree, node);

	u2->i = 2;
	u2->j = 2;
	node = &u2->node;
	node->key.v = 1000;
	node->key.v2 = 2;
	vs_rbtree_insert(&test_rbtree, node);
}
test_struct_t* look1()
{
	vs_rbtree_node_t						*node;
	test_struct_t						*u;
    vs_rbtree_key_t                      key;

    key.v = 1000;
    key.v2 = 1;
	if (vs_OK != vs_rbtree_get(&test_rbtree, key, &vs_sentinel, &node)) {
        printf("get u1 fail\n");
        return NULL;
    }
    u = container_of(node, test_struct_t, node);
	printf("%d\n", u->j);
	return u;
}

void del1(test_struct_t* u)
{
	vs_rbtree_delete(&test_rbtree, &u->node);
	free(u);
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
	u1->i = 1;
	u2->timer.key.v = 10;
	u2->timer.key.v2 = 3;
	u2->i = 2;
	u3->timer.key.v = 10;
	u3->timer.key.v2 = 2;
	u3->i = 3;
	u4->timer.key.v = 5;
	u4->timer.key.v2 = 0;
	u4->i = 4;
	u5->timer.key.v = 12;
	u5->timer.key.v2 = 0;
	u5->i = 5;

	vs_rbtree_insert( &vs_cln_timer_rbtree, &u1->timer );
	vs_rbtree_insert( &vs_cln_timer_rbtree, &u2->timer );
	vs_rbtree_insert( &vs_cln_timer_rbtree, &u3->timer );
	vs_rbtree_insert( &vs_cln_timer_rbtree, &u4->timer );
	vs_rbtree_insert( &vs_cln_timer_rbtree, &u5->timer );

}
void lookTime()
{
	vs_rbtree_node_t      *node, *root, *sentinel;
	test_struct_time_t *t;

	sentinel = vs_cln_timer_rbtree.sentinel;
	for ( ;; ){
		root = vs_cln_timer_rbtree.root;
		if (root == sentinel) {
			return;
		}
		node = vs_rbtree_min(root, sentinel);
		printf("%d %d\n",node->key.v,node->key.v2 );
		vs_rbtree_delete(&vs_cln_timer_rbtree, node);
		t = (test_struct_time_t*)node;
		printf("%d\n",t->i );
		free(t);
	}
	return;
}