#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "minheap-internal.h"

int main(int argc, char const *argv[])
{
	min_heap_t hp;
	min_heap_ctor(&hp);

	int arr[]={54,5,5,5,5,41,7,54,1,87,638,4,84,48,7};
	for (int i = 0; i < sizeof(arr) / sizeof(int); ++i)
	{
		struct hvs_event* p = (struct hvs_event*)malloc(sizeof(struct hvs_event));
		min_heap_elem_init(p);
		p->ev_timeout.tv_sec=arr[i];
		p->ev_timeout.tv_usec=500;
		min_heap_push(&hp,p);
	}

	for (int i = 0; i < 4; ++i)
	{
		printf("%d\n", (min_heap_top(&hp))->ev_timeout.tv_sec);
		struct hvs_event* p = min_heap_pop(&hp);
		free(p);
	}
	
	int arrb[]={7,100,9};
	for (int i = 0; i < sizeof(arrb) / sizeof(int); ++i)
	{
		struct hvs_event* p = (struct hvs_event*)malloc(sizeof(struct hvs_event));
		min_heap_elem_init(p);
		p->ev_timeout.tv_sec=arrb[i];
		p->ev_timeout.tv_usec=500;
		min_heap_push(&hp,p);
	}

	struct hvs_event* temp;
	while((temp = min_heap_top(&hp)) != 0){
		printf("%d\n", temp->ev_timeout.tv_sec);
		struct hvs_event* p = min_heap_pop(&hp);
		free(p);
	}

	min_heap_dtor(&hp);

	return 0;
}