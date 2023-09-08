//
// The purpose of this file is to, as much as possible, facilitate the 
// compilation of the NGC GNU files by the VC6 compiler.  Not all problems
// can be solved via this file.  So far, a couple of patches are required
// directly within the headers themselves.
//

//
// sn dolphin defines: SNVSI triggers patch in types.h for 64bit types
//
#define __SN__ 1
#define __SNVSI__ 1

//
// GXGeom: remove inline from static def
//
#define __size_t__
#define inline 

//
// GXVert at al: remove GNU __attribute__ 
//
#define __attribute__(x) 

// GNU IEEEFP
#define __PPC__ 1


