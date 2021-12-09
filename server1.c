#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include <signal.h>
#include <setjmp.h>
 
//the thread function
void *connection_handler(void *);

struct user{
    char username[10];
    char password[10];
    int login_status;
    pthread_t tid;
    char playwith[10];
    int game_status;  //0:初始 1:發送邀請 2:回覆yea 3:回覆no 4:遊戲中 5:書了 6:平手 
    int playernum;
};
int turn=1;
/*struct login_information{
    int login_status;
    pthread_t tid;
};*/
 /*void handler(int signo)
    {
         pthread_t current_tid
        printf("aa");
        fflush(stdout);
        if(signo==SIGUSR1)
        {
            printf("bb");
            fflush(stdout);
            current_tid=pthread_self();
            //printf("%d",usernumber);
            //printf("cc");
        }
    }*/
//my_s_t.name = "John";
struct user userrecord[4];
struct user current_user;
char chessboard[10]={"          "};
//struct login_information login_info[4];

/*user[0]={ 
                    .username = "aaaa\0",
                  .password = "aaaa\0",
                  };
user[1]={ 
                    .username = "bbbb\0",
                  .password = "b\0",
                  };*/
/*user user2 = {{'b', 'b', 'b', 'b', '\0'} ,{'b', 'b', 'b', 'b', '\0'}};
user user3 = {{'c', 'c', 'c', 'c', '\0'} ,{'c', 'c', 'c', 'c', '\0'}};
user user4 = {{'d', 'd', 'd', 'd', '\0'} ,{'d', 'd', 'd', 'd', '\0'}};*/
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
    printf("%s",chessboard);
    fflush(stdout);
    strcpy((userrecord[0].username),"aaaa");
    strcpy((userrecord[0].password),"aaaa");
     strcpy((userrecord[1].username),"bbbb");
    strcpy((userrecord[1].password),"bbbb");
     strcpy((userrecord[2].username),"cccc");
    strcpy((userrecord[2].password),"cccc");
     strcpy((userrecord[3].username),"dddd");
    strcpy((userrecord[3].password),"dddd");
    int i=0;
    for(i=0;i<4;i++)
    {
        userrecord[i].login_status=0;
        userrecord[i].tid=0;
        userrecord[i].game_status=0;
        //printf("%s %s\n",userrecord[i].username,userrecord[i].password);
    }

    /*if(signal(SIGUSR1, handler) == SIG_ERR) 
	{ //設定出錯
        perror("Can't set handler for SIGUSR1\n");
        exit(1);
    }*/
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8080 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
	pthread_t thread_id;
	
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
 /*void handler(int signo)
    {
        printf("aa");
        if(signo==SIGUSR1)
        {
            printf("bb");
            //printf("%d",usernumber);
            printf("cc");
        }
    }*/
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    /*printf("Aww");
    fflush(stdout);*/
    static __thread int sock ;
    sock = *(int*)socket_desc;
    int read_size;
    static __thread char *message , client_message[2000];
    int i=0;
    static __thread char temp[100];
    static __thread int usernumber=0;
    static __thread int pos=0;
    static __thread int judge_result=0;
    static __thread jmp_buf jmpbuffer;

    int judge()
    {
        if((chessboard[1]=='o')&&(chessboard[2]=='o')&&(chessboard[3]=='o'))
        {
            return 1;
        }
        else if((chessboard[4]=='o')&&(chessboard[5]=='o')&&(chessboard[6]=='o'))
        {
            return 1;
        }
         else if((chessboard[7]=='o')&&(chessboard[8]=='o')&&(chessboard[9]=='o'))
        {
            return 1;
        }
         else if((chessboard[1]=='o')&&(chessboard[4]=='o')&&(chessboard[7]=='o'))
        {
            return 1;
        }
        else if((chessboard[2]=='o')&&(chessboard[5]=='o')&&(chessboard[8]=='o'))
        {
            return 1;
        }
        else if((chessboard[3]=='o')&&(chessboard[6]=='o')&&(chessboard[9]=='o'))
        {
            return 1;
        }
         else if((chessboard[1]=='o')&&(chessboard[5]=='o')&&(chessboard[9]=='o'))
        {
            return 1;
        }
        else if((chessboard[3]=='o')&&(chessboard[5]=='o')&&(chessboard[7]=='o'))
        {
            return 1;
        }
        else if((chessboard[1]=='x')&&(chessboard[2]=='x')&&(chessboard[3]=='x'))     //player2 win
        {
            return 2;
        }
        else if((chessboard[4]=='x')&&(chessboard[5]=='x')&&(chessboard[6]=='x'))
        {
            return 2;
        }
        else if((chessboard[7]=='x')&&(chessboard[8]=='x')&&(chessboard[9]=='x'))
        {
            return 2;
        }
         else if((chessboard[1]=='x')&&(chessboard[4]=='x')&&(chessboard[7]=='x'))
        {
            return 2;
        }
        else if((chessboard[2]=='x')&&(chessboard[5]=='x')&&(chessboard[8]=='x'))
        {
            return 2;
        }
        else if((chessboard[3]=='x')&&(chessboard[6]=='x')&&(chessboard[9]=='x'))
        {
            return 2;
        }
         else if((chessboard[1]=='x')&&(chessboard[5]=='x')&&(chessboard[9]=='x'))
        {
            return 2;
        }
        else if((chessboard[3]=='x')&&(chessboard[5]=='x')&&(chessboard[7]=='x'))
        {
            return 2;
        }
        else if((chessboard[1]!=' ')&&(chessboard[2]!=' ')&&(chessboard[3]!=' ')&&(chessboard[4]!=' ')&&(chessboard[5]!=' ')
        &&(chessboard[6]!=' ')&&(chessboard[7]!=' ')&&(chessboard[8]!=' ')&&(chessboard[9]!=' '))
        {
            return 3;
        }
        else
        {
            return 0;
        }
        

    }
    int draw(int judge_result)
    {
        int i=0;
        for(i=1;i<=9;i++)
        {
            //printf("%c | %c | %c\n",chessboard[i],chessboard[i+1],chessboard[i+2]);
            strncat(client_message,&chessboard[i],1);
            strcat(client_message,"|");
            if(i%3==0)
            {
                strcat(client_message,"\n-------\n");
                //printf("-----\n");
            }
            /*printf("%s",client_message);
            fflush(stdout);*/
        }
         /*printf("%s\n\n\n\n",client_message);
         fflush(stdout);*/
        if(judge_result==1)
        {
            if(userrecord[usernumber].game_status==6)
            {
                strncpy(chessboard,"             ",10);
            }
            strcat(client_message,"you win\n");
            send(sock,client_message, strlen(client_message), 0);
            userrecord[usernumber].game_status=0;
            longjmp(jmpbuffer, 1);
        }
        if(judge_result==2)
        {
            if(userrecord[usernumber].game_status==5)
            {
                strncpy(chessboard,"             ",10);
            }
            strcat(client_message,"you lose\n");
            send(sock,client_message, strlen(client_message), 0);
            userrecord[usernumber].game_status=0;
            longjmp(jmpbuffer, 1);
        }
        if(judge_result==3)
        {
           if(userrecord[usernumber].game_status==6)
            {
                strncpy(chessboard,"             ",10);
            }
            strcat(client_message,"tie\n");
            send(sock,client_message, strlen(client_message), 0);
            userrecord[usernumber].game_status=0;
            longjmp(jmpbuffer, 1);
        }
        send(sock,client_message, strlen(client_message), 0);
    }
    void game()
    {
        printf("%s\n",userrecord[usernumber].username);
        fflush(stdout);
        while(1)
        {
            if(turn==userrecord[usernumber].playernum)
            {
                if(userrecord[usernumber].game_status==5)
                {
                    draw(2);
                }
                if(userrecord[usernumber].game_status==6)
                {
                    draw(3);
                }
                //send(sock ,"your turn", strlen("your turn"), 0);
                strcpy(client_message,"your turn\n");
                draw(0);
                while(1)
                {
                    if(userrecord[usernumber].playernum==1)
                    {

                        recv(sock , client_message , 2000 , 0);
                        if((client_message[0]<=57)&&(client_message[0]>=49))
                        {
                            pos=atoi(client_message);
                            //printf("%d\n",pos);
                            printf("%s\n",chessboard);
                            fflush(stdout);
                            if((chessboard[pos]==' '))
                            {   
                                chessboard[pos]='o';
                            

                            judge_result=judge();
                            printf("%d\n",judge_result);
                            if(judge_result==1)      //win
                            {

                                 for(i=0;i<4;i++)
                                {
                                    /*printf("qqq");
                                    fflush(stdout);*/
                                    if(strcmp(userrecord[usernumber].playwith,userrecord[i].username)==0)
                                    {
                                        printf("%s %ld\n",userrecord[i].username,userrecord[i].tid);
                                        fflush(stdout);
                                        /*strcpy(userrecord[i].playwith,userrecord[usernumber].username);
                                        strcpy(userrecord[usernumber].playwith,userrecord[i].username);*/
                                        userrecord[i].game_status=5;
                                        /*if(pthread_kill(userrecord[i].tid,SIGUSR1)==-1)
                                        {
                                            printf("send signal error");
                                        }*/
                                    }
                                }
                                 turn=2;
                                draw(1);
                            }
                            /*if(judge_result==2)
                            {
                                draw(2);
                            }*/
                             if(judge_result==3)
                            {
                                turn=2;
                               for(i=0;i<4;i++)
                                {
                                    printf("qqq");
                                    fflush(stdout);
                                    if(strcmp(userrecord[usernumber].playwith,userrecord[i].username)==0)
                                    {
                                        printf("%s %ld\n",userrecord[i].username,userrecord[i].tid);
                                        fflush(stdout);
                                        /*strcpy(userrecord[i].playwith,userrecord[usernumber].username);
                                        strcpy(userrecord[usernumber].playwith,userrecord[i].username);*/
                                        userrecord[i].game_status=6;
                                        /*if(pthread_kill(userrecord[i].tid,SIGUSR1)==-1)
                                        {
                                            printf("send signal error");
                                        }*/
                                    }
                                }
                                 turn=2;
                                draw(3);
                            }
                            /*printf("%s\n",chessboard);
                            fflush(stdout);*/
                            turn=2;
                            break;
                            }
                             else
                            {
                                send(sock ,"非法輸入", strlen("非法輸入"), 0);
                            }
                        }
                        else
                        {
                            send(sock ,"請輸入1~9", strlen("請輸入1~9"), 0);
                        }
                    }
                    else if(userrecord[usernumber].playernum==2)
                    {
                        recv(sock , client_message , 2000 , 0);
                        if((client_message[0]<=57)&&(client_message[0]>=49))
                        {
                            pos=atoi(client_message);
                            if((chessboard[pos]==' '))
                            {
                                chessboard[pos]='x';
                                judge_result=judge();
                                if(judge_result==2)
                                {
                                      for(i=0;i<4;i++)
                                    {
                                        /*printf("qqq");
                                        fflush(stdout);*/
                                        if(strcmp(userrecord[usernumber].playwith,userrecord[i].username)==0)
                                        {
                                            printf("%s %ld\n",userrecord[i].username,userrecord[i].tid);
                                            fflush(stdout);
                                            /*strcpy(userrecord[i].playwith,userrecord[usernumber].username);
                                            strcpy(userrecord[usernumber].playwith,userrecord[i].username);*/
                                            userrecord[i].game_status=5;
                                            /*if(pthread_kill(userrecord[i].tid,SIGUSR1)==-1)
                                            {
                                                printf("send signal error");
                                            }*/
                                        }
                                    }
                                     turn=1;
                                    draw(1);
                                     
                                }
                                /*if(judge_result==2)
                                {
                                    draw(1);
                                }*/
                                if(judge_result==3)
                                {
                                   
                                      for(i=0;i<4;i++)
                                    {
                                        /*printf("qqq");
                                        fflush(stdout);*/
                                        if(strcmp(userrecord[usernumber].playwith,userrecord[i].username)==0)
                                        {
                                            printf("%s %ld\n",userrecord[i].username,userrecord[i].tid);
                                            fflush(stdout);
                                            /*strcpy(userrecord[i].playwith,userrecord[usernumber].username);
                                            strcpy(userrecord[usernumber].playwith,userrecord[i].username);*/
                                            userrecord[i].game_status=6;
                                            /*if(pthread_kill(userrecord[i].tid,SIGUSR1)==-1)
                                            {
                                                printf("send signal error");
                                            }*/
                                        }
                                    }
                                     turn=1;
                                    draw(3);
                                }
                                printf("%s\n",chessboard);
                                fflush(stdout);
                                turn=1;
                                break;
                            }
                            else
                            {
                                send(sock ,"非法輸入", strlen("非法輸入"), 0);
                            }
                        }
                        else
                        {
                            send(sock ,"請輸入1~9", strlen("請輸入1~9"), 0);
                        }
                    }
                }
            }
        }
    }
    //signal(SIGUSR1, handler);
void handler(int signo)
    {
        //printf("aa");
        fflush;
        char mess[100];
        int i=0;
        int thread_user=0;
        if(signo==SIGUSR1)
        {
            /*printf("%ld\n",pthread_self());
            fflush(stdout);
            printf("%d",usernumber);
            fflush(stdout);*/
            /*for(i=0;i<4;i++)
            {
                if(userrecord[i].tid==pthread_self())
                {
                    /*strcpy(mess,userrecord[i].playwith);
                    strcat(mess," want play with you");
                    break;*/
                    /*thread_user=i;
                }
            }*/
            if(userrecord[usernumber].game_status==0)      // 接收邀請
            {
                strcpy(mess,userrecord[usernumber].playwith);
                strcat(mess," want play with you");
                if((send(sock ,mess, strlen(mess), 0))<0)
                {
                perror("send error:");
                //fflush(stdout);
                }
                recv(sock , client_message , 2000 , 0);
                //printf("%sww",client_message);
                fflush(stdout);
                while(1)
                {
                    recv(sock , client_message , 2000 , 0);
                   //printf("%sww",client_message);
                    if(strncmp(client_message,"yes",3)==0)
                    {
                        //printf("qq");
                        fflush(stdout);
                        //send(sock ,"game start", strlen("game start"), 0);
                        userrecord[usernumber].game_status=4;
                        userrecord[usernumber].playernum=2;
                        for(i=0;i<4;i++)
                        {
                            if(strcmp(userrecord[usernumber].playwith,userrecord[i].username)==0)
                            {
                                userrecord[i].game_status=2;
                                userrecord[usernumber].playernum=2;
                                pthread_kill(userrecord[i].tid,SIGUSR1);
                                game();
                            }
                        }
                    }
                    else if((strncmp(client_message,"no",2))==0)             //拒絕邀請
                    {
                        userrecord[usernumber].game_status=0;
                        send(sock ,"\n", 1, 0);
                        for(i=0;i<4;i++)
                        {
                            if(strcmp(userrecord[usernumber].playwith,userrecord[i].username)==0)
                            {
                                userrecord[i].game_status=3;
                                pthread_kill(userrecord[i].tid,SIGUSR1);
                                //goto game();
                            }
                        }
                    }
                    else
                    {
                        send(sock ,"請輸入yes或no", strlen("請輸入yes或no"), 0);
                    }
                }
            }
            else if(userrecord[usernumber].game_status==2)     //回覆yes
            {
                //send(sock ,"game start", strlen("game start"), 0);
                userrecord[usernumber].playernum=1;
                userrecord[usernumber].game_status==4;
                game();
            }
            else if(userrecord[usernumber].game_status==3)     //回覆no
            {
                send(sock ,"refuse", strlen("refuse"), 0);
                userrecord[usernumber].game_status=0;
            }
            else if((userrecord[usernumber].game_status==5))
            {
                printf("lose");
                fflush(stdout);
                draw(2);
                userrecord[usernumber].game_status=0;
            }
            else if((userrecord[usernumber].game_status==6))
            {
                draw(3);
                userrecord[usernumber].game_status=0;
            }


            //strcpy(mess,"someone want play with you");
            /*if((send(sock ,mess, strlen(mess), 0))<0)
            {
                perror("send error:");
                //fflush(stdout);
            }*/
            /*printf("bb");
            fflush(stdout);*/
        }
    }

    signal(SIGUSR1, handler);

    int login_success=0;
    static __thread int jmpVal ;
     jmpVal = setjmp(jmpbuffer);
     int count=0;
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        printf("%s",client_message);
        fflush(stdout);
        //end of string marker
		//client_message[read_size+1] = '\0';
		if((login_success==0)&&strncmp(client_message,"login",5)==0)
        {
            for(i=0;i<4;i++)
            {
                if((strncmp(&client_message[6],userrecord[i].username,4)==0)&&(strncmp(&client_message[11],userrecord[i].password,4)==0))
                {
                    //write(sock , "login successful", strlen("login successful"));
                    /*if( send(sock ,"login successful", strlen("login successful"), 0) < 0)
                    {
                        puts("Send failed");
                    }*/
                    login_success=1;
                    userrecord[i].login_status=1;
                    userrecord[i]. tid=pthread_self();
                    usernumber=i;
                    //current_user=userrecord[i];
                    break;
                }
                /*else
                {
                    //write(sock , "login fail", strlen("login fail"));
                    if( send(sock ,"login fail", strlen("login fail"), 0) < 0)
                    {
                        puts("Send failed");
			        
                    }
                    else
                    {
                        printf("ww");
                        fflush(stdout);
                    }
                }*/
            }
            if(login_success==1)                     //登入成功  
            {
                 if( send(sock ,"login successful\0", strlen("login successful")+1, 0) < 0)
                    {
                        puts("Send failed");
                    }
                    memset(client_message, 0, 2000);
                //login_success=0;
            }
            else
            {
                 if( send(sock ,"login fail\0", strlen("login fail")+1, 0) < 0)
                    {
                        puts("Send failed");
                    }
                memset(client_message, 0, 2000);
            }

        }
        else if(login_success==1&&(strncmp(client_message,"user",4)==0))
        {
                for(i=0;i<4;i++)
                {
                    /*printf("qqq");
                    fflush(stdout);*/
                    if(userrecord[i].login_status==1)
                    {
                        /*printf("wwww");
                        fflush(stdout);*/
                        strcat(temp,userrecord[i].username);
                        strcat(temp,"\n");
                        //send(sock ,temp, strlen(temp), 0);
                        
                        //send(sock ,"\n", 1, 0);
                        //printf("%s %ld\n\n",userrecord[i].username,userrecord[i].tid);
                        fflush(stdout);
                    }
                }
                 send(sock ,temp, strlen(temp), 0);
                memset(temp,0,100);
        }
        else if((login_success==1)&&(strncmp(client_message,"play",4)==0))
        {
                for(i=0;i<4;i++)
                {
                    /*printf("qqq");
                    fflush(stdout);*/
                    if((strncmp(&client_message[5],userrecord[i].username,4)==0)&&(userrecord[i].login_status==1))
                    {
                        count++;
                        printf("%s %ld\n",userrecord[i].username,userrecord[i].tid);
                        fflush(stdout);
                        strcpy(userrecord[i].playwith,userrecord[usernumber].username);
                        strcpy(userrecord[usernumber].playwith,userrecord[i].username);
                        userrecord[usernumber].game_status=1;
                        if(pthread_kill(userrecord[i].tid,SIGUSR1)==-1)
                        {
                            printf("send signal error");
                        }
                    }
                }
                if(count==0)
                {
                    send(sock ,"不存在的user", strlen("不存在的user"), 0);
                }
                count=0;
        }
        else
        {
            send(sock ,"\n", 1, 0);
        }
		//Send the message back to client
        //write(sock , client_message , strlen(client_message));
        //send(sock ,"w", 1, 0);
        /*printf("Aqq");
        fflush(stdout);*/
		//clear the message buffer
		memset(client_message, 0, 2000);
    }
    //send(sock ," ", 1, 0);
    if(read_size == 0)
    {
        puts("Client disconnected");
        userrecord[usernumber].login_status=0;
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    return 0;
} 