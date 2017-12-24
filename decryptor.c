#define _XOPEN_SOURCE
#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<unistd.h>
void decrypt(string s);
int main(int argc, string argv[])
{
	if(argc==2){
	    string s=argv[1];
	    decrypt(s);
	}
	else {
	printf("Usage: ./crack hash\n");
	return(1);
	}
}
void decrypt(string s){
	char salt[2]={s[0],s[1]};
	char pt[5];
	int n=91,l=91,b=91,r=91;
	string str;
		
		for(int i=65;i<=n;i++){
			if(i==91){
				i=97;
				n=122;
			}
			pt[0]=i;
			for(int v=64;v<=l;v++){
				if(v==91){
					v=97;
					l=122;
				}
				if(v==64){
				      pt[1]='\0';  
				}
				else{
					pt[1]=v;
				}
				
				
				for(int d=64;d<=b;d++){
				if(d==91){
					d=97;
					b=122;
				}
				if(d==64){
				       pt[2]='\0';
				}
				else{
					pt[2]=d;
				}
				     
			
		      for(int u=64;u<=r;u++){
			      if(u==91){
				      u=97;
				      r=122;
			      }
			      if(u==64){
				      pt[3]='\0';
			      }
			      else{
				      pt[3]=u;
			      }
			pt[4]='\0';     
		      str=crypt(pt,salt);
		      if(strcmp(s,str)==0)
		      {     printf("%s \n",pt);
			    exit(0);
			    
		      }
		      
			      
		      }
			}
			 
				
			}
			
			
		       
		      
		}
}