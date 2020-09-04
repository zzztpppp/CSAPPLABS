#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *read_trace_line(FILE *file_stream){
    /* The maximum length of a line is limited to 20 characters long*/
    int i = 0;
	char *buffer = malloc(20);
    char c;


    while ((!feof(file_stream)) && (i < 20)){
	c = fgetc(file_stream);
        if (c == 10){
            break;
        }
	if (feof(file_stream)){
	   return NULL; 
	}
        buffer[i] = c;
        i++;
    }

    return buffer;
}
unsigned long ulong_power(unsigned long x, unsigned long y) {
	    unsigned long i = 0u;
	        unsigned long result = 1;
		    for (;i < y; i++){
			            result = result * x;
				        }

		        return result;
}
unsigned long hex_to_ulong(char c) {
	    unsigned long result;

	        if ((c >= 48) && (c <= 57)){
			        result = c - 48;
				    }

		    else{
			            if ((c >= 65) && (c <= 70)){
					                result = c-55;
							        }
				            else {
						                printf("Arguement %c is not a valid hex value", c);
								            exit(1);
									            }
					        }

		        return result;
}
 unsigned long convert_hex_string(char * hex_string) {
	      int length = strlen(hex_string);
	           unsigned long result = 0u;

		        for(unsigned long i = 0u; i < length; i++){
				        result = ulong_power(16u, i) * hex_to_ulong(hex_string[i]) + result;
					     }

			     return result;
			      }

int main(int argc, char *argv[]) {
    unsigned long r; 
    FILE *f;
    char *f_name = argv[1];
	char *line;
    
	f = fopen(f_name, "r");
	if (f == NULL){
		printf("Failed to open");
		exit(8);
	   }

    while (1){
        line = read_trace_line(f);
		if (line == NULL){
			break;
		}
		printf("%s\n", line);
	}

	return 0;
}