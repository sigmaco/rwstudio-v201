#ifndef _FLAG_DEFS_H_
#define _FLAG_DEFS_H_

#include "..\xboxvertexshaderbasedefs.h"


#define VSCONST_REG_LIGHT_POS_OFFSET         VSCONST_LAST_CAMERA_REG_OFFSET   + VSCONST_LAST_CAMERA_REG_SIZE
#define VSCONST_REG_LIGHT_POS_SIZE           1

#define VSCONST_REG_TIME_OFFSET              VSCONST_REG_LIGHT_POS_OFFSET     + VSCONST_REG_LIGHT_POS_SIZE  
#define VSCONST_REG_TIME_SIZE                1

#define VSCONST_REG_ONE_OFFSET               VSCONST_REG_TIME_OFFSET          + VSCONST_REG_TIME_SIZE       
#define VSCONST_REG_ONE_SIZE                 1

#define VSCONST_REG_ZERO_OFFSET              VSCONST_REG_ONE_OFFSET           + VSCONST_REG_ONE_SIZE        
#define VSCONST_REG_ZERO_SIZE                1

#define VSCONST_REG_MAT_COLOR_OFFSET         VSCONST_REG_ZERO_OFFSET          + VSCONST_REG_ZERO_SIZE       
#define VSCONST_REG_MAT_COLOR_SIZE           1

#define VSCONST_REG_MAT_AMB_OFFSET           VSCONST_REG_MAT_COLOR_OFFSET     + VSCONST_REG_MAT_COLOR_SIZE  
#define VSCONST_REG_MAT_AMB_SIZE             1

#define VSCONST_REG_HFREQA_OFFSET            VSCONST_REG_MAT_AMB_OFFSET       + VSCONST_REG_MAT_AMB_SIZE
#define VSCONST_REG_HFREQA_SIZE              1

#define VSCONST_REG_HSPEEDA_OFFSET           VSCONST_REG_HFREQA_OFFSET        + VSCONST_REG_HFREQA_SIZE     
#define VSCONST_REG_HSPEEDA_SIZE             1

#define VSCONST_REG_HAMPLITUDEA_OFFSET       VSCONST_REG_HSPEEDA_OFFSET       + VSCONST_REG_HSPEEDA_SIZE    
#define VSCONST_REG_HAMPLITUDEA_SIZE         1

#define VSCONST_REG_HFREQB_OFFSET            VSCONST_REG_HAMPLITUDEA_OFFSET   + VSCONST_REG_HAMPLITUDEA_SIZE
#define VSCONST_REG_HFREQB_SIZE              1

#define VSCONST_REG_HSPEEDB_OFFSET           VSCONST_REG_HFREQB_OFFSET        + VSCONST_REG_HFREQB_SIZE     
#define VSCONST_REG_HSPEEDB_SIZE             1

#define VSCONST_REG_HAMPLITUDEB_OFFSET       VSCONST_REG_HSPEEDB_OFFSET       + VSCONST_REG_HSPEEDB_SIZE    
#define VSCONST_REG_HAMPLITUDEB_SIZE         1
                                                                                
#define VSCONST_REG_VFREQA_OFFSET            VSCONST_REG_HAMPLITUDEB_OFFSET   + VSCONST_REG_HAMPLITUDEB_SIZE
#define VSCONST_REG_VFREQA_SIZE              1

#define VSCONST_REG_VSPEEDA_OFFSET           VSCONST_REG_VFREQA_OFFSET        + VSCONST_REG_VFREQA_SIZE     
#define VSCONST_REG_VSPEEDA_SIZE             1

#define VSCONST_REG_VAMPLITUDEA_OFFSET       VSCONST_REG_VSPEEDA_OFFSET       + VSCONST_REG_VSPEEDA_SIZE    
#define VSCONST_REG_VAMPLITUDEA_SIZE         1


/* Constant register - For use in the vertex shader code */

#define VSCONST_REG_LIGHT_POS           c[VSCONST_REG_LIGHT_POS_OFFSET + 0]
#define VSCONST_REG_TIME                c[VSCONST_REG_TIME_OFFSET + 0]
#define VSCONST_REG_ONE                 c[VSCONST_REG_ONE_OFFSET + 0]
#define VSCONST_REG_ZERO                c[VSCONST_REG_ZERO_OFFSET  + 0]
#define VSCONST_REG_MAT_COLOR           c[VSCONST_REG_MAT_COLOR_OFFSET + 0]
#define VSCONST_REG_MAT_AMB             c[VSCONST_REG_MAT_AMB_OFFSET + 0]


#define VSCONST_REG_HFREQA              c[VSCONST_REG_HFREQA_OFFSET        + 0]
#define VSCONST_REG_HSPEEDA             c[VSCONST_REG_HSPEEDA_OFFSET       + 0]
#define VSCONST_REG_HAMPLITUDEA         c[VSCONST_REG_HAMPLITUDEA_OFFSET   + 0] 
#define VSCONST_REG_HFREQB              c[VSCONST_REG_HFREQB_OFFSET        + 0] 
#define VSCONST_REG_HSPEEDB             c[VSCONST_REG_HSPEEDB_OFFSET       + 0] 
#define VSCONST_REG_HAMPLITUDEB         c[VSCONST_REG_HAMPLITUDEB_OFFSET   + 0] 
#define VSCONST_REG_VFREQA              c[VSCONST_REG_VFREQA_OFFSET        + 0] 
#define VSCONST_REG_VSPEEDA             c[VSCONST_REG_VSPEEDA_OFFSET       + 0] 
#define VSCONST_REG_VAMPLITUDEA         c[VSCONST_REG_VAMPLITUDEA_OFFSET   + 0] 

#endif /* VERT_DEFS_H */