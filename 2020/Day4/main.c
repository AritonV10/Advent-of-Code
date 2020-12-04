#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include "..\Utils\string\include\string.h"



#define MAIN__Height()                                        \
{                                                             \
    for(u8Index = 0; (*pcData >> 4) == 0x03; ++pcData) {      \
        acHeight[u8Index++] = *pcData;                        \
    }                                                         \  
    acHeight[u8Index] = 0;                                    \
    u32Height = MAIN__u32Stoi((const char *)&acHeight);       \
}                                                             \

#define MAIN__Clamped(Value, Low, High)                       \
{                                                             \
    if((Value >= Low) && (Value <= High)) {                   \
        return(MAIN__nOk);                                    \
    } else {                                                  \
        return(MAIN__nFail);                                  \
    }                                                         \      
}                                                             \

#define MAIN__nCodeLength (uint32_t)(3)
#define MAIN__nCodes      (uint32_t)(8)

#define MAIN__nEyeColors  (uint32_t)(7)

#define MAIN__nFail       (uint8_t)(0)
#define MAIN__nOk         (uint8_t)(1)



typedef enum {
    MAIN__nenBYR,
    MAIN__nenIYR,
    MAIN__nenEYR,
    MAIN__nenHGT,
    MAIN__nenHCL,
    MAIN__nenECL,
    MAIN__nenPID,
    MAIN__nenCID,
    MAIN__nenInvalid
}MAIN__tenCodeEnum;

typedef struct {
    MAIN__tenCodeEnum enEnum;
    const char *pcCode;
}MAIN__tstCode;

static const char *MAIN__astrEyeColors[MAIN__nEyeColors] = {
    "amb", 
    "blu", 
    "brn", 
    "gry", 
    "grn", 
    "hzl", 
    "oth"
};


static MAIN__tstCode MAIN__astPassportCodes[MAIN__nCodes] = {  
    {MAIN__nenBYR, "byr"}, /* (Birth Year)      */
    {MAIN__nenIYR, "iyr"}, /* (Issue Year)      */
    {MAIN__nenEYR, "eyr"}, /* (Expiration Year) */
    {MAIN__nenHGT, "hgt"}, /* (Height)          */
    {MAIN__nenHCL, "hcl"}, /* (Hair Color)      */
    {MAIN__nenECL, "ecl"}, /* (Eye Color)       */
    {MAIN__nenPID, "pid"}, /* (Passport ID)     */
    {MAIN__nenCID, "cid"}  /* (Country ID)      */
};


static uint32_t
MAIN__u32Stoi(const char *pcString) {
    uint32_t u32Number;
    
    for(u32Number = 0; *pcString != 0; ++pcString) {
        u32Number = (u32Number * 10) + (*pcString & 0x0F);
    }

    return(u32Number);
}

static uint32_t
MAIN__u32Strlen(const char *pcString) {
    uint32_t u32Len;

    for(u32Len = 0; *pcString != 0; ++pcString, ++u32Len)
        ;
    
    return(u32Len);
}

static uint8_t 
MAIN__u8Compare(const char *pcA, const char *pcB) {

    for(; *pcA != 0; ++pcA, ++pcB) {

        if((*pcA) != (*pcB)) {

            return(MAIN__nFail);
        }

    }

    return(MAIN__nOk);
}


static MAIN__tenCodeEnum
MAIN__enGetCode(const char *pcCode) {
    uint8_t u8Index;

    for(u8Index = 0; u8Index < (MAIN__nCodes - 1); ++u8Index) {
        
        if(MAIN__u8Compare(pcCode, MAIN__astPassportCodes[u8Index].pcCode) == MAIN__nOk)
            return(MAIN__astPassportCodes[u8Index].enEnum);

    }

    return(MAIN__nenInvalid);
}

static uint8_t 
MAIN__u8ValidateData(const char *pcData, MAIN__tenCodeEnum enEnum) {

    switch(enEnum) {
        case MAIN__nenBYR:
        case MAIN__nenIYR:
        case MAIN__nenEYR:
        {
            uint32_t u32Length;
            uint32_t u32Integer;

            /* Get the length */
            u32Length = MAIN__u32Strlen(pcData);

            if((u32Length >= 0u) && (u32Length <= 4u)) {
                
                /* Convert to integer */
                u32Integer = MAIN__u32Stoi(pcData);

                switch(enEnum) {
                    case MAIN__nenBYR:
                        MAIN__Clamped(u32Integer ,1920u, 2020u);
                        break;
                    case MAIN__nenIYR:
                        MAIN__Clamped(u32Integer, 2010u, 2020u);
                        break;
                    case MAIN__nenEYR:
                        MAIN__Clamped(u32Integer, 2020u, 2030u);
                        break;
                }
                
            } else {
                /* Invalid */
                return(MAIN__nFail);
            }
        }
        break;
        
        case MAIN__nenHGT:
        {

            char        acHeight[4];
            const char *pcHead;

            uint8_t u32Height;
            uint8_t u8Index;
            pcHead = pcData;

            for(; ((*pcHead >> 4) == 0x03); ++pcHead)
                ;
   
            if(MAIN__u8Compare("in", pcHead) == MAIN__nOk) {

                MAIN__Height();
                
                MAIN__Clamped(u32Height, 59u, 76u);

            } else if(MAIN__u8Compare("cm", pcHead) == MAIN__nOk) {
                
                MAIN__Height();
                
                MAIN__Clamped(u32Height, 150u, 193u);

            } else {
                return(MAIN__nFail);
            }

        }
        break;
        case MAIN__nenHCL:
        {
            uint8_t u8Length;

            if(*pcData != '#')
                return(MAIN__nFail);
            
            /* Move past the # */
            ++pcData;

            u8Length = MAIN__u32Strlen(pcData);
            
            /* Needs to have length 6 */
            if(u8Length != 6u)
                return(MAIN__nFail);

            for(; *pcData != 0; ++pcData) {
                if(((*pcData >= 0x30u) && (*pcData <= 0x39u)) || ((*pcData >= 0x61u) && (*pcData <= 0x66u))) {
                    /* Good */
                } else {
                    return(MAIN__nFail);
                }
            }

            return(MAIN__nOk);

        }
        break;
        case MAIN__nenECL:
        {
            uint8_t u8Index;

            for(u8Index = 0; u8Index < MAIN__nEyeColors; ++u8Index) {
                if(MAIN__u8Compare(MAIN__astrEyeColors[u8Index], pcData) == MAIN__nOk) {
                    return(MAIN__nOk);
                }
            }

            return(MAIN__nFail);
        }
        break;
        case MAIN__nenPID:
        {
            uint32_t u32Length;

            u32Length = MAIN__u32Strlen(pcData);

            printf("Len: %d\n", u32Length);

            if(u32Length != 9)
                return(MAIN__nFail);
            
            for(; *pcData != 0; ++pcData) {
                MAIN__Clamped(*pcData, 0x30u, 0x39u);
            }
        }
        break;
        
    }
}

static uint8_t
MAIN__u8FindCode(const char *pcCode) {
    uint8_t u8Index;

    for(u8Index = 0; u8Index < (MAIN__nCodes - 1); ++u8Index) {
        
        if(MAIN__u8Compare(pcCode, MAIN__astPassportCodes[u8Index].pcCode) == MAIN__nOk)
            return(MAIN__nOk);

    }

    return(MAIN__nFail);
}

static uint8_t
MAIN__u8ValidatePassport(const char *pcPassport) {
    
    uint8_t u8Validity;
    uint8_t u8CodesIndex;
    uint8_t u8PassportIndex;
    uint8_t u8Count;
    char    cBuf[32];

    MAIN__tenCodeEnum enCode;

    char    *pcHead;
    printf("%s\n", pcPassport);

    u8Count = 0;

    for(; *pcPassport != 0; ++pcPassport) {
        
        if(*pcPassport == ':'){
        
            cBuf[3] = 0;
            cBuf[2] = *(pcPassport - 1);
            cBuf[1] = *(pcPassport - 2);
            cBuf[0] = *(pcPassport - 3);

            /* cid is optional so we skip it */
            if(MAIN__u8Compare((const char *)cBuf, "cid") == MAIN__nFail) {

                if(MAIN__u8FindCode((const char *)cBuf) == MAIN__nOk) {
                    
                    
                    enCode = MAIN__enGetCode((const char *)cBuf);

                    printf("Code: %s %d ===", cBuf, enCode);

                    if(enCode == MAIN__nenInvalid)
                        return(MAIN__nFail);

                    /* Move past : */
                    ++pcPassport;

                    for(u8CodesIndex = 0;; ++pcPassport) {

                        if((*pcPassport == '\n') || (*pcPassport == ' ') || (*pcPassport == 0)) {
                            
                            cBuf[u8CodesIndex] = 0;
                            printf(" Data: %s\n", cBuf);

                            u8Count += MAIN__u8ValidateData((const char *)&cBuf, enCode);
                            break;
                        }

                        cBuf[u8CodesIndex++] = *pcPassport;
                    }
                
                }

            } else {
                
            }
        }
    }
    
    printf("Validity: %d\n\n", u8Count);
    return((u8Count == 7 ? MAIN__nOk : MAIN__nFail));

}

static void
MAIN__Day4Part1_2(const char *pcPath) {
    
    uint8_t u8Index;
    char    cByte;
    char    cPreviousByte;
    char    acString[1024];

    int iFD;
    uint32_t u32ValidPassports;

    iFD = open(pcPath, O_RDONLY);

    for(u32ValidPassports = 0, u8Index = 0;;) {
        if((read(iFD, &cByte, 1)) != 0) {
            if(cByte == '\n') {
                if(cPreviousByte == '\n') {
                    acString[u8Index] = 0;
                    u32ValidPassports += (MAIN__u8ValidatePassport((const char *)acString));
                    u8Index = 0;
                } else {
                    acString[u8Index++] = cByte;
                }
            } else {
                acString[u8Index++] = cByte;
            }
            cPreviousByte = cByte;
        } else {
            acString[u8Index] = 0;
            u32ValidPassports += (MAIN__u8ValidatePassport((const char *)acString));
            break;
        }
    }

    printf("Valid Passports: %d", u32ValidPassports);
    close(iFD);
    
}



int main(void) {
    
    MAIN__Day4Part1_2("C:\\Users\\vio\\Desktop\\VS_C\\viorel.txt");
    
    /*
    printf("%d\n", MAIN__u8ValidateData("2010", MAIN__nenIYR));
    printf("%d\n", MAIN__u8ValidateData("623705680", MAIN__nenPID));
    printf("%d\n", MAIN__u8ValidateData("hzl", MAIN__nenECL));
    printf("%d\n", MAIN__u8ValidateData("181cm", MAIN__nenHGT));
    printf("%d\n", MAIN__u8ValidateData("1980", MAIN__nenBYR));
    printf("%d\n", MAIN__u8ValidateData("#341e13", MAIN__nenHCL));
    printf("%d\n", MAIN__u8ValidateData("2028", MAIN__nenEYR));
    */

    return(0);
}
