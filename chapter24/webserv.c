#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
#define BUF_SIZE 1024
#define SMALL_BUF 100

void *request_handler(void *arg);
char *content_type(char *file);
void send_data(FILE *fp, char *ct, char *file_name);
void send_error(FILE *fp);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_adr, clnt_adr;
    pthread_t t_id;

    if(argc != 2)
    {
        puts("argc error: <port>");
        return -1;
    }
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));


    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        puts("bind error");
        return -1;
    }
    if(listen(serv_sock, 5) == -1)
    {
        puts("listen error");
        return -1;
    }

    while(1)
    {
        socklen_t clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        printf("connection request: %s:%d\n",
        inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
        pthread_create(&t_id, NULL, request_handler, &clnt_sock);
        pthread_detach(t_id);
    }
    close(serv_sock);
    return 0;
}

void *request_handler(void *arg)
{
    int clnt_sock = *(int*)arg;
    char req_line[SMALL_BUF];//请求命令
    char method[10];//获取方法，如get、post
    char ct[15];//网页类型，如html网页、空白页
    char file_name[20];//网页名

    FILE *clnt_read, *clnt_write;
    clnt_read = fdopen(clnt_sock, "r");
	clnt_write = fdopen(dup(clnt_sock), "w"); 
    fgets(req_line, SMALL_BUF, clnt_read);	

    if(strstr(req_line, "HTTP/") == NULL)//不是http请求
    {
        puts("http error");
        send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
        return NULL;
    }

    strcpy(method, strtok(req_line, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    strcpy(ct, content_type(file_name));
    if(strcmp(method, "GET"))//只处理get方法
    {
        puts("method get error");
        send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
        return NULL;
    }
	fclose(clnt_read);
    send_data(clnt_write, ct, file_name);
}
char *content_type(char *file)
{
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    strcpy(file_name, file);

    strtok(file_name, ".");//去除html的前部份，如index.html的index
    strcpy(extension, strtok(NULL, "."));//得到扩展名，如html
    if(!strcmp(extension, "html") || !strcmp(extension, "htm"))
    {
        return "text/html";
    }
    else
    {
        return "text/plain";
    }
}
void send_data(FILE *fp, char *ct, char *file_name)
{
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char server[] = "Server:Linux Web Server\r\n";
    char cnt_len[] = "Content-length:2048\r\n";    
    char cnt_type[SMALL_BUF];
    char buf[SMALL_BUF];

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    FILE *snd_file = fopen(file_name, "r");//打开请求的文件
    if(snd_file == NULL)
    {
        puts("open snd_file error");
        send_error(fp);
        return;
    }
    //传输网页信息
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    
    while(fgets(buf, BUF_SIZE, snd_file) != NULL)
    {
        fputs(buf, fp);
		fflush(fp);
    }
	fflush(fp);

    fclose(fp);
    fclose(snd_file);
}
void send_error(FILE *fp)
{
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server\r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\n\r\n";
    //char content[]

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
	fflush(fp);
}