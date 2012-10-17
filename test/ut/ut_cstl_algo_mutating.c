#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <google/cmockery.h>

#include "cstl/cstl_def.h"
#include "cstl/citerator.h"
#include "cstl/chash_map.h"
#include "cstl/cstring.h"
#include "cstl/cvector.h"
#include "cstl/clist.h"
#include "cstl/cdeque.h"
#include "cstl/cslist.h"
#include "cstl/cmap.h"
#include "cstl/cset.h"
#include "cstl/chash_set.h"
#include "cstl/calgorithm.h"

#include "ut_def.h"
#include "ut_cstl_algo_mutating.h"

UT_SUIT_DEFINATION(cstl_algo_mutating, algo_copy)

/*
 * test algo_copy
 */
UT_CASE_DEFINATION(algo_copy)
void test_algo_copy__invalid_source_range(void** state)
{
    vector_t* pvec = create_vector(int);
    list_t* plist = create_list(int);

    vector_init_n(pvec, 10);
    list_init_n(plist, 10);
    expect_assert_failure(algo_copy(vector_begin(pvec), list_begin(plist), list_begin(plist)));
    vector_destroy(pvec);
    list_destroy(plist);
}

void test_algo_copy__invalid_source_range2(void** state)
{
    deque_t* pdeq = create_deque(int);
    list_t* plist = create_list(int);

    deque_init_n(pdeq, 10);
    list_init_n(plist, 10);
    expect_assert_failure(algo_copy(deque_end(pdeq), deque_begin(pdeq), list_begin(plist)));
    deque_destroy(pdeq);
    list_destroy(plist);
}

void test_algo_copy__invalid_source_range3(void** state)
{
    list_t* plist = create_list(int);
    slist_t* pslist = create_slist(int);
    iterator_t it;

    list_init_n(plist, 10);
    slist_init_n(pslist, 10);
    it = list_begin(plist);
    it._t_iteratortype = 993;
    expect_assert_failure(algo_copy(it, list_end(plist), slist_begin(pslist)));
    list_destroy(plist);
    slist_destroy(pslist);
}

void test_algo_copy__invalid_dest_range(void** state)
{
    list_t* plist = create_list(int);
    slist_t* pslist = create_slist(int);
    iterator_t it;

    list_init_n(plist, 10);
    slist_init_n(pslist, 10);
    it = slist_begin(pslist);
    it._t_iteratortype = 993;
    expect_assert_failure(algo_copy(list_begin(plist), list_end(plist), it));
    list_destroy(plist);
    slist_destroy(pslist);
}

void test_algo_copy__invalid_range_not_same_type(void** state)
{
    deque_t* pdeq = create_deque(int);
    vector_t* pvec = create_vector(char*);

    deque_init_n(pdeq, 20);
    vector_init_n(pvec, 10);
    expect_assert_failure(algo_copy(deque_begin(pdeq), deque_end(pdeq), vector_begin(pvec)));
    deque_destroy(pdeq);
    vector_destroy(pvec);
}

void test_algo_copy__source_range_empty(void** state)
{
    set_t* pset = create_set(int);
    list_t* plist = create_list(int);
    iterator_t it;

    set_init(pset);
    list_init_n(plist, 10);
    it = algo_copy(set_begin(pset), set_end(pset), list_begin(plist));
    assert_true(iterator_equal(list_begin(plist), it));
    set_destroy(pset);
    list_destroy(plist);
}

void test_algo_copy__overlapping_left(void** state)
{
    list_t* plist = create_list(int);
    list_t* plist_result = create_list(int);
    int an_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int an_result[] = {4, 5, 6, 7, 8, 9, 7, 8, 9};
    iterator_t it_first;
    iterator_t it_last;
    iterator_t it_result;
    iterator_t it_end;

    list_init_copy_array(plist, an_array, sizeof(an_array)/sizeof(an_array[0]));
    list_init_copy_array(plist_result, an_result, sizeof(an_result)/sizeof(an_result[0]));
    it_first = iterator_advance(list_begin(plist), 3);
    it_last = list_end(plist);
    it_result = list_begin(plist);
    it_end = algo_copy(it_first, it_last, it_result);

    assert_true(iterator_distance(it_first, it_last) == iterator_distance(it_result, it_end));
    assert_true(list_equal(plist, plist_result));

    list_destroy(plist);
    list_destroy(plist_result);
}

void test_algo_copy__overlapping_right(void** state)
{
    list_t* plist = create_list(int);
    list_t* plist_result = create_list(int);
    int an_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int an_result[] = {1, 2, 3, 1, 2, 3, 1, 2, 3};
    iterator_t it_first;
    iterator_t it_last;
    iterator_t it_result;
    iterator_t it_end;

    list_init_copy_array(plist, an_array, sizeof(an_array)/sizeof(an_array[0]));
    list_init_copy_array(plist_result, an_result, sizeof(an_result)/sizeof(an_result[0]));
    it_first = list_begin(plist);
    it_last = iterator_advance(it_first, 6);
    it_result = iterator_advance(it_first, 3);
    it_end = algo_copy(it_first, it_last, it_result);

    assert_true(iterator_distance(it_first, it_last) == iterator_distance(it_result, it_end));
    assert_true(list_equal(plist, plist_result));

    list_destroy(plist);
    list_destroy(plist_result);
}

void test_algo_copy__c_builtin(void** state)
{
    vector_t* pvec = create_vector(int);
    list_t* plist = create_list(int);
    int an_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    iterator_t it;

    vector_init_copy_array(pvec, an_array, sizeof(an_array)/sizeof(an_array[0]));
    list_init_n(plist, 10);
    it = algo_copy(vector_begin(pvec), vector_end(pvec), list_begin(plist));
    assert_true(iterator_distance(vector_begin(pvec), vector_end(pvec)) == iterator_distance(list_begin(plist), it));
    assert_true(algo_equal(vector_begin(pvec), vector_end(pvec), list_begin(plist)));
    vector_destroy(pvec);
    list_destroy(plist);
}

void test_algo_copy__cstr(void** state)
{
    deque_t* pdeq = create_deque(char*);
    slist_t* pslist = create_slist(char*);
    const char* as_array[] = {"a", "b", "c", "d", "e", "f"};
    iterator_t it;

    deque_init_copy_array(pdeq, as_array, sizeof(as_array)/sizeof(as_array[0]));
    slist_init_n(pslist, 10);
    it = algo_copy(deque_begin(pdeq), deque_end(pdeq), slist_begin(pslist));
    assert_true(iterator_distance(deque_begin(pdeq), deque_end(pdeq)) == iterator_distance(slist_begin(pslist), it));
    assert_true(algo_equal(deque_begin(pdeq), deque_end(pdeq), slist_begin(pslist)));
    deque_destroy(pdeq);
    slist_destroy(pslist);
}

void test_algo_copy__cstl_builtin(void** state)
{
    set_t* pset = create_set(deque_t<int>);
    list_t* plist = create_list(deque_t<int>);
    deque_t* pdeq = create_deque(int);
    int aan_array[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
    iterator_t it;
    int i = 0;

    set_init(pset);
    list_init_n(plist, 10);
    deque_init(pdeq);
    for (i = 0; i < sizeof(aan_array)/sizeof(aan_array[0]); ++i) {
        deque_assign_array(pdeq, aan_array[i], sizeof(aan_array[i])/sizeof(aan_array[i][0]));
        set_insert(pset, pdeq);
    }
    it = algo_copy(set_begin(pset), set_end(pset), list_begin(plist));
    assert_true(iterator_distance(set_begin(pset), set_end(pset)) == iterator_distance(list_begin(plist), it));
    assert_true(algo_equal(set_begin(pset), set_end(pset), list_begin(plist)));
    set_destroy(pset);
    list_destroy(plist);
    deque_destroy(pdeq);
}

typedef struct _tag_test_algo_copy__user_define {
    int a;
    int b;
} _test_algo_copy__user_define_t;
void test_algo_copy__user_define(void** state)
{
    multiset_t* pmset = NULL;
    deque_t* pdeq = NULL;
    _test_algo_copy__user_define_t at_array[] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 0}};
    iterator_t it;
    int i = 0;

    type_register(_test_algo_copy__user_define_t, NULL, NULL, NULL, NULL);
    pmset = create_multiset(_test_algo_copy__user_define_t);
    pdeq = create_deque(_test_algo_copy__user_define_t);
    multiset_init(pmset);
    deque_init_n(pdeq, 10);
    for (i = 0; i < sizeof(at_array)/sizeof(at_array[0]); ++i) {
        multiset_insert(pmset, &at_array[i]);
    }
    it = algo_copy(multiset_begin(pmset), multiset_end(pmset), deque_begin(pdeq));
    assert_true(iterator_distance(multiset_begin(pmset), multiset_end(pmset)) == iterator_distance(deque_begin(pdeq), it));
    assert_true(algo_equal(multiset_begin(pmset), multiset_end(pmset), deque_begin(pdeq)));
    multiset_destroy(pmset);
    deque_destroy(pdeq);
}

/*
 * test algo_copy_n
 */
UT_CASE_DEFINATION(algo_copy_n)
void test_algo_copy_n__invalid_source_range(void** state)
{
    deque_t* pdeq = create_deque(int);
    list_t* plist = create_list(int);
    iterator_t it;

    deque_init_n(pdeq, 10);
    list_init_n(plist, 10);
    it = deque_begin(pdeq);
    it._t_iteratortype = 100;
    expect_assert_failure(algo_copy_n(it, 10, list_begin(plist)));
    deque_destroy(pdeq);
    list_destroy(plist);
}

void test_algo_copy_n__invalid_dest_range(void** state)
{
    deque_t* pdeq = create_deque(int);
    list_t* plist = create_list(int);
    iterator_t it;

    deque_init_n(pdeq, 10);
    list_init_n(plist, 10);
    it = list_begin(plist);
    it._t_iteratortype = 100;
    expect_assert_failure(algo_copy_n(deque_begin(pdeq), 10, it));
    deque_destroy(pdeq);
    list_destroy(plist);
}

void test_algo_copy_n__invalid_range_not_same_type(void** state)
{
    vector_t* pvec = create_vector(int);
    list_t* plist = create_list(char*);

    vector_init_n(pvec, 10);
    list_init_n(plist, 10);
    expect_assert_failure(algo_copy_n(vector_begin(pvec), 10, list_begin(plist)));
    vector_destroy(pvec);
    list_destroy(plist);
}

void test_algo_copy_n__source_range_empty(void** state)
{
    deque_t* pdeq = create_deque(int);
    iterator_t it;

    deque_init_n(pdeq, 10);
    it = algo_copy_n(deque_begin(pdeq), 0, iterator_next_n(deque_begin(pdeq), 5));
    assert_true(iterator_equal(it, iterator_next_n(deque_begin(pdeq), 5)));
    deque_destroy(pdeq);
}

void test_algo_copy_n__overlapping_left(void** state)
{
    list_t* plist = create_list(int);
    list_t* plist_result = create_list(int);
    int an_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    int an_result[] = {3, 4, 5, 6, 7, 8, 9, 0, 9, 0};
    iterator_t it_first;
    iterator_t it_result;
    iterator_t it;

    list_init_copy_array(plist, an_array, sizeof(an_array)/sizeof(an_array[0]));
    list_init_copy_array(plist_result, an_result, sizeof(an_result)/sizeof(an_result[0]));
    it_first = iterator_advance(list_begin(plist), 2);
    it_result = list_begin(plist);
    it = algo_copy_n(it_first, 8, it_result);
    assert_true(iterator_distance(it_result, it) == 8);
    assert_true(algo_equal(list_begin(plist), list_end(plist), list_begin(plist_result)));
    list_destroy(plist);
    list_destroy(plist_result);
}

void test_algo_copy_n__overlapping_right(void** state)
{
    list_t* plist = create_list(int);
    list_t* plist_result = create_list(int);
    int an_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    int an_result[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    iterator_t it_first;
    iterator_t it_result;
    iterator_t it;

    list_init_copy_array(plist, an_array, sizeof(an_array)/sizeof(an_array[0]));
    list_init_copy_array(plist_result, an_result, sizeof(an_result)/sizeof(an_result[0]));
    it_first = list_begin(plist);
    it_result = iterator_next(list_begin(plist));
    it = algo_copy_n(it_first, 9, it_result);
    assert_true(iterator_distance(it_result, it) == 9);
    assert_true(algo_equal(list_begin(plist), list_end(plist), list_begin(plist_result)));
    list_destroy(plist);
    list_destroy(plist_result);
}

void test_algo_copy_n__c_builtin(void** state)
{
    set_t* pset = create_set(int);
    list_t* plist = create_list(int);
    int an_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    iterator_t it;

    set_init_copy_array(pset, an_array, sizeof(an_array)/sizeof(an_array[0]));
    list_init_n(plist, 10);
    it = algo_copy_n(set_begin(pset), 10, list_begin(plist));
    assert_true(iterator_distance(list_begin(plist), it) == 10);
    assert_true(algo_equal(set_begin(pset), set_end(pset), list_begin(plist)));
    set_destroy(pset);
    list_destroy(plist);
}

void test_algo_copy_n__cstr(void** state)
{
    multiset_t* pmset = create_multiset(char*);
    vector_t* pvec = create_vector(char*);
    const char* as_array[] = {"Windows 3.1", "Windows 95", "Windows 98", "Windows ME", "Windows 2000"};
    iterator_t it;

    multiset_init_copy_array(pmset, as_array, sizeof(as_array)/sizeof(as_array[0]));
    vector_init_n(pvec, 10);
    it = algo_copy_n(multiset_begin(pmset), 5, vector_begin(pvec));
    assert_true(iterator_distance(vector_begin(pvec), it));
    assert_true(algo_equal(multiset_begin(pmset), multiset_end(pmset), vector_begin(pvec)));
    multiset_destroy(pmset);
    vector_destroy(pvec);
}

void test_algo_copy_n__cstl_builtin(void** state)
{
    hash_set_t* phset = create_hash_set(vector_t<int>);
    deque_t* pdeq = create_deque(vector_t<int>);
    vector_t* pvec = create_vector(int);
    int aan_array[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {0, 1, 2}, {3, 4, 5}};
    iterator_t it;
    int i = 0;

    hash_set_init(phset);
    deque_init_n(pdeq, 10);
    vector_init(pvec);
    for (i = 0; i < sizeof(aan_array)/sizeof(aan_array[0]); ++i) {
        vector_assign_array(pvec, aan_array[i], sizeof(aan_array[i])/sizeof(aan_array[i][0]));
        hash_set_insert(phset, pvec);
    }
    it = algo_copy_n(hash_set_begin(phset), hash_set_size(phset), deque_begin(pdeq));
    assert_true(iterator_distance(deque_begin(pdeq), it) == hash_set_size(phset));
    assert_true(algo_equal(hash_set_begin(phset), hash_set_end(phset), deque_begin(pdeq)));
    hash_set_destroy(phset);
    deque_destroy(pdeq);
    vector_destroy(pvec);
}

typedef struct _tag_test_algo_copy_n__user_define {
    int a;
    int b;
} _test_algo_copy_n__user_define_t;
void test_algo_copy_n__user_define(void** state)
{
    hash_multiset_t* phmset = NULL;
    slist_t* pslist = NULL;
    _test_algo_copy_n__user_define_t at_array[] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 0}};
    iterator_t it;
    int i = 0;

    type_register(_test_algo_copy_n__user_define_t, NULL, NULL, NULL, NULL);
    phmset = create_hash_multiset(_test_algo_copy_n__user_define_t);
    pslist = create_slist(_test_algo_copy_n__user_define_t);
    hash_multiset_init(phmset);
    slist_init_n(pslist, 10);
    for (i = 0; i < sizeof(at_array)/sizeof(at_array[0]); ++i) {
        hash_multiset_insert(phmset, &at_array[i]);
    }
    it = algo_copy_n(hash_multiset_begin(phmset), hash_multiset_size(phmset), slist_begin(pslist));
    assert_true(iterator_distance(slist_begin(pslist), it) == hash_multiset_size(phmset));
    assert_true(algo_equal(hash_multiset_begin(phmset), hash_multiset_end(phmset), slist_begin(pslist)));
    hash_multiset_destroy(phmset);
    slist_destroy(pslist);
}

/*
 * test algo_copy_backward
 */
UT_CASE_DEFINATION(algo_copy_backward)
void test_algo_copy_backward__invalid_source_range(void** state)
{
    vector_t* pvec = create_vector(int);
    iterator_t it;

    vector_init_n(pvec, 10);
    it = vector_begin(pvec);
    it._t_iteratortype = 100;
    expect_assert_failure(algo_copy_backward(it, iterator_next(vector_begin(pvec)), iterator_next_n(vector_begin(pvec), 5)));
    vector_destroy(pvec);
}

void test_algo_copy_backward__invalid_source_range2(void** state)
{
    list_t* plist = create_list(int);
    deque_t* pdeq = create_deque(int);

    list_init_n(plist, 10);
    deque_init_n(pdeq, 10);
    expect_assert_failure(algo_copy_backward(list_end(plist), list_begin(plist), deque_end(pdeq)));
    list_destroy(plist);
    deque_destroy(pdeq);
}

void test_algo_copy_backward__invalid_source_range3(void** state)
{
    list_t* plist = create_list(int);
    deque_t* pdeq = create_deque(int);

    list_init_n(plist, 10);
    deque_init_n(pdeq, 10);
    expect_assert_failure(algo_copy_backward(list_begin(plist), deque_begin(pdeq), deque_end(pdeq)));
    list_destroy(plist);
    deque_destroy(pdeq);
}

void test_algo_copy_backward__invalid_dest_range(void** state)
{
    deque_t* pdeq = create_deque(int);
    slist_t* pslist = create_slist(int);

    deque_init_n(pdeq, 10);
    slist_init_n(pslist, 10);
    expect_assert_failure(algo_copy_backward(deque_begin(pdeq), deque_end(pdeq), slist_end(pslist)));
    deque_destroy(pdeq);
    slist_destroy(pslist);
}

void test_algo_copy_backward__invalid_range_not_same_type(void** state)
{
    vector_t* pvec = create_vector(int);
    list_t* plist = create_list(char);

    vector_init_n(pvec, 10);
    list_init_n(plist, 10);
    expect_assert_failure(algo_copy_backward(vector_begin(pvec), vector_end(pvec), list_end(plist)));
    vector_destroy(pvec);
    list_destroy(plist);
}

void test_algo_copy_backward__source_range_empty(void** state)
{
    set_t* pset = create_set(int);
    list_t* plist = create_list(int);
    iterator_t it;

    set_init(pset);
    list_init_n(plist, 10);
    it = algo_copy_backward(set_begin(pset), set_end(pset), list_end(plist));
    assert_true(iterator_equal(it, list_end(plist)));
    set_destroy(pset);
    list_destroy(plist);
}

void test_algo_copy_backward__overlapping_left(void** state)
{
    list_t* plist = create_list(int);
    list_t* plist_result = create_list(int);
    int an_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    int an_result[] = {9, 0, 9, 0, 9, 0, 9, 0, 9, 0};
    iterator_t it_first;
    iterator_t it_last;
    iterator_t it_result;
    iterator_t it;

    list_init_copy_array(plist, an_array, sizeof(an_array)/sizeof(an_array[0]));
    list_init_copy_array(plist_result, an_result, sizeof(an_result)/sizeof(an_result[0]));
    it_first = iterator_advance(list_begin(plist), 2);
    it_last = list_end(plist);
    it_result = iterator_advance(list_end(plist), -2);
    it = algo_copy_backward(it_first, it_last, it_result);
    assert_true(iterator_equal(it, list_begin(plist)));
    assert_true(algo_equal(list_begin(plist), list_end(plist), list_begin(plist_result)));
    list_destroy(plist);
    list_destroy(plist_result);
}

void test_algo_copy_backward__overlapping_right(void** state)
{
    list_t* plist = create_list(int);
    list_t* plist_result = create_list(int);
    int an_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    int an_result[] = {1, 2, 3, 4, 1, 2, 3, 4, 5, 6};
    iterator_t it_first;
    iterator_t it_last;
    iterator_t it_result;
    iterator_t it;

    list_init_copy_array(plist, an_array, sizeof(an_array)/sizeof(an_array[0]));
    list_init_copy_array(plist_result, an_result, sizeof(an_result)/sizeof(an_result[0]));
    it_first = list_begin(plist);
    it_last = iterator_advance(it_first, 6);
    it_result = list_end(plist);
    it = algo_copy_backward(it_first, it_last, it_result);
    assert_true(iterator_equal(it, iterator_advance(list_begin(plist), 4)));
    assert_true(algo_equal(list_begin(plist), list_end(plist), list_begin(plist_result)));
    list_destroy(plist);
    list_destroy(plist_result);
}

void test_algo_copy_backward__c_builtin(void** state)
{
    vector_t* pvec = create_vector(int);
    list_t* plist = create_list(int);
    int an_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    iterator_t it;

    vector_init_copy_array(pvec, an_array, sizeof(an_array)/sizeof(an_array[0]));
    list_init_n(plist, 10);
    it = algo_copy_backward(vector_begin(pvec), vector_end(pvec), list_end(plist));
    assert_true(iterator_distance(it, list_end(plist)) == vector_size(pvec));
    assert_true(algo_equal(vector_begin(pvec), vector_end(pvec), list_begin(plist)));
    vector_destroy(pvec);
    list_destroy(plist);
}

void test_algo_copy_backward__cstr(void** state)
{
    set_t* pset = create_set(char*);
    deque_t* pdeq = create_deque(char*);
    const char* as_array[] = {"CentOS", "Fedora", "Ubuntu", "FreeBSD", "AIX"};
    iterator_t it;

    set_init_copy_array(pset, as_array, sizeof(as_array)/sizeof(as_array[0]));
    deque_init_n(pdeq, 10);
    it = algo_copy_backward(set_begin(pset), set_end(pset), deque_end(pdeq));
    assert_true(iterator_distance(it, deque_end(pdeq)) == set_size(pset));
    assert_true(algo_equal(set_begin(pset), set_end(pset), it));
    set_destroy(pset);
    deque_destroy(pdeq);
}

void test_algo_copy_backward__cstl_builtin(void** state)
{
    hash_set_t* phset = create_hash_set(vector_t<int>);
    list_t* plist = create_list(vector_t<int>);
    vector_t* pvec = create_vector(int);
    int aan_array[][5] = {{1, 2, 3, 4, 5}, {2, 3, 4, 5, 6}, {3, 4, 5, 6, 7}, {4, 5, 6, 7, 8}, {5, 6, 7, 8, 9}};
    iterator_t it;
    int i = 0;

    hash_set_init(phset);
    list_init_n(plist, 10);
    vector_init(pvec);
    for (i = 0; i < sizeof(aan_array)/sizeof(aan_array[0]); ++i) {
        vector_assign_array(pvec, aan_array[i], sizeof(aan_array[i])/sizeof(aan_array[i][0]));
        hash_set_insert(phset, pvec);
    }
    it = algo_copy_backward(hash_set_begin(phset), hash_set_end(phset), list_end(plist));
    assert_true(iterator_distance(it, list_end(plist)) == hash_set_size(phset));
    assert_true(algo_equal(hash_set_begin(phset), hash_set_end(phset), it));
    hash_set_destroy(phset);
    list_destroy(plist);
    vector_destroy(pvec);
}

typedef struct _tag_test_algo_copy_backward__user_define {
    int a;
    int b;
} _test_algo_copy_backward__user_define_t;
void test_algo_copy_backward__user_define(void** state)
{
    multiset_t* pmset = NULL;
    deque_t* pdeq = NULL;
    _test_algo_copy_backward__user_define_t at_array[] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 0}};
    iterator_t it;
    int i = 0;

    type_register(_test_algo_copy_backward__user_define_t, NULL, NULL, NULL, NULL);
    pmset = create_multiset(_test_algo_copy_backward__user_define_t);
    pdeq = create_deque(_test_algo_copy_backward__user_define_t);
    multiset_init(pmset);
    deque_init_n(pdeq, 10);
    for (i = 0; i < sizeof(at_array)/sizeof(at_array[0]); ++i) {
        multiset_insert(pmset, &at_array[i]);
    }
    it = algo_copy_backward(multiset_begin(pmset), multiset_end(pmset), deque_end(pdeq));
    assert_true(iterator_distance(it, deque_end(pdeq)) == multiset_size(pmset));
    assert_true(algo_equal(multiset_begin(pmset), multiset_end(pmset), it));
    multiset_destroy(pmset);
    deque_destroy(pdeq);
}

/*
 * test algo_swap and algo_iter_swap
 */
UT_CASE_DEFINATION(algo_swap_algo_iter_swap)
void test_algo_swap_algo_iter_swap__invalid_first(void** state)
{
    vector_t* pvec = create_vector(int);
    iterator_t it_first;
    iterator_t it_second;

    vector_init_n(pvec, 10);
    it_first = vector_begin(pvec);
    it_second = iterator_next(it_first);
    it_first._t_iteratortype = 199;
    expect_assert_failure(algo_swap(it_first, it_second));
    vector_destroy(pvec);
}

void test_algo_swap_algo_iter_swap__invalid_second(void** state)
{
    vector_t* pvec = create_vector(int);
    iterator_t it_first;
    iterator_t it_second;

    vector_init_n(pvec, 10);
    it_first = vector_begin(pvec);
    it_second = iterator_next(it_first);
    it_second._t_iteratortype = 199;
    expect_assert_failure(algo_swap(it_first, it_second));
    vector_destroy(pvec);
}

void test_algo_swap_algo_iter_swap__not_same_type(void** state)
{
    list_t* plist = create_list(int);
    vector_t* pvec = create_vector(char*);

    list_init_n(plist, 10);
    vector_init_n(pvec, 10);
    expect_assert_failure(algo_swap(list_begin(plist), vector_begin(pvec)));
    list_destroy(plist);
    vector_destroy(pvec);
}

void test_algo_swap_algo_iter_swap__c_builtin(void** state)
{
    list_t* plist = create_list(int);
    iterator_t it_first;
    iterator_t it_second;

    list_init(plist);
    list_push_back(plist, 10);
    list_push_back(plist, 20);
    it_first = list_begin(plist);
    it_second = iterator_next(it_first);
    assert_true(*(int*)iterator_get_pointer(it_first) == 10);
    assert_true(*(int*)iterator_get_pointer(it_second) == 20);
    algo_swap(it_first, it_second);
    assert_true(*(int*)iterator_get_pointer(it_first) == 20);
    assert_true(*(int*)iterator_get_pointer(it_second) == 10);
    list_destroy(plist);
}

void test_algo_swap_algo_iter_swap__cstr(void** state)
{
    deque_t* pdeq = create_deque(char*);
    iterator_t it_first;
    iterator_t it_second;

    deque_init(pdeq);
    deque_push_back(pdeq, "Linux");
    deque_push_back(pdeq, "Windows");
    it_first = deque_begin(pdeq);
    it_second = iterator_next(it_first);
    assert_true(strcmp((char*)iterator_get_pointer(it_first), "Linux") == 0);
    assert_true(strcmp((char*)iterator_get_pointer(it_second), "Windows") == 0);
    algo_swap(it_first, it_second);
    assert_true(strcmp((char*)iterator_get_pointer(it_first), "Windows") == 0);
    assert_true(strcmp((char*)iterator_get_pointer(it_second), "Linux") == 0);
    deque_destroy(pdeq);
}

void test_algo_swap_algo_iter_swap__cstl_builtin(void** state)
{
    slist_t* pslist = create_slist(set_t<int>);
    set_t* pset = create_set(int);
    iterator_t it_first;
    iterator_t it_second;

    slist_init(pslist);
    set_init(pset);
    slist_push_front(pslist, pset);
    set_insert(pset, 100);
    slist_push_front(pslist, pset);
    it_first = slist_begin(pslist);
    it_second = iterator_next(it_first);
    assert_true(set_size((set_t*)iterator_get_pointer(it_first)) == 1);
    assert_true(set_size((set_t*)iterator_get_pointer(it_second)) == 0);
    algo_swap(it_first, it_second);
    assert_true(set_size((set_t*)iterator_get_pointer(it_first)) == 0);
    assert_true(set_size((set_t*)iterator_get_pointer(it_second)) == 1);
    slist_destroy(pslist);
    set_destroy(pset);
}

typedef struct _tag_test_algo_swap_algo_iter_swap__user_define {
    int a;
    int b;
} _test_algo_swap_algo_iter_swap__user_define_t;
void test_algo_swap_algo_iter_swap__user_define(void** state)
{
    vector_t* pvec = NULL;
    _test_algo_swap_algo_iter_swap__user_define_t t_elem;
    iterator_t it_first;
    iterator_t it_second;

    type_register(_test_algo_swap_algo_iter_swap__user_define_t, NULL, NULL, NULL, NULL);
    pvec = create_vector(_test_algo_swap_algo_iter_swap__user_define_t);
    vector_init(pvec);
    t_elem.a = t_elem.b = 10;
    vector_push_back(pvec, &t_elem);
    t_elem.a = t_elem.b = 20;
    vector_push_back(pvec, &t_elem);
    it_first = vector_begin(pvec);
    it_second = iterator_next(it_first);
    assert_true(
        ((_test_algo_swap_algo_iter_swap__user_define_t*)iterator_get_pointer(it_first))->a == 10 &&
        ((_test_algo_swap_algo_iter_swap__user_define_t*)iterator_get_pointer(it_first))->b == 10);
    assert_true(
        ((_test_algo_swap_algo_iter_swap__user_define_t*)iterator_get_pointer(it_second))->a == 20 &&
        ((_test_algo_swap_algo_iter_swap__user_define_t*)iterator_get_pointer(it_second))->b == 20);
    algo_swap(it_first, it_second);
    assert_true(
        ((_test_algo_swap_algo_iter_swap__user_define_t*)iterator_get_pointer(it_first))->a == 20 &&
        ((_test_algo_swap_algo_iter_swap__user_define_t*)iterator_get_pointer(it_first))->b == 20);
    assert_true(
        ((_test_algo_swap_algo_iter_swap__user_define_t*)iterator_get_pointer(it_second))->a == 10 &&
        ((_test_algo_swap_algo_iter_swap__user_define_t*)iterator_get_pointer(it_second))->b == 10);
    vector_destroy(pvec);
}

/*
 * test algo_swap_ranges
 */
UT_CASE_DEFINATION(algo_swap_ranges)
void test_algo_swap_ranges__invalid_first_range(void** state)
{
    vector_t* pvec = create_vector(int);
    list_t* plist = create_list(int);

    vector_init_n(pvec, 10);
    list_init_n(plist, 10);
    expect_assert_failure(algo_swap_ranges(vector_begin(pvec), list_begin(plist), list_begin(plist)));
    vector_destroy(pvec);
    list_destroy(plist);
}

void test_algo_swap_ranges__invalid_first_range2(void** state)
{
    deque_t* pdeq = create_deque(int);
    slist_t* pslist = create_slist(int);

    deque_init_n(pdeq, 10);
    slist_init_n(pslist, 10);
    expect_assert_failure(algo_swap_ranges(deque_end(pdeq), deque_begin(pdeq), slist_begin(pslist)));
    deque_destroy(pdeq);
    slist_destroy(pslist);
}

void test_algo_swap_ranges__invalid_first_range3(void** state)
{
    set_t* pset = create_set(int);
    multiset_t* pmset = create_multiset(int);
    iterator_t it;

    set_init(pset);
    multiset_init(pmset);
    it = set_begin(pset);
    it._t_iteratortype = 199;
    expect_assert_failure(algo_swap_ranges(it, set_end(pset), multiset_begin(pmset)));
    set_destroy(pset);
    multiset_destroy(pmset);
}

void test_algo_swap_ranges__invalid_second_range(void** state)
{
    set_t* pset = create_set(int);
    multiset_t* pmset = create_multiset(int);
    iterator_t it;

    set_init(pset);
    multiset_init(pmset);
    it = multiset_begin(pmset);
    it._t_iteratortype = 199;
    expect_assert_failure(algo_swap_ranges(set_begin(pset), set_end(pset), it));
    set_destroy(pset);
    multiset_destroy(pmset);
}

void test_algo_swap_ranges__invalid_range_not_same_type(void** state)
{
    hash_set_t* phset = create_hash_set(int);
    hash_multiset_t* phmset = create_hash_multiset(char*);

    hash_set_init(phset);
    hash_multiset_init(phmset);
    expect_assert_failure(algo_swap_ranges(hash_set_begin(phset), hash_set_end(phset), hash_multiset_begin(phmset)));
    hash_set_destroy(phset);
    hash_multiset_destroy(phmset);
}

void test_algo_swap_ranges__first_range_empty(void** state)
{
    set_t* pset = create_set(int);
    multiset_t* pmset = create_multiset(int);

    set_init(pset);
    multiset_init(pmset);
    assert_true(iterator_equal(algo_swap_ranges(set_begin(pset), set_end(pset), multiset_begin(pmset)), multiset_begin(pmset)));
    set_destroy(pset);
    multiset_destroy(pmset);
}

void test_algo_swap_ranges__c_builtin(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_t* pvec_result = create_vector(int);
    int an_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    int an_result[] = {5, 6, 7, 8, 9, 0, 3, 4, 1, 2};
    iterator_t it_first1;
    iterator_t it_last1;
    iterator_t it_first2;
    iterator_t it_last2;

    vector_init_copy_array(pvec, an_array, sizeof(an_array)/sizeof(an_array[0]));
    vector_init_copy_array(pvec_result, an_result, sizeof(an_result)/sizeof(an_result[0]));
    it_first1 = vector_begin(pvec);
    it_last1 = iterator_next_n(it_first1, 6);
    it_first2 = iterator_next_n(it_first1, 4);
    it_last2 = algo_swap_ranges(it_first1, it_last1, it_first2);
    assert_true(iterator_distance(it_first1, it_last1) == iterator_distance(it_first2, it_last2));
    assert_true(vector_equal(pvec, pvec_result));
    vector_destroy(pvec);
    vector_destroy(pvec_result);
}

void test_algo_swap_ranges__cstr(void** state)
{
    list_t* plist = create_list(char*);
    list_t* plist_result = create_list(char*);
    const char* as_array[] = {"abc", "def", "hij", "klm", "nop", "<--->", "AAAA", "BBBB", "CCCC", "DDDD", "EEEE"};
    const char* as_result[] = {"AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "<--->", "abc", "def", "hij", "klm", "nop"};
    iterator_t it_first1;
    iterator_t it_last1;
    iterator_t it_first2;
    iterator_t it_last2;

    list_init_copy_array(plist, as_array, sizeof(as_array)/sizeof(as_array[0]));
    list_init_copy_array(plist_result, as_result, sizeof(as_result)/sizeof(as_result[0]));
    it_first1 = list_begin(plist);
    it_last1 = iterator_advance(it_first1, 5);
    it_first2 = iterator_advance(it_first1, 6);
    it_last2 = algo_swap_ranges(it_first1, it_last1, it_first2);
    assert_true(iterator_distance(it_first1, it_last1) == iterator_distance(it_first2, it_last2));
    assert_true(list_equal(plist, plist_result));
    list_destroy(plist);
    list_destroy(plist_result);
}

void test_algo_swap_ranges__cstl_builtin(void** state)
{
    deque_t* pdeq = create_deque(vector_t<int>);
    deque_t* pdeq_result = create_deque(vector_t<int>);
    vector_t* pvec = create_vector(int);
    int aan_array[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}, {19, 20, 21}, {22, 23, 24}};
    int aan_result[][3] = {{7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}, {19, 20, 21}, {22, 23, 24}, {1, 2, 3}, {4, 5, 6}};
    iterator_t it_first1;
    iterator_t it_last1;
    iterator_t it_first2;
    iterator_t it_last2;
    int i = 0;

    deque_init(pdeq);
    deque_init(pdeq_result);
    vector_init(pvec);
    for (i = 0; i < sizeof(aan_array)/sizeof(aan_array[0]); ++i) {
        vector_assign_array(pvec, aan_array[i], sizeof(aan_array[i])/sizeof(aan_array[i][0]));
        deque_push_back(pdeq, pvec);
    }
    for (i = 0; i < sizeof(aan_result)/sizeof(aan_result[0]); ++i) {
        vector_assign_array(pvec, aan_result[i], sizeof(aan_result[i])/sizeof(aan_result[i][0]));
        deque_push_back(pdeq_result, pvec);
    }
    it_first1 = iterator_next_n(deque_begin(pdeq), 2);
    it_last1 = deque_end(pdeq);
    it_first2 = deque_begin(pdeq);
    it_last2 = algo_swap_ranges(it_first1, it_last1, it_first2);
    assert_true(iterator_distance(it_first1, it_last1) == iterator_distance(it_first2, it_last2));
    assert_true(deque_equal(pdeq, pdeq_result));
    deque_destroy(pdeq);
    deque_destroy(pdeq_result);
    vector_destroy(pvec);
}

typedef struct _tag_test_algo_swap_ranges__user_define {
    int a;
    int b;
} _test_algo_swap_ranges__user_define_t;
void test_algo_swap_ranges__user_define(void** state)
{
    list_t* plist = NULL;
    list_t* plist_result = NULL;
    _test_algo_swap_ranges__user_define_t at_array[] = {{1, 2}, {3, 4}, {0, 0}, {101, 102}, {103, 104}};
    _test_algo_swap_ranges__user_define_t at_result[] = {{101, 102}, {103, 104}, {0, 0}, {1, 2}, {3, 4}};
    iterator_t it_first1;
    iterator_t it_last1;
    iterator_t it_first2;
    iterator_t it_last2;
    int i = 0;

    type_register(_test_algo_swap_ranges__user_define_t, NULL, NULL, NULL, NULL);
    plist = create_list(_test_algo_swap_ranges__user_define_t);
    plist_result = create_list(_test_algo_swap_ranges__user_define_t);
    list_init(plist);
    list_init(plist_result);
    for (i = 0; i < sizeof(at_array)/sizeof(at_array[0]); ++i) {
        list_push_back(plist, &at_array[i]);
    }
    for (i = 0; i <sizeof(at_result)/sizeof(at_result[0]); ++i) {
        list_push_back(plist_result, &at_result[i]);
    }
    it_first1 = iterator_advance(list_begin(plist), 3);
    it_last1 = list_end(plist);
    it_first2 = list_begin(plist);
    it_last2 = algo_swap_ranges(it_first1, it_last1, it_first2);
    assert_true(iterator_distance(it_first1, it_last1) == iterator_distance(it_first2, it_last2));
    assert_true(list_equal(plist, plist_result));
    list_destroy(plist);
    list_destroy(plist_result);
}

/*
 * test algo_transform
 */
UT_CASE_DEFINATION(algo_transform)
void test_algo_transform__invalid_source_range(void** state)
{
    vector_t* pvec = create_vector(int);
    list_t* plist = create_list(int);

    vector_init_n(pvec, 10);
    list_init_n(plist, 10);
    expect_assert_failure(algo_transform(vector_begin(pvec), list_begin(plist), list_begin(plist), NULL));
    vector_destroy(pvec);
    list_destroy(plist);
}

void test_algo_transform__invalid_source_range2(void** state)
{
    list_t* plist = create_list(int);

    list_init_n(plist, 10);
    expect_assert_failure(algo_transform(list_end(plist), list_begin(plist), list_begin(plist), NULL));
    list_destroy(plist);
}

void test_algo_transform__invalid_source_range3(void** state)
{
    deque_t* pdeq = create_deque(int);
    slist_t* pslist = create_slist(int);
    iterator_t it;

    deque_init_n(pdeq, 10);
    slist_init_n(pslist, 10);
    it = deque_begin(pdeq);
    it._t_iteratortype = 100;
    expect_assert_failure(algo_transform(it, deque_end(pdeq), slist_begin(pslist), NULL));
    deque_destroy(pdeq);
    slist_destroy(pslist);
}

void test_algo_transform__invalid_dest_range(void** state)
{
    set_t* pset = create_set(int);
    multiset_t* pmset = create_multiset(int);
    iterator_t it;

    set_init(pset);
    multiset_init(pmset);
    it = multiset_begin(pmset);
    it._t_iteratortype = 100;
    expect_assert_failure(algo_transform(set_begin(pset), set_end(pset), it, NULL));
    set_destroy(pset);
    multiset_destroy(pmset);
}

void test_algo_transform__invalid_range_not_same_type(void** state)
{
    hash_set_t* phset = create_hash_set(int);
    hash_multiset_t* phmset = create_hash_multiset(double);

    hash_set_init(phset);
    hash_multiset_init(phmset);
    expect_assert_failure(algo_transform(hash_set_begin(phset), hash_set_end(phset), hash_multiset_begin(phmset), NULL));
    hash_set_destroy(phset);
    hash_multiset_destroy(phmset);
}

void test_algo_transform__source_range_empty(void** state)
{
    set_t* pset = create_set(int);
    multiset_t* pmset = create_multiset(int);

    set_init(pset);
    multiset_init(pmset);
    assert_true(iterator_equal(algo_transform(set_begin(pset), set_end(pset), multiset_begin(pmset), NULL), multiset_begin(pmset)));
    set_destroy(pset);
    multiset_destroy(pmset);
}

void test_algo_transform__ufun_NULL(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_t* pvec_result = create_vector(int);
    int an_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    int i = 0;
    iterator_t it;

    vector_init_copy_array(pvec, an_array, sizeof(an_array)/sizeof(an_array[0]));
    vector_init_n(pvec_result, vector_size(pvec));
    it = algo_transform(vector_begin(pvec), vector_end(pvec), vector_begin(pvec_result), NULL);
    assert_true(iterator_equal(it, vector_end(pvec_result)));
    for (i = 0; i < vector_size(pvec_result); ++i) {
        assert_true(*(int*)vector_at(pvec_result, i) == 0);
    }
    vector_destroy(pvec);
    vector_destroy(pvec_result);
}

