/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_RENAME_H
#define PHP_RENAME_H

#define PHP_RENAME_VERSION "1.0.0"

extern zend_module_entry rename_module_entry;
#define phpext_rename_ptr &rename_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_FUNCTION(rename_function);

#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 4) || (PHP_MAJOR_VERSION > 5)
void rename_cache_clear_all_functions(TSRMLS_D);
#endif

#endif  /* PHP_RENAME_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
