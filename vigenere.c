#include<cs50.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
void vigenere(string s,string k,int n); 
int keyindex(char kch);
int checku(int kch, int ch); 
int checkl(int kch, int ch);
int main(int argc, string argv[]){
        if(argc==2){
                string k=argv[1];
                int n=strlen(argv[1]);
                for(int i=0;i<n;i++){
                        if(isalpha(k[i])==0){
                                printf("key must contain alphabets only."); 
                                return(1);
                        }
                        
                }
                printf("plaintext: "); 
                string s=get_string(); 
                if(s!=NULL){
                        vigenere(s,k,n);
                        return(0);
                }
                else{
                        return(1);
                }
        }
        else{
                printf("Usage: ./vigenere k");
                return(1);
        }
}

void vigenere(string s,string k,int n){
        int l=strlen(s);
        char ciph[l];
        int ck=0,cc=0,kc=0;
        int ch;
        int kch=k[ck];
        for(int i=0;i<l;i++){
                ch=(int)s[i];
                if(isalpha(ch)!=0){
                        ck=kc%n;
                        kch=k[ck];
                         kc++;
                     kch=keyindex(kch); 
                }
                if(isupper(ch)!=0){
                       ch=checku(kch,ch);
                        ciph[cc]=ch;
                        cc++;
                }
                else if(islower(ch)!=0){
                        ch=checkl(kch,ch);
                        ciph[cc]=ch;
                        cc++;
                }
                else{
                        ciph[cc]=(char)ch;
                                cc++;
                        }
                
        }
        ciph[cc]='\0';
        printf("ciphertext: %s\n",ciph);
}

int keyindex(char kch){

                
                if(isupper(kch)!=0){
                        kch=(kch-65)%26;
                        return kch;
                }
                else{
                        kch=(kch-97)%26;
                        return kch;
                }
      
}

int checku(int kch, int ch){
         ch=(ch-65)+kch;
         ch=ch%26;
         ch=ch+65;
         return ch;
}

int checkl(int kch, int ch){
        ch=(ch-97)+kch;
        ch=ch%26;
        ch=ch+97;
        return ch;
}
   
   
   
   
   
 