
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <fcntl.h>
#include <unistd.h> 


#define MAIN__nMaxFileName 64
#define MAIN__nNrCommands  (0x02u)

#define TOKEN__FName (MAIN__pcTokens[1u])
#define TOKEN__FSize (MAIN__pcTokens[0])

#define TOKEN__CName (MAIN__pcTokens[1u])
#define TOKEN__CDir  (MAIN__pcTokens[2u])

#define nMAX          (70000000u)

#define LOG(fmt, ...)                   \
            do {                        \
  fprintf(MAIN__pstWrite,  "[LOG]:" fmt "\n", __VA_ARGS__); \
}while(0);

typedef enum {
  CD     = 0,
  LS     = 1u,
  UNUSED = 2u
}MAIN__tenCmd;

typedef struct {

  uint32_t  u32FileSize;
  char     *pcFileName;

} FILE__tstFile;

struct DIR__Tag_Node {

  uint8_t               u8Type;
  uint16_t              u16NrFiles;
  uint16_t              u16NrDirs;
  uint32_t              u32DirSize;
  
  char                   *pcName;
  struct DIR__Tag_Node   *pstParent;
  FILE__tstFile          **pstFiles;
  struct DIR__Tag_Node   **pstDirectories;
  
};

typedef struct DIR__Tag_Node DIR__tstDirectory;


static void
MAIN__vTokenize(const char *);

static MAIN__tenCmd
MAIN__vCmdType(const char *);
static void *
MAIN__pvMalloc(uint32_t);
static uint8_t
MAIN__u8Compare(const char *, const char *);
static uint32_t 
MAIN__u32Stoi(const char *);


static void
DIR__vInitFileSystem(const char *);
static void
DIR__vUpdateFileSystem(void);
static void
DIR__vAddDirectory(char *);
static void
DIR__vAddFile(char *, char *);
static void
DIR__vUpdateParentSize(uint32_t);
static uint32_t 
DIR__u32FindMin(DIR__tstDirectory *);
static uint32_t 
DIR__u32FindMin2(DIR__tstDirectory *);

static DIR__tstDirectory *pstFileSystem = NULL;

static const char *MAIN__au8Commands[] = { 
  "cd", /* 0 */ 
  "ls"  /* 1u */
};

static uint32_t MAIN__u32Part2 = (uint32_t)~0;
static uint32_t MAIN__u32Part1;
static FILE *MAIN__pstFile;
static FILE *MAIN__pstWrite;
static char *MAIN__pcCmd;
static int32_t MAIN__i32BytesRead;
static char *MAIN__pcTokens[3];

static DIR__tstDirectory *DIR__pstStackNode;

int main(void) {

  uint8_t   u8Idx;
  uint64_t u64Sz;

  MAIN__pcTokens[0] = MAIN__pvMalloc(32);
  MAIN__pcTokens[1] = MAIN__pvMalloc(32);
  MAIN__pcTokens[2] = MAIN__pvMalloc(32);

  MAIN__pstFile = fopen("data.txt", "r");
  MAIN__pstWrite = fopen("output.txt", "w");
  MAIN__pcCmd = NULL;

  /* Do first read to get the home directory */
  (void)getline((char **)&MAIN__pcCmd, &u64Sz, MAIN__pstFile);

  MAIN__vTokenize(MAIN__pcCmd);

  /* Next line */
  free(MAIN__pcCmd);
  MAIN__pcCmd = NULL;

  
  DIR__vInitFileSystem(TOKEN__CDir);
  
  for(; (MAIN__i32BytesRead = getline((char **)&MAIN__pcCmd, &u64Sz, MAIN__pstFile)) != -1 ; ) {

    MAIN__vTokenize(MAIN__pcCmd);
  
    LABEL_UPDATE:
    
    switch(MAIN__vCmdType(MAIN__pcTokens[1])) {
      case CD:
        
        /* Move to parent node */
        if(MAIN__u8Compare(MAIN__pcTokens[2], (const char *)"..")) {
         //LOG("Back: [%s] -> [%s]", DIR__pstStackNode->pcName, DIR__pstStackNode->pstParent->pcName);
          if(DIR__pstStackNode->pstParent != NULL)
            DIR__pstStackNode = DIR__pstStackNode->pstParent;
          continue;
        }
        
       //LOG("Entering [%s]", MAIN__pcTokens[2]);
        /* TODO: cover case when it wasn't found */
        for(u8Idx = 0; u8Idx < DIR__pstStackNode->u16NrDirs; ++u8Idx)
          
          if(MAIN__u8Compare(MAIN__pcTokens[2], DIR__pstStackNode->pstDirectories[u8Idx]->pcName)){
           // printf("Found! %s\n", DIR__pstStackNode->pstDirectories[u8Idx]->pcName);
            DIR__pstStackNode = DIR__pstStackNode->pstDirectories[u8Idx];
            
          }
            
        /* Push on stack */
        break;
      case LS:
          
        /* We gotta use gotos here */
        DIR__vUpdateFileSystem();
        if(MAIN__i32BytesRead == -1)
          goto LABEL_END;

        goto LABEL_UPDATE;
        break;

      case UNUSED:
      break;
    }
    
   
    
    free(MAIN__pcCmd);
    MAIN__pcCmd = NULL;
          
  }
  
   LABEL_END:
  printf("Size: %u\n", pstFileSystem->u32DirSize);

  MAIN__u32Part1 = DIR__u32FindMin(pstFileSystem);
  printf("Part1: %d\n", MAIN__u32Part1);
  
  MAIN__u32Part1 = pstFileSystem->u32DirSize;
  printf("Min space: %d\n", 30000000u - (nMAX - MAIN__u32Part1));
  
  (void)DIR__u32FindMin2(pstFileSystem);
  printf("Part2: %d", MAIN__u32Part2);
    
  return(0);
}


/**************************************************/


static uint32_t 
DIR__u32FindMin(DIR__tstDirectory *pstDir) {
  uint8_t  u8Idx;
  uint32_t u32Sum;

  u32Sum = 0;
  u8Idx  = 0;
  
  if(pstDir->pstDirectories != NULL) {
    for(u8Idx = 0; u8Idx < pstDir->u16NrDirs; ++u8Idx) {
      u32Sum += DIR__u32FindMin(pstDir->pstDirectories[u8Idx]);
    }  
  }

  if(pstDir->u32DirSize <= 100000u)
    return(u32Sum + pstDir->u32DirSize);

  return(u32Sum);
  
}


static uint32_t 
DIR__u32FindMin2(DIR__tstDirectory *pstDir) {
  uint8_t  u8Idx;
  uint32_t u32Sum;
  

  u8Idx  = 0;
  
  if(pstDir->pstDirectories != NULL) {
    for(u8Idx = 0; u8Idx < pstDir->u16NrDirs; ++u8Idx) {
      (void)DIR__u32FindMin2(pstDir->pstDirectories[u8Idx]);
    }  
  }

  if((pstDir->u32DirSize >= (30000000u - (nMAX - MAIN__u32Part1))) && (pstDir->u32DirSize <= MAIN__u32Part2))
    MAIN__u32Part2 = pstDir->u32DirSize;
  
 
  return(0);
  
}


static void
MAIN__vTokenize(const char *pcCmd) {

  uint8_t u8Idx;
  char    *pcTemp;

  u8Idx = 0;

  pcTemp = MAIN__pcTokens[0];
  
  for(; *pcCmd != '\n'; ++pcCmd) {
    
    if(*pcCmd == ' ') {

      /* Add the end char */
      *pcTemp = 0;
      ++u8Idx;

      /* Get the next string */
      pcTemp = MAIN__pcTokens[u8Idx];

      continue;
    }
      
    *pcTemp++ = *pcCmd;
    
  }

  *pcTemp = 0;
  
}




static void *
MAIN__pvMalloc(uint32_t u32BlockSize) {

  void *pvBlock;

  pvBlock = malloc(u32BlockSize);

  if(pvBlock == NULL)
    exit(EXIT_FAILURE);

  return(pvBlock);
}


static uint8_t
MAIN__u8Compare(const char *pcLhs, const char *pcRhs) {

  for(; *pcLhs != 0 || *pcRhs != 0; ++pcLhs, ++pcRhs)
    if(*pcLhs - *pcRhs)
      return(0);

  return(1u);
    
}

MAIN__tenCmd
MAIN__vCmdType(const char *pcCmd) {

  uint8_t u8Idx;

  u8Idx = 0;

  for(; u8Idx < MAIN__nNrCommands; ++u8Idx) {
    if(MAIN__u8Compare(pcCmd, MAIN__au8Commands[u8Idx]))
      return((MAIN__tenCmd) u8Idx);
  }

  return(UNUSED);
  
}

uint16_t
MAIN__u16Strlen(const char *pcStr) {
  uint16_t u16Len;

  u16Len = 0;

  for(; *pcStr != 0; ++pcStr, ++u16Len)
    ;

  return(u16Len);
}

void
DIR__vInitFileSystem(const char *pcDirName) {

  char      *pcTemp;
  uint16_t u16Strlen;

  //LOG("%s [%s]", "Adding home directory", pcDirName)
  
  /* Get length */
  u16Strlen = MAIN__u16Strlen(pcDirName);

  /* Alloc mem */
  pstFileSystem = MAIN__pvMalloc(sizeof(struct DIR__Tag_Node));

  pstFileSystem->u32DirSize = 0;
  pstFileSystem->u16NrFiles = 0;
  pstFileSystem->u16NrDirs  = 0;
  pstFileSystem->u8Type     = 1u;
  pstFileSystem->pstDirectories = NULL;
  pstFileSystem->pstFiles       = NULL;

  pstFileSystem->pcName = MAIN__pvMalloc(u16Strlen);
  /* No parent since home dir */
  pstFileSystem->pstParent = NULL;
  /* Set it as top of stack */
  DIR__pstStackNode = pstFileSystem;
    
  pcTemp = DIR__pstStackNode->pcName;

  /* Set the name */
  for(; *pcDirName != 0; ++pcDirName, ++pcTemp)
    *pcTemp = *pcDirName;

  *pcTemp = 0;

}

static void
DIR__vUpdateFileSystem(void) {
  uint64_t u64Sz;
  
  for(; (MAIN__i32BytesRead = getline((char **)&MAIN__pcCmd, &u64Sz, MAIN__pstFile)) != -1; ) {
  
    MAIN__vTokenize(MAIN__pcCmd);
    if(MAIN__u8Compare(MAIN__pcTokens[0], "$"))
      break;
    
    /* Is it a directory? */
    if(MAIN__u8Compare(MAIN__pcTokens[0], (const char *)"dir"))
      DIR__vAddDirectory(MAIN__pcTokens[1]);
    else
      DIR__vAddFile(TOKEN__FName, TOKEN__FSize);
    /* Hard-coded; only two options available but can be made generic */
    
  }
  
}

static void
DIR__vAddDirectory(char *pcDirName) {

  uint32_t           u32Size;
  char              *pcTemp;
  DIR__tstDirectory *pstDir;

  LOG("Adding dir [%s] to [%s]", pcDirName, DIR__pstStackNode->pcName);

  DIR__pstStackNode->pstDirectories
    = realloc(DIR__pstStackNode->pstDirectories, sizeof(DIR__pstStackNode->pstDirectories) * (DIR__pstStackNode->u16NrDirs + 1u));
  
  DIR__pstStackNode->u16NrDirs += 1u;
   LOG("No. Dirs: [%d]", DIR__pstStackNode->u16NrDirs);
  /* Make a new directory */
  u32Size = MAIN__u16Strlen(pcDirName);
  pstDir = MAIN__pvMalloc(sizeof(*DIR__pstStackNode));
  pstDir->pcName = MAIN__pvMalloc(sizeof(char) * u32Size);
  /* Set parent node */
  pstDir->pstParent      = DIR__pstStackNode;
  pstDir->u32DirSize     = 0;
  pstDir->u16NrFiles     = 0;
  pstDir->pstDirectories = NULL;
  pstDir->pstFiles       = NULL;
  
  pcTemp = pstDir->pcName;

  /* Set name */
  for(; *pcDirName != 0; ++pcDirName, ++pcTemp)
    *pcTemp = *pcDirName;

  *pcTemp = 0;
  
  DIR__pstStackNode->pstDirectories[DIR__pstStackNode->u16NrDirs - 1] = pstDir;
  
}

static uint32_t 
MAIN__u32Stoi(const char *pStr) {

  uint32_t u32Integer;

  u32Integer = 0;

  for (; *pStr != 0; ++pStr)
    u32Integer = (u32Integer * 10u) + (*pStr & 0x0Fu);

  return (u32Integer);
}

static void
DIR__vAddFile(char *pcFileName, char *pcFileSize) {

  uint32_t       u32Strlen;
  uint32_t       u32FSize;
  char          *pcTemp;
  FILE__tstFile *pstNewFile;

  LOG("Adding file [%s] with size [%s] to [%s]", pcFileName, pcFileSize, DIR__pstStackNode->pcName);

  DIR__pstStackNode->pstFiles = realloc(DIR__pstStackNode->pstFiles, sizeof(*DIR__pstStackNode->pstFiles) * (DIR__pstStackNode->u16NrFiles + 1u));
  
  pstNewFile             = MAIN__pvMalloc(sizeof(FILE__tstFile));

  u32Strlen              = MAIN__u16Strlen(pcFileName);
  
  u32FSize               = MAIN__u32Stoi(pcFileSize);
  
  pstNewFile->pcFileName = MAIN__pvMalloc(sizeof(char) * u32Strlen);
  pcTemp = pstNewFile->pcFileName;
  /* Set name */
  for(; *pcFileName != 0; ++pcFileName, ++pcTemp)
    *pcTemp = *pcFileName;

  *pcTemp = 0;

  /* Update directory size */
  DIR__pstStackNode->u32DirSize += u32FSize;
  DIR__pstStackNode->u16NrFiles += 1u;
  DIR__pstStackNode->pstFiles[DIR__pstStackNode->u16NrFiles - 1] = pstNewFile;
  
  /* Update parent size */
  DIR__vUpdateParentSize(u32FSize);
}

static void
DIR__vUpdateParentSize(uint32_t u32Size) {

  
  DIR__tstDirectory *pstParentTemp;
  
  pstParentTemp = DIR__pstStackNode->pstParent;
  for(; pstParentTemp != NULL; ) {
    LOG("Updating: %s\n", pstParentTemp->pcName);
    pstParentTemp->u32DirSize += u32Size;
    pstParentTemp = pstParentTemp->pstParent;
  }
  
}
