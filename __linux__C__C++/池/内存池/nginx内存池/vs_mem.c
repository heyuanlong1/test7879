#include "vs_mem.h"



static ngx_inline void *ngx_palloc_small(ngx_pool_t *pool, size_t size);	//申请小块内存
static void *ngx_palloc_block(ngx_pool_t *pool, size_t size);				//看实现里的注释
static void *ngx_palloc_large(ngx_pool_t *pool, size_t size);				//申请大块内存




void *ngx_alloc(size_t size)
{
    void  *p;
    p = malloc(size);
    if (p == NULL) {
        printf("malloc fail");
    }
    return p;
}


void *ngx_calloc(size_t size)
{
    void  *p;
    p = ngx_alloc(size);
    if (p) {
        memset(p,0, size);
    }
    return p;
}


ngx_pool_t *ngx_create_pool(size_t size)
{
    ngx_pool_t  *p;

    p = ngx_alloc(size);
    if (p == NULL) {
		printf("ngx_alloc fail");
        return NULL;
    }

    p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - sizeof(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
	p->block_nums=1;
	p->large_nums=0;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = NULL;

    return p;
}


void ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler) {
            c->handler(c->data);
        }
    }

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);
        if (n == NULL) {
            break;
        }
    }
}


void ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            ngx_free(l->alloc);
			pool->large_nums--;
        }
    }

    for (p = pool; p; p = p->d.next) {
        //p->d.last = (u_char *) p + sizeof(ngx_pool_t);		//nginx 就是这样，表示有疑问
		p->d.last = (u_char *) p +  (p == pool ? sizeof(ngx_pool_t ) : sizeof(ngx_pool_data_t));
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = NULL;
    pool->large = NULL;
}


void *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
	if(size <= 0 ){
		return NULL;
	}
    if (size <= pool->max) {
        return ngx_palloc_small(pool, size);
    }

    return ngx_palloc_large(pool, size);
}



static ngx_inline void *
ngx_palloc_small(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    do {
        m = p->d.last;

        if ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;
            return m;
        }
        p = p->d.next;
    } while (p);

    return ngx_palloc_block(pool, size);
}


static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
	//(这里有个需要注意的地方，当当前内存节点的剩余空间不够分配时，nginx会重新创建一个ngx_pool_t对象，并且将pool.d->next指向新的ngx_pool_t,新分配的ngx_pool_t对象只用到了ngx_pool_data_t区域，并没有头部信息，头部信息部分已经被当做内存分配区域了)
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_alloc(psize);
    if (m == NULL) {
        return NULL;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = NULL;
    new->d.failed = 0;

    m += sizeof(ngx_pool_data_t);
    new->d.last = m + size;

    for (p = pool->current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;					//尾插法
	pool->block_nums++;
    return m;
}


static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size);
    if (p == NULL) {
        return NULL;
    }

    n = 0;

    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }

        if (n++ > 3) {
            break;
        }
    }

    large = ngx_palloc_small(pool, sizeof(ngx_pool_large_t));
    if (large == NULL) {
        ngx_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;				//头插法
	
	pool->large_nums++;
    return p;
}

void * ngx_palloc_just_large(ngx_pool_t *pool, size_t size)
{
    return ngx_palloc_large(pool, size);
}



ngx_int_t
ngx_pfree(ngx_pool_t *pool, void *p)
{
    ngx_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) {
        if (p == l->alloc) {
            ngx_free(l->alloc);
            l->alloc = NULL;
			pool->large_nums--;
            return NGX_OK;
        }
    }
	
    return NGX_OK;
}




