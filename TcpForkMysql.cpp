#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <pthread.h>

void *waiter(void *pArg) 
{ 
   int child_pid = *((int *)pArg);
   int status;
   (void)waitpid(child_pid, &status, 0);
   return NULL;
}

#include <unistd.h> /** for fork **/
#include <vector>
#include <string>
#define SERVPORT 8888 /*服务器监听端口号 */
#define BACKLOG 10 /* 最大同时连接请求数 */
std::vector<std::string> get_all_result(void);
int main(int ac,char **av)
{
	int sockfd,client_fd; /*sock_fd：监听socket；client_fd：数据传输socket */
	socklen_t sin_size;
	struct sockaddr_in my_addr; /* 本机地址信息 */
	struct sockaddr_in remote_addr; /* 客户端地址信息 */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket创建出错！"); exit(1);
	}
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind出错！");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen出错！");
		exit(1);
	}
	while(1) {
		sin_size = sizeof(struct sockaddr_in);
		if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size)) == -1) 
		{
			perror("accept出错");
			continue;
		}
		printf("received a connection from %s\n", inet_ntoa(remote_addr.sin_addr));
		int child_pid= fork();
		if (!child_pid) { /* 子进程代码段 */
			std::vector<std::string> all_result = get_all_result();
			int count = all_result.size();
			srand(time(NULL));
			printf("all_result size is %lu\n",all_result.size());
			while(1)
			{
				int get=rand()%count;
				if (send(client_fd, all_result[get].data(), all_result[get].length(), 0) == -1)
				{
					perror("send出错！");
					close(client_fd);
					exit(0);
				}
				sleep(1);
			}
		}
        else/* code of parent process */
        {
            /*TODO:wait for the new fork id */
            pthread_t hThread;
            pthread_create(&hThread, NULL, waiter, &child_pid);
            pthread_detach(hThread);
        }
		close(client_fd);
	}
}



#include <mysql.h>
#include <time.h>
std::vector<std::string> get_all_result(void)
{
	MYSQL *connection;
	MYSQL mysql;
	mysql_init(&mysql);
	std::vector<std::string> all_string;
	/*TODO: Store these database info in config file
	with list
	and try all of them
	*/
	connection = mysql_real_connect(&mysql,"localhost","root","","test",0,0,0);
	if (connection == NULL)
	{
		printf("连接失败:%s\n", mysql_error(&mysql));
		connection = mysql_real_connect(&mysql,"localhost","root","1!Gzg19841221","test",0,0,0);
		if (connection == NULL)
		{
			printf("连接失败:%s\n", mysql_error(&mysql));
		}
	}

#define GANG_QUERY_STRING "select * from test"
#define TIAN_QUERY_STRING "select * from tian"

	if (connection != NULL)
	{
		mysql_query(connection,"SET NAMES UTF8");
		printf("连接成功, 服务器版本: %s, 客户端版本: %s.\n", MYSQL_SERVER_VERSION, mysql_get_client_info());
		if(mysql_query(connection,TIAN_QUERY_STRING))
		{
			printf("查询失败:%s\n",mysql_error(connection));
			return all_string;
		}
		MYSQL_RES *result = mysql_store_result(connection);
		MYSQL_ROW row;
		while(1)
		{
			row = (MYSQL_ROW)mysql_fetch_row(result);
			if(!row)
				break;
			char out_string[1024];
			snprintf(out_string,1024,"%s\n",row[0]);
			printf("制造内容:%s\n",row[0]);
			all_string.push_back(out_string);
		}
		mysql_free_result(result);
		mysql_close(connection);
	}
	mysql_close(&mysql);
	return all_string;
}

