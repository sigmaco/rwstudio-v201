#ifndef _3DSMAX_INCLUDES_H
#define _3DSMAX_INCLUDES_H

#include "max.h"
#include "modstack.h"
#include "iparamb2.h"
#include "shaders.h"
#include "stdmat.h"
#include "istdplug.h"
#include "iparamm2.h"
#include "utilapi.h"
#include "notify.h"
#include "simpobj.h"
#include "dummy.h"
#include "ikctrl.h"
#include "custcont.h"
#include "splshape.h"
#include "ISkin.h"
#include "imenuman.h"
#include "IIKSys.h"
#include "custattrib.h"
#include "icustattribcontainer.h"
#include "iparamwire.h"

#if (MAX_RELEASE >= 6000)
#include <d3dx9.h>
#include "IDxMaterial.h"
#endif //(MAX_RELEASE >= 6000)

#if (MAX_RELEASE >= 5000)
#include "IViewportManager.h"
#include "iEditNormals.h"
#include "lslights.h"
#endif // (MAX_RELEASE >= 5000)

#endif // _3DSMAX_INCLUDES_H
