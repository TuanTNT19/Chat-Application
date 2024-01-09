#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

//#define clear() printf("\033[H\033[J") 
#define MAX_BACKLOG 10


pthread_t Accep_Thread_id;
char IP[100];
char command[70];
char command_option[10];
char temp_str[70];

typedef struct {
    int id;
    int fd;
    int port_num;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char my_ip[50];
} device;

device this_device ={0};
device device_connect[MAX_BACKLOG] = {0};
int total_device;

static void *Accep_Thread(void *para)
{
    int client_fd;
    struct sockaddr_in cli_addr;
    socklen_t len = sizeof(cli_addr);

    while(1){
        

        client_fd = accept(this_device.fd, (struct sockaddr *)&cli_addr, &len);
        if (client_fd == -1)
        {
            printf("ERROR: Can not accept new device\n");
            return ;
        }

    device_connect[total_device].fd = client_fd;
    device_connect[total_device].id = total_device;
    device_connect[total_device].addr = cli_addr;

    device_connect[total_device].port_num = ntons(cli_addr.sin_port);
    device_connect[total_device].addrlen = len ;
    inet_ntop(AF_INET, &cli_addr.sin_addr, device_connect[total_device].my_ip);

    printf("Accept a new connection from IP addreass: %s, setup at port: %d\n", device_connect[total_device].my_ip, device_connect[total_device].port_num);
    
    
    
    }
}

void print_myPort()
{
    printf("My port is: %d\n", this_device.port_num);
}

void list_peer()
{  
    printf("ID |        IP Address         | Port No.\n");
    
    for (int i =0; i< total_device; i++)
    {
        printf("%d |    %s          | %d\n", device_connect[i].id, device_connect[i].my_ip, device_connect[i].port_num);
    }
}

void print_myIP(char *ip_add)
{
    
    FILE *fd = popen("hostname -I", "r");
    
    if (fd == NULL)
    {
        printf("ERROR: Can not get IP address\n");
        return ;
    }

    if (fgets(ip_add, 100, fd) == NULL)
    {
        printf("ERROR: Can not get IP address\n");
        return;
    }

    printf("My IP is : %s", ip_add);
}

void *process_chosen(char *str, char *result)
{
    //char result[10];
    int i = 0;
     char *token = strtok(str," ");

     while(token != NULL)
     {
        strcpy(result, token);
        token = strtok(NULL, " ");
        break;
     }
}

int send_to(int client_id, char *mes)
{
    for (int i = 0; i < total_device; i++)
    if (client_id == device_connect[i].id)
    {
        if (write(device_connect[i].fd, mes, sizeof(mes)) == -1)
        {
            //printf("ERROR: Can not send message\n");
            return 0;
        }
        
    }
    return 1;
}

int main(int argc, char *argv[]){

     system("clear");
    //create socket for this device
    this_device.fd = socket(AF_INET, SOCK_STREAM, 0);
 
    if (this_device.fd == -1)
    {
        printf("ERROR: Can not create socket for this device\n");
        return 0;
    }
    
    this_device.port_num = atoi(argv[1]);

    this_device.addr.sin_family = AF_INET;
    this_device.addr.sin_port = htons(this_device.port_num);
    this_device.addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(this_device.fd, (struct sockaddr*)&this_device.addr, sizeof(this_device.addr))==-1)
    {
        printf("ERROR: Can not make bind for this socket\n");
        return 0;
    }

    if (listen(this_device.fd, MAX_BACKLOG) == -1)
    {
        printf("ERROR: Can not listen for this device\n");
        return 0;
    }

    printf("Listening on port : %d\n", this_device.port_num);

    if (pthread_create(&Accep_Thread_id, NULL, &Accep_Thread, NULL)){
        printf("ERROR: Can not create thread for accept new device\n");
        return 0;
    }

    while(1)
    {
        printf("Enter your command:  ");
        fgets(command, 70, stdin);
        command[strcspn(command, "\n")] = '\0';

        strcpy(temp_str, command);

        process_chosen(temp_str, command_option);

        if (!strcmp(command_option,"send"))
        {
            printf("ok send\n");
            char mes[50];
            int id;

            if (!send_to(id, mes))
            {
                printf("ERROR: Can not send messgage to port %d at id %d\n", this_device.port_num, id);
            }
        }

        else if (!strcmp(command_option, "myip"))
        {
            print_myIP(this_device.my_ip);
        }

        else if (!strcmp(command_option, "myport")){
            print_myPort();

        }

        else if (!strcmp(command_option,"list"))
        {
            printf("Check command list ok\n");
        }

    }

    return 0;
}