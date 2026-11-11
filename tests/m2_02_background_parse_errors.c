#include <sunit.h>
#include <msh_parse.h>

#include <string.h>

sunit_ret_t
mult_background(void)
{
	struct msh_sequence *s;
	msh_err_t ret;

	s = msh_sequence_alloc();
	SUNIT_ASSERT("sequence allocation", s != NULL);
	ret = msh_sequence_parse("hello | world & &", s);
	SUNIT_ASSERT("Two & in a pipeline", ret == MSH_ERR_MISUSED_BACKGROUND);
	msh_sequence_free(s);

	return SUNIT_SUCCESS;
}

sunit_ret_t
premature_background(void)
{
	struct msh_sequence *s;
	msh_err_t ret;

	s = msh_sequence_alloc();
	SUNIT_ASSERT("sequence allocation", s != NULL);
	ret = msh_sequence_parse("hello & | world", s);
	SUNIT_ASSERT("background & specification before end of pipeline", ret == MSH_ERR_MISUSED_BACKGROUND && s != NULL);
	msh_sequence_free(s);

	return SUNIT_SUCCESS;
}

int
main(void)
{
	struct sunit_test tests[] = {
		SUNIT_TEST("Multiple background & specification error", mult_background),
		SUNIT_TEST("Background & before end of pipeline error", premature_background),
		SUNIT_TEST_TERM
	};

	sunit_execute("background specification errors", tests);

	return 0;
}
