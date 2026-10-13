/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{   rb_init(4);
    zassert_true(rb_is_empty(),"RE-init buffer must be empty");
	zassert_equal(rb_count(),0,"re init buffer count must be 0");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */
	rb_push(42);
	uint32_t v;
	rb_pop(&v);
	zassert_equal(v,42,"pop value must be 42");
	zassert_true(rb_is_empty(),"buffer must be empty after pop");

}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */
	rb_push(1);
	rb_push(2);
	rb_push(3);
	int v1;
	rb_pop(&v1);
	zassert_equal(v1,1,"pop value must be 1");
	rb_pop(&v1);
	zassert_equal(v1,2,"pop value must be 1");
	rb_pop(&v1);
	zassert_equal(v1,3,"pop value must be 1");

}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */
	rb_push(1);
	rb_push(2);
    rb_push(3);
	rb_push(4);
    zassert_equal(rb_is_full(),true,"after pushing 4 values it should be full");
	zassert_equal(rb_push(34),-ENOSPC,"IT SHOULD RETURN ERROR AFTER PUSHING EXTRA ELEMENT");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	/* TODO(l8-task1): rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */
	rb_push(7);
	int v;
	rb_peek(&v);
	zassert_equal(v,7,"peeking value should be same as pushed value");
	rb_peek(&v);
    zassert_equal(v,7,"peeking value should be same as pushed value");


}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/*
	  TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */
	zassert_equal(rb_pop(NULL),-EINVAL,"ERROR SHOULD BE THERE");

	
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
	rb_push(1);
	rb_push(1);
	rb_push(1);
	rb_push(1);

	zassert_equal(rb_is_full(),true,"the queue should be full");
	zassert_equal(rb_count(),4,"the count should be equal to the number of push");

}
