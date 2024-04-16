#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct Field {
    uint8_t possible[95];
    uint8_t size;
    char value;
} fields;

void removeIndex(uint8_t array[95], uint8_t value, uint8_t* size){
	uint8_t m;
	//for(m=0;m< *size;m++){
	//	printf("%c",array[m]);
	//}
	//printf("\n");
	uint8_t l=0;
	uint8_t r=(uint8_t)(*size);
	//printf("stuck");
        while(l<=r){
	if(r>94||*size>94)return;
	m= l + (r - l) /2;
	//printf("\n%i\t%i:%i:%i",*size,l,m,r);	
        if (array[m] == value){
	    for(;m<*size-1;m++){ //array removal
		array[m]=array[m+1];
	    }
	    *size=*size-1;
	    //printf("\tremoved");
	    return;
	}

        // If x greater, ignore left half
        if (array[m] < value) l = m + 1;

        // If x is smaller, ignore right half
        else r = m - 1;
    // If we reach here, then element was not present
}


}

int main() {
    // Declare an array containing ASCII characters from 32 to 125
    uint8_t ascii_chars[95]={32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126};
    fields field[95][95];
    unsigned randvalue;
    uint8_t z, j, i, j1, i1;
    FILE *writer=fopen("lookup.txt","w");
    FILE *reader=fopen("/dev/urandom", "r");


    for(uint16_t y=0;y<2048;y++){

    for(i1=0;i1<95;i1++){
    for(j1=0;j1<95;j1++){
    printf("mem%i:%i\n",i1,j1); 
    // Copy the memory of the ascii_chars array into the possible array of field
    memcpy(field[i1][j1].possible, ascii_chars, sizeof(ascii_chars));
    // Set other members accordingly
    field[i1][j1].size = 94;
    field[i1][j1].value = '\0'; // default value for char is '\0'

    // Now filed has its 'possible' array initialized with ASCII values from 32 to 125
    // and size set to 94
    }
    }

	for(i=0;i<94;i++){
		for(j=0;j<94;j++){
				printf("%i:%i:%i\n", y,i,j);
				fread(&randvalue, sizeof(randvalue), 1, reader);		
				randvalue=randvalue% (field[i][j].size +1);
				fprintf(writer, "%c", field[i][j].possible[randvalue]);
				for(z=i+1;z<95;z++){
					//printf("\n%i:%i\t",z,j);
					removeIndex(field[z][j].possible, field[i][j].possible[randvalue], &(field[z][j].size));
				}
				for(z=j+1;z<95;z++){
					//printf("\n%i:%i\t",i,z);
					removeIndex(field[i][z].possible, field[i][j].possible[randvalue], &(field[i][z].size)); //if statement might be optional
				}
		}
		fprintf(writer, "%c", field[i][94].possible[0]);
		for(z=i+1;z<95;z++){
			removeIndex(field[z][94].possible, field[i][94].possible[0], &(field[z][94].size));
		}
		fprintf(writer, "\n");
	}
	for(j=0;j<95;j++){
		fprintf(writer, "%c", field[94][j].possible[0]);
	}
	fprintf(writer, "\n");
   }
   fclose(reader);
   fclose(writer);
   return 0;
}
