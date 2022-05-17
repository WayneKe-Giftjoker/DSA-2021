#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <memory.h>
#define HASHCONST 53
#define MODCONST 1000000007

//A~Z: __ - 'A' + 1 ; a~z: __ - 'a' + 1 + 26

char mail[1<<17], garbled[1<<17], cleanMail[1<<17];
int intMail[1<<17], garbledRecord[53], garbledCriteria[53];
int intCleanMail[1<<17];
int garbledStart, garbledEnd;
int VBLoc[1<<17]; // attached before element i 
int mailLen, garbledLen, cleanLen;

void reset(){
    memset(intMail, 0, strlen(mail) * sizeof(int));
    memset(garbledRecord, 0, 53 * sizeof(int));
    memset(garbledCriteria, 0, 53 * sizeof(int));
    memset(intCleanMail, 0, strlen(cleanMail) * sizeof(int));
    memset(VBLoc, 0, strlen(cleanMail) * sizeof(int));
    cleanMail[0] = '\0';
    // memset(cleanMail, 0, 1<<17);
    garbledStart = 0;
    garbledEnd = 0;
}

void preprocess(){
    mailLen = strlen(mail);
    for(int i = 0; i < mailLen; i++){
        if((int)(mail[i]) > (int)('Z'))
            intMail[i] = (int)(mail[i] - 'a') + 1 + 26;
        else
            intMail[i] = (int)(mail[i] - 'A') + 1;
    }
    garbledLen = strlen(garbled);
    for(int i = 0; i < garbledLen; i++){
        int converted;
        if((int)(garbled[i]) > (int)('Z'))
            converted = (int)(garbled[i] - 'a') + 1 + 26;
        else
            converted = (int)(garbled[i] - 'A') + 1;
        garbledCriteria[converted]++;
    }
}

void convertClean(){
    cleanLen = strlen(cleanMail);
    for(int i = 0; i < cleanLen; i++){
        if((int)(cleanMail[i]) > (int)('Z'))
            intCleanMail[i] = (int)(cleanMail[i] - 'a') + 1 + 26;
        else
            intCleanMail[i] = (int)(cleanMail[i] - 'A') + 1;
    }
}

int findGarbled(){
    int start = 0, end = 0;
    int minLen = 1<<17;
    int found = 0;
    while(start < mailLen && end < mailLen){
        if(garbledCriteria[intMail[end]] > 0){
            if(garbledRecord[intMail[end]] < garbledCriteria[intMail[end]])
                garbledLen--;
            garbledRecord[intMail[end]]++;
            while(start <= end && garbledLen == 0){
                if(end - start + 1 < minLen){
                    garbledStart = start;
                    garbledEnd = end;
                    minLen = end - start + 1;
                    found = 1;
                }
                if(garbledCriteria[intMail[start]] > 0){
                    garbledRecord[intMail[start]]--;
                    if(garbledRecord[intMail[start]] < garbledCriteria[intMail[start]])
                        garbledLen++;
                }
                start++;
            }
        }
        end++;
    }
    return found;
}

int main(int argc, char *argv[]){
    int T;
    scanf("%d", &T);
    while(T--){
        scanf("%s", mail);
        scanf("%s", garbled);
        preprocess();
        int found = findGarbled();
        if(found){
            strncat(cleanMail, mail, garbledStart);
            strncat(cleanMail, mail + garbledEnd + 1, strlen(mail) - garbledEnd - 1);
        }
        else{
            strcpy(cleanMail, mail);
        }
        convertClean();
        int Lstart = 0, Rstart = cleanLen - 1;
        int leftPtr = Lstart, rightPtr = Rstart;
        long long int leftHash = 0, rightHash = 0, rightdigit = 1;
        while(leftPtr < rightPtr){
            leftHash = (leftHash * HASHCONST + intCleanMail[leftPtr]) % MODCONST;
            rightHash = (rightHash + intCleanMail[rightPtr] * rightdigit) % MODCONST;
            rightdigit = rightdigit * HASHCONST % MODCONST;
            if(leftHash == rightHash){
                int same = 1;
                int i = Lstart, j = rightPtr;
                while(i <= leftPtr && j <= Rstart){
                    if(intCleanMail[i] != intCleanMail[j]){
                        same = 0;
                        break;
                    }
                    i++;
                    j++;
                }
                if(same){
                    VBLoc[leftPtr+1] = 1;
                    VBLoc[rightPtr] = 1;
                    leftHash = 0;
                    rightHash = 0;
                    rightdigit = 1;
                    Lstart = leftPtr + 1;
                    Rstart = rightPtr - 1;
                }
            }
            leftPtr++;
            rightPtr--;
        }
        for(int i = 0; i < cleanLen; i++){
            if(VBLoc[i]){
                if(i != cleanLen - 1)
                    printf("|%c", cleanMail[i]);
                else
                    printf("|%c\n", cleanMail[i]);
            }
            else{
                if(i != cleanLen - 1)
                    printf("%c", cleanMail[i]);
                else
                    printf("%c\n", cleanMail[i]);
            }
        }
        
        reset();
    }
    
    return 0;
}
