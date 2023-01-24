#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
 
#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)
 
int main()
{
        int fd;
        int32_t value, number,num,i;
        printf("Initiating Driver\n");
 
        printf("\nOpening Device Node\n");
        fd = open("/dev/sevenseg_device", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 	
	initial:

        printf("Enter your choice\n");
        printf("\t1. Reset\n\t2. Display a Number\n\t3. mod-100 UP counter\n\t4. mod-100 DOWN counter\n-> ");
        scanf("%d",&i);
	switch(i){
		case 1:
        		printf("Resetting the Counter\n");
        		ioctl(fd, WR_VALUE, (int32_t*) 0); 
			break;
		case 2:
        		printf("Enter value to be displayed\n-> "); // Input value is sent to the driver
        		scanf("%d",&num);
        		ioctl(fd, WR_VALUE, (int32_t*) &num);
			break;
		case 3:
		jump:
			for(number=0; number<=99; number++){
		        printf("Writing Value %d to Driver\n", number);
		        ioctl(fd, WR_VALUE, (int32_t*) &number); 
			if (number==99){
				goto jump;
				}
			}
			break;
			/*
		        printf("Reading Value from Driver\n");
		        ioctl(fd, RD_VALUE, (int32_t*) &value);
		        printf("Value is %d\n", value); */
		case 4:
		jump1:
			for(number=99; number>=0; number--){
		        printf("Writing Value %d to Driver\n", number);
		        ioctl(fd, WR_VALUE, (int32_t*) &number); 
			if (number==0){
				goto jump1;
				}
			}
			break;
		default:
		        printf("You have entered a Wrong option. Please try again.\n");
			goto initial;
		
	}
        printf("Closing Device Node\n");
        close(fd);
}

