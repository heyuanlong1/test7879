#ifndef __VS_MEM_H_
#define __VS_MEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#define NGX_MAX_ALLOC_FROM_POOL		1024
#define ngx_inline      			inline
#define ngx_free          			free

#define NGX_OK      				0



typedef struct ngx_pool_s        	ngx_pool_t;
typedef unsigned int       			ngx_uint_t;
typedef void       					ngx_chain_t;
typedef void       					ngx_log_t;
typedef int        					ngx_int_t;

typedef void (*ngx_pool_cleanup_pt)(void *data);
typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;
struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    void                 *data;
    ngx_pool_cleanup_t   *next;
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;
struct ngx_pool_large_s {
    ngx_pool_large_t     *next;
    void                 *alloc;
};


typedef struct {
    u_char               *last;
    u_char               *end;
    ngx_pool_t           *next;
    ngx_uint_t            failed;
} ngx_pool_data_t;


struct ngx_pool_s {
    ngx_pool_data_t       d;
	size_t 				block_nums;
	size_t 				large_nums;
    size_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *log;
};


extern int 	block_nums;
extern int 	large_nums;

void *ngx_alloc(size_t size);					//向系统分配内存
void *ngx_calloc(size_t size);					//向系统分配内存


ngx_pool_t *ngx_create_pool(size_t size);			//创建pool
void ngx_destroy_pool(ngx_pool_t *pool);			//销毁pool
void ngx_reset_pool(ngx_pool_t *pool);				//重置pool，但是只释放了large内存

void *ngx_palloc(ngx_pool_t *pool, size_t size);				//申请
void *ngx_palloc_just_large(ngx_pool_t *pool, size_t size);		//只申请大块内存


ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);					//只释放指定的大块内存





#endif