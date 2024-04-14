
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	unsigned long long randValue;
	unsigned long long urandValue;

	int i, j, z;
	int check, restartCounter, halfresetCounter;
	
	char array[95][95];
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
			fullreset:
			for(z=0;z<95;z++){
				strcpy(array[z],"");

			} //clearing the array for checking

			for(j=0;j<95;j++){
				halfresetCounter=0;
				halfreset:
				strcpy(array[z],"");
				for(z=0;z<95;z++){
				restartCounter=0;
				restart:
					fread(&randValue, sizeof(randValue), 1, randReader);
					fread(&urandValue, sizeof(urandValue), 1, urandReader);
					randValue+=urandValue;

					decided=(char)(randValue%95 +32); //fitting it into ascii again
					printf("%i: %i :%i,%i: %c\t",restartCounter,i,j,z,decided);					
					
					if(restartCounter>95*95*(z*2+1)/(j/2+1) && j<89){
						halfresetCounter++;
						goto halfreset; //ensuring no soft locks
					//}else if(halfresetCounter>95){
					//	goto fullreset;
					}
					for(check=0;check<z-1;check++){
						if(decided==array[j][check]){
							restartCounter++;
							goto restart;
						}
					} //checking for non repetition in horizontal line
					
					for(check=0;check<j-1;check++){
						if(decided==array[check][z]){
							restartCounter++;
							goto restart;
						}
					}
					array[j][z]=decided;
				}

			}

			for(j=0;j<95;j++){
				for(z=0;z<95;i++){
					fprintf(writer, "%c", array[j][i]); //writing the array to file
				}
				fprintf(writer, "\n");
			}
		}
	}
	fclose(writer);
	fclose(randReader);
	fclose(urandReader);
	return 0;
}
