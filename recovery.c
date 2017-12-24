#include<stdio.h>
#include<cs50.h>
#include<stdlib.h>
#include <stdint.h>
typedef uint8_t  BYTE;
int main(int argc,string argv[]){
        
        //ensure proper usage
        if(argc!=2){
                fprintf(stderr,"usage: ./recover card.raw\n");
                return 1;
        }
        
        //open input file
        char *infile=argv[1]; 
        FILE *inptr=fopen(infile,"rb");
        if(inptr==NULL){
                fprintf(stderr,"could not open %s \n",infile);
                return 2;
        }
        int i=0;
        bool flag=true;
        char* filename=malloc(sizeof(char)*3);
        
        // datatype to store bytes of infile
        BYTE* buffer=malloc(sizeof(BYTE)*512);
        
        // check if fread has reached end of file
        while(fread(buffer,512,1,inptr)==1){
                //check if bytes read are jpeg signature
                if(buffer[0]==0xff && buffer[1]==0xd8 && buffer[2]==0xff &&(buffer[3]& 0xf0)==0xe0){
                        
                        //create and open a new jpeg
                        sprintf(filename,"%03i.jpg",i);
                        FILE *img=fopen(filename,"w");
                        
                        i++;
                        //write first 512 bytes into jpg
                        fwrite(buffer,512,1,img);
                        
                        //write to jpeg until another jpeg is found or end of file is reached
                        while(flag==true && (fread(buffer,512,1,inptr)==1) ){
                                if(buffer[0]==0xff && buffer[1]==0xd8 && buffer[2]==0xff && (buffer[3]& 0xf0)==0xe0){
                                        flag=false;
                                        fseek(inptr,-512,SEEK_CUR);
                                        
                                        //close current jpeg
                                        fclose(img);
                                }
                                fwrite(buffer,512,1,img);;
                        }
                        flag=true;
                }
                
        }
        
        //close input file
        fclose(inptr);
        
        // free memory allocated using malloc 
        free(buffer);
        free(filename);
        
       return 0;
        
}
