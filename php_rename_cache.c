/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2006 The PHP Group, (c) 2008-2012 Dmitry Zenovich |
  +----------------------------------------------------------------------+
  | This source file is subject to the new BSD license,                  |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.opensource.org/licenses/BSD-3-Clause                      |
  | If you did not receive a copy of the license and are unable to       |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Sara Golemon <pollita@php.net>                               |
  | Modified by Dmitry Zenovich <dzenovich@gmail.com>                    |
  +----------------------------------------------------------------------+
  | https://github.com/zenovich/runkit                                   |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_rename.h"
#include "Zend/zend_closures.h"

typedef struct _zend_closure {
    zend_object    std;
    zend_function  func;
    HashTable     *debug_info;
} zend_closure;

#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 4) || (PHP_MAJOR_VERSION > 5)
/* {{{ rename_cache_clear_function */
static int rename_cache_clear_function(void *pDest TSRMLS_DC)
{
	zend_function *f = (zend_function *) pDest;

	if (pDest == NULL || f->type != ZEND_USER_FUNCTION ||
		f->op_array.last_cache_slot == 0 || f->op_array.run_time_cache == NULL) {
		return ZEND_HASH_APPLY_KEEP;
	}

	memset(f->op_array.run_time_cache, 0, (f->op_array.last_cache_slot) * sizeof(void*));

	return ZEND_HASH_APPLY_KEEP;
}
/* }}} */

/* {{{ rename_cache_clear_all_functions */
void rename_cache_clear_all_functions(TSRMLS_D)
{
	int i, count;
	zend_execute_data *ptr;
	HashPosition pos;

	zend_hash_apply(EG(function_table), rename_cache_clear_function TSRMLS_CC);

	zend_hash_internal_pointer_reset_ex(EG(class_table), &pos);
	count = zend_hash_num_elements(EG(class_table));
	for (i = 0; i < count; ++i) {
		zend_class_entry **curce;
		zend_hash_get_current_data_ex(EG(class_table), (void*)&curce, &pos);
		zend_hash_apply(&(*curce)->function_table, rename_cache_clear_function TSRMLS_CC);
		zend_hash_move_forward_ex(EG(class_table), &pos);
	}

	for (ptr = EG(current_execute_data); ptr != NULL; ptr = ptr->prev_execute_data) {
		if (ptr->op_array == NULL || ptr->op_array->last_cache_slot == 0 || ptr->op_array->run_time_cache == NULL) {
			continue;
		}
		memset(ptr->op_array->run_time_cache, 0, (ptr->op_array->last_cache_slot) * sizeof(void*));
	}

	if (!EG(objects_store).object_buckets) {
		return;
	}

	for (i = 1; i < EG(objects_store).top ; i++) {
		if (EG(objects_store).object_buckets[i].valid && (!EG(objects_store).object_buckets[i].destructor_called) &&
			EG(objects_store).object_buckets[i].bucket.obj.object) {
			zend_object *object;
			object = (zend_object *) EG(objects_store).object_buckets[i].bucket.obj.object;
			if (object->ce == zend_ce_closure) {
				zend_closure *cl = (zend_closure *) object;
				rename_cache_clear_function((void*) &cl->func TSRMLS_CC);
			}
		}
	}
}
/* }}} */
#endif
