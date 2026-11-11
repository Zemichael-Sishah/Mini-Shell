#include <sunit.h>
#include <msh_parse.h>

#include <string.h>

sunit_ret_t
two_cmd_two_redir(void)
{
	struct msh_sequence *s;
	struct msh_pipeline *p;
	struct msh_command *c;
	msh_err_t ret;
	char *stdout_file, *stderr_file;

	s = msh_sequence_alloc();
	SUNIT_ASSERT("sequence allocation", s != NULL);
	ret = msh_sequence_parse("hello 2>> err.txt | world 2>> err2.txt 1>> out.txt ", s);
	SUNIT_ASSERT("hello cmd parsed", ret == 0 && s != NULL);
	p = msh_sequence_pipeline(s);
	SUNIT_ASSERT("hello pipeline", p != NULL);
	c = msh_pipeline_command(p, 0);
	SUNIT_ASSERT("hello command", c != NULL);
	SUNIT_ASSERT("hello command is not final", !msh_command_final(c));
	msh_command_file_outputs(c, &stdout_file, &stderr_file);
	SUNIT_ASSERT("hello command stdout redir", stdout_file == NULL);
	SUNIT_ASSERT("hello command stderr redir", strcmp(stderr_file, "err.txt") == 0);

	c = msh_pipeline_command(p, 1);
	SUNIT_ASSERT("world command", c != NULL);
	SUNIT_ASSERT("world command is final", msh_command_final(c));
	msh_command_file_outputs(c, &stdout_file, &stderr_file);
	SUNIT_ASSERT("world command stdout redir", strcmp(stdout_file, "out.txt") == 0);
	SUNIT_ASSERT("world command stderr redir", strcmp(stderr_file, "err2.txt") == 0);

	msh_pipeline_free(p);
	msh_sequence_free(s);

	return SUNIT_SUCCESS;
}

sunit_ret_t
one_cmd_two_redir(void)
{
	struct msh_sequence *s;
	struct msh_pipeline *p;
	struct msh_command *c;
	msh_err_t ret;
	char *stdout_file, *stderr_file;

	s = msh_sequence_alloc();
	SUNIT_ASSERT("sequence allocation", s != NULL);
	ret = msh_sequence_parse("hello 2>> err.txt 1>> out.txt ", s);
	SUNIT_ASSERT("hello cmd parsed", ret == 0 && s != NULL);
	p = msh_sequence_pipeline(s);
	SUNIT_ASSERT("hello pipeline", p != NULL);
	c = msh_pipeline_command(p, 0);
	SUNIT_ASSERT("hello command", c != NULL);
	SUNIT_ASSERT("hello command is final", msh_command_final(c));

	msh_command_file_outputs(c, &stdout_file, &stderr_file);
	SUNIT_ASSERT("hello command stdout redir", strcmp(stdout_file, "out.txt") == 0);
	SUNIT_ASSERT("hello command stderr redir", strcmp(stderr_file, "err.txt") == 0);

	msh_pipeline_free(p);
	msh_sequence_free(s);

	return SUNIT_SUCCESS;
}

sunit_ret_t
one_cmd_one_redir(void)
{
	struct msh_sequence *s;
	struct msh_pipeline *p;
	struct msh_command *c;
	msh_err_t ret;
	char *stdout_file, *stderr_file;

	s = msh_sequence_alloc();
	SUNIT_ASSERT("sequence allocation", s != NULL);
	ret = msh_sequence_parse("hello 1>> out.txt ", s);
	SUNIT_ASSERT("hello cmd parsed", ret == 0 && s != NULL);
	p = msh_sequence_pipeline(s);
	SUNIT_ASSERT("hello pipeline", p != NULL);
	c = msh_pipeline_command(p, 0);
	SUNIT_ASSERT("hello command", c != NULL);
	SUNIT_ASSERT("hello command is final", msh_command_final(c));

	msh_command_file_outputs(c, &stdout_file, &stderr_file);
	SUNIT_ASSERT("hello command stdout redir", strcmp(stdout_file, "out.txt") == 0);
	SUNIT_ASSERT("hello command stderr redir", stderr_file == NULL);

	msh_pipeline_free(p);
	msh_sequence_free(s);

	return SUNIT_SUCCESS;
}

int
main(void)
{
	struct sunit_test tests[] = {
		SUNIT_TEST("one command, one redirection", one_cmd_one_redir),
		SUNIT_TEST("one command, two redirections", one_cmd_two_redir),
		SUNIT_TEST("two commands, two redirections", two_cmd_two_redir),
		SUNIT_TEST_TERM
	};

	sunit_execute("Redirection parsing.", tests);

	return 0;
}
