#include "../Tomweb.hpp"

int main()
{
    std::string aREQUEST_METHOD = "REQUEST_METHOD=GET";
	std::string aQUERY_STRING = "QUERY_STRING=name=qdo+tri&age=10";
	std::string aCONTENT_TYPE = "CONTENT_TYPE=";
	std::string aCONTENT_LENGTH = "CONTENT_LENGTH=0";
	std::string aHTTP_COOKIE = "HTTP_COOKIE=./cgi-bin/process_form_cgi.out=8; cgi-bin/process_form_cgi.out=13; obritt_cgi=20";
	std::string aSCRIPT_NAME = "SCRIPT_NAME=./process_form_cgi.out";
	std::string aREMOTE_ADD = "REMOTE_ADDR=127.0.0.1";

	char *env[8];
	char env0[aREQUEST_METHOD.size() + 1]; std::strncpy(env0, aREQUEST_METHOD.c_str(), aREQUEST_METHOD.size()); env0[aREQUEST_METHOD.size()] = 0;
	char env1[aQUERY_STRING.size() + 1]; std::strncpy(env1, aQUERY_STRING.c_str(), aQUERY_STRING.size()); env1[aQUERY_STRING.size()] = 0;
	char env2[aCONTENT_TYPE.size() + 1]; std::strncpy(env2, aCONTENT_TYPE.c_str(), aCONTENT_TYPE.size()); env2[aCONTENT_TYPE.size()] = 0;
	char env3[aCONTENT_LENGTH.size() + 1]; std::strncpy(env3, aCONTENT_LENGTH.c_str(), aCONTENT_LENGTH.size()); env3[aCONTENT_LENGTH.size()] = 0;
	char env4[aHTTP_COOKIE.size() + 1]; std::strncpy(env4, aHTTP_COOKIE.c_str(), aHTTP_COOKIE.size()); env4[aHTTP_COOKIE.size()] = 0;
	char env5[aSCRIPT_NAME.size() + 1]; std::strncpy(env5, aSCRIPT_NAME.c_str(), aSCRIPT_NAME.size()); env5[aSCRIPT_NAME.size()] = 0;
	char env6[aREMOTE_ADD.size() + 1]; std::strncpy(env6, aREMOTE_ADD.c_str(), aREMOTE_ADD.size()); env6[aREMOTE_ADD.size()] = 0;

	env[0] = env0;
	env[1] = env1;
	env[2] = env2;
	env[3] = env3;
	env[4] = env4;
	env[5] = env5;
	env[6] = env6;
	env[7] = NULL;


	char a0[] = "./process_form_cgi.out";
	char *a[2];
	a[0] = a0;
	a[1] = NULL;
    close(0);
	execve(a0, a, env);
	perror("execve");
}