#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void removeChar(char string[95],char input, int* size){
        int i;
        for(i=0;i< *size;i++){
                if(input==string[i]){
                        for(i=i;i< *size;i++){
                                string[i]=string[i+1];
                        }
                *size=*size-1;
                }
        }

}



int main(){
	unsigned long long randValue;
	unsigned long long urandValue;

	int i, j, z;
	int check;
	
	char array[95][95];
	char remaining[95];
	char remainingIteration[95];
	int remainingIterationSize;
	int remainingSize;

	char decided;
	char choice;

	FILE* randReader;
	FILE* urandReader;
	FILE* writer;

	randReader = fopen("/dev/random","r");
	urandReader = fopen("/dev/urandom","r");
	//init completed


	printf("[K stands for key, L for lookup table]\nWhat are you looking to generate? [L/K] ");
	scanf(" %c", &choice);
	if(choice!='L'&&choice!='K'){printf("Invalid input.");return 1;}
	printf("Are you sure you want to override your pervious %s? [y/n] ", (choice=='L')?"table":"key");
	scanf(" %c", &decided);
	if(decided!='y')return 0;
	//user input completed	

	printf("Generation starting...");
	if(choice=='K'){
		writer = fopen("key.txt","w");
		for(i=0;i<2048;i++){
			fread(&randValue, sizeof(randValue), 1, randReader);
			fread(&urandValue, sizeof(urandValue), 1, urandReader);
			randValue+=urandValue; //should add more obfuscation here, to make it not so simple, even if /dev/random is secure
	
			decided=(char)((randValue%95)+32); //making it fit into ascii
			fprintf(writer,"%c",decided);
			//character secured	

			fread(&randValue, sizeof(randValue), 1, randReader);
			randValue=(randValue%2048)+1000; //securing a 4 digit character
			fprintf(writer, "%llu", randValue);
		}
	}else{
		writer = fopen("lookup.txt","w");
		for(i=0;i<2048;i++){
			for(j=0;j<95;j++){
				remaining[95]='\0';
				for(z=0;z<95;z++){
					remaining[z]='\n';
				}
				for(z=0;z<95;z++){
					remainingSize=95;
					softreset:
					fread(&randValue, sizeof(randValue), 1, randReader);
					randValue=(randValue%95)+32;
					for(check=0;check<z;check++){
						if(randValue==remaining[check])goto softreset;
					}
					remaining[z]=(char)randValue;
				}
				//true random array secured
				
				
				for(z=0;z<95;z++){
					//usleep(10000);
					fread(&randValue, sizeof(randValue), 1, randReader);
					for(check=0;check<z;check++){
						removeChar(remaining,array[j][check],&remainingSize);
					}
					remainingIterationSize=remainingSize;
					for(check=0;check<remainingSize;check++){
						printf("%c",remaining[check]);
						remainingIteration[check]=remaining[check];
					}
					for(check=0;check<j;check++){
                                                removeChar(remainingIteration,array[check][z],&remainingIterationSize);
                                        }
					printf("\n%llu\t%i:%i\t%i:%i:%i",randValue, remainingSize, remainingIterationSize, i,j,z);
					if(remainingIterationSize!=0){randValue=randValue% remainingIterationSize;}else{randValue=0;}
					array[j][z]=remaining[randValue];
				}

			}

			printf("\n\n\n");
			for(j=0;j<95;j++){
				for(z=0;z<95;z++){
					printf("%i:%i\n",j,z);
					fprintf(writer, "%c", array[j][z]); //writing the array to file
				}
				//printf("%i",i);
				fprintf(writer, "\n");
			}
		}
	}
	fclose(writer);
	fclose(randReader);
	fclose(urandReader);
	return 0;
}
