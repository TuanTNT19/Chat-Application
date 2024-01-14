#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>

//#define clear() printf("\033[H\033[J") 
#define MAX_BACKLOG 10


pthread_t Accep_Thread_id, Recei_Thread_id;
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
device device_connect[MAX_BACKLOG] ;
int total_device = 0;

void sig_handler()
{
    printf("***********************************************************\n");
    printf("-------------------Turn off quickly------------------------\n");
    printf("************************************************************\n");

    exit(0);
}

static void* receive_from(void *para)
{
    device *devicet = (device *)para;
    char buff_r[50];

    while(1)
    {

    if (read(devicet->fd, buff_r, 50) < 0)
    {
        printf("ERROR: Can not read data\n");
        return ;
    }

    printf("** Message receive from: %s\n", devicet->my_ip);
    printf("** Sender Port:          %d\n",devicet->port_num);
    printf("-> Message:              %s\n", buff_r);
    }


}

int connect_to(device dev)
{


    if (connect(dev.fd, (struct sockaddr*)&dev.addr, sizeof(dev.addr)) < 0)
    {
        //printf("ERROR: Can not connect to new device\n");
        return -1 ;
    }   

    return 0;
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

int send_to(device dev, char *mes)
{
    if (write(dev.fd, mes, sizeof(mes)) < 0)
{
    printf("ERROR: Can not send message\n");
    return 0;
}
    return 1;
}

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

    device_connect[total_device].port_num = ntohs(cli_addr.sin_port);
    device_connect[total_device].addrlen = len ;
    inet_ntop(AF_INET, &cli_addr.sin_addr, device_connect[total_device].my_ip, 50);
    printf("                            ******                                   ");
    printf("\nAccept a new connection from IP addreass: %s, setup at port: %d\n", device_connect[total_device].my_ip, device_connect[total_device].port_num);
    
    if (pthread_create(&Recei_Thread_id, NULL, &receive_from, &device_connect[total_device])){
        printf("ERROR: Can not create to receive message\n");
    }
    //receive_from(device_connect[total_device]);

    total_device++;
    }
}


int main(int argc, char *argv[]){

     system("clear");

    if (signal(SIGINT,sig_handler) == SIG_ERR)
    {
        printf("Can not handler SIGINT\n");
     
    }

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
            char temp[20];
            char mes[50];
            int id;

            sscanf(command, "%s %d %[^\n]", temp, &id, mes);
            
            printf("Command: %s\n", temp);
            printf("ID: %d\n", id);
            printf("mes: %s\n", mes);

            for (int i=0; i <total_device;i++)
            {
                if (id == device_connect[i].id)
                {
                    send_to(device_connect[i], mes);
                }
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

        else if (!strcmp(command_option, "connect"))
        {
            int port_n;
            char IP_d[20];
            char temp[10];
            sscanf(command, "%s %s %d",temp, IP_d, &port_n);

            device_connect[total_device].fd = socket(AF_INET, SOCK_STREAM, 0);
            device_connect[total_device].id = total_device;
            device_connect[total_device].port_num = port_n;
            strcpy(device_connect[total_device].my_ip, IP_d);
            device_connect[total_device].addr.sin_family = AF_INET;
            device_connect[total_device].addr.sin_port = htons(device_connect[total_device].port_num);
            inet_pton(AF_INET, device_connect[total_device].my_ip, &device_connect[total_device].addr.sin_addr);

            if (connect_to(device_connect[total_device]) )
            {
                printf("ERROR: Can not connect to new device\n");

            }

            else{
                printf("Connnect OK\n");
                total_device++;
                printf("Total Device is : %d\n", total_device);
                printf("ID of this device : %d\n", total_device-1);
            }
            }

            else if (!strcmp(command_option, "exit"))
            {
                printf("**************************************************************************\n");
                printf("-----------------------ENDING PROGRAMMING---------------------------------\n");
                printf("***************************************************************************");
                return 1;
            }

    }

    return 0;
}